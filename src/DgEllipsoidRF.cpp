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
// DgEllipsoidRF.cpp: DgEllipsoidRF class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgEllipsoidRF.h"
#include "DgGeoSphRF.h"
#include "DgPolygon.h"

const DgGeoCoord DgGeoCoord::undefGeoCoord(LDBL_MAX, LDBL_MAX);
const long double DgGeoCoord::tolerance = 0.0000000005L;

////////////////////////////////////////////////////////////////////////////////
long double
DgGeoCoord::gcDist (const DgGeoCoord& g1, const DgGeoCoord& g2,
                    bool rads)
/*
   return great circle distance in radians between g1 and g2.

   CRC Math, 1991, pp. 129-130.

   Kevin Sahr, 3/10/99
*/
{
   // use spherical triangle with g1 at A, g2 at B, and north pole at C

   long double bigC = fabsl(g2.lon() - g1.lon());

   if (bigC > M_PI) // assume we want the complement
   {
      // note that in this case they can't both be negative

      long double lon1 = g1.lon();
      if (lon1 < 0.0L) lon1 += 2.0L * M_PI;
      long double lon2 = g2.lon();
      if (lon2 < 0.0L) lon2 += 2.0L * M_PI;

      bigC = fabsl(lon2 - lon1);
   }

   long double b = M_PI_2 - g1.lat();
   long double a = M_PI_2 - g2.lat();

   // use law of cosines to find c

   long double cosc = cosl(a) * cosl(b) + sinl(a) * sinl(b) * cosl(bigC);
   if (cosc > 1.0L) cosc = 1.0L;
   if (cosc < -1.0L) cosc = -1.0L;

   long double retVal = acosl(cosc);

   if (!rads) retVal *= M_180_PI;

   return retVal;

} // long double DgGeoCoord::gcDist

////////////////////////////////////////////////////////////////////////////////
DgGeoCoord
DgGeoCoord::gcIntersect (const DgGeoCoord& g11, const DgGeoCoord& g12,
          const DgGeoCoord& g21, const DgGeoCoord& /* g22 */)
/*
   Return point of intersection of the two great circle arc segments g11-g12
   and g21-g22.

   Works by calling Lian Song's routine GCintersect.
*/
{
   GeoCoord sv11, sv12, sv21;
   sv11.lon = g11.lon(); sv11.lat = g11.lat();
   sv12.lon = g12.lon(); sv12.lat = g12.lat();
   sv21.lon = g21.lon(); sv21.lat = g21.lat();
   //sv22.lon = g22.lon(); sv22.lat = g22.lat();

   GeoCoord ans = GCintersect(sv11, sv12, sv21, sv11, 1);

   return DgGeoCoord(ans.lon, ans.lat);

} // DgGeoCoord DgGeoCoord::gcIntersect

////////////////////////////////////////////////////////////////////////////////
long double
DgGeoCoord::geoTriArea (const DgGeoCoord& g1, const DgGeoCoord& g2,
                        const DgGeoCoord& g3)
/*
   return area in radians.

   CRC Math, 1991, pp. 129-130.

   Kevin Sahr, 3/10/99
*/
{
   // determine the edges

   long double a = DgGeoCoord::gcDist(g2, g3);
   long double b = DgGeoCoord::gcDist(g1, g3);
   long double c = DgGeoCoord::gcDist(g1, g2);

   // determine the angles using half-angle formulas

   long double s = (a + b + c) / 2.0L;

   long double sinsa = sinl(s - a);
   long double sinsb = sinl(s - b);
   long double sinsc = sinl(s - c);

   long double k = sqrtl(sinsa * sinsb * sinsc / sinl(s));

   long double bigA = 2.0L * atanl(k / sinsa);
   long double bigB = 2.0L * atanl(k / sinsb);
   long double bigC = 2.0L * atanl(k / sinsc);

   long double E = bigA + bigB + bigC - M_PI;

/*
   cout << "geoTriArea: " << g1 << " " << g2 << " " << g3 << endl
        << " a: " << a
        << " b: " << b
        << " c: " << c << endl
        << " A: " << bigA
        << " B: " << bigB
        << " C: " << bigC << endl
        << " E: " << E << " "
        << E / (4.0L * M_PI) << endl;
*/

   return E;

} // long double DgGeoCoord::geoTriArea

