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
// DgSqrD4Grid2D.h: DgSqrD4Grid2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGSQRD4GRID2D_H
#define DGSQRD4GRID2D_H

#include "DgDiscRF2D.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

#include <cmath>

class DgPolygon;

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgSqrD4Grid2D : public DgDiscRF2D {

   public:

      static const DgSqrD4Grid2D* makeRF (DgRFNetwork& networkIn,
                     const DgRF<DgDVec2D, long double>& contCartFrameIn,
                     const string& nameIn = "Sqr2D")
         { return new DgSqrD4Grid2D (networkIn, contCartFrameIn, nameIn); }

      DgSqrD4Grid2D& operator= (const DgSqrD4Grid2D& grd)
           { DgDiscRF2D::operator=(grd); return *this; }

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const
           { return abs(add2.i() - add1.i()) + abs(add2.j() - add1.j()); }

   protected:

      DgSqrD4Grid2D (DgRFNetwork& networkIn,
                     const DgRF<DgDVec2D, long double>& contCartFrameIn,
                     const string& nameIn = "Sqr2D")
         : DgDiscRF2D (networkIn, contCartFrameIn, nameIn, Square, D4,
               1.0L, 1.0L, M_SQRT2, 1.0L)
           { r_ = 1.0L / c(); }

      DgSqrD4Grid2D (const DgSqrD4Grid2D& grd) : DgDiscRF2D (grd) {}

      virtual void setAddVertices (const DgIVec2D& add, DgPolygon& vec) const;
      virtual void setAddNeighbors (const DgIVec2D& add, DgLocVector& vec)
                                                                          const;

      virtual DgIVec2D quantify (const DgDVec2D& point) const
           { return DgIVec2D((long long int) floor(point.x() + 0.5),
                             (long long int) floor(point.y() + 0.5)); }

      virtual DgDVec2D invQuantify (const DgIVec2D& add) const
           { return DgDVec2D(add.i(), add.j()); }

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
