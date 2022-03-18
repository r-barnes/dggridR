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
// DgPhysicalRF2D.h: DgPhysicalRF2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPHYSICALRF2D_H
#define DGPHYSICALRF2D_H

#include "DgBoundedRF2D.h"
#include "DgDiscRFS2D.h"
#include "dgF2D.h"
#include "DgIVec2D.h"
#include "DgPhysicalRF.h"

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

#include "DgPhysicalRF2D_hpp.h"

#endif
