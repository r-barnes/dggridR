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
// DgIDGGSBase.h: DgIDGGSBase class definitions
//
// Version 7.0 - Kevin Sahr, 11/16/14
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGSBASE_H
#define DGIDGGSBASE_H

#include "DgIDGGBase.h"
#include "DgDiscRFS.h"
#include "DgEllipsoidRF.h"
#include "DgEllipsoidRF.h"
#include "DgApSeq.h"
#include "DgGridTopo.h"

#include <cmath>

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgIDGGSBase : public DgDiscRFS<DgQ2DICoord, DgGeoCoord, long double> {

   public:

      static const DgIDGGSBase* makeRF (DgRFNetwork& network,
               const DgGeoSphRF& backFrame, const DgGeoCoord& vert0,
               long double azDegs, unsigned int aperture = 4, int nRes = 1,
               DgGridTopology gridTopo = Hexagon,
               DgGridMetric gridMetric = D6,
               const string& name = "IDGGS", const string& projType = "ISEA",
               bool isMixed43 = false, int numAp4 = 0,
               bool isSuperfund = false, bool isApSeq = false,
               const DgApSeq& apSeq = DgApSeq::defaultApSeq);

      // copy constructor and operator= not implemented

      virtual const DgResAdd<DgQ2DICoord>& undefAddress (void) const
           { static DgResAdd<DgQ2DICoord>
                        undef(DgQ2DICoord::undefDgQ2DICoord, -1);
             return undef; }

      const DgIDGGBase& idggBase (int res) const
             { return static_cast<const DgIDGGBase&>(operator[](res)); }

      const DgGeoSphRF& geoRF       (void) const { return geoRF_; }
      const DgGeoCoord& vert0       (void) const { return vert0_; }
      long double       azDegs      (void) const { return azDegs_; }
      bool              isPure      (void) const { return isPure_; }
      const string&     projType    (void) const { return projType_; }
/*
      DgGridTopology    gridTopo    (void) const { return gridTopo_; }
      DgGridMetric      gridMetric  (void) const { return gridMetric_; }
*/

   protected:

      DgIDGGSBase (DgRFNetwork& network,
               const DgGeoSphRF& backFrame,
               const DgGeoCoord& vert0,
               long double azDegs, int nRes = 1,
               unsigned int aperture = 4,
               const string& name = "IDGGS",
               DgGridTopology gridTopo = Hexagon,
               DgGridMetric gridMetric = D6,
               const string& projType = "ISEA",
               bool isPure = true)
        : DgDiscRFS<DgQ2DICoord, DgGeoCoord, long double> (network, backFrame,
                  nRes, aperture, gridTopo, gridMetric, true, false, name),
          geoRF_ (backFrame), vert0_ (vert0), azDegs_ (azDegs),
          projType_ (projType), isPure_ (isPure) { }

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

      const DgGeoSphRF& geoRF_;

      DgGeoCoord vert0_;
      long double azDegs_;

/*
      DgGridTopology gridTopo_;
*/
      string projType_;

      bool isPure_;
};

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgIDGGSBase& dggs)
{
   stream << "** DgIDGGSBase: " <<
         (const DgDiscRFS<DgQ2DICoord, DgGeoCoord, long double>&) dggs << endl;
   stream << "geoRF: " << dggs.geoRF();
   stream << "\nvert0: " << dggs.vert0();
   stream << "\nazDegs: " << dggs.azDegs();
   stream << "\ngridTopo: " << dggs.gridTopo();
   stream << "\ngridMetric: " << dggs.gridMetric();
   stream << "\nprojType: " << dggs.projType();

   return stream;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
