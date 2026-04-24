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
// DgDiscRFS2D.h: DgDiscRFS2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRFS2D_H
#define DGDISCRFS2D_H

#include "DgApSeq.h"
#include "DgDiscTopoRFS.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

#include <cmath>

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgDiscRFS2D : public DgDiscTopoRFS<DgIVec2D, DgDVec2D, long double> {

   public:

      static const DgDiscRFS2D* makeRF (DgRFNetwork& network,
                   const DgRF<DgDVec2D, long double>& backFrame,
                   int nRes = 1, unsigned int aperture = 4,
                   DgGridTopology gridTopo = Hexagon,
                   DgGridMetric gridMetric = D6,
                   bool isCongruent = true, bool isAligned = false,
                   const std::string& name = "DiscRFS2D",
                   bool isMixed43 = false, int numAp4 = 0, bool isSuperfund = false,
                   bool isApSeq = false, const DgApSeq& apSeq = DgApSeq::defaultApSeq);

      DgDiscRFS2D (DgRFNetwork& network,
                   const DgRF<DgDVec2D, long double>& backFrame,
                   int nRes = 1, DgGridTopology gridTopo = Hexagon,
                   DgGridMetric gridMetric = D6, unsigned int aperture = 4,
                   bool isCongruent = true, bool isAligned = false,
                   const std::string& name = "DiscRFS2D")
        : DgDiscTopoRFS<DgIVec2D, DgDVec2D, long double>
              (network, backFrame, nRes, gridTopo, gridMetric, aperture,
               isCongruent, isAligned, name)
           { setUndefLoc(makeLocation(undefAddress())); }

      DgDiscRFS2D (const DgDiscRFS2D& grd)
        : DgDiscTopoRFS<DgIVec2D, DgDVec2D, long double> (grd)
           { setUndefLoc(makeLocation(undefAddress())); }
      virtual const DgResAdd<DgIVec2D>& undefAddress (void) const
           { static DgResAdd<DgIVec2D> undef(DgIVec2D::undefDgIVec2D, -1);
             return undef; }

   protected:

      void createSubConverters (void) const;

      // remind users of the pure virtual functions remaining from above

      virtual void setAddParents (const DgResAdd<DgIVec2D>& add,
                                  DgLocVector& vec) const = 0;

      virtual void setAddInteriorChildren (const DgResAdd<DgIVec2D>& add,
                                           DgLocVector& vec) const = 0;

      virtual void setAddBoundaryChildren (const DgResAdd<DgIVec2D>& add,
                                           DgLocVector& vec) const = 0;

      virtual void setAddAllChildren (const DgResAdd<DgIVec2D>& add,
                                      DgLocVector& vec) const = 0;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
