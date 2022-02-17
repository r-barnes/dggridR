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
// DgBoundedRFS2D.cpp: DgBoundedRFS2D class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <climits>

#include "DgBoundedRFS2D.h"
#include "DgBoundedHexC2RF2D.h"
#include "DgDiscRFS2D.h"
#include "DgTriGrid2DS.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBoundedRFS2D::DgBoundedRFS2D (const DgDiscRFS2D& rf, 
                                const DgIVec2D& lowerLeft0, 
                                const DgIVec2D& upperRight0)
   : DgBoundedRF< DgResAdd<DgIVec2D>, DgDVec2D, long double > (rf, 
                  DgResAdd<DgIVec2D> (DgIVec2D(0, 0), 0),
                  DgResAdd<DgIVec2D> (DgIVec2D(0, 0), 0), rf.undefAddress()), 
     discRFS_ (rf)
{ 
   // check to see if current limitations are met

   if (lowerLeft0 != DgIVec2D(0, 0))
   {
      report("DgBoundedRFS2DS::DgBoundedRFS2DS() only lowerLeft0 = (0, 0) "
             "supported", DgBase::Fatal);
   }

   // allocate the grids

   grids_ = new vector<const DgBoundedRF2D*>(discRFS().nRes());

   int totTicks = 1;
   long long int numI = upperRight0.i() + 1;
   long long int numJ = upperRight0.j() + 1;
   if (rf.aperture() == 3) // better be hex!
   {
      for (int i = 0; i < discRFS().nRes(); i++)
      {
         bool isClassI = !(i % 2);
         if (isClassI)
         {
            (*grids_)[i] = new DgBoundedRF2D(*rf.grids()[i], DgIVec2D(0, 0),
                      DgIVec2D(totTicks * numI - 1, totTicks * numJ - 1));

            totTicks *= 3;
         }
         else
         {
            (*grids_)[i] = new DgBoundedHexC2RF2D(*rf.grids()[i], 
                                DgIVec2D(0, 0), DgIVec2D(totTicks * numI - 1, 
                                                         totTicks * numJ - 1));
         }
      }
   }
   else
   {
      // check that the aperture is a perfect square

      int sqrtApp = static_cast<int>(sqrt(static_cast<float>(rf.aperture())));
      if (static_cast<unsigned int>(sqrtApp * sqrtApp) != rf.aperture())
      {
         report("DgBoundedRFS2DS::DgBoundedRFS2DS() aperture " + 
                dgg::util::to_string(rf.aperture()) + " is not a perfect square",
                DgBase::Fatal);
      }

      for (int i = 0; i < discRFS().nRes(); i++)
      {
         (*grids_)[i] = new DgBoundedRF2D(*rf.grids()[i], DgIVec2D(0, 0),
                DgIVec2D(totTicks * numI - 1, totTicks * numJ - 1));

/*
         cout << "grid " << i << endl;
         for (DgIVec2D c = (*grids_)[i]->lowerLeft(); 
              c != (*grids_)[i]->invalidAdd();         
              c = (*grids_)[i]->incrementAddress(c)) cout << c << endl;
*/

         totTicks *= sqrtApp;
      }
   }

   // set the correct last address

   int maxRes = discRFS().nRes() - 1;
   DgResAdd<DgIVec2D> last((*grids_)[maxRes]->lastAdd(), maxRes);

   setLastAdd(last);

   // set the size

   size_ = 0;
   for (int i = 0; i < discRFS().nRes(); i++)
   {
      unsigned long long int lastSize = size_;

      const DgBoundedRF2D* g = (*grids_)[i];

      if (g->validSize()) size_ += g->size();

      if (!g->validSize() || size() < lastSize)
      {
/*
         report("DgBoundedRFS2D::DgBoundedRFS2D() invalid size setting due to "
                "possible overflow", DgBase::Warning);
*/
         validSize_ = false;
      }
   }

   validSize_ = true;

} // DgBoundedRFS2D::DgBoundedRFS2D

