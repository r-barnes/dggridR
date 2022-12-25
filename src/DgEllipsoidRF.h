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
// DgEllipsoidRF.h: DgEllipsoidRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGELLIPSOIDRF_H
#define DGELLIPSOIDRF_H

#include <climits>
#include <iostream>

#include "DgConstants.h"
#include "DgDVec2D.h"
#include "DgGeoDatumRF.h"

using namespace std;

typedef struct GeoCoord {

   long double lat;
   long double lon;

} GeoCoord;

class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgGeoCoord : public DgDVec2D {

   public:

      static const DgGeoCoord undefGeoCoord;

      static const long double tolerance; // decimal degrees

      static long double gcDist (const DgGeoCoord& g1, const DgGeoCoord& g2,
                            bool rads = true);

      static DgGeoCoord gcIntersect (const DgGeoCoord& g11,
                  const DgGeoCoord& g12, const DgGeoCoord& g21,
                  const DgGeoCoord& g22);

      static long double geoTriArea (const DgGeoCoord& g1, const DgGeoCoord& g2,
                                const DgGeoCoord& g3);

      static long double geoPolyArea (const DgPolygon& poly,
                                 const DgGeoCoord center);

      DgGeoCoord (void) { }

      DgGeoCoord (const DgGeoCoord& coord) = default;

      DgGeoCoord (const DgDVec2D& coord, bool rads = true)
        { if (rads) *this = coord; else *this = coord * M_PI_180; }

      DgGeoCoord (long double lon, long double lat, bool rads = true)
        { if (rads) *this = DgDVec2D(lon, lat);
          else *this = DgDVec2D(lon * M_PI_180, lat * M_PI_180); }

      DgGeoCoord (const GeoCoord& coord)
           { *this = DgDVec2D(coord.lon, coord.lat); }

      DgGeoCoord& operator= (const DgGeoCoord& coord)
           { DgDVec2D::operator=(coord); return *this; }

      DgGeoCoord& operator= (const DgDVec2D& coord)
           { DgDVec2D::operator=(coord); return *this; }

      operator string (void) const
           { return "(" + dgg::util::to_string(lonDegs()) + ", " +
		   dgg::util::to_string(latDegs()) + ")"; }

      long double lat (void) const { return y(); } // latitude in radians
      long double lon (void) const { return x(); } // longitude in radians

      long double latDegs (void) const { return lat() * M_180_PI; }
      long double lonDegs (void) const { return lon() * M_180_PI; }

      void setLat (long double rads) { setY(rads); }
      void setLon (long double rads) { setX(rads); }

      void setLatDeg (long double degs) { setY(degs * M_PI_180); }
      void setLonDeg (long double degs) { setX(degs * M_PI_180); }

