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
// DgProjISEA.cpp: DgProjISEA class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <climits>

#include "DgProjISEA.h"

////////////////////////////////////////////////////////////////////////////////
DgProjISEAInv::DgProjISEAInv (const DgRF<DgProjTriCoord, long double>& from,
                       const DgRF<DgGeoCoord, long double>& to)
         : DgConverter<DgProjTriCoord, long double, DgGeoCoord, long double>(from, to),
           pProjTriRF_ (0)
{
   pProjTriRF_ = dynamic_cast<const DgProjTriRF*>(&fromFrame());

   if (!pProjTriRF_)
   {
      report("DgProjISEAInv::DgProjISEAInv(): "
        " fromFrame not of type DgProjTriRF", DgBase::Fatal);
   }

} // DgProjISEAInv::DgProjISEAInv

////////////////////////////////////////////////////////////////////////////////
DgGeoCoord
DgProjISEAInv::convertTypedAddress (const DgProjTriCoord& addIn) const
{
//cout << "***DgProjISEAInv: DgProjTriCoord: " << addIn << endl;
   IcosaGridPt gridpt;
   gridpt.pt.x = addIn.coord().x();
   gridpt.pt.y = addIn.coord().y();
   gridpt.triangle = addIn.triNum();

//cout << "    gridpt.triangle .x .y: " << gridpt.triangle << ", " <<
//      gridpt.pt.x << ", " << gridpt.pt.y << endl;

   GeoCoord ll = snyderInv(gridpt, projTriRF().sphIcosa().sphIcosa());

//cout << " ll.lon, ll.lat: " << ll.lon << ", " <<
//ll.lat << endl;
   DgGeoCoord geoPt(ll.lon, ll.lat);
   geoPt.normalize();

//cout << "    geoPt: " << geoPt << endl;
   return geoPt;

} // DgGeoCoord DgProjISEAInv::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
DgProjISEAFwd::DgProjISEAFwd (const DgRF<DgGeoCoord, long double>& from,
                    const DgRF<DgProjTriCoord, long double>& to)
         : DgConverter<DgGeoCoord, long double, DgProjTriCoord, long double>(from, to)
{
   pProjTriRF_= dynamic_cast<const DgProjTriRF*>(&toFrame());

   if (!pProjTriRF_)
   {
      report("DgProjISEAFwd::DgProjISEAFwd(): "
        " toFrame not of type DgProjTriRF", DgBase::Fatal);
   }

} // DgProjISEAFwd::DgProjISEAFwd

////////////////////////////////////////////////////////////////////////////////
DgProjTriCoord
DgProjISEAFwd::convertTypedAddress (const DgGeoCoord& addIn) const
{

//cout << "***DgProjISEAFwd: geoPt: " << addIn << endl;
   GeoCoord ll;

   ll.lon = addIn.lon();
   ll.lat = addIn.lat();

//cout << "   ll.lon, ll.lat: " << ll.lon << ", " << ll.lat << endl;

   IcosaGridPt gridpt = snyderFwd(ll, projTriRF().sphIcosa());
//cout << "    gridpt.triangle .x .y: " << gridpt.triangle << ", " <<
//gridpt.pt.x << ", " << gridpt.pt.y << endl;

//cout << "DgProjTriCoord: " << DgProjTriCoord(gridpt.triangle,
//                               DgDVec2D(gridpt.pt.x, gridpt.pt.y)) << endl;

   return DgProjTriCoord(gridpt.triangle, DgDVec2D(gridpt.pt.x, gridpt.pt.y));

} // DgProjTriCoord DgProjISEAFwd::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
   The C++ methods above are wrappers for the C functions below, which were
   written by Lian Song and Kevin Sahr.
*/
////////////////////////////////////////////////////////////////////////////////

static const long double R1 = 0.9103832815L;
static const long double R1S = R1 * R1;

static const long double DH    = 37.37736814L * M_PI_180;
static const long double GH    = 36.0L * M_PI_180;
static const long double cot30 = 1.0L / tanl(30.0L * M_PI_180);
static const long double tanDH = tanl(DH);
static const long double cosDH = cosl(DH);
static const long double sinGH = sinl(GH);
static const long double cosGH = cosl(GH);

static const long double originXOff = 0.6022955029L;
static const long double originYOff = 0.3477354707L;
static const long double icosaEdge = 2.0L * originXOff;

