////////////////////////////////////////////////////////////////////////////////
//
// DgProjTriRF.cpp: DgProjTriRF class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgProjTriRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgSphIcosa::DgSphIcosa (const DgGeoCoord& vert0, long double azimuthDegs)
{
   sphIcosa_.pt.lon = vert0.lon();
   sphIcosa_.pt.lat = vert0.lat();
   sphIcosa_.azimuth = azimuthDegs * M_PI/180;

   ico12verts();

} // DgSphIcosa::DgSphIcosa
      
////////////////////////////////////////////////////////////////////////////////
ostream& operator<< (ostream& str, const DgSphIcosa& dgsi) 
{ 
   const SphIcosa& si = dgsi.sphIcosa_;

   DgGeoCoord tmp(si.pt);
   str << "vert0: " << tmp << endl;
   str << "az0: " << si.azimuth * 180.0 / M_PI << endl;

   str << "vertices:\n";
   str << "{\n";
   for (int i = 0; i < 12; i++)
   {
      DgGeoCoord tmp(si.icoverts[i]);
      str << "  " << i << ": " << tmp << endl;
   }
   str << "}\n";

   str << "triangles:\n";
   str << "{\n";
   for (int i = 0; i < 20; i++)
   {
      str << "  " << i;
      for (int j = 0; j < 3; j++)
      { 
         DgGeoCoord tmp(si.icotri[i][j]);
         str << " " << tmp;
      }
      str << endl;
   }
   str << "}\n";

   return str;

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
GeoCoord coordtrans(const GeoCoord& newNPold, const GeoCoord& ptold, 
                    long double lon0)
/* return the new coordinates of any point in orginal coordinate system.
   Define a point (newNPold) in orginal coordinate system as the North 
   Pole in new coordinate system, and the great circle connect the original 
   and new North Pole as the lon0 longitude in new coordinate system, given
   any point in orginal coordinate system, this function return the new
   coordinates. */

 {
  long double cosptnewlat, cosptnewlon;
  GeoCoord ptnew;

  cosptnewlat = sin(newNPold.lat)*sin(ptold.lat) + 
                cos(newNPold.lat)*cos(ptold.lat)*cos(newNPold.lon-ptold.lon);
  if (cosptnewlat>1.) cosptnewlat=1.0;
  if (cosptnewlat<-1.) cosptnewlat=-1.0;
  ptnew.lat = acos(cosptnewlat);
  if (fabs(ptnew.lat-0.) < PRECISION*100000)
      ptnew.lon=0.; 
  else if (fabs(ptnew.lat-M_PI) < PRECISION*100000)
      ptnew.lon=0.; 
  else
   {
    cosptnewlon = (sin(ptold.lat)*cos(newNPold.lat) - cos(ptold.lat)*
                  sin(newNPold.lat)*cos(newNPold.lon-ptold.lon))/sin(ptnew.lat);
    if (cosptnewlon>1.) cosptnewlon=1.0;
    if (cosptnewlon<-1.) cosptnewlon=-1.0;
    ptnew.lon = acos(cosptnewlon);
    if ((ptold.lon-newNPold.lon)>=0 && (ptold.lon-newNPold.lon) < M_PI)
      ptnew.lon=-ptnew.lon+lon0;
    else ptnew.lon=ptnew.lon+lon0;
    if (ptnew.lon>M_PI) ptnew.lon -= 2*M_PI;
    if (ptnew.lon<-M_PI) ptnew.lon += 2*M_PI;
   }
  ptnew.lat = M_PI/2-ptnew.lat;
  return ptnew;
 }

////////////////////////////////////////////////////////////////////////////////
int 
DgSphIcosa::whichIcosaTri (const GeoCoord& pt)
/*
   Return the index of the icosahedron triangle in which pt occurs.

   The guts of the ptinsphtri function have been in-lined here and converted
   to make use of pre-computed information (Kevin Sahr).
*/
{
   int i;
   long double p0;
   Vec3D pp = llxyz(pt);

   for (i = 0; i < 20; i++)
   {
      const PreCompInTri& intri = sphIcosa().ptin[i];

      p0 = pp.x * intri.p0x0 - pp.y * intri.p0y0 + pp.z * intri.p0z0;

      if (p0 * intri.t00 < 0.0) continue;
      
      p0 = pp.x * intri.p0x1 - pp.y * intri.p0y1 + pp.z * intri.p0z1;

      if (p0 * intri.t01 < 0.0) continue;
      
      p0 = pp.x * intri.p0x2 - pp.y * intri.p0y2 + pp.z * intri.p0z2;

      if (p0 * intri.t02 < 0.0) continue;
      
      /* if we're here we're still in the triangle */
     
      return i;

   }

   /* if we get here something is wrong */

   return -1;

} /* int DgSphIcosa::whichIcosaTri */

////////////////////////////////////////////////////////////////////////////////
void 
DgSphIcosa::ico12verts (void)
/* 
   Fill in the icosahedron particulars given one point and one edge's azimuth.
*/
{
   GeoCoord newnpold, vertsnew[12];
   int i, j;
   Vec3D ptri[3];

   static int verts[20][3] = {

	   {  0,  1,  2 },   /*  0 */
	   {  0,  2,  3 },   /*  1 */
	   {  0,  3,  4 },   /*  2 */
           {  0,  4,  5 },   /*  3 */
	   {  0,  5,  1 },   /*  4 */
	   {  6,  2,  1 },   /*  5 */
           {  7,  3,  2 },   /*  6 */
           {  8,  4,  3 },   /*  7 */
           {  9,  5,  4 },   /*  8 */
           { 10,  1,  5 },   /*  9 */
           {  2,  6,  7 },   /* 10 */
           {  3,  7,  8 },   /* 11 */
           {  4,  8,  9 },   /* 12 */
           {  5,  9, 10 },   /* 13 */
           {  1, 10,  6 },   /* 14 */
           { 11,  7,  6 },   /* 15 */
           { 11,  8,  7 },   /* 16 */
           { 11,  9,  8 },   /* 17 */
           { 11, 10,  9 },   /* 18 */
           { 11,  6, 10 }    /* 19 */
   };

   newnpold.lat = sphIcosa().pt.lat;
   newnpold.lon = 0.0;
   for (i = 1; i <= 5; i++) 
   {
     vertsnew[i].lat = 26.565051177 * M_PI / 180.0;
     vertsnew[i].lon = -sphIcosa().azimuth + 72 * (i - 1) * M_PI / 180.0;
     if (vertsnew[i].lon > M_PI-PRECISION) vertsnew[i].lon -= 2 * M_PI;
     if (vertsnew[i].lon < -(M_PI+PRECISION)) vertsnew[i].lon += 2 * M_PI;
     vertsnew[i+5].lat = -26.565051177 * M_PI / 180;
     vertsnew[i+5].lon = 
              -sphIcosa().azimuth + (36.0 + 72.0 * (i - 1)) * M_PI / 180.0;
     if (vertsnew[i + 5].lon > M_PI - PRECISION) 
     {
        vertsnew[i + 5].lon -= 2 * M_PI;
     }
     if (vertsnew[i+5].lon < -(M_PI+PRECISION)) vertsnew[i+5].lon += 2 * M_PI;
   }
   vertsnew[11].lat = -90.0 * M_PI / 180.0;
   vertsnew[11].lon = 0.0 * M_PI / 180.0;
   sphIcosa().icoverts[0].lat = sphIcosa().pt.lat;
   sphIcosa().icoverts[0].lon = sphIcosa().pt.lon;
/***** hardwire for bug test ******/

/*
vertsnew[0].lat = 90.0 * M_PI / 180.0;
vertsnew[0].lon = 0.0 * M_PI / 180.0;
for (i = 0; i < 12; i++) 
{
   sphIcosa().icoverts[i].lat = vertsnew[i].lat; 
   sphIcosa().icoverts[i].lon = vertsnew[i].lon; 
}
*/

/**********************************/
   for (i = 1; i < 12; i++) 
   {
      sphIcosa().icoverts[i] = 
                   coordtrans(newnpold, vertsnew[i], sphIcosa().pt.lon);
   }

   /* calculate the triangle-specific values */

   for (i = 0; i < 20; i++)
   {
      /* set the vertices */

      for (j = 0; j < 3; j++) 
      {
         sphIcosa().icotri[i][j] = sphIcosa().icoverts[verts[i][j]];
      }

      /* pre-calculate the center point values */

      sphIcosa().triCen[i].pt = sphTricenpoint(sphIcosa().icotri[i]);
      sphIcosa().triCen[i].sinLat = sin(sphIcosa().triCen[i].pt.lat);
      sphIcosa().triCen[i].sinLon = sin(sphIcosa().triCen[i].pt.lon);
      sphIcosa().triCen[i].cosLat = cos(sphIcosa().triCen[i].pt.lat);
      sphIcosa().triCen[i].cosLon = cos(sphIcosa().triCen[i].pt.lon);

      /* pre-calculate the dazh's */

      const GeoCoord* t = sphIcosa().icotri[i];
      const PreCompGeo& c = sphIcosa().triCen[i];
      sphIcosa().dazh[i] = atan2(cos(t[0].lat) * sin(t[0].lon - c.pt.lon),
                  c.cosLat * sin(t[0].lat) - c.sinLat * cos(t[0].lat) *
                  cos(t[0].lon - c.pt.lon));

/*
      sphIcosa().dazh[i] = atan2(cos(sphIcosa().icotri[i][0].lat) * 
                  sin(sphIcosa().icotri[i][0].lon - sphIcosa().triCen[i].pt.lon),
                  sphIcosa().triCen[i].cosLat * sin(sphIcosa().icotri[i][0].lat) -
                  sphIcosa().triCen[i].sinLat * cos(sphIcosa().icotri[i][0].lat) *
                  cos(sphIcosa().icotri[i][0].lon - sphIcosa().triCen[i].pt.lon));
*/

      /* pre-calculate the pt-in-triangle constants */

      for (j = 0; j < 3; j++) ptri[j] = llxyz(sphIcosa().icotri[i][j]);

      sphIcosa().ptin[i].p0x0 = ptri[1].y * ptri[2].z - ptri[2].y * ptri[1].z;
      sphIcosa().ptin[i].p0y0 = ptri[1].x * ptri[2].z - ptri[2].x * ptri[1].z;
      sphIcosa().ptin[i].p0z0 = ptri[1].x * ptri[2].y - ptri[2].x * ptri[1].y;

      sphIcosa().ptin[i].t00 = ptri[0].x * sphIcosa().ptin[i].p0x0 -
                              ptri[0].y * sphIcosa().ptin[i].p0y0 +
                              ptri[0].z * sphIcosa().ptin[i].p0z0;

      sphIcosa().ptin[i].p0x1 = ptri[0].y * ptri[2].z - ptri[2].y * ptri[0].z;
      sphIcosa().ptin[i].p0y1 = ptri[0].x * ptri[2].z - ptri[2].x * ptri[0].z;
      sphIcosa().ptin[i].p0z1 = ptri[0].x * ptri[2].y - ptri[2].x * ptri[0].y;

      sphIcosa().ptin[i].t01 = ptri[1].x * sphIcosa().ptin[i].p0x1 -
                              ptri[1].y * sphIcosa().ptin[i].p0y1 +
                              ptri[1].z * sphIcosa().ptin[i].p0z1;

      sphIcosa().ptin[i].p0x2 = ptri[0].y * ptri[1].z - ptri[1].y * ptri[0].z;
      sphIcosa().ptin[i].p0y2 = ptri[0].x * ptri[1].z - ptri[1].x * ptri[0].z;
      sphIcosa().ptin[i].p0z2 = ptri[0].x * ptri[1].y - ptri[1].x * ptri[0].y;

      sphIcosa().ptin[i].t02 = ptri[2].x * sphIcosa().ptin[i].p0x2 -
                              ptri[2].y * sphIcosa().ptin[i].p0y2 +
                              ptri[2].z * sphIcosa().ptin[i].p0z2;

   }

/*
   for (int i = 0; i < 20; i++) 
   {
      printf("%d\n", i);
      for (int j = 0; j < 3; j++) printGeoCoord(sphIcosa().icotri[i][j]);
      printGeoCoord(sphIcosa().triCen[i].pt);
      printf("\n");
   }
*/

} /* void ico12verts */

////////////////////////////////////////////////////////////////////////////////
const char*
DgProjTriRF::str2add (DgProjTriCoord* add, const char* str, char delimiter) 
                                                                          const
{
   if (!add) add = new DgProjTriCoord();

   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   // get the triNum

   char* tok = strtok(tmpStr, delimStr);
   int t;
   if (sscanf(tok, "%d", &t) != 1)
   {
      ::report("DgProjTriRF::fromString() invalid value in string " +
               string(tok), DgBase::Fatal);
   }

   const char* tmp = &(str[strlen(tok) + 1]);
   DgDVec2D vec;
   tmp = vec.fromString(tmp, delimiter);

   *add = DgProjTriCoord(t, vec);

   return tmp;

} // const char* DgProjTriRF::str2add
