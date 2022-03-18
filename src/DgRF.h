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
// DgRF.h: DgRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRF_H
#define DGRF_H

#include "DgAddress.h"
#include "DgRFBase.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class DgConverterBase;
class DgLocation;
class NuCell;

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> class DgRF : public DgRFBase {

   public:

      virtual ~DgRF (void);

      DgRF& operator= (const DgRF& rf)
          { DgRFBase::operator=(rf); return *this; }

      virtual DgLocation* createLocation (const DgLocation& loc,
                                          bool convert = false) const;

// USE_NUCELL is set in MakeIncludes
#ifdef USE_NUCELL
      virtual NuCell* createCell (const NuCell& cell, bool convert = false) const;
#endif

      virtual DgLocation* makeLocation (const A& addIn) const;

      virtual DgDistanceBase* distance (const DgLocation& loc1,
                                        const DgLocation& loc2,
                                        bool convert = false) const;

      const A* getAddress (const DgLocation& loc) const;

      // deletes old address
      void forceAddress (DgLocation* loc, const A& addIn) const;

      void buildLocVector (DgLocVector* vec, vector<A*> inVec) const;

      D getDistance (const DgDistanceBase& dist) const;

      virtual string toString (const DgLocation&     loc)    const;
      virtual string toString (const DgLocVector&    locVec) const;
      virtual string toString (const DgDistanceBase& dist)   const;
      virtual long double toDouble (const DgDistanceBase& dist)   const;

      virtual unsigned long long int toInt (const DgDistanceBase& dist) const;

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
      virtual unsigned long long int dist2int (const D& dist) const = 0;

      virtual const char* str2add (A* add, const char* str, char delimiter)
                                                                      const = 0;

      virtual const A& undefAddress (void) const = 0;

   protected:

      DgRF (DgRFNetwork& networkIn, const string& nameIn)
         : DgRFBase (networkIn, nameIn) { }

      DgRF (const DgRF& rf) : DgRFBase (rf) { }

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

#include "DgRF_hpp.h"

#endif
