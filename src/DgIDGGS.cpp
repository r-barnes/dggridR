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
// DgIDGGS.cpp: DgIDGGS class implementation
//
// Version 7.0 - Kevin Sahr, 11/16/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgIDGGS.h"
#include "DgIDGGS3H.h"
#include "DgIDGGS43H.h"
#include "DgIDGGS4H.h"
#include "DgIDGGS4D.h"
#include "DgIDGGS4T.h"
#include "DgSuperfund.h"

////////////////////////////////////////////////////////////////////////////////
const DgIDGGS*
DgIDGGS::makeRF (DgRFNetwork& network, const DgGeoSphRF& backFrame,
          const DgGeoCoord& vert0, long double azDegs, unsigned int apertureIn, 
          int nRes, DgGridTopology gridTopo, DgGridMetric gridMetric, 
          const string& name, const string& projTypeIn, bool isMixed43In, 
          int numAp4In, bool isSuperfundIn, bool isApSeqIn, const DgApSeq& apSeqIn)
{
   if (isApSeqIn)
      report("DgIDGGS::makeRF(): isApSeq must be false", DgBase::Fatal);

   const DgIDGGS* dg0 = 0;

   string apErrStr = string("DgIDGGS::makeRF(): invalid aperture " +
                         dgg::util::to_string(apertureIn) +
                         string(" for grid topo ") + to_string(gridTopo));

   string theName = name;
   bool defaultName = (theName == string("IDGGS"));

   if (gridTopo == Hexagon) {
      if (defaultName) {
         if (!isMixed43In) {
            if (apertureIn == 4) 
               theName = projTypeIn + string("4H");
            else if (apertureIn == 3) 
               theName = projTypeIn + string("3H");
            else 
               report(apErrStr, DgBase::Fatal);
         } else
            theName = projTypeIn + string("43H");
      }

      dg0 = DgHexIDGGS::makeRF(network, backFrame, vert0, azDegs, apertureIn, nRes,
              theName, projTypeIn, apSeqIn, isApSeqIn, isMixed43In, numAp4In, isSuperfundIn);
   } else if (gridTopo == Diamond) {

      if (apertureIn != 4)
            report(apErrStr, DgBase::Fatal);

      // assume D4
      string nameSuffix = string("4D4");
      if (gridMetric == D8)
         nameSuffix = string("4D8");

//cout << " makeRF isD4: " << ((isD4) ? "true" : "false") << endl;

      if (defaultName) theName = projTypeIn + nameSuffix;
      dg0 = DgIDGGS4D::makeRF(network, backFrame, vert0, azDegs, nRes, 
                       theName, projTypeIn, gridMetric);
   } else if (gridTopo == Triangle) {
      if (apertureIn == 4) {
         if (defaultName) theName = projTypeIn + string("4T");
         dg0 = DgIDGGS4T::makeRF(network, backFrame, vert0, azDegs, nRes, 
                       theName, projTypeIn);
      } else
            report(apErrStr, DgBase::Fatal);
   } else {
      report("DgIDGGS::makeRF() invalid or unimplemented grid topology: " +
         to_string(gridTopo), DgBase::Fatal);
   }

   return dg0;

} // const DgIDGG& makeRF

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIDGGS::DgIDGGS (DgRFNetwork& network, const DgGeoSphRF& backFrame,
                  const DgGeoCoord& vert0, long double azDegs, 
                  unsigned int aperture, int nRes, DgGridTopology gridTopo,
                  DgGridMetric gridMetric,
                  const string& name, const string& projType, bool isMixed43, 
                  int numAp4, bool isSuperfund, bool isApSeq,
                  const DgApSeq& apSeq)
        : DgIDGGSBase (network, backFrame, vert0, azDegs, nRes, aperture, name, 
                       gridTopo, gridMetric, projType, !(isMixed43 || isApSeq)),
          numAp4_ (numAp4), isSuperfund_ (isSuperfund), isApSeq_ (isApSeq),
          apSeq_ (apSeq)
{
   
   setUndefLoc(makeLocation(undefAddress()));

/* all of this has been moved to topology-specfic files
   // create the DGGs for non-hex DGGS; the grids for hex DGGS are created
   // in DgHexIDGGS.cpp

   if (gridTopo == Hexagon) {
         isAligned_ = true;
         isCongruent_ = false;
   } else {
      for (int i = 0; i < nRes; i++) {
         if (!isSuperfund)
            (*grids_)[i] = DgIDGG::makeRF(this, backFrame, vert0, azDegs, aperture, i,
               "DDG", gridTopo, gridMetric, projType, isMixed43, numAp4, isSuperfund);
         else
            (*grids_)[i] = DgIDGG::makeRF(this, backFrame, vert0, azDegs, aperture, i,
               "DDG", gridTopo, gridMetric, projType, isMixed43, numAp4, isSuperfund,
               actualRes2sfRes(i));

         Dg2WayResAddConverter<DgQ2DICoord, DgGeoCoord, long double>(*this, *(grids()[i]), i);

         isAligned_ = idgg(0).isAligned();
         isCongruent_ = idgg(0).isCongruent();
      }
   }
*/

} // DgIDGGS::DgIDGGS

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
