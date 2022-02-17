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
// DgLocBase.h: DgLocBase class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGLOCBASE_H
#define DGLOCBASE_H

#include <iostream>
#include <string>

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
