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
// DgIDGGSBase.h: DgIDGGSBase class definitions
//
// Version 7.0 - Kevin Sahr, 11/16/14
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGSBASE_H
#define DGIDGGSBASE_H

#include "DgIDGGBase.h"
#include "DgDiscTopoRFS.h"
#include "DgEllipsoidRF.h"
#include "DgApSeq.h"
#include "DgGridTopo.h"
#include "DgAddressType.h"
using namespace dgg::addtype;

#include <cmath>

using namespace dgg::topo;

class DgHierNdxSystemRFSBase;

////////////////////////////////////////////////////////////////////////////////
class DgIDGGSBase : public DgDiscTopoRFS<DgQ2DICoord, DgGeoCoord, long double> {

   public:

      static const DgIDGGSBase* makeRF (DgRFNetwork& network,
               const DgGeoSphRF& backFrame, const DgGeoCoord& vert0,
               long double azDegs, unsigned int aperture = 4, int nRes = 1,
               DgGridTopology gridTopo = Hexagon,
               DgGridMetric gridMetric = D6,
               const std::string& name = "IDGGS", const std::string& projType = "ISEA",
               bool isApSeq = false, const DgApSeq& apSeq = DgApSeq::defaultApSeq,
               bool isMixed43 = false, int numAp4 = 0,
               bool isSuperfund = false,
               const DgHierNdxSysType hierNdxSysType = InvalidHierNdxSysType);

      // copy constructor and operator= not implemented

      virtual const DgResAdd<DgQ2DICoord>& undefAddress (void) const
           { static DgResAdd<DgQ2DICoord>
                        undef(DgQ2DICoord::undefDgQ2DICoord, -1);
             return undef; }

      const DgIDGGBase& idggBase (int res) const
             { return static_cast<const DgIDGGBase&>(operator[](res)); }

      DgHierNdxSysType   heirNdxSysType (void) const { return hierNdxSysType_; }

      const DgGeoSphRF&  geoRF       (void) const { return geoRF_; }
      const DgGeoCoord&  vert0       (void) const { return vert0_; }
      long double        azDegs      (void) const { return azDegs_; }
      bool               isPure      (void) const { return isPure_; }
      const std::string& projType    (void) const { return projType_; }

      const DgHierNdxSystemRFSBase* hierNdxSystem (void) const { return hierNdxSystem_; }
/*
      DgGridTopology    gridTopo    (void) const { return gridTopo_; }
      DgGridMetric      gridMetric  (void) const { return gridMetric_; }
*/

   protected:

      DgIDGGSBase (DgRFNetwork& network,
               const DgGeoSphRF& backFrame,
               const DgGeoCoord& vert0,
               long double azDegs, DgHierNdxSysType hierNdxSysType = InvalidHierNdxSysType,
               int nRes = 1,
               DgGridTopology gridTopo = Hexagon,
               DgGridMetric gridMetric = D6,
               unsigned int aperture = 4,
               const std::string& name = "IDGGS",
               const std::string& projType = "ISEA",
               bool isPure = true)
        : DgDiscTopoRFS<DgQ2DICoord, DgGeoCoord, long double> (network, backFrame,
                  nRes, gridTopo, gridMetric, aperture, true, false, name),
          geoRF_ (backFrame), vert0_ (vert0), azDegs_ (azDegs), hierNdxSysType_ (hierNdxSysType), hierNdxSystem_ (nullptr),
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

   //private:

      const DgGeoSphRF& geoRF_;

      DgGeoCoord vert0_;
      long double azDegs_;
      DgHierNdxSysType hierNdxSysType_;
      DgHierNdxSystemRFSBase* hierNdxSystem_;

/*
      DgGridTopology gridTopo_;
*/
      std::string projType_;

      bool isPure_;
};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream& operator<< (std::ostream& stream, const DgIDGGSBase& dggs)
{
   stream << std::string("** DgIDGGSBase: ") <<
         (const DgDiscTopoRFS<DgQ2DICoord, DgGeoCoord, long double>&) dggs << std::endl;
   stream << "geoRF: " << dggs.geoRF();
   stream << "\nvert0: " << dggs.vert0();
   stream << "\nazDegs: " << dggs.azDegs();
   //stream << "\ngridTopo: " << dggs.gridTopo();
   //stream << "\ngridMetric: " << dggs.gridMetric();
   stream << "\nprojType: " << dggs.projType();

   return stream;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
