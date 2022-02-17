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
// DgProjISEA.h: DgProjISEA class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPROJ_ISEA_H
#define DGPROJ_ISEA_H

#include "Dg2WayConverter.h"
#include "DgContCartRF.h"
#include "DgConverter.h"
#include "DgDVec2D.h"
#include "DgEllipsoidRF.h"
#include "DgGeoSphRF.h"
#include "DgIcosaProj.h"
#include "DgIVec2D.h"
#include "DgLocation.h"
#include "DgPolygon.h"
#include "DgProjTriRF.h"
#include "DgRF.h"
#include "DgUtil.h"

#include <climits>
#include <iostream>

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
