////////////////////////////////////////////////////////////////////////////////
//
// DgDistance.h: DgDistance class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISTANCE_H
#define DGDISTANCE_H

#include <string>
#include <iostream>
#include <stdint.h>

#include "DgRFBase.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgDistanceBase {

   public:

      virtual ~DgDistanceBase (void);

      const DgRFBase& rf (void) const { return rf_; }

      string asString (void) const { return rf().toString(*this); }

      long double asDouble (void) const { return rf().toDouble(*this); }

      uint64_t asInt (void) const { return rf().toInt(*this); }

   protected:

      DgDistanceBase (const DgRFBase& frame) 
         : rf_ (const_cast<DgRFBase&>(frame)) {}

      DgDistanceBase (const DgDistanceBase& dist)
         : rf_ (const_cast<DgRFBase&>(dist.rf())) {}

      DgDistanceBase& operator= (const DgDistanceBase& dist)
         { rf_ = const_cast<DgRFBase&>(dist.rf()); 
           return *this; }

   private:

      DgRFBase& rf_;

};

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgDistanceBase& dist)
            { return stream << dist.asString(); }

////////////////////////////////////////////////////////////////////////////////
template <class A> class DgDistance : public DgDistanceBase {

   public:

      DgDistance (const DgRFBase& frame) 
         : DgDistanceBase (frame) {}

      DgDistance (const DgDistance<A>& dist) 
         : DgDistanceBase (dist.rf()), distance_ (dist.distance()) {}

      DgDistance (const DgRFBase& frame, const A& distance) 
         : DgDistanceBase (frame), distance_ (distance) {}

      void setDistance (const A& distance) { distance_ = distance; }

      const A& distance (void) const { return distance_; }

      DgDistance& operator= (const DgDistance<A>& dist)
         { DgDistanceBase::operator=(dist); distance_ = dist.distance();
           return *this; }

   private:

      A distance_;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