////////////////////////////////////////////////////////////////////////////////
Vec2D sllxy (const GeoCoord& geoVect, SphIcosa& sphico, int nTri)
/*
   ISEA projection: from sphere to plane
*/
{
   long double ph, fh, azh, azh1, dazh, h, dz, z, azh0, ag, cosAzh, sinAzh;
   Vec2D  Planevect;
   const PreCompGeo& cent = sphico.triCen[nTri];

   long double cosLat = cosl(geoVect.lat);
   long double sinLat = sinl(geoVect.lat);

   dazh = sphico.dazh[nTri];

   long double tmp = cent.sinLat * sinLat + cent.cosLat * cosLat *
       cosl(geoVect.lon - cent.pt.lon);
   if (tmp > M_ONE) tmp = M_ONE;
   if (tmp < -M_ONE) tmp = -M_ONE;
   z = acosl(tmp);

   if (z > DH + 0.00000005L)
   {
      dgcout << "nTri: " << nTri << "  z: " << z
             << "  DH+: " << DH + 0.00000005L << "  diff: "
             << ((DH + 0.00000005L) - z) << endl;
      dgcout << "1: The point: ";
      printGeoCoord(geoVect);
      dgcout << " is located on another polygon." << endl;
      report("Unable to continue.", DgBase::Fatal);
   }

   azh = atan2l(cosLat * sinl(geoVect.lon - cent.pt.lon),
         cent.cosLat * sinLat - cent.sinLat * cosLat * cosl(geoVect.lon -
         cent.pt.lon)) - dazh;

   if (azh < 0.0) azh = azh + 2.0 * M_PI;
   azh0 = azh;
   if ((azh >= 120.0 * M_PI_180) && (azh <= 240.0 * M_PI_180)) azh -= 120.0 * M_PI_180;
   if (azh > 240.0 * M_PI_180) azh -= 240.0 * M_PI_180;

   cosAzh = cosl(azh);
   sinAzh = sinl(azh);

   dz = atan2l(tanDH, cosAzh + cot30 * sinAzh);

   if (z > dz + 0.00000005) {
      dgcout << "1: The point: ";
      printGeoCoord(geoVect);
      dgcout << " is located on another polygon." << endl;
      report("Unable to continue.", DgBase::Fatal);
   }

   h = acosl(sinAzh * sinGH * cosDH - cosAzh * cosGH);
   ag = azh + GH + h - 180.0L * M_PI_180;
   azh1 = atan2l(2.0L * ag, R1S * tanDH * tanDH - 2.0L * ag * cot30);
   fh = tanDH / (2.0L * (cosl(azh1) + cot30 * sinl(azh1)) * sinl(dz / 2.0L));
   ph = 2.0L * R1 * fh * sinl(z / 2.0L);

   if ((azh0 >= 120.0L * M_PI_180) && (azh0 < 240.0L * M_PI_180)) azh1 += 120.0L * M_PI_180;
   if (azh0 >= 240.0L * M_PI_180) azh1 += 240L * M_PI_180;

   Planevect.x = (ph * sinl(azh1) + originXOff) / icosaEdge;
   Planevect.y = (ph * cosl(azh1) + originYOff) / icosaEdge;

   return (Planevect);

} /* Vec2D sllxy */

////////////////////////////////////////////////////////////////////////////////
IcosaGridPt snyderFwd (const GeoCoord& ll, DgSphIcosa& sphicosa)
/*
   project the point ll (lat, lon in radius) to the
   plane and return relevant info in IcosaGridPt
*/
{
   IcosaGridPt gridpt;

   gridpt.triangle = sphicosa.whichIcosaTri(ll);

   if (gridpt.triangle < 0)
   {
      dgcout << "ERROR: point in no triangle:";
      printGeoCoord(ll);
      dgcout << endl;

      gridpt.pt.x = M_ZERO;
      gridpt.pt.y = M_ZERO;

      return gridpt;
   }

   gridpt.pt = sllxy(ll, sphicosa.sphIcosa(), gridpt.triangle);

   return gridpt;

} /* IcosaGridPt snyderFwd */

