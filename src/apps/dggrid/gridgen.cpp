////////////////////////////////////////////////////////////////////////////////
//
// gridgen.cpp: gridgen class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <set>
#include <cstdlib>

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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
GridGenParam::GridGenParam (DgParamList& plist)
      : MainParam(plist), wholeEarth (false), seqToPoly(false), clipAIGen (true),
        nRandPts (0), clipRandPts (false), nDensify (1),
        nudge (0.001), ptsRand (0), doPointInPoly (true), 
        doPolyIntersect (false), sampleCount(0), nSamplePts(0), 
        doRandPts (true), cellOut (0), ptOut (0), randPtsOut (0), 
        cellOutShp (0), ptOutShp (0), concatPtOut (true), useEnumLbl (false), 
        nCellsTested(0), nCellsAccepted (0)
{ 
      using namespace dgg;

      /////// fill state variables from the parameter list //////////

      string dummy;
      getParamValue(plist, "clip_subset_type", dummy, false);
      if (dummy == "WHOLE_EARTH") 
         wholeEarth = true;
      else if (dummy == "AIGEN")
         clipAIGen  = true;
      else if (dummy == "SHAPEFILE")
         clipAIGen  = false; 
      else if (dummy == "SEQTOPOLY")
         seqToPoly = true;
      else
        ::report("Unrecognised value for 'clip_subset_type'", DgBase::Fatal);
        //throw "Unrecognised value for 'clip_subset_type'";

      //// region file names

      string regFileStr;
      getParamValue(plist, "clip_region_files", regFileStr, false);

      util::ssplit(regFileStr, regionFiles);

      getParamValue(plist, "geodetic_densify", geoDens, false);
      geoDens *= M_PI / 180.0;

      getParamValue(plist, "densification", nDensify, false);
      //nudge = 0.00000001;
      nudge = 0.0000001;
      //getParamValue(plist, "quad_bndry_nudge", nudge, false);

      getParamValue(plist, "clip_type", dummy, false);
      if (dummy == "POLY_INTERSECT") 
      {
         doPolyIntersect = true;
         doPointInPoly = false;
      }
      else
      {
         doPolyIntersect = false;
         doPointInPoly = true;
      }

      ///// output files

      getParamValue(plist, "cell_output_type", cellOutType, "NONE");
      getParamValue(plist, "point_output_type", pointOutType, "NONE");
      getParamValue(plist, "randpts_output_type", randPtsOutType, "NONE");

      getParamValue(plist, "cell_output_file_name", cellOutFileNameBase, 
                       false);
      getParamValue(plist, "point_output_file_name", ptOutFileNameBase, 
                       false);
      getParamValue(plist, "randpts_output_file_name", randPtsOutFileNameBase, 
                       false);
      getParamValue(plist, "shapefile_id_field_length", shapefileIdLen, 
                       false);
      getParamValue(plist, "kml_default_color", kmlColor, false);
      getParamValue(plist, "kml_default_width", kmlWidth, false);
      getParamValue(plist, "kml_name", kmlName, false);
      getParamValue(plist, "kml_description", kmlDescription, false);

      getParamValue(plist, "update_frequency", updateFreq, false);
      getParamValue(plist, "max_cells_per_output_file", maxCellsPerFile, false);

      string outLblType("GLOBAL_SEQUENCE");
      getParamValue(plist, "output_cell_label_type", outLblType, false);
      if (outLblType == "SUPERFUND" && !isSuperfund)
         report("output_cell_label_type of SUPERFUND"
                " requires dggs_type of SUPERFUND", DgBase::Fatal);

      if (outLblType != "SUPERFUND" && isSuperfund)
         report("dggs_type of SUPERFUND requires "
                   "output_cell_label_type of SUPERFUND", DgBase::Fatal);

      if (outLblType == "ENUMERATION") 
         useEnumLbl = true;
      else
         useEnumLbl = false;

      doRandPts = false;
      if (randPtsOutType != "NONE")
      {
         doRandPts = true;
         getParamValue(plist, "randpts_num_per_cell", nRandPts, false);
         if (nRandPts < 1)
            doRandPts = false;
         else
         {
            unsigned long int ranSeed = 0;
            getParamValue(plist, "randpts_seed", ranSeed, false);
            if (useMother) ptsRand = new DgRandMother(ranSeed);
            else ptsRand = new DgRand(ranSeed);
            getParamValue(plist, "randpts_concatenate_output", concatPtOut, false);
            //getParamValue(plist, "clip_randpts", clipRandPts, false);
         }
      }

      sprintf(formatStr, ", %%#.%dLF, %%#.%dLF\n", precision, precision);

      ///// attribute files

      buildShapeFileAttributes = false;
      outCellAttributes = false;
      outPointAttributes = false;

/*** shapefile attribute stuff
      getParamValue(plist, "build_shapefile_attributes", buildShapeFileAttributes, false);
///// alternate stuff
>       getParamValue(plist, "build_shapefile_attributes_source", dummy, false);
>       if (dummy == string("CLIP_FILES"))
>       {
>          buildShapeFileAttributes = true;
>          buildClipFileAttributes = true;
>       }
>       else if (dummy == string("SHAPEFILES"))
>       {
>          buildShapeFileAttributes = true;
>          buildClipFileAttributes = false;
>       }

      
      if (buildShapeFileAttributes)
      {
         if (clipAIGen || wholeEarth)
            report("build_shapefile_attributes "
                   "requires clip_subset_type of SHAPEFILE", DgBase::Fatal);

         if (cellOutType == "SHAPEFILE")
            outCellAttributes = true;

         if (pointOutType == "SHAPEFILE")
            outPointAttributes = true;

         if (!outCellAttributes && !outPointAttributes)
            report("using build_shapefile_attributes requires SHAPEFILE "
                   "cell or point output.", DgBase::Fatal);
      }

      getParamValue(plist, "shapefile_attribute_default_int", 
                     shapefileDefaultInt, false);
      getParamValue(plist, "shapefile_attribute_default_double", 
                     shapefileDefaultDouble, false);
      getParamValue(plist, "shapefile_attribute_default_string", 
                     shapefileDefaultString, false);

>       if (buildShapeFileAttributes && !buildClipFileAttributes)
>       {
>          getParamValue(plist, "attribute_files", regFileStr, false);
>          util::ssplit(regFileStr, attributeFiles);
>       }

***/

} // GridGenParam::GridGenParam

