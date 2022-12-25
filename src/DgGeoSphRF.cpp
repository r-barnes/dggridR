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
// DgGeoSphRF.cpp: DgGeoSphRF class implementation
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgGeoSphRF.h"
#include "DgIDGG.h"
#include "DgConstants.h"
#include "DgPolygon.h"

// these are initialized here but will get set again when the datum is created
long double DgGeoSphRF::earthRadiusKM_ = DEFAULT_RADIUS_KM;
long double DgGeoSphRF::icosaEdgeRads_ = M_ATAN2;
long double DgGeoSphRF::icosaEdgeDegs_ = icosaEdgeRads_ * M_180_PI;
long double DgGeoSphRF::icosaEdgeKM_ = icosaEdgeRads_ * earthRadiusKM_;
long double DgGeoSphRF::totalAreaKM_ = 
                      4.0L * M_PI * earthRadiusKM_ * earthRadiusKM_;

const string DgGeoSphRF::lonWrapModeStrings[] = 
             { "Wrap", "UnwrapWest", "UnwrapEast", "InvalidLonWrapMode" };

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Assumes that g is currently normalized.
//
// Returns true if wrap occurred, false otherwise.
//
int
DgGeoSphRF::lonWrap (DgGeoCoord& g, DgLonWrapMode wrapMode)
{
   // assumes g starts out wrapped (i.e., it has been normalized)
   if (wrapMode == Wrap) return false;

   int wrapped = false;
   if (wrapMode == UnwrapWest && g.lonDegs() > 0.0) {
      g.setLonDeg(g.lonDegs() - 360.0);
      wrapped = true;
   } else if (wrapMode == UnwrapEast && g.lonDegs() < 0.0) {
      g.setLonDeg(g.lonDegs() + 360.0);
      wrapped = true;
   }

   return wrapped;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Assumes that p is a polygon with less than 120' in longitude range (which
// should include all DGGS cell boundaries) and that all vertices in p are 
// currently normalized (which means p would wrap if it crosses the 
// anti-meridian).
//
// Returns true if wrap occurred, false otherwise.
//
int
DgGeoSphRF::lonWrap (DgPolygon& p, DgLonWrapMode wrapMode)
{
   // assumes p starts out wrapped (which would be the result of invoking 
   // normalize on all the vertices in p)
   if (wrapMode == Wrap) return false;

   const DgGeoSphRF* gs = dynamic_cast<const DgGeoSphRF*>(&p.rf());
   if (gs == 0) report("DgGeoSphRF::lonWrap() with non-CCRF", DgBase::Fatal);

   vector<DgAddressBase*>& v = p.addressVec();

   // first determine the range of longitude values
   long double minLon = 360.0L;
   long double maxLon = -360.0L;
   for (unsigned long i = 0; i < v.size(); i++)
   {
      DgGeoCoord g = dynamic_cast< DgAddress<DgGeoCoord>& >(*v[i]).address();
      if (g.lonDegs() < minLon) minLon = g.lonDegs();
      if (g.lonDegs() > maxLon) maxLon = g.lonDegs();
   }

   // check for wrap
   long double deltaLon = maxLon - minLon;
//cout << "DELTALON: " << deltaLon << endl;
   if (deltaLon < 120) // no wrap 
      return false;

   // perform the wrap, putting the new vertices in unwrappedVerts
   DgPolygon unwrappedVerts(*gs);
   vector<DgAddressBase*>& v2 = unwrappedVerts.addressVec();
   int wrapped = false; // has wrap occurred?
   for (unsigned long i = 0; i < v.size(); i++) {

      DgGeoCoord g = dynamic_cast< DgAddress<DgGeoCoord>& >(*v[i]).address();
      if (lonWrap(g, wrapMode)) wrapped = true;

      // add the unwrapped point
      v2.push_back(new DgAddress<DgGeoCoord>(g));
   }

   // now replace the original polygon with the unwrapped version
   p = unwrappedVerts;

   return wrapped;

} // void DgGeoSphRF::unwrap

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgGeoSphRF::densify (DgPolygon& p, long double maxDist, bool rads)
{
   if (maxDist <= 0) return;

   const DgGeoSphRF* gs = dynamic_cast<const DgGeoSphRF*>(&p.rf());
   if (gs == 0) report("DgGeoSphRF::densify() with non-CCRF", DgBase::Fatal);

   if (!rads) maxDist *= M_PI_180;

   DgPolygon densVerts(*gs);

   vector<DgAddressBase*>& v0 = p.addressVec();
   vector<DgAddressBase*>& v1 = densVerts.addressVec();

   // for each edge
   for (unsigned long i = 0; i < v0.size(); i++)
   {
      DgGeoCoord p1 = dynamic_cast< DgAddress<DgGeoCoord>& >(*v0[i]).address();
      DgGeoCoord p2 = dynamic_cast< DgAddress<DgGeoCoord>& >(
                                          *v0[(i + 1) % v0.size()]).address();

      // add the first point
      v1.push_back(new DgAddress<DgGeoCoord>(p1));

      while (DgGeoCoord::gcDist(p1, p2) > maxDist)
      {
         DgGeoCoord p3 = travelGC(p1, maxDist, azimuth(p1, p2));

         v1.push_back(new DgAddress<DgGeoCoord>(p3));

         p1 = p3;
      }

      // p2 should get pushed next time
   }

   // densify the holes recursively
   for (unsigned long int h = 0; h < p.holes().size(); h++) {
      DgPolygon* hole = new DgPolygon(*p.holes()[h]);
      densify(*hole, maxDist, rads);
      densVerts.addHole(hole);
   }

   // now replace the original with the densified version
   p = densVerts;

} // void DgGeoSphRF::densify

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgGeoCoord 
DgGeoSphRF::midPoint (const DgGeoCoord& p1, const DgGeoCoord& p2)
/*
   Return midpoint of great circle connecting two points.

   Works by calling Lian Song's routine GCmidpoint.
*/
{
   GeoCoord pp1, pp2;
   pp1.lon = p1.lon();
   pp1.lat = p1.lat();
   pp2.lon = p2.lon();
   pp2.lat = p2.lat();

   GeoCoord ans = GCmidpoint(pp1, pp2);
   
   return DgGeoCoord(ans.lon, ans.lat);

} // DgGeoCoord DgGeoSphRF::midPoint

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
long double 
DgGeoSphRF::azimuth (const DgGeoCoord& p1, const DgGeoCoord& p2, 
                     bool returnRads)
/*
   Return azimuth from p1 to p2.

   Works by calling Lian Song's routine Azimuth.
*/
{
   GeoCoord pp1, pp2;
   pp1.lon = p1.lon();
   pp1.lat = p1.lat();
   pp2.lon = p2.lon();
   pp2.lat = p2.lat();

   long double ans = Azimuth(pp1, pp2);
   
   if (!returnRads) ans *= M_180_PI;
   return ans;

} // long double DgGeoSphRF::azimuth

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgGeoCoord 
DgGeoSphRF::travelGC (const DgGeoCoord& p0, long double distance, long double azimuth,
                      bool inputRads)
/*
   Return point that is distance from p0 along azimuth. 

   Works by calling Lian Song's routine Azimuth.
*/
{
   GeoCoord pp0;
   pp0.lon = p0.lon();
   pp0.lat = p0.lat();

   if (!inputRads)
   {
      distance *= M_PI_180;
      azimuth *= M_PI_180;
   }

   GeoCoord ans = GCdaz(pp0, distance, azimuth);
   
   return DgGeoCoord(ans.lon, ans.lat);

} // long double DgGeoSphRF::azimuth

////////////////////////////////////////////////////////////////////////////////
DgGeoSphDegRF::DgGeoSphDegRF (const DgGeoSphRF& geoRFin, const string& nameIn)
         : DgContCartRF (geoRFin.network(), nameIn), geoRF_ (geoRFin)
{
   DgDegRadConverter (geoRFin, *this);
}

