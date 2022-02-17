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
// DgTriIDGG.cpp: DgTriIDGG class implementation
//
// Kevin Sahr, 8/12/20
//
////////////////////////////////////////////////////////////////////////////////

#include "DgBoundedIDGG.h"
#include "DgIDGGS4T.h"
#include "DgRadixString.h"
#include "DgSeriesConverter.h"
#include "DgTriGrid2DS.h"
#include "DgTriIDGG.h"

#include <cfloat>
#include <climits>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgTriIDGG::DgTriIDGG (const DgIDGGS4T& dggs, unsigned int aperture,
              int res, const string& name, unsigned int precision)
   : DgIDGGBase (&dggs, dggs.geoRF(), aperture, res, name, Triangle, D3, precision),
	   scaleFac_ (1.0L)
{
   initialize();

} // DgTriIDGG::DgTriIDGG

////////////////////////////////////////////////////////////////////////////////
DgTriIDGG::DgTriIDGG (const DgTriIDGG& rfIn)
   : DgIDGGBase (rfIn.dggs(), rfIn.geoRF(), rfIn.aperture(),
                 rfIn.res(), rfIn.name(), rfIn.gridTopo(), rfIn.gridMetric(),
                 rfIn.precision()),
	scaleFac_ (rfIn.scaleFac())
{
   initialize();

} // DgTriIDGG::DgTriIDGG

////////////////////////////////////////////////////////////////////////////////
DgTriIDGG::~DgTriIDGG (void) { }

////////////////////////////////////////////////////////////////////////////////
const DgIDGGS4T&
DgTriIDGG::triDggs (void) const
{ return *(static_cast<const DgIDGGS4T*>(dggs())); }

////////////////////////////////////////////////////////////////////////////////
void
DgTriIDGG::initialize (void)
{
   // verify parameter validity

   string apErrStr = string("DgTriIDGG::initialize(): invalid aperture " +
                         dgg::util::to_string(aperture()) +
                         string(" for grid topo ") + to_string(gridTopo()));

   if (gridTopo() != Triangle) {
      report("DgTriIDGG::initialize(): invalid grid topo " + to_string(gridTopo()),
             DgBase::Fatal);

      if (aperture() != 4) report(apErrStr, DgBase::Fatal);
   }

   // create some internal data structures
   setUndefLoc(makeLocation(undefAddress()));
   sphIcosa_ = new DgSphIcosa(vert0(), azDegs());

   isAligned_ = true;
   isCongruent_ = false;

   // initialize parent values as if this is grid res 0
   long double parentScaleFac = 1.0;
   unsigned long long int parentNCells = 1;

   // get actual parent values if there is a parent grid
   if (res() > 0) {
      const DgTriIDGG& parentIDGG = triDggs().triIdgg(res() - 1);

      parentScaleFac = parentIDGG.scaleFac();
      parentNCells = parentIDGG.gridStats().nCells();
   }

   // set-up local network to scale so that quad (and consequently tri) edge
   // length is 1.0
   ccFrame_ = DgContCartRF::makeRF(locNet_, name() + "CC1");
   grid2DS_ = DgTriGrid2DS::makeRF(locNet_, ccFrame(), res() + 1, 4, true, false, name() + string("H2DS"));
   //cout << "== NEW GRID2DS:" << endl;
   //cout << *grid2DS_;

   if (res() == 0)
      maxD_ = 0;
   else {
      double factor = parentScaleFac * 2.0L; // aperture 4

      scaleFac_ = factor;
      maxD_ = factor - 1.0L;

      //cout << res() << " " << aperture();
      //cout << " f: " << factor << " maxD: " << maxD_ << endl;
   }

   maxI_ = maxD();
   mag_ = maxD() + 1;
   maxJ_ = (mag() * 2) - 1;
   firstAdd_ = DgQ2DICoord(1, DgIVec2D(0, 0));
   lastAdd_ = DgQ2DICoord(10, DgIVec2D(maxI(), maxJ()));

   if (res() == 0)
      gridStats_.setNCells(20);
   else
      gridStats_.setNCells(parentNCells * 4);

   createConverters();

   ///// calculate the statistics /////

   gridStats_.setPrecision(precision());

   long double tmpLen = DgGeoSphRF::icosaEdgeKM();
////// NEEDS UPDATING
   gridStats_.setCellDistKM(tmpLen / pow(sqrt((long double) aperture()), res()));

   // a = globeArea / #cells
   gridStats_.setCellAreaKM(DgGeoSphRF::totalAreaKM() / gridStats_.nCells());

   gridStats_.setCLS(2.0L * 2.0L * DgGeoSphRF::earthRadiusKM() *
                     asinl(sqrt(gridStats_.cellAreaKM() / M_PI) /
                     (2.0L * DgGeoSphRF::earthRadiusKM())));

} // DgTriIDGG::initialize

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
