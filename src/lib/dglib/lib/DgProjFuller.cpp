////////////////////////////////////////////////////////////////////////////////
//
// DgProjFuller.cpp: DgProjFuller class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <limits>
#include "DgProjFuller.h"

////////////////////////////////////////////////////////////////////////////////
DgProjFullerInv::DgProjFullerInv (const DgRF<DgProjTriCoord, long double>& from,
                       const DgRF<DgGeoCoord, long double>& to)
         : DgConverter<DgProjTriCoord, long double, DgGeoCoord, long double>(from, to),
           pProjTriRF_ (0)
{ 
   pProjTriRF_ = dynamic_cast<const DgProjTriRF*>(&fromFrame());

   if (!pProjTriRF_)
   {
      report("DgProjFullerInv::DgProjFullerInv(): "
        " fromFrame not of type DgProjTriRF", DgBase::Fatal);
   }

} // DgProjFullerInv::DgProjFullerInv

////////////////////////////////////////////////////////////////////////////////
DgGeoCoord 
DgProjFullerInv::convertTypedAddress (const DgProjTriCoord& addIn) const
{
//cout << "***DgProjFullerInv: DgProjTriCoord: " << addIn << endl;
   IcosaGridPt gridpt;
   gridpt.pt.x = addIn.coord().x();
   gridpt.pt.y = addIn.coord().y();
   gridpt.triangle = addIn.triNum();

//cout << "    gridpt.triangle .x .y: " << gridpt.triangle << ", " <<
//      gridpt.pt.x << ", " << gridpt.pt.y << endl;

   GeoCoord ll = fullerInv(gridpt, projTriRF().sphIcosa().sphIcosa());

//cout << " ll.lon, ll.lat: " << ll.lon << ", " << 
//ll.lat << endl;
   DgGeoCoord geoPt(ll.lon, ll.lat);
   geoPt.normalize();

//cout << "    geoPt: " << geoPt << endl;
   return geoPt;

} // DgGeoCoord DgProjFullerInv::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
DgProjFullerFwd::DgProjFullerFwd (const DgRF<DgGeoCoord, long double>& from,
                    const DgRF<DgProjTriCoord, long double>& to)
         : DgConverter<DgGeoCoord, long double, DgProjTriCoord, long double>(from, to) 
{ 
   pProjTriRF_= dynamic_cast<const DgProjTriRF*>(&toFrame());

   if (!pProjTriRF_)
   {
      report("DgProjFullerFwd::DgProjFullerFwd(): "
        " toFrame not of type DgProjTriRF", DgBase::Fatal);
   }

} // DgProjFullerFwd::DgProjFullerFwd 

////////////////////////////////////////////////////////////////////////////////
DgProjTriCoord 
DgProjFullerFwd::convertTypedAddress (const DgGeoCoord& addIn) const
{
         
//cout << "***DgProjFullerFwd: geoPt: " << addIn << endl;
         GeoCoord ll;

   ll.lon = addIn.lon();
   ll.lat = addIn.lat();

//cout << "   ll.lon, ll.lat: " << ll.lon << ", " << ll.lat << endl;

   IcosaGridPt gridpt = fullerFwd(ll, projTriRF().sphIcosa());
//cout << "    gridpt.triangle .x .y: " << gridpt.triangle << ", " <<
//      gridpt.pt.x << ", " << gridpt.pt.y << endl;

//cout << "DgProjTriCoord: " << DgProjTriCoord(gridpt.triangle, 
//                               DgDVec2D(gridpt.pt.x, gridpt.pt.y)) << endl;

   return DgProjTriCoord(gridpt.triangle, DgDVec2D(gridpt.pt.x, gridpt.pt.y));

} // DgProjTriCoord DgProjFullerFwd::convertTypedAddress 

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/* 
   The C++ methods above are wrappers for the C functions below.
*/
////////////////////////////////////////////////////////////////////////////////

const long double edgeScale = M_ATAN2;
const long double originXOff = M_ATAN2_2; // edgeScale/2;
const long double originYOff = 0.3196063051256835802821381404218L;
                           //originXOff / M_SQRT3;

////////////////////////////////////////////////////////////////////////////////
IcosaGridPt fullerFwd (const GeoCoord& ll, DgSphIcosa& sphicosa)
/*
   project the point ll (lat, lon in radius) to the
   plane and return relevant info in IcosaGridPt
*/
{
   IcosaGridPt gridpt;

   gridpt.triangle = sphicosa.whichIcosaTri(ll);

   if (gridpt.triangle < 0)
   {
      printf("ERROR: point in no triangle:");
      printGeoCoord(ll); 
      printf("\n");

      gridpt.pt.x = M_ZERO;
      gridpt.pt.y = M_ZERO;

      return gridpt;
   }

   //gridpt.pt = fllxy(ll, sphicosa, gridpt.triangle);

   long double v1[3], v2[3], v3[3];
   
   v1[0] = sphicosa.sphIcosa().icotri[gridpt.triangle][0].lon;
   v1[1] = sphicosa.sphIcosa().icotri[gridpt.triangle][0].lat;

   v2[0] = sphicosa.sphIcosa().icotri[gridpt.triangle][1].lon;
   v2[1] = sphicosa.sphIcosa().icotri[gridpt.triangle][1].lat;

   v3[0] = sphicosa.sphIcosa().icotri[gridpt.triangle][2].lon;
   v3[1] = sphicosa.sphIcosa().icotri[gridpt.triangle][2].lat;

   v1[2] = v2[2] = v3[2] = M_ZERO;

   gridpt.pt = fullerFwdOneTri(ll, M_ONE, v1, v2, v3);
   gridpt.pt.x = (gridpt.pt.x + originXOff) / edgeScale;
   gridpt.pt.y = (gridpt.pt.y + originYOff) / edgeScale;

   return gridpt;

} /* IcosaGridPt fullerFwd */

