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
// DgHierNdx.h: DgHierNdx class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHIERNDX_H
#define DGHIERNDX_H

#include "DgHierNdxIntRF.h"
#include "DgHierNdxStringRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgHierNdx {

   public:

      static const DgHierNdx undefCoord;

      DgHierNdx (const DgHierNdxIntCoord& intNdxIn, const DgHierNdxStringCoord& strNdxIn,
                 bool extModeIntIn = true)
         : intNdx_ (intNdxIn), strNdx_(strNdxIn), extModeInt_ (extModeIntIn)
         { }

      DgHierNdx (const DgHierNdx& ndxIn)
       : intNdx_ (ndxIn.intNdx()), strNdx_(ndxIn.strNdx()), extModeInt_ (ndxIn.extModeInt())
       { }

      DgHierNdx (bool extModeIntIn = true);

      // either int or string
      bool extModeInt (void) const { return extModeInt_; }

      const DgHierNdxIntCoord&    intNdx (void) const { return intNdx_; }
      const DgHierNdxStringCoord& strNdx (void) const { return strNdx_; }

      void setIntNdx (const DgHierNdxIntCoord& intNdxIn) { intNdx_ = intNdxIn; }
      void setStrNdx (const DgHierNdxStringCoord& strNdxIn) { strNdx_ = strNdxIn; }

      void setIntFromStringCoord (void);

      void setOutModeInt (bool extModeIntIn) { extModeInt_ = extModeIntIn; }

      virtual const DgHierNdx& undefAddress (void) const { return undefCoord; }

      operator std::string (void) const { return valString(); }

      std::string valString (void) const;

      bool operator== (const DgHierNdx& ndx) const
        {
            return intNdx() == ndx.intNdx() &&
                 strNdx() == ndx.strNdx() &&
                 extModeInt() == ndx.extModeInt();
        }

      bool operator!= (const DgHierNdx& ndx) const
        { return !(*this == ndx); }

      DgHierNdx& operator= (const DgHierNdx& ndx)
        {
           if (ndx != *this) {
               setIntNdx(ndx.intNdx());
               setStrNdx(ndx.strNdx());
               setOutModeInt(ndx.extModeInt());
           }

           return *this;
        }

   private:

      DgHierNdxIntCoord intNdx_;
      DgHierNdxStringCoord strNdx_;
      bool extModeInt_;

    friend class DgHierNdxSystemRFBase;
};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, const DgHierNdx& ndx)
{ return stream << std::string(ndx); }

////////////////////////////////////////////////////////////////////////////////
#endif
