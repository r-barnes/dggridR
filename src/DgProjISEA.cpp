////////////////////////////////////////////////////////////////////////////////
//
// DgProjISEA.cpp: DgProjISEA class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <limits>
#include <iostream>
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
//      gridpt.pt.x << ", " << gridpt.pt.y << endl;

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

static const long double DH    = 37.37736814L * dgM_PI_180;
static const long double GH    = 36.0L * dgM_PI_180;
static const long double cot30 = 1.0L / tan(30.0 * dgM_PI_180);
static const long double tanDH = tan(DH);
static const long double cosDH = cos(DH);
static const long double sinGH = sin(GH);
static const long double cosGH = cos(GH);

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

   long double cosLat = cos(geoVect.lat);
   long double sinLat = sin(geoVect.lat);

   dazh = sphico.dazh[nTri];

   z = acos(cent.sinLat * sinLat + cent.cosLat * cosLat * 
       cos(geoVect.lon - cent.pt.lon));

   if (z > DH + 0.00000005)
   {
      // std::cout<<"nTri: "<<nTri
      //          <<"z:    "<<z
      //          <<"DH+:  "<<(DH + 0.00000005)
      //          <<"diff: "<<((DH + 0.00000005) - z)
      //          <<"\n";
      // std::cout<<"1: The point: ";
      // printGeoCoord(geoVect);
      // std::cout<<" is located on another polygon.\n";
      throw std::runtime_error("sllxy found a point in another polygon!");
   }

   azh = atan2(cosLat * sin(geoVect.lon - cent.pt.lon), 
         cent.cosLat * sinLat - cent.sinLat * cosLat * cos(geoVect.lon - 
         cent.pt.lon)) - dazh;

   if (azh < 0.0) azh = azh + 2.0 * dgM_PI;
   azh0 = azh;
   if ((azh >= 120.0 * dgM_PI_180) && (azh <= 240.0 * dgM_PI_180)) azh -= 120.0 * dgM_PI_180;
   if (azh > 240.0 * dgM_PI_180) azh -= 240.0 * dgM_PI_180;

   cosAzh = cos(azh);
   sinAzh = sin(azh);

   dz = atan2(tanDH, cosAzh + cot30 * sinAzh);

   if (z > dz + 0.00000005)
   {
      // printf("2: The point: ");
      // printGeoCoord(geoVect);
      // printf(" is located on another polygon.\n");
      throw std::runtime_error("sllxy found a point located in another polygon!");
      // exit(1);
   }

   h = acos(sinAzh * sinGH * cosDH - cosAzh * cosGH);
   ag = azh + GH + h - 180.0 * dgM_PI_180;
   azh1 = atan2(2.0 * ag, R1S * tanDH * tanDH - 2.0 * ag * cot30);
   fh = tanDH / (2.0 * (cos(azh1) + cot30 * sin(azh1)) * sin(dz / 2.0));
   ph = 2.0 * R1 * fh * sin(z / 2.0);

   if ((azh0 >= 120.0 * dgM_PI_180) && (azh0 < 240.0 * dgM_PI_180)) azh1 += 120.0 * dgM_PI_180;
   if (azh0 >= 240.0 * dgM_PI_180) azh1 += 240 * dgM_PI_180;

   Planevect.x = (ph * sin(azh1) + originXOff) / icosaEdge;
   Planevect.y = (ph * cos(azh1) + originYOff) / icosaEdge;

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
      // printf("ERROR: point in no triangle:");
      // printGeoCoord(ll); 
      // printf("\n");

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

  if ((fabs(pt.x) < PRECISION) && (fabs(pt.y) < PRECISION))
  {
    Geovect.lat=cent.pt.lat; Geovect.lon=cent.pt.lon;
  }
  else
  {  
    ph=sqrt(pt.x*pt.x+pt.y*pt.y);
    azh1=atan2(pt.x,pt.y);

    if (azh1<0.0) azh1=azh1+2*dgM_PI;
    azh0=azh1;
    if ((azh1>120.0*dgM_PI_180) && (azh1<=240.0*dgM_PI_180)) azh1=azh1-120.0*dgM_PI_180;
    if (azh1>240.0*dgM_PI_180) azh1=azh1-240.0*dgM_PI_180;

    azh=azh1;

    if (fabs(azh1) > PRECISION) 
    {
       long double agh=R1S*tanDH*tanDH/(2.0*(1.0/tan(azh1)+cot30));

       //cout << "agh: " << agh << endl;

       dazh=1.0; 
       while (fabs(dazh) > PRECISION)
        {
         h=acos(sin(azh)*sinGH*cosDH-cos(azh)*cosGH);
         fazh=agh-azh-GH-h+dgM_PI;
         flazh=((cos(azh)*sinGH*cosDH+sin(azh)*cosGH)/sin(h))-1.0;      
         dazh=-fazh/flazh;
         azh=azh+dazh;
         //cout << "loop: h: " << h << "  fazh: " << fazh*(180.0/dgM_PI) << 
         //  "  flazh: " << flazh*(180.0/dgM_PI) <<
         //  "  dazh: " << dazh*(180.0/dgM_PI) << 
         //  "  azh: " << azh*(180.0/dgM_PI) << endl;
        }   
    }
    else azh = azh1 = 0.0;

    dz=atan2(tanDH,cos(azh)+cot30*sin(azh));
    fh=tanDH/(2.0*(cos(azh1)+cot30*sin(azh1))*sin(dz/2.0));
    z=2.0*asin(ph/(2.0*R1*fh));
    if ((azh0>=120*dgM_PI_180) && (azh0<240*dgM_PI_180)) azh=azh+120*dgM_PI_180;
    if (azh0>=240*dgM_PI_180) azh=azh+240*dgM_PI_180;

    // now reposition to the actual triangle

    azh += ddazh;

    while (azh <= -dgM_PI) azh += M_2PI;
    while (azh > dgM_PI) azh -= M_2PI;

    sinlat=cent.sinLat * cos(z) + cent.cosLat * sin(z) * cos(azh);
    if (sinlat > M_ONE) sinlat = M_ONE;
    if (sinlat < -M_ONE) sinlat = -M_ONE;
    Geovect.lat = asin(sinlat);

    if (fabs(fabs(Geovect.lat) - dgM_PI_2) < M_EPSILON)
    {
       Geovect.lat = (Geovect.lat > M_ZERO) ? dgM_PI_2 : -dgM_PI_2;
       Geovect.lon = M_ZERO;
    }
    else
    {
      sinlon = sin(azh)*sin(z)/cos(Geovect.lat);
      long double coslon = (cos(z) - cent.sinLat * sin(Geovect.lat)) /
              cent.cosLat/cos(Geovect.lat);
      if (sinlon > M_ONE) sinlon = M_ONE;
      if (sinlon < -M_ONE) sinlon = -M_ONE;
      if (coslon > M_ONE) coslon = M_ONE;
      if (coslon < -M_ONE) coslon =-M_ONE;
      Geovect.lon = cent.pt.lon+asin(sinlon);
      Geovect.lon = cent.pt.lon+atan2(sinlon, coslon);
      if (Geovect.lon <= -dgM_PI) Geovect.lon += M_2PI;
      if (Geovect.lon >= dgM_PI) Geovect.lon -= M_2PI;
    }
  }
  return Geovect;

} /* GeoCoord snyderInv */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
