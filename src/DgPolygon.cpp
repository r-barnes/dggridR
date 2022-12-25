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
// DgPolygon.cpp: DgPolygon class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgPolygon.h"
#include "DgContCartRF.h"

////////////////////////////////////////////////////////////////////////////////
DgPolygon::DgPolygon (const DgPolygon& poly)
         : DgLocVector (poly)
{
   if (poly.hasHoles()) {
      for (unsigned long int h = 0; h < poly.holes().size(); h++)
         holes_.push_back(new DgPolygon(*poly.holes()[h]));
   }
}

////////////////////////////////////////////////////////////////////////////////
const DgPolygon&
DgPolygon::operator= (const DgPolygon& poly)
{
   reinterpret_cast<DgPolygon&>(DgLocVector::operator=(poly));
   clearHoles();
   if (poly.hasHoles()) {
      for (unsigned long h = 0; h < poly.holes().size(); h++)
         holes_.push_back(new DgPolygon(*poly.holes()[h]));
   }

   return *this;
}

////////////////////////////////////////////////////////////////////////////////
bool
DgPolygon::operator== (const DgPolygon& poly) const
{
   bool isEqual = DgLocVector::operator==(poly) &&
                       holes_.size() == poly.holes().size();

   // check holes
   if (isEqual) {
      for (unsigned long int i = 0; i < holes_.size(); i++) {
         if (*holes_[i] != *poly.holes()[i]) {
            isEqual = false;
            break;
         }
      }
   }

   return isEqual;
}

////////////////////////////////////////////////////////////////////////////////
void
DgPolygon::clearHoles (void)
{
   if (hasHoles()) {
      for (unsigned long int h = 0; h < holes_.size(); h++) {
         delete holes_[h];
         holes_[h] = 0;
      }

      holes_.resize(0);
   }
}

////////////////////////////////////////////////////////////////////////////////
DgPolygon::~DgPolygon (void)
{
   clearHoles();
}

////////////////////////////////////////////////////////////////////////////////
void
DgPolygon::densify (int ptsPerEdge)
{
   if (ptsPerEdge <= 0) return;

   const DgContCartRF* cc = dynamic_cast<const DgContCartRF*>(&rf());
   if (cc == 0) report("DgPolygon::densify() with non-CCRF", DgBase::Fatal);

   DgPolygon densVerts(*cc);

   vector<DgAddressBase*>& v0 = addressVec();
   vector<DgAddressBase*>& v1 = densVerts.addressVec();

   // for each edge
   for (unsigned long i = 0; i < v0.size(); i++) {

      DgDVec2D p1 = dynamic_cast< DgAddress<DgDVec2D>& >(*v0[i]).address();
      DgDVec2D p2 = dynamic_cast< DgAddress<DgDVec2D>& >(
                                          *v0[(i + 1) % v0.size()]).address();

      long double fracStep = 1.0 / (ptsPerEdge + 1);
      long double frac = fracStep;

      // add the first point

      v1.push_back(new DgAddress<DgDVec2D>(p1));

      // add each densification point

      for (int j = 0; j < ptsPerEdge; j++)
      {
         DgDVec2D p3 = DgDVec2D::fracPoint(p1, p2, frac);
         frac += fracStep;

         v1.push_back(new DgAddress<DgDVec2D>(p3));
      }

      // p2 should get pushed next time
   }

   // now replace the original with the densified version

   *this = densVerts;

   // now densify any holes
   if (hasHoles())
      for (unsigned long int i = 0; i < holes_.size(); i++)
         holes_[i]->densify(ptsPerEdge);

} // DgPolygon::densify

////////////////////////////////////////////////////////////////////////////////
void
DgPolygon::addHole (DgPolygon* hole)
{
   holes_.push_back(hole);
}

