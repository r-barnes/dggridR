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
// DgPolygon.h: DgPolygon class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPOLYGON_H
#define DGPOLYGON_H

#include "DgLocVector.h"

////////////////////////////////////////////////////////////////////////////////
class DgPolygon : public DgLocVector {

   public:

      DgPolygon (void) { }

      DgPolygon (const DgLocVector& vec)    // deep copy
         : DgLocVector (vec) { }

      DgPolygon (const DgPolygon& vec)    // deep copy
         : DgLocVector (vec) { }

      DgPolygon (const DgRFBase& rfIn, int sizeIn = 0)
         : DgLocVector (rfIn, sizeIn) { }

      const DgPolygon& operator= (const DgPolygon& vec) // deep copy
         { return reinterpret_cast<DgPolygon&>(DgLocVector::operator=(vec)); }

      bool operator== (const DgPolygon& vec) const
         { return DgLocVector::operator==(vec); }

      bool operator!= (const DgPolygon& vec) const
            { return !operator==(vec); }

      void densify (int ptsPerEdge);

};

////////////////////////////////////////////////////////////////////////////////

#endif
