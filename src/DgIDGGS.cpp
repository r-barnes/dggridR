////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS.cpp: DgIDGGS class implementation
//
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
          int nRes, const string& gridTopo, const string& name,
          const string& projTypeIn, bool isMixed43In, int numAp4In, 
          bool isSuperfundIn)
{
   DgIDGGS* dg0 = 0;

   string apErrStr = string("DgIDGGS::makeRF(): invalid aperture " +
                         dgg::util::to_string(apertureIn) +
                         string(" for grid topo ") + gridTopo);

   string theName = name;
   bool defaultName = (theName == string("IDGGS"));

   if (gridTopo == "HEXAGON")
   {
      if (!isMixed43In)
      {
         if (apertureIn == 4)
         {
            if (defaultName) theName = projTypeIn + string("4H");
            dg0 = new DgIDGGS4H(network, backFrame, vert0, azDegs, nRes, 
                          theName, projTypeIn);
         }
         else if (apertureIn == 3)
         {
            if (defaultName) theName = projTypeIn + string("3H");
            dg0 = new DgIDGGS3H(network, backFrame, vert0, azDegs, nRes, 
                          theName, projTypeIn);
         }
         else
            report(apErrStr, DgBase::Fatal);
      }
      else
      {
         if (defaultName) theName = projTypeIn + string("43H");
         dg0 = new DgIDGGS43H(network, backFrame, vert0, azDegs, nRes, theName,
                        projTypeIn, numAp4In, isSuperfundIn);
      }
   }
   else if (gridTopo == "DIAMOND")
   {
      if (apertureIn == 4)
      {
         if (defaultName) theName = projTypeIn + string("4D");
         dg0 = new DgIDGGS4D(network, backFrame, vert0, azDegs, nRes, 
                       theName, projTypeIn);
      }
      else
            report(apErrStr, DgBase::Fatal);
   }
   else if (gridTopo == "TRIANGLE")
   {
      if (apertureIn == 4)
      {
         if (defaultName) theName = projTypeIn + string("4T");
         dg0 = new DgIDGGS4T(network, backFrame, vert0, azDegs, nRes, 
                       theName, projTypeIn);
      }
      else
            report(apErrStr, DgBase::Fatal);
   }
   else
   {
      report("DgIDGGS::makeRF() invalid or unimplemented grid topology: " +
         gridTopo, DgBase::Fatal);
   }

   return dg0;

} // const DgIDGG& makeRF

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIDGGS::DgIDGGS (DgRFNetwork& network, const DgGeoSphRF& backFrame,
                  const DgGeoCoord& vert0, long double azDegs, 
                  unsigned int aperture, int nRes, const string& gridTopo, 
                  const string& name, const string& projType, bool isMixed43, 
                  int numAp4, bool isSuperfund)
        : DgDiscRFS<DgQ2DICoord, DgGeoCoord, long double> (network, backFrame, 
                  nRes, aperture, true, false, name),
          geoRF_ (backFrame), vert0_ (vert0), azDegs_ (azDegs),
          gridTopo_ (gridTopo), projType_ (projType), isMixed43_ (isMixed43),
          numAp4_ (numAp4), isSuperfund_ (isSuperfund)
{
   
   undefLoc_ = makeLocation(undefAddress());

   // create the DGGs

   for (int i = 0; i < nRes; i++)
   {
      if (!isSuperfund)
         (*grids_)[i] = new DgIDGG(backFrame, vert0, azDegs, aperture, i,
            "DDG", gridTopo, projType, isMixed43, numAp4, isSuperfund);
      else
         (*grids_)[i] = new DgIDGG(backFrame, vert0, azDegs, aperture, i,
            "DDG", gridTopo, projType, isMixed43, numAp4, isSuperfund,
            actualRes2sfRes(i));

      new Dg2WayResAddConverter<DgQ2DICoord, DgGeoCoord, long double> 
                                                   (*this, *(grids()[i]), i);
   }

   isAligned_ = idgg(0).isAligned();
   isCongruent_ = idgg(0).isCongruent();

} // DgIDGGS::DgIDGGS

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
