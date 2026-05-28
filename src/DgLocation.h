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
// DgLocation.h: DgLocation class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGLOCATION_H
#define DGLOCATION_H

#include "DgAddressBase.h"
#include "DgLocBase.h"
#include "DgRFBase.h"
#include "DgDataList.h"

#include <ostream>
#include <string>

class DgDistanceBase;
class DgDataList;
class NuCell;
class NuCellVector;

////////////////////////////////////////////////////////////////////////////////
class DgLocation : public DgLocBase {

   public:

      DgLocation (void) : address_ (0) { }

      DgLocation (const DgLocation& loc)
         : DgLocBase (loc.rf()),
           address_ (loc.rf().createAddress(*loc.address())) { }

      DgLocation (const DgRFBase& rfIn) : DgLocBase (rfIn), address_ (0) { }

     ~DgLocation (void);

      DgLocation& operator= (const DgLocation& loc);

      bool operator== (const DgLocation& loc) const
            { return (rf() == loc.rf() &&
                      rf().equalAddress(*address(), *loc.address())); }

      bool operator!= (const DgLocation& loc) const
            { return !operator==(loc); }

      DgDistanceBase* distance (const DgLocation& loc,
                                bool convert = false) const
               { return rf().distance(*this, loc, convert); }

      virtual std::string asString (void) const
               { return rf().toString(*this); }

      virtual std::string asString (char delimiter) const
               { return rf().toString(*this, delimiter); }

      virtual std::string asAddressString (void) const
               { return rf().toAddressString(*this); }

      virtual std::string asAddressString (char delimiter) const
               { return rf().toAddressString(*this, delimiter); }

      virtual const char* fromString (const char* str, char delimiter)
               { return rf().fromString(*this, str, delimiter); }

      const DgAddressBase*  address  (void) const { return address_; }

      bool isUndefined (void) const { return operator==(rf().undefLoc()); }

      void setNullAddress (void) { address_ = 0; }

      virtual void clearAddress (void) { delete address_; address_ = 0; }

   protected:

      virtual void convertTo (const DgRFBase& rf) { rf.convert(this); }

      DgLocation (const DgRFBase& rfIn, DgAddressBase* addIn)
         : DgLocBase (rfIn),
           address_ (addIn) {}

   protected:

      DgAddressBase* address_;

   friend class DgRFBase;
   friend class DgConverterBase;
   friend class DgLocVector;
   friend class NuCell;
   friend class DgCellVector;
   template<class A, class D> friend class DgRF;
   template<class A, class B, class DB> friend class DgBoundedRF;

};

////////////////////////////////////////////////////////////////////////////////
class DgLocationData : public DgLocation {

   public:

      DgLocationData (void) : dataList_ (nullptr) { }

      DgLocationData (const DgRFBase& rfIn, DgDataList* _dataList = nullptr)
         : DgLocation (rfIn), dataList_ (_dataList) { }

      DgLocationData (const DgLocation& loc, DgDataList* _dataList = nullptr)
         : DgLocation (loc), dataList_ (_dataList) { }

     ~DgLocationData (void);

      virtual std::string asString (void) const
               {
                 std::stringstream ss;
                 ss << DgLocation::asString();
                 if (dataList_) ss << ":" << *dataList_;
                 return ss.str();
               }

      virtual std::string asString (char delimiter) const
               {
                 std::stringstream ss;
                 ss << DgLocation::asString(delimiter);
                 if (dataList_) ss << ":" << *dataList_;
                 return ss.str();
               }

      DgDataList* dataList (void) { return dataList_; }
      const DgDataList* dataList (void) const { return dataList_; }

      void setDataList (DgDataList* _dataList) { dataList_ = _dataList; }

   private:

      DgDataList* dataList_;
};

////////////////////////////////////////////////////////////////////////////////
inline DgLocation&
DgLocation::operator= (const DgLocation& loc)
{
   if (this != &loc)
   {
      delete address_;

      rf_ = &loc.rf();
      address_ = loc.rf().createAddress(*loc.address());
   }

   return *this;

} // DgLocation& DgLocation::operator=

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
