#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGBase.cpp: DgIDGGBase class implementation
//
// Version 7.0 - Kevin Sahr, 11/15/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cfloat>
#include <climits>
#include <cmath>
#include <string.h>

#include "DgBoundedIDGG.h"
#include "DgDmdD4Grid2DS.h"
#include "DgHexGrid2DS.h"
#include "DgIDGGBase.h"
#include "DgIDGGSBase.h"
#include "DgHexIDGGS.h"
#include "DgProjFuller.h"
#include "DgProjISEA.h"
#include "DgRadixString.h"
#include "DgSeriesConverter.h"
#include "DgTriGrid2DS.h"
#include "DgHierNdxSystemRFSBase.h"

////////////////////////////////////////////////////////////////////////////////
const DgGeoSphRF&  DgIDGGBase::geoRF      (void) const { return dggs()->geoRF(); }
const DgGeoCoord&  DgIDGGBase::vert0      (void) const { return dggs()->vert0(); }
long double        DgIDGGBase::azDegs     (void) const { return dggs()->azDegs(); }
const std::string& DgIDGGBase::projType   (void) const { return dggs()->projType(); }
DgGridTopology     DgIDGGBase::gridTopo   (void) const { return dggs()->gridTopo(); }
DgGridMetric       DgIDGGBase::gridMetric (void) const { return dggs()->gridMetric(); }

////////////////////////////////////////////////////////////////////////////////
const DgQuadEdgeCells DgIDGGBase::edgeTable_[12] = {

   DgQuadEdgeCells(0,  true,  0,  0, 0, 0, 0), // quad 0 should never occur
   DgQuadEdgeCells(1,  true,  0,  2, 10, 6, 5),
   DgQuadEdgeCells(2,  true,  0,  3, 6, 7, 1),
   DgQuadEdgeCells(3,  true,  0,  4, 7, 8, 2),
   DgQuadEdgeCells(4,  true,  0,  5, 8, 9, 3),
   DgQuadEdgeCells(5,  true,  0,  1, 9, 10, 4),
   DgQuadEdgeCells(6,  false, 11, 2, 10, 7, 1),
   DgQuadEdgeCells(7,  false, 11, 3, 6, 8, 2),
   DgQuadEdgeCells(8,  false, 11, 4, 7, 9, 3),
   DgQuadEdgeCells(9,  false, 11, 5, 8, 10, 4),
   DgQuadEdgeCells(10, false, 11, 1, 9, 6, 5),
   DgQuadEdgeCells(11, false, 11, 0, 0, 0, 0)  // quad 11 should never occur

};

////////////////////////////////////////////////////////////////////////////////
const char*
DgIDGGBase::str2add (DgQ2DICoord* add, const char* str, char delimiter) const
{
   if (!str || strlen(str) == 0)
      ::report("DgQ2DIRF::fromString() invalid input ", DgBase::Fatal);

   if (!add) add = new DgQ2DICoord();

   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   // get the quadNum

   char* tok = strtok(tmpStr, delimStr);
   if (!tok || strlen(tok) == 0)
      ::report("DgQ2DIRF::fromString() invalid input ", DgBase::Fatal);

   int q = -1;
   if (!tok || sscanf(tok, "%d", &q) != 1) {
      ::report("DgQ2DIRF::fromString() invalid value in string " +
               ((tok) ? std::string(tok) : std::string("\"\"")), DgBase::Fatal);
   }

   const char* tmp = &(str[strlen(tok) + 1]);
   if (!tmp || strlen(tmp) == 0)
      ::report("DgQ2DIRF::fromString() invalid input ", DgBase::Fatal);

   DgIVec2D vec;
   tmp = vec.fromString(tmp, delimiter);

   *add = DgQ2DICoord(q, vec);

   return tmp;

} // const char* DgIDGGBase::str2add

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIDGGBase::DgIDGGBase (const DgIDGGSBase* dggs, const DgGeoSphRF& geoRF,
             unsigned int aperture, int res, const std::string& name,
             DgGridTopology gridTopo, DgGridMetric gridMetric,
             unsigned int precision)
   : DgDiscTopoRF<DgQ2DICoord, DgGeoCoord, long double>
          (geoRF.network(), geoRF, name, gridTopo, gridMetric),
     dggs_ (dggs), sphIcosa_(0), aperture_(aperture), res_(res),
     precision_(precision), grid2D_(0), grid2DS_(0), ccFrame_(0),
     projTriRF_(0), vertexRF_(0), q2ddRF_(0), bndRF_(0), planeRF_(0)
{
   //initialize();

} // DgIDGGBase::DgIDGGBase