////////////////////////////////////////////////////////////////////////////////
GeoCoord snyderInv (const IcosaGridPt& icosaPt, SphIcosa& sphicosa)
/*
    project the point icosaPt.pt (x, y in ISEA in the coordinate
    system of the specified triangle) to lon, lat in radians

    this replaces the old snyderInv/sxyll combo.
*/
{
  long double ddazh,ph,fh,azh,azh1,dazh,h,fazh,flazh,dz,z;
  long double sinlat,sinlon;
  long double azh0;
  GeoCoord Geovect;
  const PreCompGeo& cent = sphicosa.triCen[icosaPt.triangle];

  Vec2D pt;
  pt.x = icosaPt.pt.x * icosaEdge - originXOff;
  pt.y = icosaPt.pt.y * icosaEdge - originYOff;

  ddazh = sphicosa.dazh[icosaPt.triangle];

  if ((fabsl(pt.x) < PRECISION) && (fabsl(pt.y) < PRECISION))
  {
    Geovect.lat=cent.pt.lat; Geovect.lon=cent.pt.lon;
  }
  else
  {
    ph=sqrtl(pt.x*pt.x+pt.y*pt.y);
    azh1=atan2l(pt.x,pt.y);

    if (azh1<0.0L) azh1=azh1+2*M_PI;
    azh0=azh1;
    if ((azh1>120.0L*M_PI_180) && (azh1<=240.0L*M_PI_180)) azh1=azh1-120.0L*M_PI_180;
    if (azh1>240.0L*M_PI_180) azh1=azh1-240.0L*M_PI_180;

    azh=azh1;

    if (fabsl(azh1) > PRECISION)
    {
       long double agh=R1S*tanDH*tanDH/(2.0L*(1.0L/tanl(azh1)+cot30));

       //cout << "agh: " << agh << endl;

       dazh=1.0;
       while (fabsl(dazh) > PRECISION)
        {
         h=acosl(sinl(azh)*sinGH*cosDH-cosl(azh)*cosGH);
         fazh=agh-azh-GH-h+M_PI;
         flazh=((cosl(azh)*sinGH*cosDH+sinl(azh)*cosGH)/sinl(h))-1.0;
         dazh=-fazh/flazh;
         azh=azh+dazh;
         //cout << "loop: h: " << h << "  fazh: " << fazh*(180.0/M_PI) <<
         //  "  flazh: " << flazh*(180.0/M_PI) <<
         //  "  dazh: " << dazh*(180.0/M_PI) <<
         //  "  azh: " << azh*(180.0/M_PI) << endl;
        }
    }
    else azh = azh1 = 0.0;

    dz=atan2l(tanDH,cosl(azh)+cot30*sinl(azh));
    fh=tanDH/(2.0L*(cosl(azh1)+cot30*sinl(azh1))*sinl(dz/2.0L));
    z=2.0*asinl(ph/(2.0L*R1*fh));
    if ((azh0>=120*M_PI_180) && (azh0<240.0L*M_PI_180)) azh=azh+120*M_PI_180;
    if (azh0>=240.0L*M_PI_180) azh=azh+240.0L*M_PI_180;

    // now reposition to the actual triangle

    azh += ddazh;

    while (azh <= -M_PI) azh += M_2PI;
    while (azh > M_PI) azh -= M_2PI;

    sinlat=cent.sinLat * cosl(z) + cent.cosLat * sinl(z) * cosl(azh);
    if (sinlat > M_ONE) sinlat = M_ONE;
    if (sinlat < -M_ONE) sinlat = -M_ONE;
    Geovect.lat = asinl(sinlat);

    if (fabsl(fabsl(Geovect.lat) - M_PI_2) < M_EPSILON)
    {
       Geovect.lat = (Geovect.lat > M_ZERO) ? M_PI_2 : -M_PI_2;
       Geovect.lon = M_ZERO;
    }
    else
    {
      sinlon = sinl(azh)*sinl(z)/cosl(Geovect.lat);
      long double coslon = (cosl(z) - cent.sinLat * sinl(Geovect.lat)) /
              cent.cosLat/cosl(Geovect.lat);
      if (sinlon > M_ONE) sinlon = M_ONE;
      if (sinlon < -M_ONE) sinlon = -M_ONE;
      if (coslon > M_ONE) coslon = M_ONE;
      if (coslon < -M_ONE) coslon =-M_ONE;
      Geovect.lon = cent.pt.lon+asinl(sinlon);
      Geovect.lon = cent.pt.lon+atan2l(sinlon, coslon);
      if (Geovect.lon <= -M_PI) Geovect.lon += M_2PI;
      if (Geovect.lon >= M_PI) Geovect.lon -= M_2PI;
    }
  }
  return Geovect;

} /* GeoCoord snyderInv */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
