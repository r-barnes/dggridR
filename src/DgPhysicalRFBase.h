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
// DgPhysicalRFBase.h: DgPhysicalRFBase class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPHYSICALRFBASE_H
#define DGPHYSICALRFBASE_H

#include "DgBase.h"
#include "DgBoundedRFBase.h"
#include "DgDiscRF.h"

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

#include "DgPhysicalRFBase_hpp.h"

#endif