////////////////////////////////////////////////////////////////////////////////
long double
DgGeoCoord::geoPolyArea (const DgPolygon& poly, const DgGeoCoord center)
//
// returns area of spherical polygon in radians;
//
// assumes DgAddressBase for poly is DgGeoCoord
//
// Assumes poly is a "reasonably" convex polygon (i.e. a cell bouncary) and
// that center is an interior point.
//
{
   long double totArea = 0.0L;

   const DgGeoSphRF* geoRF = dynamic_cast<const DgGeoSphRF*>(&poly.rf());
   if (geoRF == 0)
      report("DgGeoCoord::geoPolyArea() non-geo polygon", DgBase::Fatal);

   // do each sub-triangle
   for (int i = 0; i < poly.size(); i++)
   {
      const DgGeoCoord& v1 = *geoRF->getAddress(poly[i]);
      const DgGeoCoord& v2 = *geoRF->getAddress(poly[(i + 1) % poly.size()]);

      totArea += DgGeoCoord::geoTriArea(center, v1, v2);
   }

   return totArea;

} // long double DgGeoCoord::geoPolyArea

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char*
DgEllipsoidRF::str2add (DgGeoCoord* add, const char* str, char delimiter) const
{
   if (!add) add = new DgGeoCoord();

   DgDVec2D vec;
   const char* tmp = vec.fromString(str, delimiter);

   *add = DgGeoCoord(vec.x(), vec.y(), false);

   return tmp;

} // const char* DgEllipsoidRF::str2add

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
/*
   sphTrig.C: vector and spherical trig utilities

   Lian Song and Kevin Sahr, July 24, 1994.

   Kevin Sahr, merged & optimized, July 20, 2001
*/
/******************************************************************************/

#include "DgBase.h"

#include <cmath>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/
void sphTriInit (SphTri* tri)
/*
   Initialize tri with UNDEFVAL values.
*/
{
   int i;

   for (i = 0; i < 3; i++)
   {
      tri->verts[i].lon  = tri->verts[i].lat  = UNDEFVAL;
      tri->edges[i] = tri->angles[i] = UNDEFVAL;
   }
   tri->code = UNDEFINT;
   tri->area = tri->perimeter = UNDEFVAL;
   tri->compactness = UNDEFVAL;

} /* void sphTriInit */

/******************************************************************************/
void planeTriInit (PlaneTri* tri)
/*
   Initialize tri with UNDEFVAL values.
*/
{
   int i;

   for (i = 0; i < 3; i++)
      tri->points[i].x  = tri->points[i].y = UNDEFVAL;
   tri->code = tri->direction =  UNDEFINT;
   tri->cenpoint.x = tri->cenpoint.y = UNDEFVAL;

} /* void planeTriInit */

/******************************************************************************/
void printInt (long long int val)
/*
   Print val or 'UNDEFVAL' to stdout as appropriate.
*/
{
   if (val == UNDEFVAL)
      dgcout << "UNDEFVAL";
   else
      dgcout << val;

} /* void printInt */

/******************************************************************************/
void printVal (long double val)
/*
   Print val or 'UNDEFVAL' to stdout as appropriate.
*/
{
   if (val == UNDEFVAL)
      dgcout << "UNDEFVAL";
   else
      dgcout << val;

} /* void printVal */

/******************************************************************************/
void printGeoCoord (const GeoCoord& p)
/*
   Print p to stdout.
*/
{
   dgcout << "(" << p.lon * M_180_PI << ", " << p.lat * M_180_PI << ")";

} /* void printGeoCoord */

