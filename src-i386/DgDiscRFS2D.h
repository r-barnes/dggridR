////////////////////////////////////////////////////////////////////////////////
//
// DgDiscRFS2D.h: DgDiscRFS2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRFS2D_H 
#define DGDISCRFS2D_H

#include <cmath>

#include "DgDiscRFS.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////
class DgDiscRFS2D : public DgDiscRFS<DgIVec2D, DgDVec2D, long double> {

   public:

      static DgDiscRFS2D* makeRF (DgRFNetwork& network, 
                   const DgRF<DgDVec2D, long double>& backFrame,
                   int nRes = 1, unsigned int aperture = 4,
                   bool isCongruent = true, bool isAligned = false,
                   const string& name = "DiscRFS2D", 
                   const string geometry = "sqr8", bool isMixed43 = false,
                   int numAp4 = 0, bool isSuperfund = false);

      DgDiscRFS2D (DgRFNetwork& network, 
                   const DgRF<DgDVec2D, long double>& backFrame,
                   int nRes = 1, unsigned int aperture = 4,
                   bool isCongruent = true, bool isAligned = false,
                   const string& name = "DiscRFS2D")
        : DgDiscRFS<DgIVec2D, DgDVec2D, long double> 
              (network, backFrame, nRes, aperture, 
                               isCongruent, isAligned, name)
           { undefLoc_ = makeLocation(undefAddress()); }

      DgDiscRFS2D (const DgDiscRFS2D& grd) 
        : DgDiscRFS<DgIVec2D, DgDVec2D, long double> (grd) 
           { undefLoc_ = makeLocation(undefAddress()); }

      DgDiscRFS2D& operator= (const DgDiscRFS2D& grd)
           { DgDiscRFS<DgIVec2D, DgDVec2D, long double>::operator=(grd); 
               return *this; }

      virtual const DgResAdd<DgIVec2D>& undefAddress (void) const 
           { static DgResAdd<DgIVec2D> undef(DgIVec2D::undefDgIVec2D, -1); 
             return undef; }

   protected:

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
