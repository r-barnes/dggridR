////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS.h: DgIDGGS class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGS_H 
#define DGIDGGS_H

#include <cmath>

#include "DgDiscRFS.h"
#include "DgIDGG.h"
#include "DgEllipsoidRF.h"

////////////////////////////////////////////////////////////////////////////////
class DgIDGGS : public DgDiscRFS<DgQ2DICoord, DgGeoCoord, long double> {

   public:

      static const DgIDGGS* makeRF (DgRFNetwork& network, 
               const DgGeoSphRF& backFrame, const DgGeoCoord& vert0,
               long double azDegs, unsigned int aperture = 4, int nRes = 1,
               const string& gridTopo = string("HEXAGON"),
               const string& name = "IDGGS", const string& projType = "ISEA", 
               bool isMixed43 = false, int numAp4 = 0, 
               bool isSuperfund = false);

      // copy constructor and operator= not implemented

      virtual const DgResAdd<DgQ2DICoord>& undefAddress (void) const 
           { static DgResAdd<DgQ2DICoord> 
                        undef(DgQ2DICoord::undefDgQ2DICoord, -1); 
             return undef; }

      const DgIDGG& idgg (int res) const
             { return static_cast<const DgIDGG&>(operator[](res)); }

      const DgGeoSphRF&  geoRF       (void) const { return geoRF_; }
      const DgGeoCoord&  vert0       (void) const { return vert0_; }
      long double           azDegs      (void) const { return azDegs_; }
      unsigned int       aperture    (void) const { return aperture_; }
      bool               isMixed43   (void) const { return isMixed43_; }
      bool               isSuperfund (void) const { return isSuperfund_; }
      int                numAp4      (void) const { return numAp4_; }
      const string&      gridTopo    (void) const { return gridTopo_; }
      const string&      projType    (void) const { return projType_; }

   protected:

      DgIDGGS (DgRFNetwork& network, 
               const DgGeoSphRF& backFrame,
               const DgGeoCoord& vert0,
               long double azDegs, unsigned int aperture = 4, int nRes = 1,
               const string& gridTopo = string("HEXAGON"),
               const string& name = "IDGGS", 
               const string& projType = "ISEA", 
               bool isMixed43 = false, int numAp4 = 0, 
               bool isSuperfund = false);

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

      string gridTopo_;
      string projType_;

      bool isMixed43_;
      int  numAp4_;
      bool isSuperfund_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
