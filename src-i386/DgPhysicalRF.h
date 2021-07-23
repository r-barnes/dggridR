////////////////////////////////////////////////////////////////////////////////
//
// DgPhysicalRF.h: DgPhysicalRF class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPHYSICALRF_H
#define DGPHYSICALRF_H

#include "DgPhysicalRFBase.h"
#include "DgBoundedRF.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
template<class A, class C, class B, class DB> class DgPhysicalRF 
                                      : public DgPhysicalRFBase<C> {

   public:

      DgPhysicalRF<A, C> (const DgBoundedRF<A, B, DB>& rfIn)
          : DgPhysicalRFBase<C> (rfIn), boundedRF_ (rfIn) { }

      const DgBoundedRF<A, B, DB>& boundedRF (void) const { return boundedRF_; }

      const DgDiscRF<A, B, DB>& discRF (void) const 
                { return boundedRF().discRF(); }
          
      virtual C* getContents (const DgLocation& loc, bool convert = true,
                              bool allocate = false) const;

      virtual void replaceContents (const DgLocation& loc, C* cont, 
                                       bool convert = true); // no copy

      virtual void setContents (const DgLocation& loc, const C& cont, 
                                bool convert = true); // makes copy

      virtual void deleteContents (const DgLocation& loc, bool convert = true);

      // pure virtual functions that must be defined by sub-classes
      
      virtual C* getAddContents (const A& add, bool allocate = false) const = 0;

      virtual void replaceAddContents (const A& add, C* cont) = 0;

      virtual void setAddContents (const A& add, const C& cont) = 0;

      virtual void deleteAddContents (const A& add) = 0;

   private:

      const DgBoundedRF<A, B, DB>& boundedRF_;

};

#include "DgPhysicalRF_template.h"

#endif
