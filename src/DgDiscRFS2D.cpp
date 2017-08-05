////////////////////////////////////////////////////////////////////////////////
//
// DgDiscRFS2D.cpp: DgDiscRFS2D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgDiscRFS2D.h"
#include "DgDmdD4Grid2DS.h"
#include "DgDmdD8Grid2DS.h"
#include "DgHexGrid2DS.h"
#include "DgSqrD4Grid2DS.h"
#include "DgSqrD8Grid2DS.h"
#include "DgTriGrid2DS.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgDiscRFS2D* 
DgDiscRFS2D::makeRF (DgRFNetwork& net, const DgRF<DgDVec2D, long double>& cc0,
   int nRes, unsigned int aperture, bool isCongruent, bool isAligned, 
   const string& name, const string geometry, bool isMixed43, int numAp4,
   bool isSuperfund)
{
   DgDiscRFS2D* dg0 = 0;
   if (geometry == "sqr8")
   {
      dg0 = new DgSqrD8Grid2DS(net, cc0, nRes, aperture, isCongruent,
                               isAligned, "SqrD82DS");
   }
   else if (geometry == "sqr4")
   {
      dg0 = new DgSqrD4Grid2DS(net, cc0, nRes, aperture, isCongruent,
                               isAligned, "SqrD42DS");
   }
   else if (geometry == "dmd8")
   {
      dg0 = new DgDmdD8Grid2DS(net, cc0, nRes, aperture, isCongruent,
                               isAligned, "DmdD82DS");
   }
   else if (geometry == "dmd4")
   {
      dg0 = new DgDmdD4Grid2DS(net, cc0, nRes, aperture, isCongruent,
                               isAligned, "DmdD82DS");
   }
   else if (geometry == "hex")
   {
      dg0 = new DgHexGrid2DS(net, cc0, nRes, aperture, isCongruent,
                   isAligned, "HexC12DS", isMixed43, numAp4, isSuperfund);
   }
   else if (geometry == "tri")
   {
      dg0 = new DgTriGrid2DS(net, cc0, nRes, aperture, isCongruent,
                             isAligned, "Tri2DS");
   }
   else
   {
     report("DgDiscRFS2D::makeRF() invalid or unimplemented geometry type: " + 
            geometry, DgBase::Fatal);
   }

   return dg0;

} // DgDiscRFS2D* DgDiscRFS2D::makeRF

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