/******************************************************************************/
void printVec2D (const Vec2D& p)
/*
   Print p to stdout.
*/
{
   dgcout << "(" << p.x << ", " << p.y << ")";

} /* void printVec2D */

/******************************************************************************/
void printVec3D (const Vec3D& p)
/*
   Print p to stdout.
*/
{
   dgcout << "(" << p.x << ", " << p.y << ", " << p.z << ")";

} /* void printVec3D */

/******************************************************************************/
void printSphTri (const SphTri& tri)
/*
   Print tri to stdout.
*/
{
   int i;

   dgcout << "{\n  code: ";
   printInt(tri.code);
   dgcout << "\n  vertices: ";
   for (i = 0; i < 3; i++)
   {
      dgcout << " ";
      printGeoCoord(tri.verts[i]);
   }
   dgcout << "\n";

   dgcout << "  A: "; printVal(tri.edges[0]);
   dgcout << "  B: "; printVal(tri.edges[1]);
   dgcout << "  C: "; printVal(tri.edges[2]);
   dgcout << "\n";

   dgcout << "  a: "; printVal(tri.angles[0]* M_180_PI);
   dgcout << "  b: "; printVal(tri.angles[1]* M_180_PI);
   dgcout << "  c: "; printVal(tri.angles[2]* M_180_PI);
   dgcout << "\n";

   dgcout << "  area: "; printVal(tri.area);
   dgcout << "  perimeter: "; printVal(tri.perimeter);
   dgcout << "  compactness: "; printVal(tri.compactness);
   dgcout << "\n";

   dgcout << "}\n";

} /* void printSphTri */

/******************************************************************************/
void printPlaneTri (const PlaneTri& tri)
/*
   Print tri to stdout.
*/
{
   int i;

   dgcout << "{\n  code: "; printInt(tri.code);
   dgcout << "\n  vertices: ";
   for (i = 0; i < 3; i++)
   {
      dgcout << " ";
      printVec2D(tri.points[i]);
   }
   dgcout << "\n";
   printVec2D(tri.cenpoint);
   dgcout << "\n";

   dgcout << "}\n";

} /* void PlaneTri */

/******************************************************************************/
Vec3D vecAdd (const Vec3D& A, const Vec3D& B)
/*
   Calculate and return A + B.
*/
{
   Vec3D C;

   C.x = A.x + B.x;
   C.y = A.y + B.y;
   C.z = A.z + B.z;

   return C;

} /* Vec3D vecAdd */

/******************************************************************************/
Vec3D vecSub (const Vec3D& A, const Vec3D& B)
/*
   Calculate and return A - B.
*/
{
   Vec3D C;

   C.x = A.x - B.x;
   C.y = A.y - B.y;
   C.z = A.z - B.z;

   return C;

} /* Vec3D vecSub */

/******************************************************************************/
Vec3D vecCross (const Vec3D& A, const Vec3D& B)
/*
   Calculate and return A x B.
*/
{
   Vec3D C;

   C.x = A.y * B.z - A.z * B.y;
   C.y = A.z * B.x - A.x * B.z;
   C.z = A.x * B.y - A.y * B.x;

   return C;

} /* Vec3D vecCross */

/******************************************************************************/
long double vecMag (const Vec3D& V)
/*
   Calculate and return the magnitude of a vector V.
*/
{
   return sqrtl(V.x * V.x + V.y * V.y + V.z * V.z);

} /* long double vecMag */

/******************************************************************************/
Vec3D vecNormalize  (const Vec3D& V)
/*
   Return the normalized form of V.
*/
{
   Vec3D C;

   C.x = V.x/sqrtl(V.x * V.x + V.y * V.y + V.z * V.z);
   C.y = V.y/sqrtl(V.x * V.x + V.y * V.y + V.z * V.z);
   C.z = V.z/sqrtl(V.x * V.x + V.y * V.y + V.z * V.z);

   return C;

}  /* Vec3D vecNormalize */

