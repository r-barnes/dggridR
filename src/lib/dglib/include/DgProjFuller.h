////////////////////////////////////////////////////////////////////////////////
//
// DgProjFuller.h: DgProjFuller class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPROJ_FULLER_H
#define DGPROJ_FULLER_H

#include <climits>
#include <iostream>

#include "DgIVec2D.h"
#include "DgDVec2D.h"
#include "DgPolygon.h"
#include "DgRF.h"
#include "DgLocation.h"
#include "DgConverter.h"
#include "Dg2WayConverter.h"
#include "DgContCartRF.h"
#include "DgGeoSphRF.h"
#include "DgProjTriRF.h"
#include "DgEllipsoidRF.h"
#include "DgUtil.h"
#include "DgIcosaProj.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgProjFullerFwd : public DgConverter<DgGeoCoord, long double,
                                        DgProjTriCoord, long double> {

   public:

      DgProjFullerFwd (const DgRF<DgGeoCoord, long double>& geoRF,
                       const DgRF<DgProjTriCoord, long double>& projTriRF);

      const DgProjTriRF& projTriRF (void) const { return *pProjTriRF_; }

      virtual DgProjTriCoord convertTypedAddress (const DgGeoCoord& addIn) 
                                                               const;

   private:

      const DgProjTriRF* pProjTriRF_;

};

////////////////////////////////////////////////////////////////////////////////
class DgProjFullerInv : public DgConverter<DgProjTriCoord, long double, 
                                           DgGeoCoord, long double> {

   public:

      DgProjFullerInv (const DgRF<DgProjTriCoord, long double>& projTriRF,
                          const DgRF<DgGeoCoord, long double>& geoRF);

      const DgProjTriRF& projTriRF (void) const { return *pProjTriRF_; }

      virtual DgGeoCoord convertTypedAddress (const DgProjTriCoord& addIn) 
                                                                      const;

   private:

      const DgProjTriRF* pProjTriRF_;

};

////////////////////////////////////////////////////////////////////////////////
class DgProjFuller : public DgIcosaProj {

   public:

      DgProjFuller (const DgRF<DgGeoCoord, long double>& geoRF,
                    const DgRF<DgProjTriCoord, long double>& projTriRF)
         : DgIcosaProj(*(new DgProjFullerFwd(geoRF, projTriRF)), 
                       *(new DgProjFullerInv(projTriRF, geoRF))) {}
};

IcosaGridPt fullerFwd (const GeoCoord& ll, DgSphIcosa& sphicosa);
GeoCoord fullerInv (const IcosaGridPt& pt, SphIcosa& sphicosa);

////////////////////////////////////////////////////////////////////////////////
/*
   The C++ methods above are wrappers for the C functions below
*/
////////////////////////////////////////////////////////////////////////////////

Vec2D fullerFwdOneTri (const GeoCoord geo, long double R, long double * v1, 
                       long double * v2, long double * v3);

GeoCoord fullerInvOneTri (const IcosaGridPt pt, long double R, long double* pAzimuth, 
                       long double* pTheta);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
