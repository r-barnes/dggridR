////////////////////////////////////////////////////////////////////////////////
//
// DgPhysicalRFS2D.h: DgPhysicalRFS2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPHYSICALRFS2D_H
#define DGPHYSICALRFS2D_H

#include "DgPhysicalRF.h"
#include "DgPhysicalRF2D.h"
#include "DgBoundedRFS2D.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
template<class C> class DgPhysicalRFS2D : 
          public DgPhysicalRF< DgResAdd<DgIVec2D>, C> {

   public:

      DgPhysicalRFS2D<C> (const DgBoundedRFS2D& rfIn, bool allocate = false);

      const DgBoundedRFS2D& boundedRFS2D (void) const { return boundedRFS2D_; }

      virtual void replaceAddContents (const DgResAdd<DgIVec2D>& add, 
                                       C* cont); // no copy

      virtual void setAddContents (const DgResAdd<DgIVec2D>& add, 
                                   const C& cont); // copy

      virtual C* getAddContents (const  DgResAdd<DgIVec2D>& add, 
                                 bool allocate = false) const;

      virtual void deleteAddContents (const DgResAdd<DgIVec2D>& add);

      const vector<DgPhysicalRF2D<C>*>& grids (void) const { return *grids_; }

   protected:

      const DgBoundedRFS2D& boundedRFS2D_;
      
      vector<DgPhysicalRF2D<C>*>* grids_;

};

#include "DgPhysicalRFS2D.hpp"

#endif
