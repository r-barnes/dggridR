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
// DgHexIDGGS.cpp: DgHexIDGGS class implementation
//
// Version 7.0 - Kevin Sahr, 11/16/14
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgHexIDGGS.h"
#include "DgContCartRF.h"
#include "DgDiscRF.h"
#include "DgHexC1Grid2D.h"
#include "DgHexC2Grid2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgHexIDGGS::DgHexIDGGS (DgRFNetwork& network, const DgGeoSphRF& backFrame,
                  const DgGeoCoord& vert0, long double azDegs, 
                  unsigned int aperture, int nRes, 
                  const string& name, const string& projType, 
                  const DgApSeq& apSeq, bool isApSeq,
                  bool isMixed43, int numAp4, bool isSuperfund)
        : DgIDGGS (network, backFrame, vert0, azDegs, aperture, nRes, 
                       Hexagon, D6, name, projType, isMixed43, numAp4,
                       isSuperfund, isApSeq, apSeq),
          apSeq_ (apSeq)
{
   if (!isApSeq) // need to build the apSeq
   {  
      int r;
         
      if (isMixed43)
      {  
         for (r = 0; r < numAp4; r++)
            apSeq_.addAperture(DgAperture(4));
            
         for (; r < nRes - 1; r++)
            apSeq_.addAperture(DgAperture(3));
      }
      else
         for (r = 0; r < nRes - 1; r++)
            apSeq_.addAperture(DgAperture((aperture == 3) ? 3 : 4));
   }
         
   if (nRes > apSeq_.nRes() + 1) // remember +1 resolution for res 0
      report("DgHexIDGGS::DgHexIDGGS res " + dgg::util::to_string(nRes) +
             " exceeds aperture sequence string length", DgBase::Fatal);
   
   setUndefLoc(makeLocation(undefAddress())); 
   isAligned_ = true;
   isCongruent_ = false;

   // create the DGGs

   // make res 0 ap 4 so it will be Class I
   (*grids_)[0] = new DgHexIDGG(*this, 4, 0, name + dgg::util::to_string(0, 2));
   //cout << "========================\nRES 0: " << hexIdgg(0);

   for (int r = 1; r < nRes; r++)
   {
      int ap = apSeq_.getAperture(r).aperture();

      (*grids_)[r] = new DgHexIDGG(*this, ap, r, 
                                   name + dgg::util::to_string(r, 2));
   //cout << "========================\nRES " << r << ": " << hexIdgg(r);
   }

   for (int r = 0; r < nRes; r++)
      Dg2WayResAddConverter<DgQ2DICoord, DgGeoCoord, long double>(*this, *(grids()[r]), r);

} // DgHexIDGGS::DgHexIDGGS

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void 
DgHexIDGGS::setAddParents (const DgResAdd<DgQ2DICoord>& add, 
                             DgLocVector& vec) const
{
   DgPolygon verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res()]->setVertices(*tmpLoc, verts);
   delete tmpLoc;

   // vertices lie in parents

   grids()[add.res() - 1]->convert(verts);

   for (int i = 0; i < verts.size(); i++)
   {
      // check if already present

      bool found = false;
      for (int j = 0; j < vec.size(); j++)
      {
         if (vec[j] == verts[i])
         {
            found = true;
            break;
         }
      }

      if (!found) vec.push_back(verts[i]);
   }

} // void DgHexIDGGS::setAddParents

////////////////////////////////////////////////////////////////////////////////
void 
DgHexIDGGS::setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   // single center hex at next res
   DgLocVector verts;
   DgLocation* centerLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res() + 1]->convert(centerLoc);
   vec.push_back(*centerLoc);

   delete centerLoc;

} // void DgHexIDGGS::setAddInteriorChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgHexIDGGS::setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   const DgHexIDGG& dgg = hexIdgg(add.res());
   const DgHexIDGG& dggr = hexIdgg(add.res() + 1);

   // the neighbors of the center hex at next res
   DgLocation* centerLoc = dgg.makeLocation(add.address());
   dggr.convert(centerLoc);
   const DgQ2DICoord& centerAdd = 
       *dggr.getAddress(*centerLoc);

   dggr.setAddNeighbors(centerAdd, vec);

   delete centerLoc;

} // void DgHexIDGGS::setAddBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgHexIDGGS::setAddBoundary2Children (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   const DgHexIDGG& dgg = hexIdgg(add.res());
   const DgHexIDGG& dggr = hexIdgg(add.res() + 1);
   if (dggr.aperture() != 7)
      return;

   // the neighbors of the center hex at next res
   DgLocation* centerLoc = dgg.makeLocation(add.address());
   dggr.convert(centerLoc);
   const DgQ2DICoord& centerAdd = 
       *dggr.getAddress(*centerLoc);

   dggr.setAddNeighborsBdry2(centerAdd, vec);

   delete centerLoc;

} // void DgHexIDGGS::setAddBoundary2Children

////////////////////////////////////////////////////////////////////////////////
void 
DgHexIDGGS::setAddAllChildren (const DgResAdd<DgQ2DICoord>& add, 
                                   DgLocVector& vec) const
{
   setAddInteriorChildren(add, vec);

   DgLocVector bndVec(vec.rf());
   setAddBoundaryChildren(add, bndVec);
   for (int i = 0; i < bndVec.size(); i++) vec.push_back(bndVec[i]);

   DgLocVector bnd2Vec(vec.rf());
   setAddBoundary2Children(add, bnd2Vec);
   for (int i = 0; i < bnd2Vec.size(); i++) vec.push_back(bnd2Vec[i]);

} // void DgHexIDGGS::setAddAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

