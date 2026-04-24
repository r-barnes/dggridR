#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

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
// DgIDGGS7H.h: DgIDGGS7H class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGS7H_H
#define DGIDGGS7H_H

#include "DgHexIDGGS.h"
#include "DgIVec2D.h"
#include "DgLocVector.h"
#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgIDGGS7H : public DgHexIDGGS {

   public:

      static const DgIDGGS7H* makeRF (DgRFNetwork& networkIn, const DgGeoSphRF& backFrameIn,
               const DgGeoCoord& vert0, long double azDegs, int nResIn = 1,
               const std::string& nameIn = "ISEA7H", const std::string& projType = "ISEA")
         { return new DgIDGGS7H(networkIn, backFrameIn, vert0, azDegs, nResIn, nameIn, projType); }

     ~DgIDGGS7H (void);

      DgIDGGS7H& operator= (const DgIDGGS7H& rf);

      long double frequency (void) const { return frequency_; }

   protected:

      DgIDGGS7H (DgRFNetwork& networkIn, const DgGeoSphRF& backFrameIn,
               const DgGeoCoord& vert0, long double azDegs, int nResIn = 1,
               const std::string& nameIn = "ISEA7H", const std::string& projType = "ISEA")
         : DgHexIDGGS (networkIn, backFrameIn, vert0, azDegs, 7, nResIn, nameIn, projType)
           { frequency_ = sqrtl(aperture()); }

      DgIDGGS7H (const DgIDGGS7H& rf);

      long double frequency_;

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
