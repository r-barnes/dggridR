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
// DgSqrD4Grid2D.cpp: DgSqrD4Grid2D class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgSqrD4Grid2D.h"
#include "DgPolygon.h"

////////////////////////////////////////////////////////////////////////////////
void
DgSqrD4Grid2D::setAddVertices (const DgIVec2D& add, DgPolygon& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   // find the center

   DgLocation tmpLoc;
   setAddPoint(add, tmpLoc);
   const DgDVec2D& cent = *(backFrame().getAddress(tmpLoc));

   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-0.5, -0.5)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( 0.5, -0.5)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( 0.5,  0.5)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-0.5,  0.5)));

} // void DgSqrD4Grid2D::setAddVertices

////////////////////////////////////////////////////////////////////////////////
void
DgSqrD4Grid2D::setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() + 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j())));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j())));

} // void DgSqrD4Grid2D::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