/******************************************************************************/
long double vecDot (const Vec3D& A, const Vec3D& B)
/*
   Calculate and return the dot product of two vectors.
*/
{
   return A.x * B.x + A.y * B.y + A.z * B.z;

}  /*  long double vecDot */

/******************************************************************************/
long double sqrMetersToExcessD (long double area)
{
   return area * 360.0L / (4.0L * M_PI * DgGeoSphRF::earthRadiusKM() *
          DgGeoSphRF::earthRadiusKM());

} /* long double metersToExcessD */

/******************************************************************************/
long double metersToGCDegrees (long double meters)
{
   long double earthCircum = (2.0L * M_PI * DgGeoSphRF::earthRadiusKM());
   return meters * 360.0L / earthCircum;

} /* long double metersToGCDegrees */

/******************************************************************************/
long double maxval(long double val1, long double val2)
/*
  return the maxmum of two variables
*/
 {
  long double maxx;
  if (val1>val2) maxx=val1;
  else maxx=val2;
  return maxx;
 } /* long double maxval */

/******************************************************************************/
long double minval(long double val1, long double val2)
/*
  return the minmum of two variables
*/
 {
  long double minn;
  if (val1<val2) minn=val1;
  else minn=val2;
  return minn;
 } /* long double minval */

/******************************************************************************/
Vec3D llxyz(const GeoCoord& sv)
/*
  Transformation of a point on the sphere from xyz cordinates to latitude and longitude
  Input unit is degree. Output unit regard the earth's radius as one unit.
*/
 {
  Vec3D v;
  v.x=cosl(sv.lat)*cosl(sv.lon);
  v.y=cosl(sv.lat)*sinl(sv.lon);
  v.z=sinl(sv.lat);

  const long double epsilon = 0.000000000000001L;
  if (fabsl(v.x) < epsilon) v.x = 0.0L;
  if (fabsl(v.y) < epsilon) v.y = 0.0L;
  if (fabsl(v.z) < epsilon) v.z = 0.0L;

  return v;

 } /* Vec3D llxyz */

/******************************************************************************/
GeoCoord xyzll(const Vec3D& v0)
/*
   Transformation of a point on the sphere from xyz cordinates to latitude and
   longitude Input unit regard the earth's radius as one unit. Output unit is
   degree.
*/
 {
  GeoCoord sv;
  Vec3D v = v0;

  if (fabsl(v.z)-1.0L < PRECISION)
  {
     if (v.z>1.0L) v.z = 1.0L;
     if (v.z<-1.0L) v.z = -1.0L;
     sv.lat=asinl(v.z);
     if ((sv.lat== M_PI_2) || (sv.lat==-M_PI_2)) sv.lon=0.0L;
     else sv.lon=atan2l(v.y,v.x);
     return sv;
  }
  else
  {
     dgcerr << "Error: in function xyzll, asin domain error.\n";
     return sv;
  }

 } /* GeoCoord xyzll */

/******************************************************************************/
GeoCoord sphTricenpoint(GeoCoord sp[3])
/*
   Calculate and return the center point of a sphere triangle
   Input and output unit is degree.
*/
 {
  Vec3D p[3], cp, cpn;
  GeoCoord cv;
  int i;

  for (i=0;i<3;i++) p[i]=llxyz(sp[i]);
  cp.x=(p[0].x+p[1].x+p[2].x)/3;
  cp.y=(p[0].y+p[1].y+p[2].y)/3;
  cp.z=(p[0].z+p[1].z+p[2].z)/3;
  cpn=vecNormalize(cp);
  cv=xyzll(cpn);
  return cv;
 } /* GeoCoord sphTricenpoint */

