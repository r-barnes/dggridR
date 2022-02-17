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
// DgIDGGS.h: original v6.1 DgIDGGS class for pre-mixed aperture grids
//
// Version 7.0 - Kevin Sahr,11/16/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGS_H
#define DGIDGGS_H

#include "DgIDGGSBase.h"
#include "DgIDGG.h"

#include <cmath>

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgIDGGS : public DgIDGGSBase {

   public:

      static const DgIDGGS* makeRF (DgRFNetwork& network,
               const DgGeoSphRF& backFrame, const DgGeoCoord& vert0,
               long double azDegs, unsigned int aperture = 4, int nRes = 1,
               DgGridTopology gridTopo = Hexagon,
               DgGridMetric gridMetric = D6,
               const string& name = "IDGGS", const string& projType = "ISEA",
               bool isMixed43 = false, int numAp4 = 0,
               bool isSuperfund = false, bool isApSeq = false,
               const DgApSeq& apSeq = DgApSeq::defaultApSeq);

      // copy constructor and operator= not implemented

      const DgIDGG& idgg (int res) const
             { return static_cast<const DgIDGG&>(idggBase(res)); }

      bool               isApSeq     (void) const { return isApSeq_; }
      const DgApSeq&     apSeq       (void) const { return apSeq_; }
      //bool               isMixed43   (void) const { return !(isPure() || isApSeq()); }
      bool               isMixed43   (void) const { return isMixed43_; }
      bool               isSuperfund (void) const { return isSuperfund_; }
      int                numAp4      (void) const { return numAp4_; }

   protected:

      DgIDGGS (DgRFNetwork& network,
               const DgGeoSphRF& backFrame,
               const DgGeoCoord& vert0,
               long double azDegs, unsigned int aperture = 4, int nRes = 1,
               DgGridTopology gridTopo = Hexagon,
               DgGridMetric gridMetric = D6,
               const string& name = "IDGGS",
               const string& projType = "ISEA",
               bool isMixed43 = false, int numAp4 = 0,
               bool isSuperfund = false, bool isApSeq = false,
               const DgApSeq& apSeq = DgApSeq::defaultApSeq);

      // remind sub-classes of the pure virtual functions remaining from above

      virtual void setAddParents (const DgResAdd<DgQ2DICoord>& add,
                                  DgLocVector& vec) const = 0;

      virtual void setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const = 0;

      virtual void setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const = 0;

      virtual void setAddAllChildren (const DgResAdd<DgQ2DICoord>& add,
                                      DgLocVector& vec) const = 0;

   private:

      int  numAp4_;
      bool isSuperfund_;
      bool isMixed43_;
      bool isApSeq_;
      DgApSeq apSeq_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
