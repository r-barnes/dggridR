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
// DgPhysicalRFS2D.h: DgPhysicalRFS2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPHYSICALRFS2D_H
#define DGPHYSICALRFS2D_H

#include "DgBoundedRFS2D.h"
#include "DgPhysicalRF.h"
#include "DgPhysicalRF2D.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
template<class C> class DgPhysicalRFS2D :
          public DgPhysicalRF< DgResAdd<DgIVec2D>, C> {

   public:

      DgPhysicalRFS2D<C> (const DgBoundedRFS2D& rfIn, bool allocate = false);

      const DgBoundedRFS2D& boundedRFS2D (void) const { return boundedRFS2D_; }

      virtual void replaceAddContents (const DgResAdd<DgIVec2D>& add,
                                       C* cont); // no copy

      virtual void setAddContents (const DgResAdd<DgIVec2D>& add,
                                   const C& cont); // copy

      virtual C* getAddContents (const  DgResAdd<DgIVec2D>& add,
                                 bool allocate = false) const;

      virtual void deleteAddContents (const DgResAdd<DgIVec2D>& add);

      const vector<const DgPhysicalRF2D<C>*>& grids (void) const { return *grids_; }

   protected:

      const DgBoundedRFS2D& boundedRFS2D_;

      vector<DgPhysicalRF2D<C>*>* grids_;

};

#include "DgPhysicalRFS2D_hpp.h"

#endif