/******************************************************************************/
long double chorddist(const GeoCoord& ll1, const GeoCoord& ll2)
/*
   Calculate the chord distance between two points on the sphere
   Input unit is degree. Output unit regard the earth's radius as one unit.
*/
 {
  //long double la1,lo1,la2,lo2;
  Vec3D p1,p2;
  p1.x=cosl(ll1.lat)*cosl(ll1.lon);
  p1.y=cosl(ll1.lat)*sinl(ll1.lon);
  p1.z=sinl(ll1.lat);
  p2.x=cosl(ll2.lat)*cosl(ll2.lon);
  p2.y=cosl(ll2.lat)*sinl(ll2.lon);
  p2.z=sinl(ll2.lat);
  return
    sqrtl((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z));

 } /* long double chorddist */

/******************************************************************************/
long double spheredist(const GeoCoord& ll1, const GeoCoord& ll2)
/*
   Calculate the sphere distance between two points on the sphere
   Input and Output unit is in radius.
*/
 {
  long double sd;
  sd=cosl(M_PI_2-ll1.lat)*cosl(M_PI_2-ll2.lat)+
     sinl(M_PI_2-ll1.lat)*sinl(M_PI_2-ll2.lat)*cosl(ll1.lon-ll2.lon);
  if (sd>1) sd=1;
  if (sd<-1) sd=-1;
  return acosl(sd);
 }/* long double spheredist */

/******************************************************************************/
void sphTriSolve(SphTri* tri)
/*
   Input:  three vertices's lat and lon in radius.
   Output: three edges, three angles, area.
	   the unit for length is kilometer, the unit for angle is degree
*/
 {
  int i;
  long double l1[2],l2[2],l3[2],p,mindist;

  mindist=5;
  l1[0]=tri->verts[0].lat; l1[1]=tri->verts[0].lon;
  l2[0]=tri->verts[1].lat; l2[1]=tri->verts[1].lon;
  l3[0]=tri->verts[2].lat; l3[1]=tri->verts[2].lon;
  tri->edges[0]=acosl(cosl(M_PI_2-l2[0])*cosl(M_PI_2-l3[0])+
               sinl(M_PI_2-l2[0])*sinl(M_PI_2-l3[0])*cosl(l2[1]-l3[1]));
  tri->edges[1]=acosl(cosl(M_PI_2-l1[0])*cosl(M_PI_2-l3[0])+
               sinl(M_PI_2-l1[0])*sinl(M_PI_2-l3[0])*cosl(l1[1]-l3[1]));
  tri->edges[2]=acosl(cosl(M_PI_2-l2[0])*cosl(M_PI_2-l1[0])+
               sinl(M_PI_2-l2[0])*sinl(M_PI_2-l1[0])*cosl(l2[1]-l1[1]));
  tri->angles[0]=acosl((cosl(tri->edges[0])-cosl(tri->edges[1])*
               cosl(tri->edges[2]))/(sinl(tri->edges[1])*sinl(tri->edges[2])));
  tri->angles[1]=acosl((cosl(tri->edges[1])-cosl(tri->edges[0])*
               cosl(tri->edges[2]))/(sinl(tri->edges[0])*sinl(tri->edges[2])));
  tri->angles[2]=acosl((cosl(tri->edges[2])-cosl(tri->edges[0])*
               cosl(tri->edges[1]))/(sinl(tri->edges[0])*sinl(tri->edges[1])));
  for (i=0;i<3;i++) tri->edges[i]=tri->edges[i]*DgGeoSphRF::earthRadiusKM();
  if (tri->edges[0]<mindist)
   {
    p=(tri->edges[0]+tri->edges[1]+tri->edges[2])/2;
    tri->area=sqrtl(p*(p-tri->edges[0])*(p-tri->edges[1])*(p-tri->edges[2]));
   }
  else tri->area=(tri->angles[0]+tri->angles[1]+tri->angles[2]-M_PI)*
              DgGeoSphRF::earthRadiusKM()*DgGeoSphRF::earthRadiusKM();

 } /* void sphTriSolve(SphTri* tri) */

