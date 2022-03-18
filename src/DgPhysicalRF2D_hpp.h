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
// DgPhysicalRF2D_hpp.h: DgPhysicalRF2D template class definition.
//
////////////////////////////////////////////////////////////////////////////////

#include "DgBoundedRF2D.h"

////////////////////////////////////////////////////////////////////////////////
template<class C>
DgPhysicalRF2D<C>::DgPhysicalRF2D (const DgBoundedRF2D& rfIn, bool allocate)
   : DgPhysicalRF<DgIVec2D, C> (rfIn), boundedRF2D_ (rfIn) 
{
   matrix_ = new (C (**[boundedRF2D().numI()]));
   for (int i = 0; i < boundedRF2D().numI(); i++)
   {
      matrix_[i] = new (C (*[boundedRF2D().numJ()]));

      if (allocate)
      {
         for (int j = 0; j < boundedRF2D().numJ(); j++) matrix_[i][j] = new C();
      }
      else
      {
         for (int j = 0; j < boundedRF2D().numJ(); j++) matrix_[i][j] = 0;
      }
   }

   initAllCells();

} // DgPhysicalRF2D<C>::DgPhysicalRF2D

////////////////////////////////////////////////////////////////////////////////
template<class C> void
DgPhysicalRF2D<C>::replaceAddContents (const DgIVec2D& add, C* cont)
{
   if (!boundedRF2D().validAddress(add))
   {
      report("DgPhysicalRF2D<C>::replaceAddContents() invalid address", 
      DgBase::Fatal);
   }

   DgIVec2D tmp = add - boundedRF2D().lowerLeft();
   deleteAddContents(tmp);
   matrix_[tmp.i()][tmp.j()] = cont;

} // void DgPhysicalRF2D<C>::replaceAddContents

////////////////////////////////////////////////////////////////////////////////
template<class C> void
DgPhysicalRF2D<C>::setAddContents (const DgIVec2D& add, const C& cont)
{
   if (!boundedRF2D().validAddress(add))
   {
      report("DgPhysicalRF2D<C>::setAddContents() invalid address", 
      DgBase::Fatal);
   }

   DgIVec2D tmp = add - boundedRF2D().lowerLeft();
   if (!matrix_[tmp.i()][tmp.j()]) 
   {
      matrix_[tmp.i()][tmp.j()] = new C();

      DgLocation* tmpLoc = discRF().makeLocation(add);
      initCell(matrix_[tmp.i()][tmp.j()], *tmpLoc);
      delete tmpLoc;
   }
   *matrix_[tmp.i()][tmp.j()] = cont;

} // void DgPhysicalRF2D<C>::setAddContents

////////////////////////////////////////////////////////////////////////////////
template<class C> C*
DgPhysicalRF2D<C>::getAddContents (const DgIVec2D& add, bool allocate) const
{
   if (!boundedRF2D().validAddress(add))
   {
      report("DgPhysicalRF2D<C>::getAddContents() invalid address", 
      DgBase::Fatal);
   }

   DgIVec2D tmp = add - boundedRF2D().lowerLeft();
   if (matrix_[tmp.i()][tmp.j()] == 0 && allocate) 
   {
      matrix_[tmp.i()][tmp.j()] = new C();
      DgLocation* tmpLoc = discRF().makeLocation(add);
      const_cast<DgPhysicalRF2D<C>*>(this)->
            initCell(matrix_[tmp.i()][tmp.j()], *tmpLoc);
      delete tmpLoc;
   }

   return matrix_[tmp.i()][tmp.j()];

} // C* DgPhysicalRF2D<C>::getAddContents

////////////////////////////////////////////////////////////////////////////////
template<class C> void
DgPhysicalRF2D<C>::deleteAddContents (const DgIVec2D& add)
{
   if (!boundedRF2D().validAddress(add))
   {
      report("DgPhysicalRF2D<C>::deleteAddContents() invalid address",
             DgBase::Fatal);
   }

   DgIVec2D tmp = add - boundedRF2D().lowerLeft();
   delete matrix_[tmp.i()][tmp.j()];

} // void DgPhysicalRF2D<C>::deleteAddContents

////////////////////////////////////////////////////////////////////////////////