      void normalize (void)
           {
              if (fabsl(latDegs()) >= (90.0L - tolerance))
              {
                 setLon(0.0L);
              }
              else
              {
                 while (lonDegs() <= -180.0L) setLonDeg(lonDegs() + 360.0L);
                 while (lonDegs() > 180.0L) setLonDeg(lonDegs() - 360.0L);
              }
           }

};

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgGeoCoord& pt)
{
   return stream << string(pt);

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgEllipsoidRF : public DgGeoDatumRF<DgGeoCoord, long double> {

   public:

      DgEllipsoidRF& operator= (const DgEllipsoidRF& ell)
               {
                  if (&ell != this)
                  {
                     a_ = ell.a();
                     b_ = ell.b();
                     f_ = ell.f();
                     e_ = ell.e();
                     es_ = ell.es();
                     as_ = ell.as();
                     bs_ = ell.bs();
                     ra_ = ell.ra();
                     one_es_ = ell.one_es();
                     rone_es_ = ell.rone_es();
                  }

                  return *this;

               } // DgEllipsoidRF::operator=

/*
      bool operator== (const DgEllipsoidRF& ell) const
               {
                  return (a() == ell.a() &&
                          b() == ell.b());

               } // bool DgEllipsoidRF::operator==

      bool operator!= (const DgEllipsoidRF& ell) const
               {
                  return !operator==(ell);

               } // bool DgEllipsoidRF::operator!=
*/

      // get methods

      long double a (void) const { return a_; }

      long double b (void) const { return b_; }

      long double f (void) const { return f_; }

      long double e (void) const { return e_; }

      long double es (void) const { return es_; }

      long double as (void) const { return as_; }

      long double bs (void) const { return bs_; }

      long double ra (void) const { return ra_; }

      long double one_es (void) const { return one_es_; }

      long double rone_es (void) const { return rone_es_; }

      // set methods

      void setA (long double aIn) { a_ = aIn; }

      void setB (long double bIn) { b_ = bIn; }

      void setF (long double fIn) { f_ = fIn; }

      void setE (long double eIn) { e_ = eIn; }

      void setEs (long double esIn) { es_ = esIn; }

      void setAs (long double asIn) { as_ = asIn; }

      void setBs (long double bsIn) { bs_ = bsIn; }

      void setRa (long double raIn) { ra_ = raIn; }

      void setOne_es (long double one_esIn) { one_es_ = one_esIn; }

      void setRone_es (long double rone_esIn) { rone_es_ = rone_esIn; }

      virtual const DgGeoCoord& undefAddress (void) const
               { return DgGeoCoord::undefGeoCoord; }

      // the following routines are "back-doors" included for speed;
      // use with care!

      virtual DgAddressBase* vecAddress (const DgDVec2D& v) const
                    { return new DgAddress<DgGeoCoord>(DgGeoCoord(v, false)); }

      virtual DgLocation* vecLocation (const DgDVec2D& v) const
                    { return makeLocation(DgGeoCoord(v, false)); }

      virtual DgDVec2D getVecAddress (const DgAddressBase& add) const
         {
            const DgGeoCoord& g =
                  static_cast< const DgAddress<DgGeoCoord> &>(add).address();
            return DgDVec2D(g.lonDegs(), g.latDegs());
         }

      virtual DgDVec2D getVecLocation (const DgLocation& loc) const
                    { return DgDVec2D(getAddress(loc)->lonDegs(),
                                      getAddress(loc)->latDegs()); }

      // distance in km; not currently defined (except for DgGeoSphRF)

      virtual long double dist (const DgGeoCoord&, const DgGeoCoord&) const
         { return 1.0L; }

      virtual string add2str (const DgGeoCoord& add) const
                       { return string(add); }

      virtual string add2str (const DgGeoCoord& add, char delimiter) const
                { return dgg::util::to_string(add.lonDegs(), formatStr()) + delimiter +
                         dgg::util::to_string(add.latDegs(), formatStr()); }

      virtual const char* str2add (DgGeoCoord* add, const char* str,
                                   char delimiter) const;

      virtual string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual unsigned long long int dist2int (const long double& dist) const
                       { return static_cast<unsigned long long int>(dist); }

   protected:

      DgEllipsoidRF (DgRFNetwork& networkIn, const string& nameIn)
               : DgGeoDatumRF<DgGeoCoord, long double> (networkIn, nameIn),
                 a_ (-1.0L), b_ (-1.0L), f_ (-1.0L), e_ (-1.0L), es_ (-1.0L),
                 as_ (-1.0L), bs_ (-1.0L), ra_ (-1.0L), one_es_ (-1.0L),
                 rone_es_ (-1.0L) { }

      DgEllipsoidRF (DgRFNetwork& networkIn, const string& nameIn,
                   const DgEllipsoidRF& ell)
               : DgGeoDatumRF<DgGeoCoord, long double> (networkIn, nameIn),
                 a_ (ell.a()), b_ (ell.b()), f_ (ell.f()),
                 e_ (ell.e()), es_ (ell.es()), as_ (ell.as()), bs_ (ell.bs()),
                 ra_ (ell.ra()), one_es_ (ell.one_es()),
                 rone_es_ (ell.rone_es())
           { undefLoc_ = makeLocation(undefAddress()); }

      DgEllipsoidRF (DgRFNetwork& networkIn, const string& nameIn,
                   long double aIn, long double bIn)
               : DgGeoDatumRF<DgGeoCoord, long double> (networkIn, nameIn),
                 a_ (aIn), b_ (bIn)
                {
                   setUndefLoc(makeLocation(undefAddress()));

                   f_ = (a() - b()) / a();
                   as_ = a() * a();
                   bs_ = b() * b();
                   es_ = 1.0L - bs() / as();
                   e_ = sqrtl(es());
                   ra_ = 1.0L / a();
                   one_es_ = 1.0L - es();
                   rone_es_ = 1.0L / one_es();
                }

   private:

      long double a_; // semi-major axis in meters

      long double b_; // semi-minor axis in meters

      long double f_; // flattening = (a - b) / a

      long double e_; // eccentricity

      long double es_; // e squared

      long double as_; // a squared

      long double bs_; // b squared

      long double ra_; // 1 / a

      long double one_es_; // 1 / es

      long double rone_es_; // 1 / one_es

}; // class DgEllipsoidRF

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgEllipsoidRF& ell)
{
   return stream << "a: " << ell.a() << " " << "b: " << ell.b() << " "
                 << "f: " << ell.f() << " " << "e: " << ell.e();

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
   Below are typedefs, defines, and function prototypes for vector and
   spherical trig routines.

   By Lian Song and Kevin Sahr.
*/
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

/**** macros ****/

#define UNDEFVAL  LDBL_MAX
#define UNDEFINT  INT_MAX

#define PRECISION 0.0000000000005L /* precise 12 digits after dot  */

/**** typedefs ****/

typedef struct Vec2D {

   long double x;
   long double y;

   Vec2D()
    : x(0.0L), y(0.0L)
   {}

} Vec2D;

typedef struct Vec3D {

   long double x;
   long double y;
   long double z;

   Vec3D()
    : x(0.0L), y(0.0L), z(0.0L)
   {}

} Vec3D;

typedef struct SphTri {

   long long int code;
   GeoCoord verts[3];  /* vertices in degrees */
   long double   edges[3];  /* edges opposite to verts in central angle degrees */
   long double   angles[3]; /* interior angles in degrees corresponding to verts */
   long double   area;      /* area in km */
   long double   perimeter;  /* perimeter in km */
   long double   compactness;

} SphTri;

typedef struct PlaneTri {

   long long int code;
   long long int direction; /* 0: up, 1: down */
   Vec2D    points[3]; /* points in plane triangle in km */
   Vec2D    cenpoint;  /* central point of plane triangle in km */

} PlaneTri;

typedef struct SCtri {

  long long int code;
  GeoCoord verts[3]; /* vertices in radius*/
  GeoCoord poles[3];
  long double edges[3];
  long double area;
  long double perimeter;
  long double sclat[3];
         /* latitude from pole of small circle v1v2 in radius */
/*  long double   darea[3];
     area surounded by great circle and small circle of two points.
       positive means small circle is more convex than great circle
       from the center of the triangle. */

} SCtri;

/* precomputed trig info for a geodetic point */

typedef struct PreCompGeo {

   GeoCoord pt;
   long double sinLat;
   long double sinLon;
   long double cosLat;
   long double cosLon;

} PreCompGeo;

/* pre-computed info corresponding to a spherical triangle
   (for use in finding if a point lies in the triangle) */

typedef struct PreCompInTri {

   long double p0x0;
   long double p0y0;
   long double p0z0;

   long double p0x1;
   long double p0y1;
   long double p0z1;

   long double p0x2;
   long double p0y2;
   long double p0z2;

   long double t00;
   long double t01;
   long double t02;

} PreCompInTri;

typedef struct SphIcosa {

   /* sufficient icosa placement data */

   GeoCoord pt;
   long double azimuth;

   /* pre-calculated projection values */

   GeoCoord icoverts[12];
   GeoCoord icotri[20][3];
   PreCompGeo triCen[20];
   PreCompInTri ptin[20];
   long double dazh[20];

} SphIcosa;

typedef struct IcosaGridPt {

   Vec2D pt; /* the point itself in triangle coordinates */
   int triangle; /* index (0 - 19) of the triangle */

} IcosaGridPt;

/**** function prototypes ****/

/* printing functions */

void   printVal      (long double val);  /* print val or 'UNDEFVAL' to stdout */
void   printGeoCoord (const GeoCoord& p);  /* print p to stdout */
void   printVec2D    (const Vec2D& p);     /* print p to stdout */
void   printVec3D    (const Vec3D& p);     /* print p to stdout */
void   printSphTri   (const SphTri& tri);  /* print tri to stdout */
void   printPlaneTri (const PlaneTri& tri);/* print plane tri to stdout */

/* Vec3D functions */

Vec3D  vecAdd        (const Vec3D& A, const Vec3D& B); /* return A + B */
Vec3D  vecSub        (const Vec3D& A, const Vec3D& B); /* return A - B */
Vec3D  vecCross      (const Vec3D& A, const Vec3D& B); /* return A x B */
long double vecDot        (const Vec3D& A, const Vec3D& B); /* return A . B */
long double vecMag        (const Vec3D& V);          /* return |V| */
Vec3D  vecNormalize  (const Vec3D& V);          /* return V as a unit vector */

/* spherical triangle functions */

void sphTriInit (SphTri* tri); /* initialize with UNDEFVAL values */

void planeTriInit (PlaneTri* tri); /* initialize with UNDEFVAL values */

/* solve for three sides,three angles and area */
void sphTriSolve (SphTri* tri);

/* calculate the center point of a sphere triangle */
GeoCoord sphTricenpoint(GeoCoord sp[3]);

/* sphere functions */

GeoCoord xyzll(const Vec3D& v); /* transform a point from xyz to latlon */

Vec3D llxyz(const GeoCoord& sv); /* transform a point from latlon to xyz */

/* calculate the chord distance */
long double chorddist(const GeoCoord& ll1,const GeoCoord& ll2);

/* calculate the sphere distance */
long double spheredist(const GeoCoord& ll1,const GeoCoord& ll2);

/* decide if pt is in sphere triangle */
int ptinsphTri(const GeoCoord& pt, GeoCoord sTri[3]);

/* return the middle point of the great circle */
GeoCoord GCmidpoint(const GeoCoord& pp1, const GeoCoord& pp2);

/* return intersect point of two great circle */
GeoCoord GCintersect(const GeoCoord& sv11, const GeoCoord& sv12,
                     const GeoCoord& sv21, const GeoCoord& sv22, int sign);

/* return latitude of a point with known longtitude on great circle */
long double GCptlat(long double lon, const GeoCoord& sv1, const GeoCoord& sv2);

/* return azimuth from pt1 to pt2 */
long double Azimuth(const GeoCoord& pt1, const GeoCoord& pt2);

/* return Geopoint that have great-circle-distance d along azimuth az from pt */
GeoCoord GCdaz(const GeoCoord& pt, long double d, long double az);

/******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
