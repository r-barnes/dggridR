////////////////////////////////////////////////////////////////////////////////
//
// DgLocBase.h: DgLocBase class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGLOCBASE_H
#define DGLOCBASE_H

#include <string>
#include <iostream>

using namespace std;

class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class DgLocBase {

   public:

      virtual ~DgLocBase (void);

      const DgRFBase& rf (void) const { return *rf_; }

      virtual int cardinality (void) const { return 1; } // default

      virtual string asString (void) const = 0;

      virtual string asString (char delimiter) const = 0;

      virtual string asAddressString (void) const = 0;

      virtual string asAddressString (char delimiter) const = 0;

      virtual const char* fromString (const char* str, char delimiter) = 0;

      virtual void clearAddress (void) = 0;

   protected:

      DgLocBase (void) : rf_ (0) { }

      DgLocBase (const DgRFBase& rfIn) : rf_ (&rfIn) { }

      const DgRFBase* rf_;

      virtual void convertTo (const DgRFBase& rf) = 0;

   friend class DgRFBase;
   friend class DgConverterBase;
   friend class DgLocList;

};

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgLocBase& loc)
            { return stream << loc.asString(); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
