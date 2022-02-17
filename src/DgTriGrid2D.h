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
// DgTriGrid2D.h: DgTriGrid2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGTRIGRID2D_H
#define DGTRIGRID2D_H

#include "DgConstants.h"
#include "DgDiscRF2D.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

#include <cmath>

class DgPolygon;

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgTriGrid2D : public DgDiscRF2D {

   public:

      static const DgTriGrid2D* makeRF (DgRFNetwork& networkIn,
                   const DgRF<DgDVec2D, long double>& ccFrameIn,
                   const string& nameIn = "TriC12D")
         { return new DgTriGrid2D (networkIn, ccFrameIn, nameIn); }

      DgTriGrid2D& operator= (const DgTriGrid2D& grd)
           { DgDiscRF2D::operator=(grd); return *this; }

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const;

      static bool isUp (const DgIVec2D& add) { return !(add.j() % 2); }

   protected:

      DgTriGrid2D (DgRFNetwork& networkIn,
                   const DgRF<DgDVec2D, long double>& ccFrameIn,
                   const string& nameIn = "TriC12D")
         : DgDiscRF2D (networkIn, ccFrameIn, nameIn, Triangle, D3, M_SQRT3,
               1.0L, M_SQRT3_2, 3.0L) { }

      DgTriGrid2D (const DgTriGrid2D& grd) : DgDiscRF2D (grd) {}

      static const long double sin60_;

      virtual void setAddVertices (const DgIVec2D& add, DgPolygon& vec) const;
      virtual void setAddNeighbors
                                  (const DgIVec2D& add, DgLocVector& vec) const;

      virtual DgIVec2D quantify (const DgDVec2D& point) const;

      virtual DgDVec2D invQuantify (const DgIVec2D& add) const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