////////////////////////////////////////////////////////////////////////////////
DgResAdd<DgIVec2D>&
DgBoundedRFS2D::incrementAddress (DgResAdd<DgIVec2D>& add) const
{
   if (!validAddress(add)) return add = invalidAdd();
   else if (add == lastAdd()) return add = endAdd();
   else if (add == endAdd()) return add;

   const DgBoundedRF2D& grid = *(grids()[add.res()]);

   DgIVec2D v(add.address());
   add = DgResAdd<DgIVec2D>(grid.incrementAddress(v), add.res());

   if (add.address() == grid.endAdd())
   {
      if (add.res() == (discRFS().nRes() - 1)) return add = endAdd();
      else 
      {
         int newRes = add.res() + 1;
         add = DgResAdd<DgIVec2D>(grids()[newRes]->firstAdd(), newRes);
      }
   }

   return add;

/*
   if (add.address() == grid.lastAdd() ||
       add.address() == grid.endAdd())
   {
      if (add.res() == (discRFS().nRes() - 1)) return add = endAdd();
      else 
      {
         int newRes = add.res() + 1;
         return add = DgResAdd<DgIVec2D>(grids()[newRes]->firstAdd(), newRes);
      }
   }
   else
   {
      DgIVec2D v(add.address());
      return add = DgResAdd<DgIVec2D>(grid.incrementAddress(v), add.res());
   }
*/

} // DgResAdd<DgIVec2D>& DgBoundedRFS2D::incrementAddress

////////////////////////////////////////////////////////////////////////////////
DgResAdd<DgIVec2D>&
DgBoundedRFS2D::decrementAddress (DgResAdd<DgIVec2D>& add) const
{
   if (!validAddress(add) || add == firstAdd()) return add = invalidAdd();

   const DgBoundedRF2D& grid = *(grids()[add.res()]);

   if (add.address() == grid.firstAdd())
   {
      int newRes = add.res() - 1;
      return add = DgResAdd<DgIVec2D>(grids()[newRes]->lastAdd(), newRes);
   }
   else
   {
      DgIVec2D v(add.address());
      return add = DgResAdd<DgIVec2D>(grid.decrementAddress(v), add.res());
   }

} // DgResAdd<DgIVec2D>& DgBoundedRFS2D::decrementAddress

////////////////////////////////////////////////////////////////////////////////
unsigned long long int 
DgBoundedRFS2D::seqNumAddress (const DgResAdd<DgIVec2D>& add) const
{
   if (!validSize())
   {
      report("DgBoundedRFS2D::seqNumAddress() valid size required",
             DgBase::Fatal);
      return 0;
   }

   unsigned long long int sNum = 0;
   if (!zeroBased()) sNum++;

   for (int i = 0; i < add.res(); i++) sNum += (*grids_)[i]->size();

   sNum += (*grids_)[add.res()]->seqNumAddress(add.address());

   return sNum;

} // unsigned long long int DgBoundedRFS2D::seqNumAddress

////////////////////////////////////////////////////////////////////////////////
DgResAdd<DgIVec2D> 
DgBoundedRFS2D::addFromSeqNum (unsigned long long int sNum) const
{
   if (!validSize())
   {
      report("DgBoundedRFS2D::seqNumAddress() valid size required",
             DgBase::Fatal);
      return DgResAdd<DgIVec2D>();
   }

   if (!zeroBased()) sNum--;

   DgResAdd<DgIVec2D> tmp;
   unsigned long long int n = sNum;
   tmp.setRes(0);

   int r;
   for (r = 0; r < discRFS().nRes(); r++)
   {
      tmp.setRes(r);

      if (n < (*grids_)[r]->size()) break;

      n -= (*grids_)[r]->size();
   }

   if (r == discRFS().nRes()) return invalidAdd();

   tmp.setAddress((*grids_)[tmp.res()]->addFromSeqNum(n));

   return tmp;

} // DgResAdd<DgIVec2D> DgBoundedRFS2D::addFromSeqNum

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
