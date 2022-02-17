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
// DgBoundedIDGGS.cpp: DgBoundedIDGGS class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <climits>

#include "DgBoundedIDGGS.h"
#include "DgBoundedHexC2RF2D.h"
#include "DgIDGGS.h"
#include "DgTriGrid2DS.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBoundedIDGGS::DgBoundedIDGGS (const DgIDGGS& rf)
   : DgBoundedRF< DgResAdd<DgQ2DICoord>, DgGeoCoord, long double > (rf, 
                  DgResAdd<DgQ2DICoord> (DgQ2DICoord(0, DgIVec2D(0, 0)), 0),
                  DgResAdd<DgQ2DICoord> (DgQ2DICoord(0, DgIVec2D(0, 0)), 0), 
                  rf.undefAddress()), IDGGS_ (rf)
{ 
   // allocate the grids

   grids_ = new vector<const DgBoundedIDGG*>(IDGGS().nRes());

   for (int i = 0; i < IDGGS().nRes(); i++)
      (*grids_)[i] = new DgBoundedIDGG(rf.idgg(i));

   // set the correct first address
   setFirstAdd(DgResAdd<DgQ2DICoord>((*grids_)[0]->firstAdd(), 0));

   // set the correct last address

   int maxRes = IDGGS().nRes() - 1;
   setLastAdd(DgResAdd<DgQ2DICoord>((*grids_)[maxRes]->lastAdd(), maxRes));

   // set the size

   size_ = 0;
   for (int i = 0; i < IDGGS().nRes(); i++)
   {
      unsigned long long int lastSize = size_;

      const DgBoundedIDGG* g = (*grids_)[i];

      if (g->validSize()) size_ += g->size();

      if (!g->validSize() || size() < lastSize)
      {
/*
         report("DgBoundedIDGGS::DgBoundedIDGGS() invalid size setting due to "
                "possible overflow", DgBase::Warning);
*/
         validSize_ = false;
      }
   }

   validSize_ = true;

} // DgBoundedIDGGS::DgBoundedIDGGS

////////////////////////////////////////////////////////////////////////////////
DgResAdd<DgQ2DICoord>&
DgBoundedIDGGS::incrementAddress (DgResAdd<DgQ2DICoord>& add) const
{
   if (!validAddress(add)) return add = invalidAdd();
   else if (add == lastAdd()) return add = endAdd();
   else if (add == endAdd()) return add;

   const DgBoundedIDGG& grid = *(grids()[add.res()]);

   DgQ2DICoord v(add.address());
   add = DgResAdd<DgQ2DICoord>(grid.incrementAddress(v), add.res());

   if (add.address() == grid.endAdd())
   {
      if (add.res() == (IDGGS().nRes() - 1)) return add = endAdd();
      else 
      {
         int newRes = add.res() + 1;
         add = DgResAdd<DgQ2DICoord>(grids()[newRes]->firstAdd(), newRes);
      }
   }

   return add;

/*
   if (add.address() == grid.lastAdd() ||
       add.address() == grid.endAdd())
   {
      if (add.res() == (IDGGS().nRes() - 1)) return add = endAdd();
      else 
      {
         int newRes = add.res() + 1;
         return add = DgResAdd<DgQ2DICoord>(grids()[newRes]->firstAdd(), newRes);
      }
   }
   else
   {
      DgQ2DICoord v(add.address());
      return add = DgResAdd<DgQ2DICoord>(grid.incrementAddress(v), add.res());
   }
*/

} // DgResAdd<DgQ2DICoord>& DgBoundedIDGGS::incrementAddress

////////////////////////////////////////////////////////////////////////////////
DgResAdd<DgQ2DICoord>&
DgBoundedIDGGS::decrementAddress (DgResAdd<DgQ2DICoord>& add) const
{
   if (!validAddress(add) || add == firstAdd()) return add = invalidAdd();

   const DgBoundedIDGG& grid = *(grids()[add.res()]);

   if (add.address() == grid.firstAdd())
   {
      int newRes = add.res() - 1;
      return add = DgResAdd<DgQ2DICoord>(grids()[newRes]->lastAdd(), newRes);
   }
   else
   {
      DgQ2DICoord v(add.address());
      return add = DgResAdd<DgQ2DICoord>(grid.decrementAddress(v), add.res());
   }

} // DgResAdd<DgQ2DICoord>& DgBoundedIDGGS::decrementAddress

////////////////////////////////////////////////////////////////////////////////
unsigned long long int 
DgBoundedIDGGS::seqNumAddress (const DgResAdd<DgQ2DICoord>& add) const
{
   if (!validSize())
   {
      report("DgBoundedIDGGS::seqNumAddress() valid size required",
             DgBase::Fatal);
      return 0;
   }

   unsigned long long int sNum = 0;
   if (!zeroBased()) sNum++;

   for (int i = 0; i < add.res(); i++) sNum += (*grids_)[i]->size();

   sNum += (*grids_)[add.res()]->seqNumAddress(add.address());

   return sNum;

} // unsigned long long int DgBoundedIDGGS::seqNumAddress

////////////////////////////////////////////////////////////////////////////////
DgResAdd<DgQ2DICoord> 
DgBoundedIDGGS::addFromSeqNum (unsigned long long int sNum) const
{
   if (!validSize())
   {
      report("DgBoundedIDGGS::seqNumAddress() valid size required",
             DgBase::Fatal);
      return DgResAdd<DgQ2DICoord>();
   }

   if (!zeroBased()) sNum--;

   DgResAdd<DgQ2DICoord> tmp;
   unsigned long long int n = sNum;
   tmp.setRes(0);

   int r;
   for (r = 0; r < IDGGS().nRes(); r++)
   {
      tmp.setRes(r);

      if (n < (*grids_)[r]->size()) break;

      n -= (*grids_)[r]->size();
   }

   if (r == IDGGS().nRes()) return invalidAdd();

   tmp.setAddress((*grids_)[tmp.res()]->addFromSeqNum(n));

   return tmp;

} // DgResAdd<DgQ2DICoord> DgBoundedIDGGS::addFromSeqNum

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
