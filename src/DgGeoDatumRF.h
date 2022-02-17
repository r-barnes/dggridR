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
// DgGeoDatumRF.h: DgGeoDatumRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGGEODATUMRF_H
#define DGGEODATUMRF_H

using namespace std;

#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> class DgGeoDatumRF : public DgRF<A, D> {

   public:

      DgGeoDatumRF& operator= (const DgGeoDatumRF& rf)
          { DgRF<A, D>::operator=(rf); return *this; }

      // these are the pure virtual functions which subclasses need to define

      virtual D dist (const A& add1, const A& add2) const = 0;

      virtual string add2str  (const A& add)  const = 0;
      virtual string add2str  (const A& add, char delimiter) const = 0;
      virtual string dist2str (const D& dist) const = 0;
      virtual long double dist2dbl (const D& dist) const = 0;

      virtual const char* str2add (A* add, const char* str, char delimiter)
                                                                     const = 0;

      virtual unsigned long long int dist2int (const D& dist) const = 0;

      virtual const A& undefAddress (void) const = 0;

   protected:

      DgGeoDatumRF (DgRFNetwork& networkIn, const string& nameIn)
         : DgRF<A, D> (networkIn, nameIn) { }

      DgGeoDatumRF (const DgGeoDatumRF& rf) : DgRF<A, D> (rf) { }

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
