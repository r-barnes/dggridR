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
// DgBoundedHexC3RF2D.cpp: DgBoundedHexC3RF2D class implementation
//
// Version 7.0 - Kevin Sahr, 11/25/14
//
////////////////////////////////////////////////////////////////////////////////

#include <climits>

#include "DgBoundedHexC3RF2D.h"
#include "DgDiscRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBoundedHexC3RF2D::DgBoundedHexC3RF2D 
           (const DgDiscRF<DgIVec2D, DgDVec2D, long double>& rf, 
            const DgIVec2D& lowerLeft, const DgIVec2D& upperRightIn,
            unsigned long long int sizeIn)
   : DgBoundedRF2D (rf, lowerLeft, upperRightIn) 
{ 
   if (!sizeIn)
   {
       report("DgBoundedHexC3RF2D::DgBoundedHexC3RF2D() size not specified", 
              DgBase::Fatal);
       validSize_ = false;
   }
  
   size_ = sizeIn;
   //setLastAdd(addFromSeqNum(size() - 1));

   // determine the lastAdd via brute force
   DgIVec2D add = upperRight();
   if (!validAddress(add))
      decrementAddress(add);

   setLastAdd(add);

} // DgBoundedHexC3RF2D::DgBoundedHexC3RF2D

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIVec2D&
DgBoundedHexC3RF2D::incrementAddress (DgIVec2D& add) const
{
   if (!validAddress(add)) return add = invalidAdd();
   else if (add == upperRight() || add == endAdd()) return add = endAdd();

   do {

      if (add.j() == upperRight().j())
      {
         add = DgIVec2D(add.i() + 1, lowerLeft().j());
      }
      else
      {
         add.setJ(add.j() + 1);
      }
   } while (!validAddress(add) && add != upperRight());

   if (add == upperRight() && !validAddress(add)) add = endAdd();

   return add;

} // DgIVec2D& DgBoundedHexC3RF2D::incrementAddress

////////////////////////////////////////////////////////////////////////////////
DgIVec2D&
DgBoundedHexC3RF2D::decrementAddress (DgIVec2D& add) const
{
   if (add == invalidAdd() || add == lowerLeft()) 
      return add = invalidAdd();
   
   do {

      if (add.j() == lowerLeft().j())
      {
         add = DgIVec2D(add.i() - 1, upperRight().j());
      }
      else
      {
         add.setJ(add.j() - 1);
      }

   } while (!validAddress(add) && add != lowerLeft());

   if (add == lowerLeft() && !validAddress(add)) add = endAdd();

   return add;

} // DgIVec2D& DgBoundedHexC3RF2D::decrementAddress

////////////////////////////////////////////////////////////////////////////////
unsigned long long int
DgBoundedHexC3RF2D::seqNumAddress (const DgIVec2D& add) const
{
   DgIVec2D tVec = add - lowerLeft();
   unsigned long long int sNum = tVec.i() * numI() / 7;

   switch (tVec.i() % 7)
   {
      case 0: sNum += tVec.j() / 7;
              break;

      case 1: sNum += (tVec.j() - 5) / 7;
              break;

      case 2: sNum += (tVec.j() - 3) / 7;
              break;

      case 3: sNum += (tVec.j() - 1) / 7;
              break;

      case 4: sNum += (tVec.j() - 6) / 7;
              break;

      case 5: sNum += (tVec.j() - 4) / 7;
              break;

      case 6: sNum += (tVec.j() - 2) / 7;
              break;
   }

   if (!zeroBased()) sNum++;

   return sNum;
} 

////////////////////////////////////////////////////////////////////////////////
DgIVec2D
DgBoundedHexC3RF2D::addFromSeqNum (unsigned long long int sNum) const
{
/*  USE THIS AS BASIS?
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
*/
   report("DgBoundedHexC3RF2D::addFromSeqNum() NOT IMPLEMENTED YET", DgBase::Fatal);

   DgIVec2D res;

   if (!zeroBased()) 
    sNum--;

   res.setI((sNum * 3) / numI());
   res.setJ((sNum * 3) % numI());

   switch (res.i() % 3)
   {
      case 0: break;

      case 1: res.setJ(res.j() + 2);
              break;

      case 2: res.setJ(res.j() + 1);
              break;
   }

   res += lowerLeft();

   return res;

} // DgIVec2D DgBoundedHexC3RF2D::addFromSeqNum

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


