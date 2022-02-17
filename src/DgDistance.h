#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

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
// DgDistance.h: DgDistance class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISTANCE_H
#define DGDISTANCE_H

#include "DgRFBase.h"

#include <iostream>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgDistanceBase {

   public:

      virtual ~DgDistanceBase (void);

      const DgRFBase& rf (void) const { return rf_; }

      string asString (void) const { return rf().toString(*this); }

      long double asDouble (void) const { return rf().toDouble(*this); }

      unsigned long long int asInt (void) const { return rf().toInt(*this); }

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
