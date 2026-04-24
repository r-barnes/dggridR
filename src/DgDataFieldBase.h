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
// DgDataFieldBase.h: DgDataFieldBase class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDATAFIELDBASE_H
#define DGDATAFIELDBASE_H

#include <iostream>

class DgDistanceBase;

// field types
#ifdef USE_GDAL

#include <ogrsf_frmts.h>

   typedef OGRFieldType DgDataType;

   constexpr DgDataType FIELD_STRING = OFTString;
   constexpr DgDataType FIELD_INT = OFTInteger;
   constexpr DgDataType FIELD_INT64 = OFTInteger64;
   constexpr DgDataType FIELD_DOUBLE = OFTReal;
#else
   typedef int DgDataType;

   // these should be the same values as above
   constexpr int FIELD_STRING = 4;
   constexpr int FIELD_INT = 0;
   constexpr int FIELD_INT64 = 12;
   constexpr int FIELD_DOUBLE = 2;
#endif

////////////////////////////////////////////////////////////////////////////////
class DgDataFieldBase {

   public:

      DgDataFieldBase (std::string _name, DgDataType _fieldType = FIELD_DOUBLE)
         : name_ (_name), fieldType_ (_fieldType)
      { }

      std::string name (void) const { return name_; }
      void setName (std::string _name) { name_ = _name; }

      DgDataType fieldType (void) const { return fieldType_; }

      virtual ~DgDataFieldBase (void);

      virtual std::string valString (void) const = 0;

      // returns non-zero if conversion not valid
      virtual int toDouble (double& val) const { return 1; }

#ifdef USE_GDAL
      virtual void createField (OGRLayer* oLayer) const;
      virtual void setField (OGRFeature* feature) const = 0;
#endif

   protected:

      DgDataFieldBase (void) {}

      virtual std::ostream& writeTo (std::ostream& stream) const {
         return stream << std::string(*this);
      }

      virtual operator std::string (void) const {
         return name_;
      }

   private:

      std::string name_;

      DgDataType fieldType_;

   friend std::ostream& operator<< (std::ostream& stream, const DgDataFieldBase& dataField);
   friend class DgDataList;
};

inline std::ostream& operator<< (std::ostream& stream, const DgDataFieldBase& dataField)
{ return dataField.writeTo(stream); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