////////////////////////////////////////////////////////////////////////////////
GeoCoord fullerInv (const IcosaGridPt& icosaPt, SphIcosa& sphicosa)
/*
    project the point icosaPt.pt (x, y in Fuller in the coordinate
    system of the specified triangle) to lon, lat in radians

    this replaces the old fullerInv/sxyll combo.
*/
{
  long double ddazh;
  long double sinlat,sinlon;
  GeoCoord Geovect;
  const PreCompGeo& cent = sphicosa.triCen[icosaPt.triangle];
 
  IcosaGridPt iPt;
  iPt.triangle = icosaPt.triangle;
  iPt.pt.x = icosaPt.pt.x * edgeScale - originXOff;
  iPt.pt.y = icosaPt.pt.y * edgeScale - originYOff;

//printf("   iPt.triangle .x .y: %d %.15lf %.15lf\n", iPt.triangle,
    //iPt.pt.x, iPt.pt.y);

  ddazh = sphicosa.dazh[icosaPt.triangle];

  if ((fabs(iPt.pt.x) < PRECISION) && (fabs(iPt.pt.y) < PRECISION))
  {
    Geovect.lat=cent.pt.lat; Geovect.lon=cent.pt.lon;
  }
  else
  {  
    long double azh, theta;
    Geovect = fullerInvOneTri(iPt, M_ONE, &azh, &theta);

    azh = dgM_PI - azh; // 180' - azh

    // now reposition to the actual triangle

    azh += ddazh;

    while (azh <= -dgM_PI) azh += M_2PI;
    while (azh > dgM_PI) azh -= M_2PI;

    sinlat=cent.sinLat * cos(theta) + cent.cosLat * sin(theta) * cos(azh);
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
      sinlon = sin(azh)*sin(theta)/cos(Geovect.lat);
      long double coslon = (cos(theta) - cent.sinLat * sin(Geovect.lat)) /
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

} /* GeoCoord fullerInv */

void geogtocartesian (long double lon, long double lat, long double R, long double ret[])
{
   long double inc = dgM_PI_2 - lat;
   long double azi = lon + dgM_PI;

   ret[0] = R * cos(azi) * sin(inc);
   ret[1] = R * sin(azi) * sin(inc);
   ret[2] = R * cos(inc);

} // static void geogtocartesian

long double scalar_triple(long double * x, long double * y, long double * z)
{
   long double yzcross[3];

   yzcross[0] = y[1]*z[2] - y[2]*z[1];
   yzcross[1] = y[2]*z[0] - y[0]*z[2];
   yzcross[2] = y[0]*z[1] - y[1]*z[0];

   long double xyztrip = x[0] * yzcross[0] + x[1]*yzcross[1] + x[2]*yzcross[2];

   return xyztrip;

} // static long double scalar_triple

Vec2D fullerFwdOneTri (const GeoCoord geo, long double R, long double * v1, 
                       long double * v2, long double * v3)
{
   const long double cosarc = 0.447213595499957939281834733746255L; // cos(atan(2))

   long double ret [3];
   long double ret2 [3];
   long double ret3 [3];
   long double ret4 [3];

   geogtocartesian(geo.lon, geo.lat, R, ret);
   geogtocartesian(v1[0], v1[1], R, ret2);
   geogtocartesian(v2[0], v2[1], R, ret3);
   geogtocartesian(v3[0], v3[1], R, ret4);

   long double v1v2p = scalar_triple(ret2, ret3, ret);
   long double v1v3p = scalar_triple(ret2, ret4, ret);
   long double v2v1p = scalar_triple(ret3, ret2, ret);
   long double v2v3p = scalar_triple(ret3, ret4, ret);
   long double v3v1p = scalar_triple(ret4, ret2, ret);
   long double v3v2p = scalar_triple(ret4, ret3, ret);
   
   long double a1 = atan((2.0L*v2v3p*cosarc) / (v2v3p*cosarc-(v2v1p+v1v3p))); 
   long double a2 = atan((2.0L*v1v2p*cosarc) / (v1v2p*cosarc - (v1v3p + v3v2p)));
   long double a3 = atan((2.0L*v3v1p*cosarc) / (v3v1p*cosarc - (v3v2p + v2v1p)));

   long double x = R * (a2-a3)/2.0L;
   long double y = R * (2.0L * a1 - a2 - a3)/(2.0L * M_SQRT3);

   //IcosaGridPt p;
   Vec2D pt;
   pt.x = x;
   pt.y = y;

   return pt;

} // Vec2D fullerFwdOneTri

GeoCoord fullerInvOneTri(const IcosaGridPt pt, long double R, long double* pAzimuth, 
                         long double* pTheta)
{
   long double arc, alpha, el, dve; /* constants used with Fuller projection*/
   long double x, y, z;             /* 3-D x, y, z coords of output  */
   long double x1, y1, z1;        /* 3-D x, y, z coords of template triangle plane*/
   long double x2, y2;              /* initial planar coords*/
   long double a1, a2, a3;          /* arc lengths on plane and sphere*/
   long double a1p, a2p;            /* arc lengths on template triangle plane*/ 
   long double s;                   /* length from unit sphere origin to plane*/
   long double group1, group2;      /* grouping of constants for use in finding a2*/
   long double inc;                 /* increment to obtain an accurate a2*/
   long double theta, azimuth;      /* conversion angles from x,y,z to lat/lon*/
   long double test, answer;        /* compared values for use in finding a2*/

   //el = M_SQRT8 / sqrt(5.0L + M_SQRT5);
   el = 1.05146222423826721205133816969575L;
   //dve = sqrt(3.0L + M_SQRT5) / sqrt(5.0L + M_SQRT5);
   dve = 0.85065080835203993218154049706301L;
   //arc = 2.0L * asin (sqrt (5.0L - M_SQRT5) / M_SQRT10);
   arc = M_ATAN2;
   //alpha = 0.5L * arc;
   alpha = M_ATAN2_2;
   
   x2 = pt.pt.x / R;
   y2 = pt.pt.y / R;
   
   group1 = (M_SQRT3 * y2) - x2 - alpha;
   group2 = (2.0L * x2) + alpha;
   //answer = -el / (2.0L * dve);
   answer = -0.6180339887498948482045868343656L;
   test = M_ONE;

   inc = alpha;
   a2 = inc;

   long double hi = answer-test;
   long double lastHi = hi;
   while (fabs(hi) > PRECISION) 
   { 
      test = tan (a2 + group1) + tan (a2 - alpha) + tan (a2 - group2);
      inc = inc / 2.0L;
   
      hi = answer - test;
      if (hi == lastHi)
      {
         fprintf(stderr, "ERROR: fullerInvOneTri: iterative step failed.\n");
         fprintf(stderr, "loss of precision: %.20LF.\n", (fabs(hi) - PRECISION));
         break;
      }
      lastHi = hi;

      if (fabs(hi) < PRECISION) break;

      if (test > answer)
         a2 = a2 - inc;
      else
         a2 = a2 + inc;
   }

   a3 = a2 - (2.0L * x2);
   a1 = (M_SQRT3 * y2) + (M_HALF * a2) + (M_HALF * a3);

   a1p = dve * tan (a1 - alpha) + (el / 2.0L);
   a2p = dve * tan (a2 - alpha) + (el / 2.0L);

   /* Find x1, y1 and z1 on template planar triangle*/

   //y1 = a1p * sin (60.0L * dgM_PI_180) - (el / (2.0L * M_SQRT3));
   y1 = a1p * M_SIN60 - 0.30353099910334311154769579070999L;
   //x1 = a2p + (y1 / M_SQRT3) - (el / 3.0L);
   x1 = a2p + (y1 / M_SQRT3) - 0.3504874080794224040171127232319L;
   //z1 = sqrt (5.0L + (2.0L * M_SQRT5)) / M_SQRT15;
   z1 = 0.79465447229176612295553092832759L;

   s = sqrt(x1 * x1 + y1 * y1 + z1 * z1);

   x = x1 / s;
   y = y1 / s;
   z = z1 / s;
   test = sqrt(x*x + y*y + z*z);

   //printf("XYZ: %f, %f, %f\n", x, y, z);

   /* Convert x, y, z coords to lat/lon*/

   theta = acos (z);
   azimuth = atan2 (y, x);
   long double j = dgM_PI / 2.0L;
   if (azimuth <= j )
      azimuth = azimuth + dgM_PI_2;
   else
   {
      //azimuth = azimuth - 3.0L * dgM_PI / 2.0L;
      azimuth -= 4.712388980384689857693965074919L;
   }

   *pTheta = theta;
   *pAzimuth = azimuth;
   
   long double lat = (dgM_PI_2 - theta) * M_180_PI;
   long double lon = azimuth;
   if (fabs (dgM_PI_2 - lat) < PRECISION)
      lon = M_ZERO;

   lon = lon * M_180_PI;

   GeoCoord geo;
   geo.lon = lon;
   geo.lat = lat;
   
   return geo;

} // GeoCoord fullerInvOneTri

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


