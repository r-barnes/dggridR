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
// DgHierNdxSystemRF.h: DgHierNdxSystemRF header file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHIERNDXSYSTEMRF_H
#define DGHIERNDXSYSTEMRF_H

#include <climits>
#include <iostream>

#include "DgDiscRF.h"
#include "DgIDGGS.h"
#include "DgIDGG.h"
#include "DgHierNdxSystemRFBase.h"
//#include "DgHierNdxSystemRFS.h"

class DgHierNdxRFInt;
class DgHierNdxSystemRFSBase;
template <typename TINT, typename TSTR> class DgHierNdxSystemRFS;

////////////////////////////////////////////////////////////////////////////////
template <class TINT, class TSTR> class DgHierNdxSystemRF :
                   public DgHierNdxSystemRFBase {

   public:

      const TINT* typedIntRF (void) { return curRes_.intRF_; }
      const TSTR* typedStrRF (void) { return curRes_.strRF_; }

      // abstract methods from above with default dummy definitions
      //virtual DgHierNdxIntCoord toIntCoord (const DgHierNdxStringCoord& c) const;
      //virtual DgHierNdxStringCoord toStringCoord (const DgHierNdxIntCoord& c) const;

   //protected:

      DgHierNdxSystemRF (
            const DgHierNdxSystemRFS<TINT, TSTR>& hierNdxRFSIn,
            int resIn = 0, const std::string& nameIn = "DgHierNdxSystemRF")
         : DgHierNdxSystemRFBase(hierNdxRFSIn, resIn, nameIn)
      {
         if (resIn < 0 || resIn >= hierNdxRFS_.nRes()) return;

         curRes_.dgg_ = &dggs().idggBase(resIn);
         curRes_.intRF_ = new TINT(*this, resIn, name() + "IntRF"); // use default name
         curRes_.strRF_ = new TSTR(*this, resIn, name() + "StrRF"); // use default name
         DgHierNdx2WayIntToStringConverter* c2way =
                               new DgHierNdx2WayIntToStringConverter(*this);
         (void) c2way;
      }
};

////////////////////////////////////////////////////////////////////////////////
//#include "DgHierNdxRF_hpp.h"

////////////////////////////////////////////////////////////////////////////////
#endif
