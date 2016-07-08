////////////////////////////////////////////////////////////////////////////////
//
// DgProjISEA.h: DgProjISEA class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPROJ_ISEA_H
#define DGPROJ_ISEA_H

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
class DgProjISEAFwd : public DgConverter<DgGeoCoord, long double,
                                        DgProjTriCoord, long double> {

   public:

      DgProjISEAFwd (const DgRF<DgGeoCoord, long double>& geoRF,
                       const DgRF<DgProjTriCoord, long double>& projTriRF);

      const DgProjTriRF& projTriRF (void) const { return *pProjTriRF_; }

      virtual DgProjTriCoord convertTypedAddress (const DgGeoCoord& addIn) 
                                                               const;

   private:

      const DgProjTriRF* pProjTriRF_;

};

////////////////////////////////////////////////////////////////////////////////
class DgProjISEAInv : public DgConverter<DgProjTriCoord, long double, 
                                           DgGeoCoord, long double> {

   public:

      DgProjISEAInv (const DgRF<DgProjTriCoord, long double>& projTriRF,
                          const DgRF<DgGeoCoord, long double>& geoRF);

      const DgProjTriRF& projTriRF (void) const { return *pProjTriRF_; }

      virtual DgGeoCoord convertTypedAddress (const DgProjTriCoord& addIn) 
                                                                      const;

   private:

      const DgProjTriRF* pProjTriRF_;

};

////////////////////////////////////////////////////////////////////////////////
class DgProjISEA : public DgIcosaProj {

   public:

      DgProjISEA (const DgRF<DgGeoCoord, long double>& geoRF,
                    const DgRF<DgProjTriCoord, long double>& projTriRF)
         : DgIcosaProj(*(new DgProjISEAFwd(geoRF, projTriRF)), 
                       *(new DgProjISEAInv(projTriRF, geoRF))) {}
};

/* C projection functions */

Vec2D sllxy (const GeoCoord& Geovect, SphIcosa& sphico, 
             int tri);

IcosaGridPt snyderFwd (const GeoCoord& ll, DgSphIcosa& sphicosa);

GeoCoord snyderInv (const IcosaGridPt& pt, SphIcosa& sphicosa);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
