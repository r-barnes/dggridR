#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

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
// DgZ7StringRF.h: DgZ7StringRF header file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGZ7STRINGRF_H
#define DGZ7STRINGRF_H

#include <climits>
#include <iostream>

#include "DgHierNdxStringRF.h"

class DgZ7System;

////////////////////////////////////////////////////////////////////////////////
class DgZ7StringRF : public DgHierNdxStringRF {

   public:

      // abstract methods from above
      // these have dummy definitions from the superclass
      virtual DgHierNdxStringCoord quantify (const DgQ2DICoord& point) const;
      virtual DgQ2DICoord invQuantify (const DgHierNdxStringCoord& add) const;

   protected:

    DgZ7StringRF (const DgHierNdxSystemRFBase& sysIn, int resIn, const std::string& nameIn);

    unsigned long long int unitScaleClassIres_;

    friend DgZ7System;
    template<class TINT, class TSTR> friend class DgHierNdxSystemRF;

};

////////////////////////////////////////////////////////////////////////////////
#endif
