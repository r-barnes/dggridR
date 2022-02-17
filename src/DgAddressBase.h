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
// DgAddressBase.h: DgAddressBase class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGADDRESSBASE_H
#define DGADDRESSBASE_H

#include <iostream>

using namespace std;

class DgDistanceBase;

////////////////////////////////////////////////////////////////////////////////
class DgAddressBase {

   public:

      virtual ~DgAddressBase (void);

   protected:

      DgAddressBase (void) {}

      virtual ostream& writeTo (ostream& stream) const = 0;

   friend ostream& operator<< (ostream& stream, const DgAddressBase& add);

};

inline ostream& operator<< (ostream& stream, const DgAddressBase& add)
{ return add.writeTo(stream); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
