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
// DgHierNdxIntRF.h: DgHierNdxIntRF header file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHIERNDXINTRF_H
#define DGHIERNDXINTRF_H

//#include <climits>
#include <ostream>

#include "DgHierNdxRF.h"

#define HIERNDX_INT_TYPE uint64_t

////////////////////////////////////////////////////////////////////////////////
class DgHierNdxIntCoord : public DgHierNdxCoord<HIERNDX_INT_TYPE> {

   public:

      DgHierNdxIntCoord (void);

      DgHierNdxIntCoord (HIERNDX_INT_TYPE val)
         : DgHierNdxCoord<HIERNDX_INT_TYPE>(val) { }

      // define abstract method from above
      // output as hexadecimal string
      virtual std::string valString (void) const;
};

////////////////////////////////////////////////////////////////////////////////
class DgHierNdxIntRF : public DgHierNdxRF<DgHierNdxIntCoord> {

   public:

      static const DgHierNdxIntCoord undefCoord;

      virtual const DgHierNdxIntCoord& undefAddress (void) const
                       { return undefCoord; }

      // these assume input/output strings are in hexadecimal
      virtual std::string add2str (const DgHierNdxIntCoord& add) const { return add.valString(); }
      virtual const char* str2add (DgHierNdxIntCoord* c, const char* str, char delimiter) const;

      // these need to be defined by specializations
      // they have dummy definitions from the superclass
      //virtual DgHierNdxIntCoord quantify (const DgQ2DICoord& point) const
      //virtual DgQ2DICoord invQuantify (const DgHierNdxIntCoord& add) const

   protected:

      DgHierNdxIntRF (const DgHierNdxSystemRFBase& sysIn, int resIn, const std::string& nameIn)
         : DgHierNdxRF<DgHierNdxIntCoord>(sysIn, resIn, nameIn) { }

};

////////////////////////////////////////////////////////////////////////////////
#endif