////////////////////////////////////////////////////////////////////////////////
GridGenParam::~GridGenParam ()
{
 delete ptsRand;

 // Flush and close any input files we may have used:
 delete cellOut; 
 delete ptOut;
 delete randPtsOut;

}

////////////////////////////////////////////////////////////////////////////////
void outputStatus (const GridGenParam& dp, bool force)
{
   if (force || (dp.nCellsTested && 
             (dp.updateFreq && (dp.nCellsTested % dp.updateFreq == 0))))
   {
      if (dp.wholeEarth)
         cout << "* generated " << dgg::util::addCommas(dp.nCellsAccepted) 
              << " cells" << endl;
      else
      {
         cout << "accepted " << dgg::util::addCommas(dp.nCellsAccepted) 
              << " cells / ";
         cout << dgg::util::addCommas(dp.nCellsTested) << " tested" << endl; 
      }
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void GridGenParam::dump (void)
{
   MainParam::dump();

   cout << "BEGIN GEN PARAMETER DUMP" << endl;
   
   cout << " wholeEarth: " << wholeEarth << endl;
   cout << " clipAIGen: " << clipAIGen << endl;
   
   cout << " regionFiles: " << endl;
   for (unsigned long i = 0; i < regionFiles.size(); i++)
      cout << "  " << i << " " << regionFiles[i] << endl;
   
   cout << " nRandPts: " << nRandPts << endl;
   //cout << " clipRandPts: " << clipRandPts << endl;
   cout << " nDensify: " << nDensify << endl;
   cout << " precision: " << precision << endl;
   cout << " nudge: " << nudge << endl;

   cout << " *ptsRand: ";
   if (ptsRand)
      cout << *ptsRand << endl;
   else
      cout << "null" << endl; 

   cout << " cellOutType: " << cellOutType << endl;
   cout << " pointOutType: " << pointOutType << endl;
   cout << " randPtsOutType: " << randPtsOutType << endl;
   cout << " cellOutFileNameBase: " << cellOutFileNameBase << endl;
   cout << " ptOutFileNameBase: " << ptOutFileNameBase << endl;
   cout << " randPtsOutFileNameBase: " << randPtsOutFileNameBase << endl;
   cout << " doPointInPoly: " << doPointInPoly << endl;
   cout << " doPolyIntersect: " << doPolyIntersect << endl;
   cout << " sampleCount: " << sampleCount << endl;
   cout << " nSamplePts: " << nSamplePts << endl;
   cout << " doRandPts: " << doRandPts << endl;

   cout << " *cellOut: ";
   if (cellOut)
      cout << *cellOut << endl;
   else
      cout << "null" << endl; 

   cout << " *ptOut: ";
   if (cellOut)
      cout << *ptOut << endl;
   else
      cout << "null" << endl; 

   cout << " *randPtsOut: ";
   if (randPtsOut)
      cout << *randPtsOut << endl;
   else
      cout << "null" << endl; 

   cout << " concatPtOut: " << concatPtOut << endl;
   cout << " formatStr: " << formatStr << endl;
   cout << " useEnumLbl: " << useEnumLbl << endl;
   cout << " nCellsTested: " << nCellsTested << endl;
   cout << " nCellsAccepted: " << nCellsAccepted << endl;
   cout << " geoDens: " << geoDens << endl;
   cout << " updateFreq: " << updateFreq << endl;
   cout << " maxCellsPerFile: " << maxCellsPerFile << endl;
   
   cout << "END GEN PARAMETER DUMP" << endl;

} // void GridGenParam::dump

////////////////////////////////////////////////////////////////////////////////
bool evalCell (GridGenParam& dp,  const DgIDGG& dgg, const DgContCartRF& cc1,
               const DgDiscRF2D& grid, DgQuadClipRegion& clipRegion,
               const DgIVec2D& add2D)
{
   if (!dgg.isClassI() && (add2D.j() + add2D.i()) % 3) 
    return false;
  
   dp.nCellsTested++;

   if (dp.megaVerbose) 
      cout << "Testing #" << dp.nCellsTested << ": " << add2D << endl;

   bool accepted = false; 

   // start by checking the points
   set<DgIVec2D>::iterator it = clipRegion.points().find(add2D);
   if (it != clipRegion.points().end())
   {
      accepted = true;
      clipRegion.points().erase(it);

      if (dp.buildShapeFileAttributes)
      {
         // add the fields for this point

         map<DgIVec2D, set<DgDBFfield> >::iterator itFields =
                           clipRegion.ptFields().find(add2D);
         const set<DgDBFfield>& fields = itFields->second;
         for (set<DgDBFfield>::iterator it = fields.begin();
                it != fields.end(); it++)
            dp.curFields.insert(*it);

         clipRegion.ptFields().erase(itFields);
      }
      else // only need one intersection
         return accepted;
   }
    
   // generate the boundary

   DgLocation* loc = grid.makeLocation(add2D);

   DgPolygon verts;
   grid.setVertices(add2D, verts);

   cc1.convert(loc);
   DgDVec2D cp = *cc1.getAddress(*loc);
   delete loc;

   cc1.convert(verts);

   // discard cells that don't meet poly-intersect clipping criteria if
   // applicable

   if (dp.doPolyIntersect)
   {
      bool failure = true;

      ClipperLib::Paths cellPoly(1);
      for (int i = 0; i < verts.size(); i++)
        cellPoly[0] << ClipperLib::IntPoint( DBL_TO_INT*cc1.getAddress((verts)[i])->x() , DBL_TO_INT*cc1.getAddress((verts)[i])->y() );

      for (unsigned int i = 0; i < clipRegion.clpPolys().size(); i++){

        ClipperLib::Clipper c;

        c.AddPaths(cellPoly,                 ClipperLib::ptSubject, true);
        c.AddPaths(clipRegion.clpPolys()[i], ClipperLib::ptClip,    true);

        ClipperLib::Paths solution;
        c.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

        if (solution.size()!=0){
      	  accepted = true;
      	  failure  = false;

          if (dp.buildShapeFileAttributes){
             // add the fields for this polygon
             const set<DgDBFfield>& fields = clipRegion.polyFields()[i];
             for (set<DgDBFfield>::iterator it = fields.begin();
                    it != fields.end(); it++)
                dp.curFields.insert(*it);
          } else { // only need one intersection
            goto EVALCELL_FINISH;
          }
        }
      }

     // If we are here, we did not fail:
     failure = false;

     goto EVALCELL_FINISH;
           
EVALCELL_FINISH:
      if(failure)
        throw "Out of memory in evalCell()";

      return accepted;
    }

  return accepted;

} // bool evalCell

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool evalCell (DgEvalData* data, DgIVec2D& add2D)
{
   if (data->dp.buildShapeFileAttributes)
      data->dp.curFields.clear();

   bool accepted = false;

   if (!data->overageSet.empty())
   {
      set<DgIVec2D>::iterator it = data->overageSet.find(add2D);
      if (it != data->overageSet.end()) // found add2D
      {
         accepted = true;

         data->overageSet.erase(it);

         if (data->dp.buildShapeFileAttributes)
         {
            // add the fields for this polygon

            map<DgIVec2D, set<DgDBFfield> >::iterator itFields =
                              data->overageFields.find(add2D);
            const set<DgDBFfield>& fields = itFields->second;
            for (set<DgDBFfield>::iterator it = fields.begin();
                   it != fields.end(); it++)
               data->dp.curFields.insert(*it);

            data->overageFields.erase(itFields);
         }
         else // only need one intersection
            return true;
      }
   }

   if (data->dp.buildShapeFileAttributes && accepted)
   {
      evalCell(data->dp, data->dgg, data->cc1, data->grid, 
                      data->clipRegion, add2D);
      return true;
   }

   if (add2D.i() < data->lLeft.i() || add2D.i() > data->uRight.i() ||
       add2D.j() < data->lLeft.j() || add2D.j() > data->uRight.j())
      return false;
   else
      return evalCell(data->dp, data->dgg, data->cc1, data->grid, 
                      data->clipRegion, add2D);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void genPoints (GridGenParam& dp, const DgIDGG& dgg, const DgQ2DICoord& add2D, 
                const DgContCartRF& deg, const string& label)
{
   const DgDiscRF2D& grid = dgg.grid2D();
   const DgRF<DgDVec2D, long double>& ccRF = grid.backFrame();
   int q = add2D.quadNum();

   DgLocation* cp = new DgLocation();
   grid.setPoint(add2D.coord(), *cp);
   ccRF.convert(cp);

   if (dp.megaVerbose) cout << "RP: " << add2D << " " << *cp;

   DgLocVector rpts(dgg.q2ddRF());
   DgDVec2D cpv = *ccRF.getAddress(*cp);

   grid.setPoint(DgIVec2D(0, 0), *cp);
   DgDVec2D cpv00 = *ccRF.getAddress(*cp);

   if (dp.megaVerbose) cout << " " << *cp;
   
   delete cp;

   if (dp.megaVerbose)
   {
      DgDVec2D tvec = cpv - cpv00;
      cout << " " << tvec << endl;
   }

   DgDVec2D anchor;
   if (dp.gridTopo == "HEXAGON")
   {
      anchor = cpv; // use center
   }
   else // DIAMOND or TRIANGLE happen to be the same
   {
      DgPolygon verts;
      grid.setVertices(add2D.coord(), verts);
      ccRF.convert(verts);
      anchor = *ccRF.getAddress(verts[0]); // use lower left corner
   }
   if (dp.megaVerbose) cout << "anchor: " << anchor << endl;

   for (int i = 0; i < dp.nRandPts; i++)
   {
      // first generate point on (0,0) diamond
        
      DgDVec2D rp(dp.ptsRand->randInRange(0.0, 1.0),
         dp.ptsRand->randInRange(0.0, 2.0 * DgDmdD4Grid2D::yOff()));

      if (dp.megaVerbose) cout << i << " " << rp; 
   
      rp.setX(rp.x() - (sqrt(3.0) / 3.0) * rp.y()); // shear

      if (dp.megaVerbose) cout << " " << rp; 

      DgLocation* nloc = 0;
      if (dp.gridTopo != string("DIAMOND"))
      {
         // force into (0, 0) triangle

         if (rp.y() > sqrt(3.0) * rp.x()) rp.rotate(-60.0);

         if (dp.gridTopo == "TRIANGLE")
         {
            rp *= M_SQRT3; // scale correctly

            const DgTriGrid2D& triG0 =
                             dynamic_cast<const DgTriGrid2D&>(grid);

            if (!triG0.isUp(add2D.coord())) rp.rotate(-60.0);

         }
         else if (dp.gridTopo == "HEXAGON")
         {
            DgDVec2D origrp(rp);
            bool goodPt = false;

            while (!goodPt)
            {
               rp = origrp * (1.0 / sqrt(3.0)); // scale correctly

               // position in one of the subtris

               int ndx = dp.ptsRand->nextInt() % 6;
               rp.rotate(-60.0 * ndx);
   
               // adjust for class I

               if (dgg.isClassI()) rp.rotate(30.0);

               // check for possible pentagon non-keeper

               goodPt = true;
               if (add2D.coord() == DgIVec2D(0, 0)) // pentagon
               {
                  DgLocation* tLoc = 
                         dgg.q2ddRF().makeLocation(DgQ2DDCoord(q, rp));

                  dgg.vertexRF().convert(tLoc);
                            
                  const DgAddress<DgVertex2DDCoord>& vc = 
                                        *dgg.vertexRF().getAddress(*tLoc);

                  if (!vc.address().keep()) goodPt = false;

                  delete tLoc;
               }
            }

            if (dp.megaVerbose) cout << rp << endl;
         }

         rp += anchor;
         if (dp.megaVerbose) cout << " =final rp: " << rp; 

         DgLocation* tloc = ccRF.makeLocation(rp);
         dgg.ccFrame().convert(tloc);

         if (dp.megaVerbose) cout << "->" << *tloc; 

         nloc = dgg.q2ddRF().makeLocation(
                       DgQ2DDCoord(q, *dgg.ccFrame().getAddress(*tloc)));

         delete tloc;

         if (dp.megaVerbose) cout << "->" << *nloc << endl;

         rpts.push_back(*nloc);
         delete nloc;
      }
   }

   // convert

   if (dp.megaVerbose) cout << "rpts: " << rpts << endl;

   dgg.geoRF().convert(rpts);

   if (dp.megaVerbose) cout << "-> " << rpts << endl;

   deg.convert(rpts);

   if (dp.megaVerbose) cout << "-> " << rpts << endl;

   DgLocList pts;
   for (int i = 0; i < rpts.size(); i++)
   {
      dp.nSamplePts++;   // pt # in this grid
      dp.sampleCount++;  // pt # overall

      DgCell cell(deg, dgg::util::to_string(dp.sampleCount), rpts[i]);

      if (!dp.randPtsOutType.compare("TEXT"))
      {
          // pack more info into the cell label
          ostringstream os;
          os << dp.sampleCount << ", " << dp.curGrid << ", "
                   << label << ", " << dp.nSamplePts << ", ";

          cell.setLabel(os.str());
      }

      *dp.randPtsOut << cell;
   }

} // void genPoints

////////////////////////////////////////////////////////////////////////////////
void outputCell (GridGenParam& dp, const DgIDGG& dgg,
                   const DgLocation& add2D, const DgPolygon& verts,
                   const DgContCartRF& deg, const string& label)
{
   dp.nCellsOutputToFile++;

   if (dp.maxCellsPerFile && dp.nCellsOutputToFile > dp.maxCellsPerFile)
   {
      dp.nCellsOutputToFile = 1;
      dp.nOutputFile++;

      if (dp.cellOut)
      {
         delete dp.cellOut;
         dp.cellOut = NULL;
         dp.cellOutShp = NULL;

         string fileName = dp.cellOutFileName + string("_") + 
                                       dgg::util::to_string(dp.nOutputFile);
         dp.cellOut = DgOutLocFile::makeOutLocFile(dp.cellOutType, fileName,
                         deg, false, dp.precision, dp.shapefileIdLen,
                         dp.kmlColor, dp.kmlWidth, dp.kmlName, 
                         dp.kmlDescription);

         if (dp.outCellAttributes)
         {
            dp.cellOutShp = static_cast<DgOutShapefile*>(dp.cellOut);
            dp.cellOutShp->setDefIntAttribute(dp.shapefileDefaultInt);
            dp.cellOutShp->setDefDblAttribute(dp.shapefileDefaultDouble);
            dp.cellOutShp->setDefStrAttribute(dp.shapefileDefaultString);

            dp.cellOutShp->addFields(dp.allFields);
         }
      }

      if (dp.ptOut)
      {
         delete dp.ptOut;
         dp.ptOut = NULL;
         dp.ptOutShp = NULL;

         string fileName = dp.ptOutFileName + string("_") + 
                                       dgg::util::to_string(dp.nOutputFile);
         dp.ptOut = DgOutLocFile::makeOutLocFile(dp.pointOutType, fileName,
                      deg, true, dp.precision, dp.shapefileIdLen,
                      dp.kmlColor, dp.kmlWidth, dp.kmlName, dp.kmlDescription);

         if (dp.outPointAttributes)
         {
            dp.ptOutShp = static_cast<DgOutShapefile*>(dp.ptOut);
            dp.ptOutShp->setDefIntAttribute(dp.shapefileDefaultInt);
            dp.ptOutShp->setDefDblAttribute(dp.shapefileDefaultDouble);
            dp.ptOutShp->setDefStrAttribute(dp.shapefileDefaultString);

            dp.ptOutShp->addFields(dp.allFields);
         }
      }

      if (dp.doRandPts && dp.randPtsOut)
      {
         delete dp.randPtsOut;
         dp.randPtsOut = NULL;

         string fileName = dp.randPtsOutFileName + string("_") + 
                                       dgg::util::to_string(dp.nOutputFile);
         if (dp.curGrid == 1 || !dp.concatPtOut)
         {
            if (!dp.randPtsOutType.compare("TEXT"))
               dp.randPtsOut = new DgOutRandPtsText(deg, fileName, dp.precision);
            else
               dp.randPtsOut = DgOutLocFile::makeOutLocFile(dp.randPtsOutType,
                    fileName, deg, true, dp.precision, dp.shapefileIdLen,
                    dp.kmlColor, dp.kmlWidth, dp.kmlName, dp.kmlDescription);
         }
      }
   }

   DgLocation* tmpLoc = new DgLocation(add2D);
   DgCell cell(dgg.geoRF(), label, *tmpLoc, new DgPolygon(verts));

/* 
   //output cell area
   dgg.geoRF().convert(tmpLoc);
   long double area = 
         DgGeoCoord::geoPolyArea(verts, *dgg.geoRF().getAddress(*tmpLoc));
   cout << std::setprecision(15);
   cout << label << " " << area << " " 
        << area * dgg.geoRF().earthRadiusKM() * dgg.geoRF().earthRadiusKM() 
        << endl;
*/

   delete tmpLoc;

/* interleaved coord not currently used
   tmpLoc = new DgLocation(add2D);
   dgg.intRF().convert(tmpLoc);
   DgInterleaveCoord intCoord = *dgg.intRF().getAddress(*tmpLoc);
   delete tmpLoc;
*/
         
   if (dp.megaVerbose) 
      cout << "accepted " << label << " " << add2D << endl;

   if (dp.cellOut)
   {
      if (dp.outCellAttributes)
         dp.cellOutShp->setCurFields(dp.curFields);

      *dp.cellOut << cell;
   }
	
   if (dp.ptOut)
   {
      if (dp.outPointAttributes)
         dp.ptOutShp->setCurFields(dp.curFields);

      *dp.ptOut << cell;
   }
	
   ///// generate random points if applicable ///
   if (dp.doRandPts) 
      genPoints(dp, dgg, *dgg.getAddress(add2D), deg, cell.label());

} // void outputCell

////////////////////////////////////////////////////////////////////////////////
void outputCellAdd2D (GridGenParam& dp, const DgIDGG& dgg, const DgLocation& add2D, 
                 const DgPolygon& verts, const DgContCartRF& deg)
{
   // create the various output forms

   unsigned long long int sn = dgg.bndRF().seqNum(add2D);
   string *label;
   if (dp.useEnumLbl)
      label = new string(dgg::util::to_string(dp.nCellsAccepted));
   else
      label = new string(dgg::util::to_string(sn));

   outputCell(dp, dgg, add2D, verts, deg, *label);
   delete label;

} // void outputCellAdd2D

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void genGrid (GridGenParam& dp)
{
   ////// create the reference frames ////////

   DgRFNetwork net0;
   DgGeoSphRF geoRF(net0, dp.datum, dp.earthRadius);
   DgIDGG dgg(geoRF, dp.vert0, dp.azimuthDegs, dp.aperture, dp.actualRes,
              "DDG", dp.gridTopo, dp.projType, dp.isMixed43, dp.numAp4, 
              dp.isSuperfund, dp.sfRes, dp.precision);

   cout << "Res " << dgg.outputRes() << " " << dgg.gridStats();

   // set-up to convert to degrees
   DgGeoSphDegRF deg(geoRF, geoRF.name() + "Deg");

   // create output files that rely on having the RF's created

   dp.nCellsOutputToFile = 0;
   dp.nOutputFile = 1;

   string cellOutFileName = dp.cellOutFileName;
   string ptOutFileName = dp.ptOutFileName;
   string randPtsOutFileName = dp.randPtsOutFileName;
   if (dp.maxCellsPerFile)
   {
      cellOutFileName += "_1";
      ptOutFileName += "_1";
      randPtsOutFileName += "_1";
   }
   
   dp.cellOut = DgOutLocFile::makeOutLocFile(dp.cellOutType, cellOutFileName,
                   deg, false, dp.precision, dp.shapefileIdLen,
                   dp.kmlColor, dp.kmlWidth, dp.kmlName, dp.kmlDescription);

   dp.cellOutShp = NULL;
   if (dp.outCellAttributes)
   {
      dp.cellOutShp = static_cast<DgOutShapefile*>(dp.cellOut);
      dp.cellOutShp->setDefIntAttribute(dp.shapefileDefaultInt);
      dp.cellOutShp->setDefDblAttribute(dp.shapefileDefaultDouble);
      dp.cellOutShp->setDefStrAttribute(dp.shapefileDefaultString);
   }

   dp.ptOut = DgOutLocFile::makeOutLocFile(dp.pointOutType, ptOutFileName,
                   deg, true, dp.precision, dp.shapefileIdLen,
                   dp.kmlColor, dp.kmlWidth, dp.kmlName, dp.kmlDescription);

   dp.ptOutShp = NULL;
   if (dp.outPointAttributes)
   {
      dp.ptOutShp = static_cast<DgOutShapefile*>(dp.ptOut);
      dp.ptOutShp->setDefIntAttribute(dp.shapefileDefaultInt);
      dp.ptOutShp->setDefDblAttribute(dp.shapefileDefaultDouble);
      dp.ptOutShp->setDefStrAttribute(dp.shapefileDefaultString);
   }

   dp.randPtsOut = NULL;
   if (dp.doRandPts) 
   {
      if (dp.curGrid == 1 || !dp.concatPtOut)
      {
         if (!dp.randPtsOutType.compare("TEXT"))
            dp.randPtsOut = new DgOutRandPtsText(deg, randPtsOutFileName,
                      dp.precision);
         else
            dp.randPtsOut = DgOutLocFile::makeOutLocFile(dp.randPtsOutType, 
                      randPtsOutFileName, deg, true, dp.precision, dp.shapefileIdLen,
                      dp.kmlColor, dp.kmlWidth, dp.kmlName, dp.kmlDescription);
      }
   }

   ////// do whole earth grid if applicable /////

   if (dp.seqToPoly)
   {
      dp.nCellsAccepted = 0;
      dp.nCellsTested = 0;

      const int maxLine = 1000;
      char buff[maxLine];

      std::ifstream fin(dp.regionFiles[0].c_str());
      unsigned long int seqnum;
      set<unsigned long int> seqnums; //To ensure each cell is printed once
      while(1){
        dp.nCellsTested++;

        fin.getline(buff, maxLine);
        if (fin.eof()) break;

        unsigned long int sNum;
        if (sscanf(buff, "%ld", &sNum) != 1)
          ::report("doTransform(): invalid SEQNUM " + string(buff), DgBase::Fatal);

        seqnums.insert(sNum);
      }

      for(set<unsigned long int>::iterator i=seqnums.begin();i!=seqnums.end();i++){
        dp.nCellsAccepted++;

        DgLocation* loc = static_cast<const DgIDGG&>(dgg).bndRF().locFromSeqNum(*i);
        if (!dgg.bndRF().validLocation(*loc)){
          std::cerr<<"doTransform(): SEQNUM " << (*i)<< " not a valid location"<<std::endl;
          ::report("doTransform(): Invalid SEQNUM found.", DgBase::Warning);
        }

        outputStatus(dp);

        DgPolygon verts(dgg);
        dgg.setVertices(*loc, verts, dp.nDensify);

        outputCellAdd2D(dp, dgg, *loc, verts, deg);

        delete loc;
      }
   }
   else if (dp.wholeEarth)
   {
      dp.nCellsAccepted = 0;
      dp.nCellsTested = 0;
      if (!dp.isSuperfund)
      {
         DgLocation* addLoc = new DgLocation(dgg.bndRF().first());
         while (1)
         {
            dp.nCellsAccepted++;
            dp.nCellsTested++;
            outputStatus(dp);

            DgPolygon verts(dgg);
            dgg.setVertices(*addLoc, verts, dp.nDensify);

            outputCellAdd2D(dp, dgg, *addLoc, verts, deg);

            dgg.bndRF().incrementLocation(*addLoc);
            if (!dgg.bndRF().validLocation(*addLoc)) break;
         }
         delete addLoc;
      }
      else // dp.isSuperfund
      {
         for (int q = 0; q < 12; q++)
         {
            DgHexSF baseTile(0, 0, 0, 0, true, q);
            baseTile.setType('P');
            baseTile.depthFirstTraversal(dp, dgg, deg, 2);
         }
      }
   }
   else // use clip regions
   {
      DgQuadClipRegion clipRegions[12]; // clip regions for each quad
      set<DgIVec2D> overageSet[12];     // overage sets
      map<DgIVec2D, set<DgDBFfield> > overageFields[12]; // associated fields

      createClipRegions(dp, dgg, clipRegions, overageSet, overageFields);

      if (dp.buildShapeFileAttributes)
      {
         if (dp.outCellAttributes)
            dp.cellOutShp->addFields(dp.allFields);

         if (dp.outPointAttributes)
            dp.ptOutShp->addFields(dp.allFields);
      }

      //// now process the cells by quad ////

      const DgContCartRF& cc1 = dgg.ccFrame();
      const DgDiscRF2D& grid = dgg.grid2D();

      cout << "\n";
      for (int q = 0; q < 12; q++)
      {
         if (overageSet[q].empty() && !clipRegions[q].isQuadUsed())
         {
            cout << string("* No intersections in quad ") 
                 << dgg::util::to_string(q) << "." << endl;
            continue;
         }

         cout << string("* Testing quad ") << dgg::util::to_string(q) 
              << "... " << endl;

         if (dp.megaVerbose) 
            cout << "Generating: " << q << " " << clipRegions[q].offset() 
                 << " " << clipRegions[q].upperRight() << endl;

         DgIVec2D lLeft;
         DgIVec2D uRight;

         if (clipRegions[q].isQuadUsed())
         {
            lLeft = clipRegions[q].offset();
            uRight = clipRegions[q].upperRight();
         }

         // assume dp.isSuperfund
         if (dp.isSuperfund)
         {
            DgEvalData ed(dp, dgg, cc1, grid, clipRegions[q], overageSet[q],
                          overageFields[q], deg, lLeft, uRight);

            DgHexSF baseTile(0, 0, 0, 0, true, q);
            baseTile.setType('P');
            baseTile.depthFirstTraversal(dp, dgg, deg, 2, &ed);
         }
         else // !dp.isSuperfund
         {
            DgBoundedRF2D b1(grid, DgIVec2D(0, 0), (uRight - lLeft));
            DgIVec2D tCoord = lLeft; // where are we on the grid?
            while (!overageSet[q].empty() || clipRegions[q].isQuadUsed()) 
            {
               DgIVec2D coord = tCoord;
               bool accepted = false;

               // first check if there are cells on the overage set

               if (!overageSet[q].empty())
               {
                  if (clipRegions[q].isQuadUsed())
                  {
                     set<DgIVec2D>::iterator it = overageSet[q].find(tCoord);
                     if (it != overageSet[q].end()) // found tCoord
                     {
                        accepted = true;
                        overageSet[q].erase(it);
                        if (dp.megaVerbose) cout << "found OVERAGE coord " << coord << endl;
 
                        tCoord -= lLeft;
                        tCoord = b1.incrementAddress(tCoord);
                        if (tCoord == b1.invalidAdd()) 
                           clipRegions[q].setIsQuadUsed(false);

                        tCoord += lLeft;
                     }
                     else 
                     {
                        set<DgIVec2D>::iterator it = overageSet[q].begin();
                        if (*it < tCoord)
                        {
                           accepted = true;
                           coord = *it;
                           overageSet[q].erase(it);
                           if (dp.megaVerbose) cout << "processing OVERAGE " << coord << endl;
                        }
                        else
                        {
                           tCoord -= lLeft;
                           tCoord = b1.incrementAddress(tCoord);
                           if (tCoord == b1.invalidAdd()) 
                              clipRegions[q].setIsQuadUsed(false);

                           tCoord += lLeft;
                        }
                     }
                  }
                  else
                  {
                     set<DgIVec2D>::iterator it = overageSet[q].begin();
                     coord = *it;
                     overageSet[q].erase(it);
                     accepted = true;
                     if (dp.megaVerbose) cout << "processing OVERAGE " << coord << endl;
                  }
               }
               else if (clipRegions[q].isQuadUsed())
               {
                  tCoord -= lLeft;
                  tCoord = b1.incrementAddress(tCoord);
                  if (tCoord == b1.invalidAdd()) 
                     clipRegions[q].setIsQuadUsed(false);

                  tCoord += lLeft;
               }

               // skip subfrequency cells as appropriate if doing classII
               // (this should all be done using the seqNum methods, would be
               // much cleaner)

               if (!dgg.isClassI()) 
	          if ((coord.j() + coord.i()) % 3) continue;

               outputStatus(dp);

               if (!accepted)
                  accepted = evalCell(dp, dgg, cc1, grid, clipRegions[q], coord);

               if (!accepted) continue;

               // if we're here we have a good one

               dp.nCellsAccepted++;
               //cout << "XX " << q << " " << coord << endl;

               DgLocation* addLoc = dgg.makeLocation(DgQ2DICoord(q, coord));
               DgPolygon verts(dgg);
               dgg.setVertices(*addLoc, verts, dp.nDensify);

               outputCellAdd2D(dp, dgg, *addLoc, verts, deg);

               // check for special cases 
               if (q == 0 || q == 11) break; // only one cell
            }
         } // else !dp.isSuperfund

         cout << "...quad " << q << " complete." << endl;
      }

   } // end if wholeEarth else

   // close the output files
   delete dp.cellOut;
   dp.cellOut = NULL;
   delete dp.ptOut;
   dp.ptOut = NULL;

   if (dp.numGrids == 1 || !dp.concatPtOut)
   {
      delete dp.randPtsOut;
      dp.randPtsOut = NULL;
   }

   cout << "\n** grid generation complete **" << endl;
   outputStatus(dp, true);
   if (!dp.wholeEarth && !dp.seqToPoly)
      cout << "acceptance rate is " << 
          100.0 * (long double) dp.nCellsAccepted / (long double) dp.nCellsTested <<
          "%" << endl;

} // void genGrid

////////////////////////////////////////////////////////////////////////////////
void doGridGen (GridGenParam& dp, DgGridPList& plist)
{
   for (dp.curGrid = 1; dp.curGrid <= dp.numGrids; dp.curGrid++)
   {
      // first get the grid placement
      dp.cellOutFileName = dp.cellOutFileNameBase;
      dp.ptOutFileName = dp.ptOutFileNameBase;
      dp.randPtsOutFileName = dp.randPtsOutFileNameBase;
      dp.metaOutFileName = dp.metaOutFileNameBase; 

      if (dp.placeRandom && dp.ptsRand != 0) 
         plist.setParam("randpts_seed", 
           dgg::util::to_string(dp.ptsRand->status()));

      orientGrid(dp, plist);

      if (dp.numGrids > 1) 
      {
         string suffix = string(".") + dgg::util::to_string(dp.curGrid, 4);
         dp.metaOutFileName = dp.metaOutFileName + suffix;
         dp.cellOutFileName = dp.cellOutFileName + suffix;
         dp.ptOutFileName = dp.ptOutFileName + suffix;

         if (!dp.concatPtOut)
            dp.randPtsOutFileName = dp.randPtsOutFileName + suffix;
      }
      
      // open the output files as applicable; most files need to wait for
      // the RFs to be created

      if (dp.numGrids > 1 || dp.placeRandom) 
      {
         ofstream metaOutFile;
         metaOutFile.open(dp.metaOutFileName.c_str());
         metaOutFile.setf(ios::fixed, ios::floatfield);
         metaOutFile.precision(12);
         metaOutFile << plist;
         metaOutFile.close();
      }

      // now do it

      if (dp.curGrid == dp.numGrids) dp.lastGrid = true;

      genGrid(dp);

      cout << endl;
   }

} // void doGridGen
