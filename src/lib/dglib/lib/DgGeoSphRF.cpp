////////////////////////////////////////////////////////////////////////////////
//
// DgGeoSphRF.cpp: DgGeoSphRF class implementation
//
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
   new DgDegConverter(geoRFin, *this);
}

