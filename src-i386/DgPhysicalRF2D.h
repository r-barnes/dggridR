////////////////////////////////////////////////////////////////////////////////
//
// DgPhysicalRF2D.h: DgPhysicalRF2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPHYSICALRF2D_H
#define DGPHYSICALRF2D_H

#include "DgPhysicalRF.h"
#include "DgBoundedRF2D.h"
#include "DgIVec2D.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
template<class C> class DgPhysicalRF2D : public DgPhysicalRF<DgIVec2D, C> {

   public:

      DgPhysicalRF2D<C> (const DgBoundedRF2D& rfIn, bool allocate = false);

      const DgBoundedRF2D& boundedRF2D (void) const { return boundedRF2D_; }

      virtual void replaceAddContents (const DgIVec2D& add, C* cont); // no copy

      virtual void setAddContents (const DgIVec2D& add, const C& cont); // copy

      virtual C* getAddContents (const DgIVec2D& add, 
                                 bool allocate = false) const;

      virtual void deleteAddContents (const DgIVec2D& add);

   protected:

      const DgBoundedRF2D& boundedRF2D_;
      C*** matrix_;

};

#include "DgPhysicalRF2D_template.h"

#endif
