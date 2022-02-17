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
// DgIcosaMap.h: DgIcosaMap class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGICOSAMAP_H
#define DGICOSAMAP_H

////////////////////////////////////////////////////////////////////////////////

#include "DgConstants.h"
#include "DgDVec2D.h"

////////////////////////////////////////////////////////////////////////////////
class DgIcosaTri {

   public:

      DgIcosaTri (bool mapped = false, int rot60 = 0, 
                   const DgDVec2D& offset = DgDVec2D(M_ZERO, M_ZERO)) 
         : rot60_ (rot60), offset_ (offset), mapped_ (mapped) { }

      static const DgDVec2D origin_;

      int rot60_;

      DgDVec2D offset_;
      bool mapped_;

      void translate (DgDVec2D* pt) const;
      void untranslate (DgDVec2D* pt, bool shiftOrigin = true) const;

   friend class DgIcosaMap;

};

////////////////////////////////////////////////////////////////////////////////
class DgIcosaMap {

   public:

      static DgIcosaMap defIcosaMap;
      //static DgIcosaTri defIcosaTri[20];

      DgIcosaMap (const DgIcosaTri icosaTri[20]);

      void translate (int nTri, DgDVec2D* pt) const;
      void untranslate (int nTri, DgDVec2D* pt, bool shiftOrigin = true) const;
      DgDVec2D maxOffset (void);

      DgIcosaTri icosaTri_[20];

};

////////////////////////////////////////////////////////////////////////////////

#endif
