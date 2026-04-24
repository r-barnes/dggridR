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
// DgZOrderStringRF.h: DgZOrderStringRF header file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGZOrderSTRINGRF_H
#define DGZOrderSTRINGRF_H

#include <climits>
#include <iostream>

#include "DgHierNdxStringRF.h"

class DgZOrderSystem;

////////////////////////////////////////////////////////////////////////////////
class DgZOrderStringRF : public DgHierNdxStringRF {

   public:

      // abstract method from above
      // these have dummy definitions from the superclass
      virtual DgHierNdxStringCoord quantify (const DgQ2DICoord& point) const;
      virtual DgQ2DICoord invQuantify (const DgHierNdxStringCoord& add) const;

   protected:

    DgZOrderStringRF (const DgHierNdxSystemRFBase& sysIn, int resIn, const std::string& nameIn);

    unsigned long long int unitScaleClassIres_;

    friend DgZOrderSystem;
    template<class TINT, class TSTR> friend class DgHierNdxSystemRF;

};

////////////////////////////////////////////////////////////////////////////////
#endif
