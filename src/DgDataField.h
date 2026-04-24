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
// DgDataField.h: DgDataField class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDATAFIELD_H
#define DGDATAFIELD_H

#include "DgDataFieldBase.h"

class DgDistanceBase;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <typename T> class DgDataField : public DgDataFieldBase {

   public:

      DgDataField (void) {}
      DgDataField (std::string _name, DgDataType _type, T _value)
         : DgDataFieldBase (_name, _type), value_ (_value)
      { }

      void setValue (const T& value) { value_ = value; }

      T& value (void) { return value_; }
      const T& value (void) const { return value_; }

      virtual std::string valString (void) const = 0;

#ifdef USE_GDAL
      virtual void setField (OGRFeature* feature) const
         {  feature->SetField(name().c_str(), value()); }
#endif

   protected:

      virtual std::ostream& writeTo (std::ostream& stream) const
                          { return stream << value_; }

      T value_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<>
class DgDataField<long int> : public DgDataFieldBase {

   public:

      DgDataField (void) {}
      DgDataField (std::string _name, DgDataType _type, long int _value)
         : DgDataFieldBase (_name, _type), value_ (_value)
      { }

      void setValue (const long int& value) { value_ = value; }

      long int& value (void) { return value_; }
      const long int& value (void) const { return value_; }

      virtual std::string valString (void) const {
         return dgg::util::to_string(value_);
      }

#ifdef USE_GDAL
      virtual void setField (OGRFeature* feature) const
         {  GIntBig v = value();
            feature->SetField(name().c_str(), v); }
#endif

   protected:

      virtual std::ostream& writeTo (std::ostream& stream) const
                          { return stream << value_; }

      long int value_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgDataFieldDouble : public DgDataField<double> {

   public:

      DgDataFieldDouble (std::string _name, double _value = 0.0,
                std::string _fmtStr = "%#.6LF")
         : DgDataField<double> (_name, FIELD_DOUBLE, _value),
           fmtStr (_fmtStr)
      { }

      virtual int toDouble (double& val) const {
            val = value_;
            return 0;
         }

      virtual std::string valString (void) const {
         return dgg::util::to_string(value_, fmtStr.c_str());
      }

      std::string fmtStr;

};

////////////////////////////////////////////////////////////////////////////////
class DgDataFieldInt : public DgDataField<int> {

   public:

      DgDataFieldInt (std::string _name, int _value = 0)
         : DgDataField<int> (_name, FIELD_INT, _value)
      { }

      virtual int toDouble (double& val) const {
            val = value_;
            return 0;
         }

      virtual std::string valString (void) const {
         return dgg::util::to_string(value_);
      }
};

////////////////////////////////////////////////////////////////////////////////
class DgDataFieldInt64 : public DgDataField<int64_t> {

   public:

      DgDataFieldInt64 (std::string _name, int64_t _value = 0)
         : DgDataField<int64_t> (_name, FIELD_INT64, _value)
      { }

      virtual int toDouble (double& val) const {
            val = value_;
            return 0;
         }

      virtual std::string valString (void) const {
         return dgg::util::to_string(value_);
      }

#ifdef USE_GDAL
      virtual void setField (OGRFeature* feature) const
         {  GIntBig v = value();
            feature->SetField(name().c_str(), v); }
#endif

};

////////////////////////////////////////////////////////////////////////////////
class DgDataFieldString : public DgDataField<char*> {

   public:

      DgDataFieldString (const std::string& _name, char* _value = nullptr, int _fldWdth = 32)
         : DgDataField<char*> (_name, FIELD_STRING, _value),
           fldWdth_ (_fldWdth)
      { }

      DgDataFieldString (const std::string& _name, const std::string& _value = "", int _fldWdth = 32)
         : DgDataField<char*> (_name, FIELD_STRING, nullptr),
           fldWdth_ (_fldWdth)
      {
         char* valStr = new char[_value.length() + 1];
         strcpy(valStr, _value.c_str());
         setValue(valStr);
      }

      ~DgDataFieldString (void) { delete value_; }

      virtual int toDouble (double& val) const {
            //val = value_;
            return 1;
         }

      virtual std::string valString (void) const {
         return std::string(value_);
      }

      int fldWdth_;
};

////////////////////////////////////////////////////////////////////////////////
//template class DgDataField<long int>;

////////////////////////////////////////////////////////////////////////////////

#endif
