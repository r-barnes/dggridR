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
#include "DgIDGGS7H.h"
#include "DgIDGGS43H.h"
#include "DgIDGGS4D.h"
#include "DgIDGGS4T.h"
#include "DgSuperfund.h"

////////////////////////////////////////////////////////////////////////////////
const DgIDGGS*
DgIDGGS::makeRF (DgRFNetwork& network, const DgGeoSphRF& backFrame,
          const DgGeoCoord& vert0, long double azDegs, unsigned int apertureIn,
          int nRes, DgGridTopology gridTopo, DgGridMetric gridMetric,
          const std::string& name, const std::string& projTypeIn, bool isApSeqIn,
          const DgApSeq& apSeqIn, bool isMixed43In, int numAp4In, bool isSuperfundIn,
          DgHierNdxSysType hierNdxSysType)
{
   if (isApSeqIn)
      report("DgIDGGS::makeRF(): isApSeq must be false", DgBase::Fatal);

   const DgIDGGS* dg0 = 0;

   std::string apErrStr = std::string("DgIDGGS::makeRF(): invalid aperture " +
                         dgg::util::to_string(apertureIn) +
                         std::string(" for grid topo ") + to_string(gridTopo));

   std::string theName = name;
   bool defaultName = (theName == std::string("IDGGS"));

   if (gridTopo == Hexagon) {
      if (defaultName) {
         if (!isMixed43In) {
            if (apertureIn == 4)
               theName = projTypeIn + std::string("4H");
            else if (apertureIn == 3)
               theName = projTypeIn + std::string("3H");
            else if (apertureIn == 7)
               theName = projTypeIn + std::string("7H");
            else
               report(apErrStr, DgBase::Fatal);
         } else
            theName = projTypeIn + std::string("43H");
      }

      dg0 = DgHexIDGGS::makeRF(network, backFrame, vert0, azDegs, apertureIn, nRes,
              theName, projTypeIn, isApSeqIn, apSeqIn, isMixed43In, numAp4In, isSuperfundIn, hierNdxSysType);
   } else if (gridTopo == Diamond) {

       if (apertureIn != 4)
            report(apErrStr, DgBase::Fatal);

      // assume D4
      std::string nameSuffix = std::string("4D4");
      if (gridMetric == D8)
         nameSuffix = std::string("4D8");

//cout << " makeRF isD4: " << ((isD4) ? "true" : "false") << std::endl;

      if (defaultName) theName = projTypeIn + nameSuffix;
      dg0 = DgIDGGS4D::makeRF(network, backFrame, vert0, azDegs, nRes,
                       theName, projTypeIn, gridMetric);
   } else if (gridTopo == Triangle) {
      if (apertureIn == 4) {
         if (defaultName) theName = projTypeIn + std::string("4T");
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
                  const std::string& name, const std::string& projType,
                  bool isApSeq, const DgApSeq& apSeq,
                  bool isMixed43, int numAp4, bool isSuperfund,
                  DgHierNdxSysType hierNdxSysType)
/*
const DgApSeq& apSeq = DgApSeq::defaultApSeq, bool isApSeq = false,
bool isMixed43 = false, int numAp4 = 0, bool isSuperfund = false,
const std::string& hierNdxSysType = "NONE");
 */
        : DgIDGGSBase (network, backFrame, vert0, azDegs, hierNdxSysType,
                       nRes, gridTopo, gridMetric,
                       aperture, name, projType, !(isMixed43 || isApSeq)),
          numAp4_ (numAp4), isSuperfund_ (isSuperfund), isApSeq_ (isApSeq),
          apSeq_ (apSeq)
{

   setUndefLoc(makeLocation(undefAddress()));

} // DgIDGGS::DgIDGGS

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
