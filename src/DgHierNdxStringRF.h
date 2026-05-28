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
// DgHierNdxStringRF.h: DgHierNdxStringRF header file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHIERNDXSTRINGRF_H
#define DGHIERNDXSTRINGRF_H

#include <climits>
#include <ostream>

#include "DgHierNdxRF.h"

////////////////////////////////////////////////////////////////////////////////
class DgHierNdxStringCoord : public DgHierNdxCoord<std::string> {

   public:

      DgHierNdxStringCoord (void);

      DgHierNdxStringCoord (std::string val)
          : DgHierNdxCoord<std::string>(val) { }

      // abstract method from above
      virtual std::string valString (void) const {
         return value();
      }
};

////////////////////////////////////////////////////////////////////////////////
class DgHierNdxStringRF : public DgHierNdxRF<DgHierNdxStringCoord> {

   public:

      static const DgHierNdxStringCoord undefCoord;

      virtual const DgHierNdxStringCoord& undefAddress (void) const
                       { return undefCoord; }

      virtual std::string add2str (const DgHierNdxStringCoord& add) const
                        { return add.value(); }

      virtual const char* str2add (DgHierNdxStringCoord* c, const char* str, char delimiter) const;

      // these need to be defined by specializations
      // they have dummy definitions from the superclass
      //virtual DgHierNdxStringCoord quantify (const DgQ2DICoord& point) const
      //virtual DgQ2DICoord invQuantify (const DgHierNdxStringCoord& add) const

   protected:

      DgHierNdxStringRF (const DgHierNdxSystemRFBase& sysIn, int resIn, const std::string& nameIn)
         : DgHierNdxRF<DgHierNdxStringCoord>(sysIn, resIn, nameIn) { }

};

////////////////////////////////////////////////////////////////////////////////
#endif
