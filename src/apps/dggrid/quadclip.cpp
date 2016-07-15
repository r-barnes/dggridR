////////////////////////////////////////////////////////////////////////////////
//
// quadclip.cpp: quadclip class implementation
//
// Version 6.1  - Kevin Sahr, 5/23/13
// Version 6.2r - Richard Barnes, 2016-07-14
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>
#include <map>

using namespace std;

#include "dggrid.h"
#include "gridgen.h"
#include "clipper.hpp"
#include "DgIVec2D.h"
#include "DgInAIGenFile.h"
#include "DgInShapefile.h"
#include "DgOutShapefile.h"
#include "DgInShapefileAtt.h"
#include "DgOutLocFile.h"
#include "DgIDGG.h"
#include "DgBoundedIDGG.h"
#include "DgGeoSphRF.h"
#include "DgBoundedRF2D.h"
#include "DgParamList.h"
#include "DgProjGnomonicRF.h"
#include "DgGeoProjConverter.h"
#include "DgRandom.h"
#include "DgCell.h"
#include "DgLocList.h"
#include "DgDmdD4Grid2D.h"
#include "DgDmdD4Grid2DS.h"
#include "DgTriGrid2D.h"
#include "DgOutRandPtsText.h"
#include "DgHexSF.h"

//////////////////////////////////////////////////////////////////////////////
void createClipRegions (GridGenParam& dp, const DgIDGG& dgg, 
             DgQuadClipRegion clipRegions[], set<DgIVec2D> overageSet[],
             map<DgIVec2D, set<DgDBFfield> > overageFields[])
{
   cout << "\n* building clipping regions..." << endl;

   // initialize the i,j bounds

   for (int q = 1; q < 11; q++) 
   {
      clipRegions[q].setOffset(DgIVec2D(dgg.maxD(), dgg.maxJ()));
      clipRegions[q].setUpperRight(DgIVec2D(0, 0));
   }

   // create the gnomonic quad boundaries to clip against

   if (dp.megaVerbose) 
      cout << "creating gnomonic quad boundaries..." << endl;

   //// find center for gnomonic; use a dummy diamond

   DgRFNetwork tmpNet;
   DgContCartRF ccRF(tmpNet);
   DgDmdD4Grid2DS* dmd = 
        new DgDmdD4Grid2DS(tmpNet, ccRF, 1, 4, true, false);

   const DgDmdD4Grid2D& dmd0 = 
        *(dynamic_cast<const DgDmdD4Grid2D*>(dmd->grids()[0]));

   // generate the center on the dummy grid

   DgLocation cent;
   dmd0.setPoint(DgIVec2D(0, 0), cent);
   ccRF.convert(&cent);

   // generate the boundary on the dummy grid; adjust the boundary so
   // it will definately be on the quad after reprojection

   DgPolygon verts;
   dmd0.setVertices(DgIVec2D(0, 0), verts);
   ccRF.convert(verts);

   if (dp.megaVerbose) cout << "verts:\n" << verts << endl;

   // lower left

   DgDVec2D p = *ccRF.getAddress(verts[0]);
   p += DgDVec2D(0.0, dp.nudge);
   DgLocation* tloc = ccRF.makeLocation(p);
   verts.setLoc(0, *tloc);
   delete tloc;
      
   // lower right

   p = *ccRF.getAddress(verts[1]);
   p += DgDVec2D(-2.0 * dp.nudge, dp.nudge);
   tloc = ccRF.makeLocation(p);
   verts.setLoc(1, *tloc);
   delete tloc;

   // upper right

   p = *ccRF.getAddress(verts[2]);
   p += DgDVec2D(0.0, -dp.nudge);
   tloc = ccRF.makeLocation(p);
   verts.setLoc(2, *tloc);
   delete tloc;

   // upper left

   p = *ccRF.getAddress(verts[3]);
   p += DgDVec2D(2.0 * dp.nudge, -dp.nudge);
   tloc = ccRF.makeLocation(p);
   verts.setLoc(3, *tloc);
   delete tloc;

   if (dp.megaVerbose) cout << "(nudge) ->" << verts << endl;

   for (int q = 1; q < 11; q++)
   {
      if (dp.megaVerbose) cout << "quad " << q << endl;

      // find gnomonic projection center for this quad

      tloc = dgg.q2ddRF().makeLocation(
                                  DgQ2DDCoord(q, *ccRF.getAddress(cent)));

      if (dp.megaVerbose) cout << "center: " << *tloc;

      dgg.geoRF().convert(tloc);

      if (dp.megaVerbose) cout << " -> " << *tloc << endl;
   
      clipRegions[q].setGnomProj(new DgProjGnomonicRF(dgg.network(), 
           string("gnom") + dgg::util::to_string(q, 2), *dgg.geoRF().getAddress(*tloc)));

      delete tloc;

      Dg2WayGeoProjConverter(dgg.geoRF(), clipRegions[q].gnomProj());
   
      // now find gnomonic quad boundary

      // kludge to jump nets

      DgPolygon v0(dgg.q2ddRF());
      for (int i = 0; i < verts.size(); i++)
      {
         tloc = dgg.q2ddRF().makeLocation(
                               DgQ2DDCoord(q, *ccRF.getAddress(verts[i])));
         v0.push_back(*tloc);
         delete tloc;
      }

      if (dp.megaVerbose) 
         cout << v0 << endl;

      dgg.geoRF().convert(v0);

      if (dp.megaVerbose) 
         cout << " -> " << v0 << endl;

      clipRegions[q].gnomProj().convert(v0);

      if (dp.megaVerbose) 
         cout << " -> " << v0 << endl;

      // finally store the boundary as a clipper polygon
      {
         ClipperLib::Path contour;

         for (int i = 0; i < v0.size(); i++)
         {
            const DgDVec2D& p0 = *clipRegions[q].gnomProj().getAddress(v0[i]);
            contour << ClipperLib::IntPoint( DBL_TO_INT*p0.x() , DBL_TO_INT*p0.y() );
         }

         clipRegions[q].gnomBndry().push_back(contour);
	   }
	}

   //// read in the region boundary files

   for (unsigned long fc = 0; fc < dp.regionFiles.size(); fc++)
   {
      DgInLocFile* pRegionFile = NULL;
      DgInShapefileAtt* pAttributeFile = NULL;
      if (dp.clipAIGen)
         pRegionFile = new DgInAIGenFile(dgg.geoRF(), &dp.regionFiles[fc]);
      else
      {
         if (dp.buildShapeFileAttributes)
         {
            pRegionFile = pAttributeFile = 
                  new DgInShapefileAtt(dgg.geoRF(), &dp.regionFiles[fc]);

            // add any new fields to the global list
            const set<DgDBFfield>& fields = pAttributeFile->fields();
            for (set<DgDBFfield>::iterator it = fields.begin(); 
                 it != fields.end(); it++)
            {
               if (it->fieldName() == "global_id") continue;

               set<DgDBFfield>::iterator cur = dp.allFields.find(*it);
               if (cur == dp.allFields.end()) // new field
                  dp.allFields.insert(*it);
               else if (cur->type() != it->type())
                  report("input files contain incompatible definitions "
                         "of attribute field " + it->fieldName(), DgBase::Fatal);
            }
         }
         else
            pRegionFile = new DgInShapefile(dgg.geoRF(), &dp.regionFiles[fc]);
      }
      DgInLocFile& regionFile = *pRegionFile;

      if (!regionFile.isPointFile())
      {
         // read in each poly

         while (true) 
         {
            bool quadInt[12]; // which quads are intersected?
            for (int q = 0; q < 12; q++) 
               quadInt[q] = false;

            DgPolygon v;
            regionFile >> v;
            if (regionFile.isEOF()) break;
   
            if (dp.megaVerbose) cout << "input: " << v << endl;
   
            if (dp.geoDens > 0.000000000001) 
               DgGeoSphRF::densify(v, dp.geoDens);
   
            if (dp.megaVerbose) cout << "densified: " << v << endl;
   
            // create a copy to test for intersected quads
   
            DgPolygon quadVec(v);
   
            if (dp.megaVerbose) cout << "quadVec(v): " << quadVec << endl;
   
            dgg.q2ddRF().convert(quadVec);
   
            if (dp.megaVerbose) cout << "->: " << quadVec << endl;
   
            // test where vertices fall
   
            for (int i = 0; i < quadVec.size(); i++) 
            {
               const DgQ2DDCoord& qc = *dgg.q2ddRF().getAddress(quadVec[i]);

               quadInt[qc.quadNum()] = true;
            }

            // also test for possible arc crossings

            DgPolygon quadVec2(v);
            for (int q = 1; q < 11; q++)
            {
               const DgGeoCoord& cp = clipRegions[q].gnomProj().proj0();
               bool allGood = true;
               for (int i = 0; i < quadVec2.size(); i++) 
               {
                  if (DgGeoCoord::gcDist(
                       *dgg.geoRF().getAddress(quadVec2[i]), cp, false) > 90.0)
                  {
                     if (quadInt[q]) // problem!
                     {
                        cerr << "ERROR: polygon intersects quad #" 
                          << dgg::util::to_string(q) << " but a vertex of that polygon is "
                          << "more than 90' from the quad center." << endl;
                        cerr << "polygon is: " << endl;
                        cerr << v << endl;
                        report("break-up polygon or reorient grid", 
                                DgBase::Fatal);
                     }

                     allGood = false;

                     break;
                  }
               }

               if (allGood) quadInt[q] = true;
            }

            // now perform the intersection for each quad intersected

            int nQuadsInt = 0;
            for (int q = 1; q < 11; q++)
            {
               if (dp.megaVerbose) cout << "intersecting with quads..." << endl;

               if (!quadInt[q]) continue;

               if (dp.megaVerbose) cout << "quad " << q << endl;

               //// create a local copy and project to gnomonic

               if (dp.megaVerbose) cout << "input: " << v << endl;

               DgPolygon polyVec(v);

               if (dp.megaVerbose) cout << "polyVec(v): " << polyVec << endl;

               clipRegions[q].gnomProj().convert(polyVec);

               if (dp.megaVerbose) cout << " -> " << polyVec << endl;

               //// now create a clipper poly version

               ClipperLib::Paths clpPoly(1);

               for (int i = 0; i < polyVec.size(); i++)
               {
                  const DgDVec2D& p0 = 
                          *clipRegions[q].gnomProj().getAddress(polyVec[i]);
                  clpPoly[0] << ClipperLib::IntPoint( DBL_TO_INT*p0.x() , DBL_TO_INT*p0.y() );
               }

               //// find the intersections

               ClipperLib::Clipper c;

               c.AddPaths(clpPoly,                    ClipperLib::ptSubject, true);
               c.AddPaths(clipRegions[q].gnomBndry(), ClipperLib::ptClip,    true);

               ClipperLib::Paths solution;
               c.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);


               // check for intersection(s)
 
               if (solution.size()==0)
               {
                  if (dp.megaVerbose)
                     cout << "no intersection in quad " << q << endl;
                  continue;
               }

               // if we're here we have intersection(s)

               nQuadsInt++;
               clipRegions[q].setIsQuadUsed(true);

               ////// now convert back to Snyder and add to the clipRegions

               for (size_t i = 0; i < solution.size(); i++)
               {
                  DgPolygon locv(clipRegions[q].gnomProj());
                  for (size_t j = 0; j < solution[i].size(); j++)
                  {
                     DgLocation* tloc = clipRegions[q].gnomProj().makeLocation(
                                DgDVec2D(INT_TO_DBL*solution[i][j].X,
                                         INT_TO_DBL*solution[i][j].Y));
                     locv.push_back(*tloc);
                     delete tloc;
                  }

                  if (dp.megaVerbose) cout << "locv: " << locv << endl;

                  dgg.geoRF().convert(locv);

                  if (dp.megaVerbose) cout << "->" << locv << endl;

                  dgg.q2ddRF().convert(locv);

                  if (dp.megaVerbose) cout << "->" << locv << endl;

                  // add the intersection to our clipper list

                  ClipperLib::Paths cfinVerts(1);

                  for (int j = 0; j < locv.size(); j++)
                  {
                     const DgQ2DDCoord& qc = *dgg.q2ddRF().getAddress(locv[j]);

                     if (qc.quadNum() != q)
                        report("intersect poly crosses quad boundary; adjust "
                            "nudge", DgBase::Fatal);

                     cfinVerts[0]<<ClipperLib::IntPoint( DBL_TO_INT*qc.coord().x() , DBL_TO_INT*qc.coord().y() );
                  }

                  clipRegions[q].clpPolys().push_back(cfinVerts); //TODO: Original code made not of resPoly.hole[i] here

                  //// add the attributes for this polygon
                  if (dp.buildShapeFileAttributes)
                  {
                     const set<DgDBFfield>& fields = 
                              pAttributeFile->curObjFields();
                     clipRegions[q].polyFields().push_back(fields);
                  }

                  //// update the i,j bounds for this quad

                  for (int j = 0; j < locv.size(); j++)
                  {
                     DgLocation* tloc = dgg.ccFrame().makeLocation(
                               dgg.q2ddRF().getAddress(locv[j])->coord());

                     dgg.grid2D().convert(tloc);
                     const DgIVec2D& coord = *dgg.grid2D().getAddress(*tloc);
   
                     if (coord.i() < clipRegions[q].offset().i())
                        clipRegions[q].setOffset(
                           DgIVec2D(coord.i(), clipRegions[q].offset().j()));

                     if (coord.j() < clipRegions[q].offset().j())
                        clipRegions[q].setOffset(
                           DgIVec2D(clipRegions[q].offset().i(), coord.j()));

                     if (coord.i() > clipRegions[q].upperRight().i())
                        clipRegions[q].setUpperRight(
                         DgIVec2D(coord.i(), clipRegions[q].upperRight().j()));

                     if (coord.j() > clipRegions[q].upperRight().j())
                        clipRegions[q].setUpperRight(
                         DgIVec2D(clipRegions[q].upperRight().i(), coord.j()));

                     delete tloc;
                  }
               }
            }
         }
      }
      else // point file
      {
         // read in each point and add to sets

         while (true) 
         {
            DgLocVector v;
            regionFile >> v;
            if (regionFile.isEOF()) break;
   
            if (dp.megaVerbose) cout << "input: " << v << endl;
   
            dgg.convert(&v);
            for (int i = 0; i < v.size(); i++)
            {
               const DgQ2DICoord& q2di = *dgg.getAddress(v[i]);
               int q = q2di.quadNum();
               const DgIVec2D& coord = q2di.coord();
               clipRegions[q].setIsQuadUsed(true);

               clipRegions[q].points().insert(coord);

               //// add the attributes for this point
               if (dp.buildShapeFileAttributes)
               {
                  const set<DgDBFfield>& fields = 
                        pAttributeFile->curObjFields();
                  clipRegions[q].ptFields().insert(
                        pair<DgIVec2D, set<DgDBFfield> >(coord, fields));
               }

               //// update the i,j bounds for this quad

               if (coord.i() < clipRegions[q].offset().i())
                  clipRegions[q].setOffset(
                     DgIVec2D(coord.i(), clipRegions[q].offset().j()));

               if (coord.j() < clipRegions[q].offset().j())
                  clipRegions[q].setOffset(
                     DgIVec2D(clipRegions[q].offset().i(), coord.j()));

               if (coord.i() > clipRegions[q].upperRight().i())
                  clipRegions[q].setUpperRight(
                   DgIVec2D(coord.i(), clipRegions[q].upperRight().j()));

               if (coord.j() > clipRegions[q].upperRight().j())
                  clipRegions[q].setUpperRight(
                   DgIVec2D(clipRegions[q].upperRight().i(), coord.j()));
            }
         }
      }

      regionFile.close();
      delete pRegionFile;
   }  

   //// adjust the bounds for boundary buffers if needed ////

   int skipVal = (dp.aperture == 3) ? 3 : 1;
   for (int q = 1; q < 11; q++) 
   {
      if (!clipRegions[q].isQuadUsed()) continue;

      if (dp.megaVerbose) 
      {
         cout << "POLY FINAL: ";
         cout << "q: " << q << " " << clipRegions[q].offset()
                   << " " << clipRegions[q].upperRight() << endl;
      }

      // create a buffer zone if applicable

      if (dp.gridTopo == "TRIANGLE" || dp.gridTopo == "HEXAGON")
      {
         // build the adjusted minimum

         DgIVec2D newOff(clipRegions[q].offset().i() - skipVal,
                         clipRegions[q].offset().j() - skipVal);

         if (newOff.i() < 0) newOff = DgIVec2D(0, newOff.j());
         if (newOff.j() < 0) newOff = DgIVec2D(newOff.i(), 0);

         // build the adjusted upper right; track for quad overage

         DgIVec2D newUR(clipRegions[q].upperRight());

         if (newUR.i() <= dgg.maxI()) 
            newUR = DgIVec2D(newUR.i() + skipVal, newUR.j());

         if (newUR.j() <= dgg.maxJ()) 
            newUR = DgIVec2D(newUR.i(), newUR.j() + skipVal);

         if (newUR.i() > dgg.maxI()) 
         {
            clipRegions[q].setOverI(true);
            newUR = DgIVec2D(dgg.maxI(), newUR.j());
         }

         if (newUR.j() > dgg.maxJ()) 
         {
            clipRegions[q].setOverJ(true);
            newUR = DgIVec2D(newUR.i(), dgg.maxJ());
         }

         clipRegions[q].setUpperRight(newUR);
         clipRegions[q].setOffset(newOff);

         if (dp.megaVerbose) 
         {
            cout << "AFTER ADJUSTMENT: ";
            cout << "q: " << q << " " << clipRegions[q].offset()
                   << " " << clipRegions[q].upperRight() << endl;
         }
      }
   }

   //////// now do a quad at a time ///////

   // look for possible overage

   dp.nCellsTested = 0;
   dp.nCellsAccepted = 0;
   dp.nSamplePts = 0;

   const DgContCartRF& cc1 = dgg.ccFrame();
   const DgDiscRF2D& grid = dgg.grid2D();
   
   if (!dgg.isCongruent())
   {
      for (int q = 1; q < 11; q++)
      {
         if (dp.verbosity > 0) cout << "Checking OVERAGE quad " << q << endl;
         if (!clipRegions[q].isQuadUsed()) continue;
   
         // check for over J

         DgIVec2D lLeft(clipRegions[q].offset());
         DgIVec2D uRight(clipRegions[q].upperRight());
         if (clipRegions[q].overJ())
         {
            lLeft = DgIVec2D(lLeft.i(), dgg.maxJ() + 1);
            uRight = DgIVec2D(uRight.i(), dgg.maxJ() + 1);

            if (dp.megaVerbose) 
               cout << "OVERJ in quad " << q << "-" << lLeft << 
                       " " << uRight << endl;

            DgIVec2D tCoord = lLeft;
            while (true)
            {
               DgIVec2D coord = tCoord;

               if (dp.buildShapeFileAttributes) 
                  dp.curFields.clear();

               bool accepted = evalCell(dp, dgg, cc1, grid, clipRegions[q], 
                                        coord);

               if (!accepted) 
               {
                  if (tCoord == uRight) break;
                  tCoord.setI(tCoord.i() + 1);
                  continue;
               }

               // transfer to correct quad

               const DgQuadEdgeCells& ec = DgIDGG::edgeTable(q);
               int newQ = q;
               DgIVec2D newC(coord);
               if (ec.isType0())
               {
                  if (coord.j() == (dgg.maxJ() + 1))
                  {
                     if (coord.i() == 0)
                     {
                        newQ = ec.loneVert();
                        newC = DgIVec2D(0, 0);
                     }
                     else
                     {
                        newQ = ec.upQuad();
                        newC = DgIVec2D(0, (dgg.maxI() + 1) - coord.i());
                     }
                  }
                  else // i == (dgg.maxI() + 1)
                  {
                     newQ = ec.rightQuad();
                     newC.setI(0);
                  }
               }
               else // type 1
               {
                  if (coord.i() == (dgg.maxI() + 1))
                  {
                     if (coord.j() == 0)
                     {
                        newQ = ec.loneVert();
                        newC = DgIVec2D(0, 0);
                     }
                     else
                     {
                        newQ = ec.rightQuad();
                        newC = DgIVec2D((dgg.maxJ() + 1) - coord.j(), 0);
                     }
                  }
                  else // j == (dgg.maxJ() + 1)
                  {
                     newQ = ec.upQuad();
                     newC.setJ(0);
                  }
               }

               // add to the overage set
               overageSet[newQ].insert(newC);
               if (dp.buildShapeFileAttributes)
                  overageFields[newQ].insert(
                    pair<DgIVec2D, set<DgDBFfield> >(newC, dp.curFields));

               if (dp.megaVerbose)
                  cout << "PUSH OVERAGE: " << q << ": " << coord <<
                       " -> " << newQ << ": " << newC << endl;

               if (tCoord == uRight) break;
               tCoord.setI(tCoord.i() + 1);
            }
         }

         // check for over I

         if (clipRegions[q].overI())
         {
            lLeft = DgIVec2D(dgg.maxI() + 1, lLeft.j());
            uRight = DgIVec2D(dgg.maxI() + 1, uRight.j());
   
            if (uRight.j() == dgg.maxJ() && clipRegions[q].overJ())
               uRight = DgIVec2D(uRight.i(), dgg.maxJ() + 1);

            if (dp.megaVerbose) 
               cout << "OVERI in quad " << q << "-" << lLeft << " " << uRight << endl;

            DgBoundedRF2D b1(grid, lLeft, uRight);
            for (DgIVec2D tCoord = b1.lowerLeft(); tCoord != b1.invalidAdd();
                 tCoord = b1.incrementAddress(tCoord))
            {
               DgIVec2D coord = tCoord;

               if (dp.buildShapeFileAttributes) 
                  dp.curFields.clear();

               bool accepted = evalCell(dp, dgg, cc1, grid, clipRegions[q], 
                                        coord);

               if (!accepted) continue;

               // transfer to correct quad

               const DgQuadEdgeCells& ec = DgIDGG::edgeTable(q);
               int newQ = q;
               DgIVec2D newC(coord);
               if (ec.isType0())
               {
                  if (coord.j() == (dgg.maxJ() + 1))
                  {
                     if (coord.i() == 0)
                     {
                        newQ = ec.loneVert();
                        newC = DgIVec2D(0, 0);
                     }
                     else
                     {
                        newQ = ec.upQuad();
                        newC = DgIVec2D(0, (dgg.maxI() + 1) - coord.i());
                     }
                  }
                  else // i == (dgg.maxI() + 1)
                  {
                     newQ = ec.rightQuad();
                     newC.setI(0);
                  }
               }
               else // type 1
               {
                  if (coord.i() == (dgg.maxI() + 1))
                  {
                     if (coord.j() == 0)
                     {
                        newQ = ec.loneVert();
                        newC = DgIVec2D(0, 0);
                     }
                     else
                     {
                        newQ = ec.rightQuad();
                        newC = DgIVec2D((dgg.maxJ() + 1) - coord.j(), 0);
                     }
                  }
                  else // j == (dgg.maxJ() + 1)
                  {
                     newQ = ec.upQuad();
                     newC.setJ(0);
                  }
               }

               // add to the overage set
               overageSet[newQ].insert(newC);
               if (dp.buildShapeFileAttributes)
                  overageFields[newQ].insert(
                    pair<DgIVec2D, set<DgDBFfield> >(newC, dp.curFields));

               if (dp.megaVerbose)
                  cout << "ADD OVERAGE: " << q << ": " << coord <<
                          " -> " << newQ << ": " << newC << endl;
            }
         }
      }
   }

} // void createClipRegions

