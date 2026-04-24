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
// DgZ3RF.h: DgZ3RF header file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGZ3RF_H
#define DGZ3RF_H

#include "DgHierNdxIntRF.h"

class DgZ3System;

////////////////////////////////////////////////////////////////////////////////
class DgZ3RF : public DgHierNdxIntRF {

   public:

      static const DgHierNdxIntCoord undefZ3IntCoord;

      // abstract method from above
      virtual const char* str2add (DgHierNdxIntCoord* c, const char* str,
                   char delimiter) const;

      // these have dummy definitions from the superclass
      virtual DgHierNdxIntCoord quantify (const DgQ2DICoord& point) const;
      virtual DgQ2DICoord invQuantify (const DgHierNdxIntCoord& add) const;

   protected:

    DgZ3RF (const DgHierNdxSystemRFBase& sysIn, int resIn, const std::string& nameIn);

    friend DgZ3System;
    template<class TINT, class TSTR> friend class DgHierNdxSystemRF;
};

////////////////////////////////////////////////////////////////////////////////
#endif
