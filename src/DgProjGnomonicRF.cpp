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
// DgProjGnomonicRF.cpp: DgProjGnomonicRF class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgProjGnomonicRF.h"
#include "DgEllipsoidRF.h"
#include "proj4.h"

#define EPS10 1.e-10L
#define N_POLE 0
#define S_POLE 1
#define EQUIT  2
#define OBLIQ  3

////////////////////////////////////////////////////////////////////////////////
DgProjGnomonicRF::DgProjGnomonicRF(DgRFNetwork& networkIn, const string& nameIn,
       const DgGeoCoord& proj0In, long double x0In, long double y0In, long double k0In, 
       long double to_meterIn, long double fr_meterIn)
   : DgGeoProjRF (networkIn, nameIn, proj0In, x0In, y0In, k0In, 
                  to_meterIn, fr_meterIn)
{ 
   if (fabs(fabs(phi0()) - M_PI_2) < EPS10)
      mode_ = phi0() < 0.0L ? S_POLE : N_POLE;
   else if (fabs(phi0()) < EPS10)
      mode_ = EQUIT;
   else {
      mode_ = OBLIQ;
      sinph0_ = sin(phi0());
      cosph0_ = cos(phi0());
   }

} // DgProjGnomonicRF::DgProjGnomonicRF

////////////////////////////////////////////////////////////////////////////////
DgDVec2D
DgProjGnomonicRF::projForward (const DgGeoCoord& addIn,
                               const DgEllipsoidRF&) const
{
   //cout << "gnom projForward: " << *this << " coord: " << addIn << endl;

   DgDVec2D xy(DgDVec2D::undefDgDVec2D);

   long double  coslam, cosphi, sinphi;

   sinphi = sin(addIn.lat());
   cosphi = cos(addIn.lat());
   coslam = cos(addIn.lon());

   switch (mode_) {
   case EQUIT:
      xy.setY(cosphi * coslam);
      break;
   case OBLIQ:
      xy.setY(sinph0_ * sinphi + cosph0_ * cosphi * coslam);
      break;
   case S_POLE:
      xy.setY(- sinphi);
      break;
   case N_POLE:
      xy.setY(sinphi);
      break;
   }

   if (xy.y() <= EPS10) 
   {
      ::report(string("DgProjGnomonicRF::projForward() point out of range\n") +
           string("proj0: ") + string(proj0()) + 
           string("\nprojecting point: ") + string(addIn), DgBase::Fatal);
   }

   xy.setY(1.0L / xy.y());
   xy.setX(xy.y() * cosphi * sin(addIn.lon()));

   switch (mode_) {
   case EQUIT:
      xy.setY(xy.y() * sinphi);
      break;
   case OBLIQ:
      xy.setY(xy.y() * (cosph0_ * sinphi - sinph0_ * cosphi * coslam));
      break;
   case N_POLE:
      coslam = - coslam;
      FALLTHROUGH
   case S_POLE:
      xy.setY(xy.y() * cosphi * coslam);
      break;
   }

   //cout << "     => " << xy << endl;

   return (xy);

} // DgDVec2D DgProjGnomonicRF::projForward

////////////////////////////////////////////////////////////////////////////////
DgGeoCoord
DgProjGnomonicRF::projInverse (const DgDVec2D& addIn, 
                               const DgEllipsoidRF&) const
//
// spheroid only; needs to be verified at some point
//
{
   DgDVec2D xy(addIn);
   DgGeoCoord lp;

   long double  rh, cosz, sinz;

   rh = hypot(xy.x(), xy.y());
   lp.setLat(atan(rh));
   sinz = sin(lp.lat());
   cosz = sqrt(1. - sinz * sinz);
   if (fabs(rh) <= EPS10) {
      lp.setLat(phi0());
      lp.setLon(0.0L);
   } else {
      switch (mode_) {
      case OBLIQ:
         lp.setLat(cosz * sinph0_ + xy.y() * sinz * cosph0_ / rh);
         if (fabs(lp.lat()) >= 1.0L)
            lp.setLat(lp.lat() > 0.0L ? M_PI_2 : - M_PI_2);
         else
            lp.setLat(asin(lp.lat()));
         xy.setY((cosz - sinph0_ * sin(lp.lat())) * rh);
         xy.setX(xy.x() * sinz * cosph0_);
         break;
      case EQUIT:
         lp.setLat(xy.y() * sinz / rh);
         if (fabs(lp.lat()) >= 1.0L)
            lp.setLat(lp.lat() > 0.0L ? M_PI_2 : - M_PI_2);
         else
            lp.setLat(asin(lp.lat()));
         xy.setY(cosz * rh);
         xy.setX(xy.x() * sinz);
         break;
      case S_POLE:
         lp.setLat(lp.lat() - M_PI_2);
         break;
      case N_POLE:
         lp.setLat(M_PI_2 - lp.lat());
         xy.setY(-xy.y());
         break;
      }
      lp.setLon(atan2(xy.x(), xy.y()));
   }
   return (lp);

} // DgGeoCoord DgProjGnomonicRF::projInverse

////////////////////////////////////////////////////////////////////////////////
