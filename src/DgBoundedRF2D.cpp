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
// DgBoundedRF2D.cpp: DgBoundedRF2D class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <climits>

#include "DgBoundedRF2D.h"
#include "DgDiscRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBoundedRF2D::DgBoundedRF2D (const DgDiscRF<DgIVec2D, DgDVec2D, long double>& rf, 
                              const DgIVec2D& lowerLeftIn, 
                              const DgIVec2D& upperRightIn)
   : DgBoundedRF<DgIVec2D, DgDVec2D, long double> 
                    (rf, lowerLeftIn, upperRightIn, rf.undefAddress()),
     discRF_ (rf), lowerLeft_ (lowerLeftIn), upperRight_ (upperRightIn), 
     numI_ (upperRightIn.i() - lowerLeftIn.i() + 1), 
     numJ_ (upperRightIn.j() - lowerLeftIn.j() + 1)
{ 
   if (numI() <= 0 || numJ() <= 0)
   {
      report("DgBoundedRF2D::DgBoundedRF2D() invalid bounds", DgBase::Fatal);
   }

   size_ = numI() * numJ();
   if ((size_ / numI()) != static_cast<unsigned long>(numJ()))
   {
/*
       report("DgBoundedRF2D::DgBoundedRF2D() invalid size setting due to "
              "possible overflow", DgBase::Warning);
*/
       validSize_ = false;
   }
   else validSize_ = true;
   
} // DgBoundedRF2D::DgBoundedRF2D

////////////////////////////////////////////////////////////////////////////////
DgIVec2D&
DgBoundedRF2D::incrementAddress (DgIVec2D& add) const
{
   if (!validAddress(add)) return add = invalidAdd();
   else if (add == upperRight() || add == endAdd()) return add = endAdd();

   if (add.j() == upperRight().j())
   {
      add = DgIVec2D(add.i() + 1, lowerLeft().j());
   }
   else
   {
      add.setJ(add.j() + 1);
   }

   return add;

} // DgIVec2D& DgBoundedRF2D::incrementAddress

////////////////////////////////////////////////////////////////////////////////
DgIVec2D&
DgBoundedRF2D::decrementAddress (DgIVec2D& add) const
{
   if (!validAddress(add) || add == lowerLeft()) return add = invalidAdd();

   if (add.j() == lowerLeft().j())
   {
      add = DgIVec2D(add.i() - 1, upperRight().j());
   }
   else
   {
      add.setJ(add.j() - 1);
   }

   return add;

} // DgIVec2D& DgBoundedRF2D::decrementAddress

////////////////////////////////////////////////////////////////////////////////
unsigned long long int
DgBoundedRF2D::seqNumAddress (const DgIVec2D& add) const
{
   DgIVec2D tVec = add - lowerLeft();

   long long int sNum = tVec.i() * numJ() + tVec.j();

   if (!zeroBased()) 
    sNum++;

   return sNum;
} // unsigned long long int DgBoundedRF2D::seqNumAddress

////////////////////////////////////////////////////////////////////////////////
DgIVec2D 
DgBoundedRF2D::addFromSeqNum (unsigned long long int sNum) const
{
   DgIVec2D res;

   if (!zeroBased()) 
    sNum--;

   res.setI(sNum / numJ());
   res.setJ(sNum % numJ());

   res += lowerLeft();

   return res;

} // DgIVec2D DgBoundedRF2D::addFromSeqNum

////////////////////////////////////////////////////////////////////////////////
