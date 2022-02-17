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
// DgIDGGS4D.h: DgIDGGS4D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGS4D_H
#define DGIDGGS4D_H

#include "DgRF.h"
#include "DgLocVector.h"
#include "DgDmdIDGG.h"
#include "DgIDGGS.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgIDGGS4D : public DgIDGGS {

   public:

      static const DgIDGGS4D* makeRF (DgRFNetwork& networkIn, const DgGeoSphRF& backFrameIn,
               const DgGeoCoord& vert0, long double azDegs, int nResIn = 1,
               const string& nameIn = "ISEA4D", const string& projType = "ISEA",
               DgGridMetric gridMetric = D4)
         { return new DgIDGGS4D (networkIn, backFrameIn, vert0, azDegs, nResIn,
               nameIn, projType, gridMetric); }

     ~DgIDGGS4D (void);

      // copy constructor and operator= not implemented
      DgIDGGS4D& operator= (const DgIDGGS4D& rf);

      const DgDmdIDGG& dmdIdgg (int res) const
             { return static_cast<const DgDmdIDGG&>(idggBase(res)); }

   protected:

      DgIDGGS4D (DgRFNetwork& networkIn, const DgGeoSphRF& backFrameIn,
               const DgGeoCoord& vert0, long double azDegs, int nResIn = 1,
               const string& nameIn = "ISEA4D", const string& projType = "ISEA",
               DgGridMetric gridMetric = D4);

      DgIDGGS4D (const DgIDGGS4D& rf);

      // pure virtual functions from DgDiscRFS

      virtual void setAddParents (const DgResAdd<DgQ2DICoord>& add,
                                  DgLocVector& vec) const;

      virtual void setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const;

      virtual void setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const;

      virtual void setAddAllChildren (const DgResAdd<DgQ2DICoord>& add,
                                      DgLocVector& vec) const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
