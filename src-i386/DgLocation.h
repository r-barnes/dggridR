////////////////////////////////////////////////////////////////////////////////
//
// DgLocation.h: DgLocation class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGLOCATION_H
#define DGLOCATION_H

#include <string>
#include <iostream>

#include "DgLocBase.h"
#include "DgAddressBase.h"
#include "DgRFBase.h"

using namespace std;

class DgDistanceBase;

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

      virtual string asString (void) const 
               { return rf().toString(*this); }

      virtual string asString (char delimiter) const 
               { return rf().toString(*this, delimiter); }

      virtual string asAddressString (void) const
               { return rf().toAddressString(*this); }

      virtual string asAddressString (char delimiter) const
               { return rf().toAddressString(*this, delimiter); }

      virtual const char* fromString (const char* str, char delimiter) 
               { return rf().fromString(*this, str, delimiter); }

      const DgAddressBase*  address  (void) const { return address_; }

      bool isUndefined (void) const { return operator==(rf().undefLoc()); }

      void setNullAddress (void) { address_ = 0; }

      virtual void clearAddress (void) { delete address_; address_ = 0; }
      
      DgLocation (const DgRFBase& rfIn, DgAddressBase* addIn) : DgLocBase (rfIn), address_ (addIn) {}

   protected:

      virtual void convertTo (const DgRFBase& rf) { rf.convert(this); }


   private:

      DgAddressBase* address_;

   friend class DgRFBase;
   friend class DgConverterBase;
   friend class DgLocVector;

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