/******************************************************************************/
GeoCoord GCmidpoint(const GeoCoord& pp1, const GeoCoord& pp2)
{
  Vec3D pt1,pt2,mpt;
  GeoCoord mp;
  pt1=llxyz(pp1);
  pt2=llxyz(pp2);
  mpt.x = (pt1.x+pt2.x)/2.0L;
  mpt.y = (pt1.y+pt2.y)/2.0L;
  mpt.z = (pt1.z+pt2.z)/2.0L;
  mpt = vecNormalize(mpt);
  mp = xyzll(mpt);
  return mp;
}

/******************************************************************************/
long double Azimuth(const GeoCoord& pt1, const GeoCoord& pt2)
/*
  return the azimuth of pt2 relatived to pt1
  output azimuth unit is radius
*/
 {
  long double az;

  az=atan2l(cosl(pt2.lat)*sinl(pt2.lon-pt1.lon), cosl(pt1.lat)*sinl(pt2.lat)-
     sinl(pt1.lat)*cosl(pt2.lat)*cosl(pt2.lon-pt1.lon));
  return az;
 } /* long double Azimuth */

/******************************************************************************/
GeoCoord GCintersect(const GeoCoord& sv11, const GeoCoord& sv12,
                     const GeoCoord& sv21, const GeoCoord& sv22, int sign)
/*
  return the intersect point of two great circle
  sign=1: two great circle segment with ends of sv11 and sv12,
          sv21 and sv22 respectively
  sign=0: two whole great circle with one pass sv11 and sv12, one pass
          sv21 and sv22, return the intersect point on North Hemisphere
*/
 {
  GeoCoord pt;
  Vec3D nn1,nn2,
	p11,p12,
	p21,p22,
	pp,pp2;
  long double a,b,maxlon,minlon;

/* calculate the intersect point of two great circle */
  p11=llxyz(sv11);
  p12=llxyz(sv12);
  p21=llxyz(sv21);
  p22=llxyz(sv22);
  nn1.x=p11.y*p12.z-p12.y*p11.z;
  nn1.y=-p11.x*p12.z+p12.x*p11.z;
  nn1.z=p11.x*p12.y-p12.x*p11.y;
  nn2.x=p21.y*p22.z-p22.y*p21.z;
  nn2.y=-p21.x*p22.z+p22.x*p21.z;
  nn2.z=p21.x*p22.y-p22.x*p21.y;
  if ((nn2.z*nn1.y-nn1.z*nn2.y)!= 0.0L) {
    b=(nn1.x*nn2.y-nn2.x*nn1.y)/(nn2.z*nn1.y-nn1.z*nn2.y);
    a=(nn2.x*nn1.z-nn1.x*nn2.z)/(nn1.y*nn2.z-nn2.y*nn1.z);
    pp.x=1/sqrtl(a*a+b*b+1);
    pp.y=a*pp.x;
    pp.z=b*pp.x;
   } else if (((nn2.z*nn1.y-nn1.z*nn2.y)==0.0L) &&
         ((nn1.x*nn2.y-nn2.x*nn1.y)==0.0L) && ((nn1.x*nn2.z-nn2.x*nn1.z)==0.0L)) {
    report("Error in GCintersect: the two great circle planes are parallel.\n",
           DgBase::Fatal);
   } else if (((nn2.z*nn1.y-nn1.z*nn2.y)==0.0L) && (nn1.z!=0.0L)) {
    pp.x=0.0L;
    pp.y=1.0L/sqrtl(1+nn1.y*nn1.y/nn1.z/nn1.z);
    pp.z=-nn1.y/nn1.z*pp.y;
   } else if (((nn2.z*nn1.y-nn1.z*nn2.y)==0.0L) && (nn2.z!=0.0L)) {
    pp.x=0.0L;
    pp.y=1.0L/sqrtl(1.0L+nn2.y*nn2.y/nn2.z/nn2.z);
    pp.z=-nn2.y/nn2.z*pp.y;
   } else if (((nn2.z*nn1.y-nn1.z*nn2.y)==0.0L) && (nn1.y!=0.0L)) {
    pp.x=0.0L;
    pp.z=1/sqrtl(1+nn1.z*nn1.z/nn1.y/nn1.y);
    pp.y=-nn1.z/nn1.y*pp.z;
   } else if (((nn2.z*nn1.y-nn1.z*nn2.y)==0.0L) && (nn2.y!=0.0L)) {
    pp.x=0.0L;
    pp.z=1.0L/sqrtl(1.0L+nn2.z*nn2.z/nn2.y/nn2.y);
    pp.y=-nn2.z/nn2.y*pp.z;
   }

  if (sign==0) {
    if (pp.z<0.0L) {
      pp.x=0.0L-pp.x;
      pp.y=-pp.y;
      pp.z=-pp.z;
    }
    pt=xyzll(pp);
    return pt;
  } else {
   /* judge if the point is on the two great circle segment */

  pt=xyzll(pp);
  maxlon=maxval(sv11.lon,sv12.lon);
  minlon=minval(sv11.lon,sv12.lon);
  if ((pt.lon<=maxlon) && (pt.lon>=minlon))
   return pt;
  else {
    pp2.x=-pp.x;
    pp2.y=-pp.y;
    pp2.z=-pp.z;
    pt=xyzll(pp2);
    if ((pt.lon<=maxlon) && (pt.lat>=minlon))
     return pt;
    else {
      dgcerr << "Error of GCintersect: the point is not on great circle segment.\n";
      pt.lat=UNDEFVAL; pt.lon=UNDEFVAL;
      return pt;
    }
  }
  }
} /* GeoCoord GCintersect */

