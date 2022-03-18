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
// DgPhysicalRF.h: DgPhysicalRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPHYSICALRF_H
#define DGPHYSICALRF_H

#include "DgBoundedRF.h"
#include "DgPhysicalRFBase.h"
#include "dgRF.h"

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

#include "DgPhysicalRF_hpp.h"

#endif
