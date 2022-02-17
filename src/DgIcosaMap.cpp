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
// DgIcosaMap.cpp: DgIcosaMap class implementation
//
////////////////////////////////////////////////////////////////////////////////
//
//   Projects points in geographic form (longitude and latitude in
//   decimal degrees) to the corresponding ISEA equal area icosahedron
//   projection with the icosahedron unfolded in the standard layout.
//
////////////////////////////////////////////////////////////////////////////////

#include "DgIcosaMap.h"

////////////////////////////////////////////////////////////////////////////////

const DgDVec2D DgIcosaTri::origin_ = DgDVec2D(0.5L, 0.5L / M_SQRT3);

const DgIcosaTri defIcosaTri[20] = {

   DgIcosaTri(true, 0, DgDVec2D(0.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(1.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(2.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(3.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(4.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(1.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(2.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(3.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(4.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(5.0L, 2.0L * M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(0.5L, M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(1.5L, M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(2.5L, M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(3.5L, M_SIN60)),
   DgIcosaTri(true, 0, DgDVec2D(4.5L, M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(1.5L, M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(2.5L, M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(3.5L, M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(4.5L, M_SIN60)),
   DgIcosaTri(true, 3, DgDVec2D(5.5L, M_SIN60))

};

DgIcosaMap DgIcosaMap::defIcosaMap(defIcosaTri);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIcosaMap::DgIcosaMap (const DgIcosaTri icosaTri[20])
{
   for (int i = 0; i < 20; i++) icosaTri_[i] = icosaTri[i];

} // DgIcosaMap::DgIcosaMap

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgIcosaTri::translate (DgDVec2D* pt) const
{
   // move the origin to the lower left corner

   //*pt += origin_;

   // first rotate 

   long double x0 = pt->x();
   long double y0 = pt->y();
   if (rot60_)
   {
      long double cosAng = cosl((rot60_ * 60.0L) * M_PI_180);
      long double sinAng = sinl((rot60_ * 60.0L) * M_PI_180);
      pt->setX(x0 * cosAng - y0 * sinAng);
      pt->setY(x0 * sinAng + y0 * cosAng);
   }

   *pt += offset_;

} // void DgIcosaTri::translate

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgIcosaTri::untranslate (DgDVec2D* pt, bool shiftOrigin) const
{
   // translate to origin

   *pt -= offset_;

   // rotate 

   long double x0 = pt->x();
   long double y0 = pt->y();
   if (rot60_)
   {
      long double cosAng = cosl(-1.0L * (rot60_ * 60.0L) * M_PI_180);
      long double sinAng = sinl(-1.0L * (rot60_ * 60.0L) * M_PI_180);
      pt->setX(x0 * cosAng - y0 * sinAng);
      pt->setY(x0 * sinAng + y0 * cosAng);
   }

   if (shiftOrigin) // move the origin from the lower left corner
   {
      *pt -= origin_;
   }

} // void DgIcosaTri::untranslate

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgIcosaMap::translate (int nTri, DgDVec2D* pt) const
{
   icosaTri_[nTri].translate(pt);

} // void DgIcosaMap::translate

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgIcosaMap::untranslate (int nTri, DgDVec2D* pt, bool shiftOrigin) const
{
   icosaTri_[nTri].untranslate(pt, shiftOrigin);

} // void DgIcosaMap::untranslate

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgDVec2D
DgIcosaMap::maxOffset (void)
{
   DgDVec2D maxOff(-1.0L, -1.0L);

   for (int i = 0; i < 20; i++)
   {
      if (!icosaTri_[i].mapped_) continue;

      // find the max X and Y offsets for this triangle in edge-length units 

      long double offX = icosaTri_[i].offset_.x();
      long double offY = icosaTri_[i].offset_.y() / M_SIN60;

      switch (icosaTri_[i].rot60_)
      {
         case 0:
         case 5:

            offX += 1.0L;
            break;

         case 1:
         case 4:

            offX += 0.5L;
            break;
      }

      switch (icosaTri_[i].rot60_)
      {
         case 0:
         case 1:
         case 2:

            offY += 1.0L;
            break;
      }

      if (offX > maxOff.x()) maxOff.setX(offX);
      if (offY > maxOff.y()) maxOff.setY(offY);
      
   }

   return maxOff;

} // void DgIcosaMap::maxOffset

////////////////////////////////////////////////////////////////////////////////