/******************************************************************************/
long double GCptlat(long double lon, const GeoCoord& sv1, const GeoCoord& sv2)
/*
  return latitude of the point on great circle segment with known longtitude
*/
 {
  long double lat,a,b,c;
  Vec3D p1,p2;

  p1.x=cosl(sv1.lat)*cosl(sv1.lon);
  p1.y=cosl(sv1.lat)*sinl(sv1.lon);
  p1.z=sinl(sv1.lat);
  p2.x=cosl(sv2.lat)*cosl(sv2.lon);
  p2.y=cosl(sv2.lat)*sinl(sv2.lon);
  p2.z=sinl(sv2.lat);

  a=(p1.y*p2.z-p1.z*p2.y)*cosl(lon);
  b=(p1.x*p2.z-p1.z*p2.x)*sinl(lon);
  c=(p1.x*p2.y-p1.y*p2.x);
  if (c!=0.0L) lat=atanl((b-a)/c);
  else {
     lat = UNDEFVAL;
     dgcerr << "Error in GCptlat: the two end points are at one longitude.\n";
  }
  return(lat);
 } /* long double GCptlat */

/******************************************************************************/
int ptinsphtri(const GeoCoord& pt, GeoCoord sTri[3])
/*
  decide if a point is in a spherical triangle
  return 1 if it is in and return 0 if not
*/
{
   int i;
   long double p0, t0;
   Vec3D pp, ptri[3];

   for (i = 0; i < 3; i++) ptri[i] = llxyz(sTri[i]);
   pp = llxyz(pt);
   p0 = pp.x * (ptri[1].y * ptri[2].z - ptri[2].y * ptri[1].z) -
        pp.y * (ptri[1].x * ptri[2].z - ptri[2].x * ptri[1].z) +
        pp.z * (ptri[1].x * ptri[2].y - ptri[2].x * ptri[1].y);

   t0 = ptri[0].x * (ptri[1].y * ptri[2].z - ptri[2].y * ptri[1].z) -
        ptri[0].y * (ptri[1].x * ptri[2].z - ptri[2].x * ptri[1].z) +
        ptri[0].z * (ptri[1].x * ptri[2].y - ptri[2].x * ptri[1].y);

   if (p0 * t0 < 0.0L)
   {
      return 0;
   }
   else
   {
      p0 = pp.x * (ptri[0].y * ptri[2].z - ptri[2].y * ptri[0].z) -
           pp.y * (ptri[0].x * ptri[2].z - ptri[2].x * ptri[0].z) +
           pp.z * (ptri[0].x * ptri[2].y - ptri[2].x * ptri[0].y);

      t0 = ptri[1].x * (ptri[0].y * ptri[2].z - ptri[2].y * ptri[0].z) -
           ptri[1].y * (ptri[0].x * ptri[2].z - ptri[2].x * ptri[0].z) +
           ptri[1].z * (ptri[0].x * ptri[2].y - ptri[2].x * ptri[0].y);

      if (p0 * t0 < 0.0L)
      {
         return 0;
      }
      else
      {
         p0 = pp.x * (ptri[0].y * ptri[1].z - ptri[1].y * ptri[0].z) -
              pp.y * (ptri[0].x * ptri[1].z - ptri[1].x * ptri[0].z) +
              pp.z * (ptri[0].x * ptri[1].y - ptri[1].x * ptri[0].y);

         t0 = ptri[2].x * (ptri[0].y * ptri[1].z - ptri[1].y * ptri[0].z) -
              ptri[2].y * (ptri[0].x * ptri[1].z - ptri[1].x * ptri[0].z) +
              ptri[2].z * (ptri[0].x * ptri[1].y - ptri[1].x * ptri[0].y);

         return (p0 * t0 >= 0.0L);
      }
   }

}  /* ptinsphtri */

