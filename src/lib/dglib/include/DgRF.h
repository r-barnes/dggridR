////////////////////////////////////////////////////////////////////////////////
//
// DgRF.h: DgRF class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRF_H
#define DGRF_H

#include <string>
#include <vector>
#include <iostream>
#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "DgRFBase.h"
#include "DgAddress.h"

using namespace std;

class DgConverterBase;
class DgLocation;

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> class DgRF : public DgRFBase {

   public:

      virtual ~DgRF (void);

      DgRF (DgRFNetwork& networkIn, const string& nameIn)
         : DgRFBase (networkIn, nameIn) { }

      DgRF (const DgRF& rf) : DgRFBase (rf) { }

      DgRF& operator= (const DgRF& rf)
          { DgRFBase::operator=(rf); return *this; }

      virtual DgLocation* createLocation (const DgLocation& loc,
                                          bool convert = false) const;

      virtual DgLocation* makeLocation (const A& addIn) const;

      virtual DgDistanceBase* distance (const DgLocation& loc1, 
                                        const DgLocation& loc2,
                                        bool convert = false) const;

      const A* getAddress (const DgLocation& loc) const;

      void buildLocVector (DgLocVector* vec, vector<A*> inVec) const; 

      D getDistance (const DgDistanceBase& dist) const;

      virtual string toString (const DgLocation&     loc)    const;
      virtual string toString (const DgLocVector&    locVec) const;
      virtual string toString (const DgDistanceBase& dist)   const;
      virtual long double toDouble (const DgDistanceBase& dist)   const;
      
      virtual uint64_t toInt (const DgDistanceBase& dist) const;

      virtual string toAddressString (const DgLocation&  loc)    const;
      virtual string toAddressString (const DgLocVector& locVec) const;

      virtual string toString (const DgLocation& loc, char delimiter) const;
      virtual string toString (const DgLocVector& loc, char delimiter) const;
      virtual string toAddressString (const DgLocation& loc, char delimiter) 
                                                                         const;
      virtual string toAddressString (const DgLocVector& locVec, char delimiter)
                                                                         const;

      virtual const char* fromString (DgLocation& loc, const char* str, 
                                      char delimiter) const;

      virtual void jumpRF (DgLocation* loc);
      
      // these are the pure virtual functions which subclasses need to define

      virtual D dist (const A& add1, const A& add2) const = 0;

      virtual string add2str  (const A& add)  const = 0;
      virtual string add2str  (const A& add, char delimiter)  const = 0;

      virtual string dist2str (const D& dist) const = 0;
      virtual long double dist2dbl (const D& dist) const = 0;
      virtual uint64_t dist2int (const D& dist) const = 0;

      virtual const char* str2add (A* add, const char* str, char delimiter) 
                                                                      const = 0;

      virtual const A& undefAddress (void) const = 0;

   protected:

      // note these assume the addresses are from this system; they are for
      // internal use only

      virtual DgAddressBase* createAddress (void) const 
                                   { return new DgAddress<A>(); }

      virtual DgAddressBase* createAddress (const DgAddressBase& addIn) const
          { return new DgAddress<A>(static_cast<const DgAddress<A>&>(addIn)); }

      virtual void copyAddress (const DgAddressBase& from,
                                      DgAddressBase* to) const
          { *(static_cast<DgAddress<A>*>(to)) = 
                                      static_cast<const DgAddress<A>&>(from); } 

      A& getTypedAddress (DgAddressBase& base) const
          { return (static_cast<DgAddress<A>&>(base)).address(); }

      const A& getTypedAddress (const DgAddressBase& base) const
          { return (static_cast<const DgAddress<A>&>(base)).address(); }

      virtual bool equalAddress (const DgAddressBase& add1, 
                                 const DgAddressBase& add2) const
          { return static_cast<const DgAddress<A>&>(add1).address() ==
                   static_cast<const DgAddress<A>&>(add2).address(); }

   friend class DgInArcGen;
   friend class DgOutAIGenFile;

};

#include "../lib/DgRF.hpp"

#endif
