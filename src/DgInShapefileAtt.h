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
// DgInShapefileAtt.h: DgInShapefileAtt class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINSHAPEFILEATT_H
#define DGINSHAPEFILEATT_H

#include "DgInShapefile.h"

#include "shapefil.h"

#include <set>
#include <string>

using namespace std;

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgCell;
class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class DgDBFfield {

   public:

      DgDBFfield (const string& fieldNameIn, DBFFieldType typeIn,
                 int fieldNumIn, int widthIn, int precisionIn)
         : fieldName_(fieldNameIn), type_(typeIn), fieldNum_(fieldNumIn),
           width_(widthIn), precision_(precisionIn)
      { }

      const string& fieldName (void) const { return fieldName_; }
      DBFFieldType  type      (void) const { return type_; }
      int           fieldNum  (void) const { return fieldNum_; }
      int           width     (void) const { return width_; }
      int           precision (void) const { return precision_; }

      friend bool operator== (const DgDBFfield& f1, const DgDBFfield& f2);
      friend bool operator!= (const DgDBFfield& f1, const DgDBFfield& f2);
      friend bool operator<  (const DgDBFfield& f1, const DgDBFfield& f2);
      friend bool operator<= (const DgDBFfield& f1, const DgDBFfield& f2);
      friend bool operator>  (const DgDBFfield& f1, const DgDBFfield& f2);
      friend bool operator>= (const DgDBFfield& f1, const DgDBFfield& f2);

      friend ostream& operator<< (ostream& stream, const DgDBFfield& pt);

   protected:

      string fieldName_;
      DBFFieldType type_;
      int fieldNum_;
      int width_;
      int precision_;
};

////////////////////////////////////////////////////////////////////////////////
class DgInShapefileAtt : public DgInShapefile {

   public:

      DgInShapefileAtt (const DgGeoSphRF& geoRFIn,
                     const string* fileNameIn = NULL,
                     DgReportLevel failLevel = DgBase::Fatal);

      virtual bool open (const string* fileName = NULL,
                 DgReportLevel failLevel = DgBase::Fatal);

      virtual void close (void);

      const set<DgDBFfield>& fields (void) const { return fields_; }

      int numFields (void) const { return numFields_; }

      const set<DgDBFfield>& curObjFields (void) const
                                             { return curObjFields_; }

   protected:

      virtual void getNextEntity (void);

      DBFHandle dbfFile_;
      int numFields_;
      set<DgDBFfield> fields_;
      set<DgDBFfield> curObjFields_; // non-null fields in the curShpObj_
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline bool
operator== (const DgDBFfield& f1, const DgDBFfield& f2)
{
   return (f1.fieldName().compare(f2.fieldName()) == 0);

} // bool operator==

////////////////////////////////////////////////////////////////////////////////
inline bool
operator!= (const DgDBFfield& f1, const DgDBFfield& f2)
{
   return (f1.fieldName().compare(f2.fieldName()) != 0);

} // bool operator!=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator> (const DgDBFfield& f1, const DgDBFfield& f2)
{
   return (f1.fieldName().compare(f2.fieldName()) > 0);

} // bool operator>

////////////////////////////////////////////////////////////////////////////////
inline bool
operator>= (const DgDBFfield& f1, const DgDBFfield& f2)
{
   return (f1.fieldName().compare(f2.fieldName()) >= 0);

} // bool operator>=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator< (const DgDBFfield& f1, const DgDBFfield& f2)
{
   return (f1.fieldName().compare(f2.fieldName()) < 0);

} // bool operator<

////////////////////////////////////////////////////////////////////////////////
inline bool
operator<= (const DgDBFfield& f1, const DgDBFfield& f2)
{
   return (f1.fieldName().compare(f2.fieldName()) <= 0);

} // bool operator<=

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgDBFfield& f)
{
   stream << "field: " << f.fieldName();

   char type;
   switch (f.type())
   {
      case FTString: type = 'C'; break;
      case FTInteger: type = 'I'; break;
      case FTDouble: type = 'D'; break;
      case FTLogical: type = 'B'; break;
      case FTInvalid: type = 'X'; break;
      default: type = 'X'; break;
   }
   return stream << " type: " << type << " #" << f.fieldNum()
                 << " (" << f.width() << "/" << f.precision() << ")";

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