/******************************************************************************/
GeoCoord GCdaz(const GeoCoord& pt, long double distance, long double az)
/*
  compute and return the point whose azimuth (az) and distance
  (distance) relative to a known point (pt) are given.
  input parameter distance and az are in radius, pt in degree.
*/
 {
  GeoCoord pt2;
  long double sinlat, sinlon, coslon;

  if ((fabsl(az)<PRECISION) || (fabsl(fabsl(az)-M_PI)<PRECISION))
  {
     if (fabsl(az)<PRECISION) pt2.lat = pt.lat + distance;
     else pt2.lat = pt.lat - distance;
     pt2.lon = pt.lon;
     if (fabsl(pt2.lat-M_PI_2)<PRECISION)
     {
        pt2.lat = M_PI_2;
        pt2.lon = 0.0L;
     }
     if (fabsl(pt2.lat+M_PI_2)<PRECISION)
     {
        pt2.lat = -M_PI;
        pt2.lon = 0.0L;
     }
  }
  else
  {
     sinlat=sinl(pt.lat)*cosl(distance)+cosl(pt.lat)*sinl(distance)*cosl(az);
     if (sinlat>1.0L) sinlat=1.0L;
     if (sinlat<-1.0L) sinlat=-1.0L;
     pt2.lat=asinl(sinlat);
     if ((pt2.lat==M_PI_2) || (pt2.lat==-M_PI_2)) pt2.lon=0.0L;
     else
      {
       sinlon=sinl(az)*sinl(distance)/cosl(pt2.lat);
       coslon=(cosl(distance)-sinl(pt.lat)*sinl(pt2.lat))/
	      cosl(pt.lat)/cosl(pt2.lat);
       if (sinlon>1.0L) sinlon=1.0L;
       if (sinlon<-1.0L) sinlon=-1.0L;
       if (coslon>1.0L) coslon=1.0L;
       if (coslon<-1.0L) coslon=-1.0L;
       pt2.lon=pt.lon+atan2l(sinlon,coslon);
      }
     if (pt2.lon>M_PI+PRECISION) pt2.lon -=2.0L*M_PI;
     if (pt2.lon<-M_PI-PRECISION) pt2.lon +=2.0L*M_PI;
  }

  return pt2;
 } /* GeoCoord GCdaz */

/******************************************************************************/
