////////////////////////////////////////////////////////////////////////////////
//
// DgPhysicalRFBase.h: DgPhysicalRFBase class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPHYSICALRFBASE_H
#define DGPHYSICALRFBASE_H

#include "DgDiscRF.h"
#include "DgBoundedRFBase.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
template<class C> class DgPhysicalRFBase {

   public:
   
      DgPhysicalRFBase (const DgBoundedRFBase& rfIn)
          : boundedRFBase_ (rfIn) { }
          
      virtual ~DgPhysicalRFBase (void);

      const DgBoundedRFBase& boundedRFBase (void) const 
                                 { return boundedRFBase_; }

      operator const DgBoundedRFBase& (void) const { return boundedRFBase(); }
      
      const DgRFBase& rfBase (void) const { return boundedRFBase().rf(); }
      
      bool operator== (const DgPhysicalRFBase<C>& rfIn) const
                            { return this == &rfIn; }

      bool operator!= (const DgPhysicalRFBase<C>& rfIn) const
                            { return !operator==(rfIn); }

      virtual void initAllCells (void); 

      virtual void initCell (C* cell, const DgLocation& loc) { }

      virtual C* getContents (const DgLocation& loc, bool convert = true,
                              bool allocate = false) const = 0;

      virtual void replaceContents (const DgLocation& loc, C* cont, 
                                    bool convert = true) = 0; // no copy

      virtual void setContents (const DgLocation& loc, const C& cont, 
                                bool convert = true) = 0; // makes copy

      virtual void deleteContents (const DgLocation& loc, 
                                   bool convert = true) = 0;

   private:
   
      const DgBoundedRFBase& boundedRFBase_;
};

#include "../lib/DgPhysicalRFBase.hpp"

#endif
