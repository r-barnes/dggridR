#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

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
// DgHexIDGG.cpp: DgHexIDGG class implementation
//
// Version 7.0 - Kevin Sahr, 11/15/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <climits>
#include <cfloat>

#include "DgHexIDGG.h"
#include "DgHexIDGGS.h"
#include "DgHexGrid2DS.h"
#include "DgSeriesConverter.h"
#include "DgRadixString.h"
#include "DgBoundedIDGG.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgHexIDGG::DgHexIDGG (const DgHexIDGGS& dggs, unsigned int aperture,
              int res, const string& name, unsigned int precision)
   : DgIDGG (&dggs, aperture, res, name, Hexagon, D6, precision),
	   scaleFac_ (1.0L), rotRads_ (0.0L)
{ 
   initialize();

} // DgHexIDGG::DgHexIDGG

////////////////////////////////////////////////////////////////////////////////
DgHexIDGG::DgHexIDGG (const DgHexIDGG& rfIn)
   : DgIDGG (rfIn.dggs(), rfIn.aperture(), 
                 rfIn.res(), rfIn.name(), rfIn.gridTopo(), rfIn.gridMetric(),
                 rfIn.precision()),
	scaleFac_ (rfIn.scaleFac()), rotRads_ (rfIn.rotRads())
{ 
   initialize();

} // DgHexIDGG::DgHexIDGG

////////////////////////////////////////////////////////////////////////////////
DgHexIDGG::~DgHexIDGG (void) { }

////////////////////////////////////////////////////////////////////////////////
const DgHexIDGGS& 
DgHexIDGG::hexDggs (void) const 
{ return *(static_cast<const DgHexIDGGS*>(dggs())); }

////////////////////////////////////////////////////////////////////////////////
void
DgHexIDGG::initialize (void)
{
   // verify parameter validity

   string apErrStr = string("DgHexIDGG::initialize(): invalid aperture " + 
             dgg::util::to_string(aperture()) + string(" for grid topo ") + 
             to_string(gridTopo()));

   if (gridTopo() != Hexagon)
   {
      report("DgHexIDGG::initialize(): invalid grid topo " + 
             to_string(gridTopo()), DgBase::Fatal);

      if (aperture() != 3 && aperture() != 4 && aperture() != 7) 
         report(apErrStr, DgBase::Fatal);
   }

   // create some internal data structures
   setUndefLoc(makeLocation(undefAddress())); 
   sphIcosa_ = new DgSphIcosa(vert0(), azDegs());

   radix_ = (int) sqrt((long double) aperture());

   isAligned_ = true;
   isCongruent_ = false;

   // initialize parent values as if this is grid res 0
   int parentIsClassI = 1;
   int parentIsClassIII = 0;
   long double parentScaleFac = 1.0;
   unsigned long long int parentNCells = 1;

   // get actual parent values if there is a parent grid
   if (res() > 0) 
   {
      const DgHexIDGG& parentIDGG = hexDggs().hexIdgg(res() - 1);

      parentIsClassI = parentIDGG.isClassI();
      parentIsClassIII = parentIDGG.isClassIII();
      parentScaleFac = parentIDGG.scaleFac();
      parentNCells = parentIDGG.gridStats().nCells();
   }

   // set our values

   if (aperture() == 3)
   {
      isClassI_ = !parentIsClassI;
      isClassIII_ = parentIsClassIII;
   }
   else if (aperture() == 4)
   {
      isClassI_ = parentIsClassI;
      isClassIII_ = parentIsClassIII;
   }
   else if (aperture() == 7)
   {
      isClassI_ = parentIsClassI;
      isClassIII_ = !parentIsClassIII;
   }

   allocRes_ = res();
   if (!isClassI() || isClassIII()) 
      ++allocRes_;

   // set-up local network to scale so that quad (and consequently tri) edge 
   // length is 1.0
   ccFrame_ = DgContCartRF::makeRF(locNet_, name() + "CC1");
   grid2DS_ = DgHexGrid2DS::makeRF(locNet_, ccFrame(), allocRes() + 1, hexDggs().apSeq(),
        name() + string("H2DS"));
   //cout << "== NEW GRID2DS:" << endl;
   //cout << *grid2DS_;

   if (res() == 0)
      maxD_ = 0;
   else
   {
      double factor = parentScaleFac;
      if (aperture() == 3)
         factor *= M_SQRT3;
      else if (aperture() == 4)
         factor *= 2.0L;
      else if (aperture() == 7)
         factor *= M_SQRT7;

      scaleFac_ = factor;
      maxD_ = 1.0L;

      // find maxD, which may be for a Class I substrate grid
      //long long int maxD2 = 0;
      if (!isClassI())
         factor *= M_SQRT3;

      if (isClassIII())
         factor *= M_SQRT7;

      maxD_ = factor - 1.0;

      //cout << res() << " " << aperture();
      //cout << " f: " << factor << " maxD: " << maxD_ << endl;
   }

   maxI_ = maxD();
   maxJ_ = maxD();
   mag_ = maxD() + 1;
   firstAdd_ = DgQ2DICoord(0, DgIVec2D(0, 0));
   lastAdd_ = DgQ2DICoord(11, DgIVec2D(0, 0));

   if (res() == 0)
      gridStats_.setNCells(12);
   else
      gridStats_.setNCells((parentNCells - 2) * aperture() + 2);

   createConverters();

   ///// calculate the statistics /////

   gridStats_.setPrecision(precision());
   //gridStats_.setNCells(bndRF().size());

   long double tmpLen = DgGeoSphRF::icosaEdgeKM();
////// NEEDS UPDATING
   gridStats_.setCellDistKM(tmpLen / pow(sqrt((long double) aperture()), res()));
/*
   else // mixed43
   {
      if (res() < numAp4())
      {
         tmpLen = tmpLen / pow((long double) 2.0, (long double) res());
      }
      else
      {
         tmpLen = tmpLen / pow((long double) 2.0, (long double) numAp4());
         if (res() > numAp4())
            tmpLen = tmpLen / pow(M_SQRT3, (long double) (res() - numAp4()));
      }
      gridStats_.setCellDistKM(tmpLen);
   }
*/

      // a = globeArea / ((#cells - 12) + (12 * 5/6))
      //   = globeArea / (#cells - 2);
      gridStats_.setCellAreaKM(DgGeoSphRF::totalAreaKM() / 
                       (gridStats_.nCells() - 2));

   gridStats_.setCLS(2.0L * 2.0L * DgGeoSphRF::earthRadiusKM() * 
                     asinl(sqrt(gridStats_.cellAreaKM() / M_PI) / 
                     (2.0L * DgGeoSphRF::earthRadiusKM())));

} // DgHexIDGG::initialize

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
