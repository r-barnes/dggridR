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
// DgDmdD4Grid2D.cpp: DgDmdD4Grid2D class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgDmdD4Grid2D.h"
#include "DgPolygon.h"

const long double DgDmdD4Grid2D::yOff_ = M_SQRT3 / 4.0L;
const long double DgDmdD4Grid2D::yOff2_ = yOff_ * 2.0L;
const long double DgDmdD4Grid2D::xOff_ = 0.25L;
const long double DgDmdD4Grid2D::xOffComp_ = 1.0L - xOff_;
const long double DgDmdD4Grid2D::skewFac_ = M_SQRT3 / 3.0L;

////////////////////////////////////////////////////////////////////////////////
void
DgDmdD4Grid2D::setAddVertices (const DgIVec2D& add, DgPolygon& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   // find the center

   DgLocation tmpLoc;
   setAddPoint(add, tmpLoc);
   const DgDVec2D& cent = *(backFrame().getAddress(tmpLoc));

   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-xOff_,     -yOff_)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( xOffComp_, -yOff_)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( xOff_,      yOff_)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-xOffComp_,  yOff_)));

} // void DgDmdD4Grid2D::setAddVertices

////////////////////////////////////////////////////////////////////////////////
void
DgDmdD4Grid2D::setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() + 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j())));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j())));

} // void DgDmdD4Grid2D::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
