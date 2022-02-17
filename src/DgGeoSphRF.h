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
// DgGeoSphRF.h: DgGeoSphRF class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGGEOSPHRF_H
#define DGGEOSPHRF_H

#include "DgContCartRF.h"
#include "DgEllipsoidRF.h"

////////////////////////////////////////////////////////////////////////////////
class DgGeoSphRF : public DgEllipsoidRF {

   public:

      enum DgLonWrapMode { Wrap, UnwrapWest, UnwrapEast, InvalidLonWrapMode };
      static const string lonWrapModeStrings[];

      static const DgGeoSphRF* makeRF (DgRFNetwork& networkIn, const string& nameIn = "GeodeticSph",
                  long double earthRadiusKMin = DEFAULT_RADIUS_KM)
         { return new DgGeoSphRF (networkIn, nameIn, earthRadiusKMin); }

      DgGeoSphRF& operator= (const DgGeoSphRF& rf)
         { DgEllipsoidRF::operator=(rf); return *this; }

      // distance between locations in radians
      virtual long double gcDist (const DgLocation& loc1, const DgLocation& loc2) const
         {
            DgLocation tmp1(loc1);
            DgLocation tmp2(loc2);
            convert(&tmp1);
            convert(&tmp2);
            return DgGeoCoord::gcDist(*getAddress(tmp1), *getAddress(tmp2));
         }

      // distance in km

      virtual long double dist (const DgLocation& loc1, const DgLocation& loc2) const
         { return earthRadiusKM() * gcDist(loc1, loc2); }

      virtual long double dist (const DgGeoCoord& add1, const DgGeoCoord& add2) const
         { return earthRadiusKM() * DgGeoCoord::gcDist(add1, add2); }

      static long double earthRadiusKM (void) { return earthRadiusKM_; }
      static long double icosaEdgeKM   (void) { return icosaEdgeKM_; }
      static long double icosaEdgeDegs (void) { return icosaEdgeDegs_; }
      static long double icosaEdgeRads (void) { return icosaEdgeRads_; }
      static long double totalAreaKM   (void) { return totalAreaKM_; }

      // midpoint of great circle connecting two points
      static DgGeoCoord midPoint(const DgGeoCoord& p1, const DgGeoCoord& p2);

      // azimuth from pt1 to pt2
      static long double azimuth(const DgGeoCoord& p1, const DgGeoCoord& p2,
                            bool returnRads = true);

      // point that is distance (rads) from p0 along azimuth (rads)
      static DgGeoCoord travelGC (const DgGeoCoord& p0, long double distance,
                                  long double azimuth, bool inputRads = true);

      // densify polygon in geodetic coordinates
      static void densify (DgPolygon& p, long double maxDist, bool rads = true);

      // unwrap a point based on longitude wrap mode (assumes the point is 
      // associated with a cell being wrapped
      static int lonWrap (DgGeoCoord& g, DgLonWrapMode wrapMode);

      // unwrap any cells in polygon that cross the anti-meridian
      static int lonWrap (DgPolygon& p, DgLonWrapMode wrapMode);

   protected:

      DgGeoSphRF (DgRFNetwork& networkIn, const string& nameIn = "GeodeticSph",
                  long double earthRadiusKMin = DEFAULT_RADIUS_KM)
         : DgEllipsoidRF (networkIn, nameIn, earthRadiusKMin * 1000L,
                earthRadiusKMin * 1000L)
           {
              earthRadiusKM_ = earthRadiusKMin;
              icosaEdgeRads_ = M_ATAN2;
              icosaEdgeDegs_ = icosaEdgeRads_ * M_180_PI;
              icosaEdgeKM_ = icosaEdgeRads_ * earthRadiusKM_;
              totalAreaKM_ = 4.0L * M_PI * earthRadiusKM_ * earthRadiusKM_;
           }

      DgGeoSphRF (const DgGeoSphRF& rf)
         : DgEllipsoidRF(rf) { /* earthRadius_ = rf.earthRadius(); */ }

   private:

      static long double earthRadiusKM_;  // earth radius in km
      static long double icosaEdgeKM_;    // spherical icosahedron edge length
      static long double icosaEdgeDegs_;  // spherical icosahedron edge length
      static long double icosaEdgeRads_;  // spherical icosahedron edge length
      static long double totalAreaKM_;    // spherical icosahedron edge length
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgGeoSphDegRF : public DgContCartRF {

   public:

      static const DgGeoSphDegRF* makeRF(const DgGeoSphRF& geoRFin,
                     const string& nameIn = "GeodeticSphDeg")
         { return new DgGeoSphDegRF(geoRFin, nameIn); }

      const DgGeoSphRF& geoRF (void) const { return geoRF_; }

   protected:

      DgGeoSphDegRF (const DgGeoSphRF& geoRFin,
                     const string& nameIn = "GeodeticSphDeg");

      const DgGeoSphRF& geoRF_;
};

#endif
