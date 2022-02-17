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
// DgBoundedRFBase.h: DgBoundedRFBase class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDRFBASE_H
#define DGBOUNDEDRFBASE_H

#include "DgBoundedRFBase0.h"
#include "DgDiscRF.h"

class DgLocation;
class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
template<class B, class DB> class DgBoundedRFBase : public DgBoundedRFBase0 {

   public:

      // new abstract methods
      virtual const DgRF<B, DB>& backFrame (void) const = 0;

      // abstract methods from above
      virtual bool validLocation (const DgLocation& loc,
                                  bool convert = true) const = 0;

      virtual DgLocation& incrementLocation (DgLocation& loc,
                                             bool convert = true) const = 0;

      virtual DgLocation& decrementLocation (DgLocation& loc,
                                             bool convert = true) const = 0;

      virtual unsigned long long int seqNum (const DgLocation& loc,
                                        bool convert = true) const = 0;

      virtual DgLocation* locFromSeqNum (unsigned long long int sNum) const = 0;

      virtual string dist2str (const long long int& dist) const = 0;
      virtual long double dist2dbl (const long long int& dist) const = 0;

      virtual unsigned long long int dist2int (const long long int& dist) const = 0;

      virtual void setPoint (const DgLocation& loc, DgLocation& point)
                                                                     const = 0;

      virtual void setPoint (const DgLocation& loc, const DgRFBase& rf,
                                                  DgLocation& point) const = 0;

      virtual DgLocation* makePoint (const DgLocation& loc) const = 0;

      virtual void setVertices (const DgLocation& loc, DgPolygon& vec)
                                                                     const = 0;

      virtual void setVertices (const DgLocation& loc, const DgRFBase& rf,
                                                   DgPolygon& vec) const = 0;

      virtual DgPolygon* makeVertices (const DgLocation& loc) const = 0;

      virtual void setNeighbors (const DgLocation& loc, DgLocVector& vec)
                                                                     const = 0;

      virtual DgLocVector* makeNeighbors (const DgLocation& loc) const = 0;

   protected:

      DgBoundedRFBase (const DgRFBase& rfIn, DgLocation* firstIn,
                       DgLocation* lastIn, DgLocation* endIn,
                       bool zBasedIn = true)
         : DgBoundedRFBase0(rfIn, firstIn, lastIn, endIn, zBasedIn) { }

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
