////////////////////////////////////////////////////////////////////////////////
//
// DgGeoDatumRF.h: DgGeoDatumRF class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGGEODATUMRF_H
#define DGGEODATUMRF_H

using namespace std;

#include <cstdint>

#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> class DgGeoDatumRF : public DgRF<A, D> {

   public:

      DgGeoDatumRF (DgRFNetwork& networkIn, const string& nameIn)
         : DgRF<A, D> (networkIn, nameIn) { }

      DgGeoDatumRF (const DgGeoDatumRF& rf) : DgRF<A, D> (rf) { }

      DgGeoDatumRF& operator= (const DgGeoDatumRF& rf)
          { DgRF<A, D>::operator=(rf); return *this; }

      // these are the pure virtual functions which subclasses need to define

      virtual D dist (const A& add1, const A& add2) const = 0;

      virtual string add2str  (const A& add)  const = 0;
      virtual string add2str  (const A& add, char delimiter) const = 0;
      virtual string dist2str (const D& dist) const = 0;
      virtual long double dist2dbl (const D& dist) const = 0;

      virtual const char* str2add (A* add, const char* str, char delimiter) 
                                                                     const = 0;

      virtual std::uint64_t dist2int (const D& dist) const = 0;

      virtual const A& undefAddress (void) const = 0;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
