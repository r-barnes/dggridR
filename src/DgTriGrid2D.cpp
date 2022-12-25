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
// DgTriGrid2D.cpp: DgTriGrid2D class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgTriGrid2D.h"
#include "DgPolygon.h"

////////////////////////////////////////////////////////////////////////////////
void
DgTriGrid2D::setAddVertices (const DgIVec2D& add, DgPolygon& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   // find the center

   DgLocation tmpLoc;
   setAddPoint(add, tmpLoc);
   const DgDVec2D& cent = *(backFrame().getAddress(tmpLoc));

   if (isUp(add))
   {
      v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-e() / 2.0, -0.5)));
      v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(       0.0,  1.0)));
      v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( e() / 2.0, -0.5)));
   }
   else
   {
      v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-e() / 2.0,  0.5)));
      v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( e() / 2.0,  0.5)));
      v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(       0.0, -1.0)));
   }

/*
   cout << "triangle: " << add << " " << tmpLoc << endl;
   cout << vec << endl;
*/

} // void DgTriGrid2D::setAddVertices

////////////////////////////////////////////////////////////////////////////////
void
DgTriGrid2D::setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   if (isUp(add))
   {
      v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(),     add.j() + 1)));
      v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j() + 1)));
      v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(),     add.j() - 1)));
   }
   else
   {
      v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(),     add.j() + 1)));
      v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(),     add.j() - 1)));
      v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j() - 1)));
   }

} // void DgTriGrid2D::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
DgIVec2D 
DgTriGrid2D::quantify (const DgDVec2D& point) const
{
   DgDVec2D p(point);  // p prime
   p += DgDVec2D(e() / 2.0L, 0.5L);   // move origin to lower left

   DgDVec2D pp(p.x() + p.y() / M_SQRT3, p.y()); // p long double prime

   long long int i    = static_cast<long long int>(floor(pp.x() / e()));
   long long int jTmp = static_cast<long long int>(floor(2.0/3.0 * pp.y()));

   DgDVec2D ppp(p.x() - p.y() / M_SQRT3, p.y()); // p triple prime
   long long int k = static_cast<long long int>(floor(ppp.x() / e()));

   long long int j = jTmp * 2;
   if (jTmp % 2) // i + j + k should be odd
   {
      if (!((i + j + k) % 2)) j++;
   }
   else // i + j + k should be even
   {
      if ((i + j + k) % 2) j++;
   }

   return DgIVec2D(i, j);

} // DgIVec2D DgTriGrid2D::quantify

////////////////////////////////////////////////////////////////////////////////
DgDVec2D 
DgTriGrid2D::invQuantify (const DgIVec2D& add) const
{
   long double x = ((long double) add.i() * e()) - 
                 ((long double) ((add.j() + 1) / 2) * e() / 2.0);
   long double y = 1.5 * (long double) (add.j() / 2) + 0.5 * (long double) (add.j() % 2);

   return DgDVec2D(x, y);

} // DgDVec2D DgTriGrid2D::invQuantify

////////////////////////////////////////////////////////////////////////////////
long long int
DgTriGrid2D::dist (const DgIVec2D&, const DgIVec2D&) const
{
   return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