////////////////////////////////////////////////////////////////////////////////
/*
DgIDGGBase::DgIDGGBase (const DgIDGGBase& rfIn)
   : DgDiscRF<DgQ2DICoord, DgGeoCoord, long double> (rfIn),
        dggs_ (NULL), sphIcosa_(0), aperture_(rfIn.aperture()),
        res_(rfIn.res()), precision_(rfIn.precision()),
        grid2D_(0), grid2DS_(0), ccFrame_(0), projTriRF_(0),
        vertexRF_(0), q2ddRF_(0), bndRF_(0), planeRF_(0),
        zorderRF_ (0), zorderStrRF_ (0), z3RF_ (0), z3StrRF_ (0), z7RF_ (0), z7StrRF_ (0)
{
   //initialize();

} // DgIDGGBase::DgIDGGBase
*/

////////////////////////////////////////////////////////////////////////////////
DgIDGGBase::~DgIDGGBase()
{
 delete sphIcosa_;
 delete bndRF_;
}

////////////////////////////////////////////////////////////////////////////////
void
DgIDGGBase::createConverters (void)
{
   grid2D_ = dynamic_cast<const DgDiscRF2D*>(grid2DS().grids()[res()]);
   //cout << "== GRID2D: " << std::string(*grid2D_);

   bndRF_ = new DgBoundedIDGG(*this);
   //cout << "== BNDRF:: " << std::string(*bndRF_) << std::endl;

   // create the intermediate RFs

   projTriRF_ = DgProjTriRF::makeRF(network(), name() + std::string("projTri"),
                sphIcosa_);
   vertexRF_ = DgVertex2DDRF::makeRF(network(), name() + std::string("vertex"));
   q2ddRF_ = DgQ2DDRF::makeRF(network(), name() + std::string("q2dd"));
   planeRF_ = DgPlaneTriRF::makeRF(network(), name() + std::string("plane"));

   // create the converters; for convenience use where they are in overall
   // sequence for name

   DgIcosaProj* icosaProj = NULL;
   if (projType() == "ISEA")
      icosaProj = new DgProjISEA(geoRF(), projTriRF());
   else if (projType() == "FULLER")
      icosaProj = new DgProjFuller(geoRF(), projTriRF());
   else
      report("DgIDGGBase::initialize(): invalid projection type " + projType(),
             DgBase::Fatal);

   const DgConverterBase* c1to2 = &(icosaProj->forward());
   const DgConverterBase* c2to3 = new DgProjTriToVertex2DD(projTriRF(), vertexRF());
   const DgConverterBase* c3to4 = new DgVertex2DDToQ2DDConverter(vertexRF(), q2ddRF());
   const DgConverterBase* c4to5 = new DgQ2DDtoIConverter(q2ddRF(), *this);

   const DgConverterBase* c5to4 = new DgQ2DItoDConverter(*this, q2ddRF());
   const DgConverterBase* c4to3 = new DgQ2DDtoVertex2DDConverter(q2ddRF(), vertexRF());
   const DgConverterBase* c3to2 = new DgVertex2DDtoProjTri(vertexRF(), projTriRF());
   const DgConverterBase* c2to1 = &(icosaProj->inverse());

   // done with icosaProj; the fwd/inv converters are in the RFNetwork
   delete icosaProj;

   DgConverterBase* toPlane = new DgPlaneTriProj(projTriRF(), planeRF());

   // create the series converters that will replace the default DgDiscRF
   // converters

   std::vector<const DgConverterBase*> sc;
   sc.push_back(c1to2);
   sc.push_back(c2to3);
   sc.push_back(c3to4);
   sc.push_back(c4to5);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   sc.push_back(c5to4);
   sc.push_back(c4to3);
   sc.push_back(c3to2);
   sc.push_back(c2to1);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   //// now fill in the other connections; begin with from vertexRF and then
   //// connect to it as needed

   // vertexRF -> geoRF
   sc.push_back(c3to2);
   sc.push_back(c2to1);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // vertexRF -> Q2DI
   sc.push_back(c3to4);
   sc.push_back(c4to5);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // vertexRF -> projTriRF is c3to2 above

   // vertexRF -> planeRF
   sc.push_back(c3to2);
   sc.push_back(toPlane);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // vertexRF -> Q2DD is c3to4 above

   /// now do from projTriRF

   // projTriRF -> geoRF is c2to1 above

   // projTriRF -> Q2DI
   sc.push_back(c2to3);
   sc.push_back(network().getConverter(vertexRF(), *this));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // projTriRF -> vertexRF is c2to3 above
   // projTriRF -> planeRF is toPlane above

   // projTriRF -> Q2DD
   sc.push_back(c2to3);
   sc.push_back(c3to4);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   /// do from Q2DD

   // Q2DD -> geoRF
   sc.push_back(c4to3);
   sc.push_back(network().getConverter(vertexRF(), geoRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DD -> Q2DI is c4to5 above
   // Q2DD -> vertexRF is c4to3 above

   // Q2DD -> projTriRF
   sc.push_back(c4to3);
   sc.push_back(network().getConverter(vertexRF(), projTriRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DD -> planeRF
   sc.push_back(c4to3);
   sc.push_back(network().getConverter(vertexRF(), planeRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   /// do from Q2DI

   // Q2DI -> geoRF is series converter given above

   // Q2DI -> vertexRF
   sc.push_back(c5to4);
   sc.push_back(c4to3);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DI -> projTriRF
   sc.push_back(c5to4);
   sc.push_back(network().getConverter(q2ddRF(), projTriRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DI -> planeRF
   sc.push_back(c5to4);
   sc.push_back(network().getConverter(q2ddRF(), planeRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DI -> Q2DD is c5to4 above

   /// finally from geoRF

   // geoRF -> Q2DI is series converter given above

   // geoRF -> vertexRF
   sc.push_back(c1to2);
   sc.push_back(c2to3);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // geoRF -> projTriRF is c1to2 above

   // geoRF -> planeRF
   sc.push_back(network().getConverter(geoRF(), vertexRF()));
   sc.push_back(network().getConverter(vertexRF(), planeRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // geoRF -> Q2DD
   sc.push_back(c1to2);
   sc.push_back(c2to3);
   sc.push_back(c3to4);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

} // DgIDGGBase::createConverters

////////////////////////////////////////////////////////////////////////////////
void
DgIDGGBase::setVertices (const DgLocation& loc, DgPolygon& vec,
                     int densify) const
{
   vec.clearAddress();
   backFrame().convert(vec);

   DgLocation tLoc(loc);
//cout << "*** " << loc << std::endl;
   convert(&tLoc);
//cout << "**** " << tLoc << std::endl;

   setAddVertices(*getAddress(tLoc), vec, densify);

} // void DgDiscRF::setVertices

////////////////////////////////////////////////////////////////////////////////
void
DgIDGGBase::setAddVertices (const DgQ2DICoord& add, DgPolygon& vec,
                        int densify) const
{
   DgLocation* tmpLoc = grid2D().makeLocation(add.coord());
//cout << "a: " << *tmpLoc << std::endl;
    DgPolygon dummy(ccFrame());
    vec = dummy;  // force empty RF to allow for network change
    grid2D().setVertices(*tmpLoc, vec);
   delete tmpLoc;

//cout << "A: " << vec << std::endl;
   ccFrame().convert(vec);
//cout << "B: " << vec << std::endl;

   // densify
   vec.densify(densify);
//cout << "C: " << vec << std::endl;

   // kludge to jump nets and add the quad number

   DgPolygon tmpVec(q2ddRF());
   std::vector<DgAddressBase*>& v = tmpVec.addressVec();
   for (int i = 0; i < vec.size(); i++)
   {
      v.push_back(new DgAddress<DgQ2DDCoord>(DgQ2DDCoord(add.quadNum(),
                       *(ccFrame().getAddress(vec[i])))));
   }
   vec = tmpVec;

//cout << "D: " << vec << std::endl;

   vertexRF().convert(vec);

//cout << "E: " << vec << std::endl;

   if (!isCongruent() && add.coord() == DgIVec2D(0, 0))
   {
      // we need to explicitly go to vertexRF to look for non-keepers
      // to clip
      std::vector<DgAddressBase*>& v = vec.addressVec();
      std::vector<DgAddressBase*> newV;
      for (unsigned long i = 0; i < v.size(); i++) {
         DgAddress<DgVertex2DDCoord>* fullAdd =
                   dynamic_cast< DgAddress<DgVertex2DDCoord>* >(v[i]);
         DgVertex2DDCoord& add = fullAdd->address();
         if (add.keep()) {
            newV.push_back(v[i]);
         } else {
            delete v[i];
         }
      }

      v.resize(0);
      for (unsigned long i = 0; i < newV.size(); i++) {
         v.push_back(newV[i]);
      }
   }

   // now convert to the geoRF

//cout << "F: " << vec << std::endl;
   geoRF().convert(&vec);
//cout << "G: " << vec << std::endl;

   // Release the Kraken... I mean, the vector's pointers:
   dgg::util::release(v);

} // DgIDGGBase::setAddVertices

////////////////////////////////////////////////////////////////////////////////
void
DgIDGGBase::setAddNeighbors (const DgQ2DICoord& add,
                                   DgLocVector& vec) const
{
   DgLocVector ngh2d(grid2D());
   grid2D().setAddNeighbors(add.coord(), ngh2d);
//cout << " >> DgIDGGBase::setAddNeighbors center: " << add << std::endl;
//cout << "  ngh2d: " << ngh2d << std::endl;
//cout << " isCongruent: " << (isCongruent() ? "yes" : "no");

   int q = add.quadNum();
   DgLocVector ngh2dNoDup(*this);
   vec.clearAddress();
   DgIVec2D c;
   for (int i = 0; i < ngh2d.size(); i++)
   {
      DgQ2DICoord c2di(q, *grid2D().getAddress(ngh2d[i]));
//cout << "*** i: " << i << " " << c2di;
      c2di = bndRF().q2dixToQ2di(c2di);
//cout << " -> " << c2di << std::endl;

      // check for duplicates
      bool keeper = true;
      if (!isCongruent() && add.coord() == DgIVec2D(0, 0))
      {
         for (int i = 0; i < ngh2dNoDup.size(); i++)
         {
            const DgQ2DICoord& veci = *this->getAddress(ngh2dNoDup[i]);
//cout << "   " << i << " " << veci << " -> " << (c2di == veci) <<  std::endl;
            if (c2di == veci)
            {
               keeper = false;
               break;
            }
         }
      }

      if (keeper)
      {
         DgLocation* tmpLoc = this->makeLocation(c2di);
         ngh2dNoDup.push_back(*tmpLoc);
         delete tmpLoc;
      }
   }

//cout << "ngh2dNoDup: " << ngh2dNoDup << std::endl;
   // now build the vector; the push_back will take care of converting
   for (int i = 0; i < ngh2dNoDup.size(); i++)
      vec.push_back(ngh2dNoDup[i]);

//cout << "final neigh vec for add: " << add << std::endl;
//cout << vec << std::endl;
//cout << "-------" << std::endl;

}


////////////////////////////////////////////////////////////////////////////////
void
DgIDGGBase::setAddNeighborsBdry2 (const DgQ2DICoord& add,
                                   DgLocVector& vec) const
{
   DgLocVector ngh2d(grid2D());
   grid2D().setAddNeighborsBdry2(add.coord(), ngh2d);
//cout << " >> DgIDGGBase::setAddNeighborsBdry2:  ngh2d: " << std::endl;
//cout << ngh2d << std::endl;

   int q = add.quadNum();
   DgLocVector ngh2dNoDup(*this);
   vec.clearAddress();
   DgIVec2D c;
   for (int i = 0; i < ngh2d.size(); i++)
   {
      DgQ2DICoord c2di(q, *grid2D().getAddress(ngh2d[i]));
//cout << "*** i: " << i << " " << c2di;
      c2di = bndRF().q2dixToQ2di(c2di);
//cout << " -> " << c2di << std::endl;

      // check for duplicates
      bool keeper = true;
      if (!isCongruent() && add.coord() == DgIVec2D(0, 0))
      {
         for (int i = 0; i < ngh2dNoDup.size(); i++)
         {
            const DgQ2DICoord& veci = *this->getAddress(ngh2dNoDup[i]);
//cout << "   " << i << " " << veci << " -> " << (c2di == veci) <<  std::endl;
            if (c2di == veci)
            {
               keeper = false;
               break;
            }
         }
      }

      if (keeper)
      {
         DgLocation* tmpLoc = this->makeLocation(c2di);
         ngh2dNoDup.push_back(*tmpLoc);
         delete tmpLoc;
      }
   }

//cout << "ngh2dNoDup: " << ngh2dNoDup << std::endl;
   // now build the vector; the push_back will take care of converting
   for (int i = 0; i < ngh2dNoDup.size(); i++)
      vec.push_back(ngh2dNoDup[i]);

//cout << "vec: " << vec << std::endl;

} // DgIDGGBase::setAddNeighborsBdry2

////////////////////////////////////////////////////////////////////////////////
/*
const DgHierNdxSystemRFSBase*
DgIDGGBase::zXSystemBase (void) const
{ return static_cast<const DgHierNdxSystemRFSBase*>(zXSystem_); }
 */

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
