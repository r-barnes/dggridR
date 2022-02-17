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
// DgDmdD4Grid2D.h: DgDmdD4Grid2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDMDD4GRID2D_H
#define DGDMDD4GRID2D_H

#include "DgConstants.h"
#include "DgDiscRF2D.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

#include <cmath>

class DgPolygon;

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgDmdD4Grid2D : public DgDiscRF2D {

   public:

      static const DgDmdD4Grid2D* makeRF (DgRFNetwork& networkIn,
                     const DgRF<DgDVec2D, long double>& contCartFrameIn,
                     const string& nameIn = "Dmd2D")
         { return new DgDmdD4Grid2D (networkIn, contCartFrameIn, nameIn); }

      DgDmdD4Grid2D& operator= (const DgDmdD4Grid2D& grd)
           { DgDiscRF2D::operator=(grd); return *this; }

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const
           { return abs(add2.i() - add1.i()) + abs(add2.j() - add1.j()); }

      static long double yOff (void) { return yOff_; }
      static long double xOff (void) { return xOff_; }

   protected:

      DgDmdD4Grid2D (DgRFNetwork& networkIn,
                     const DgRF<DgDVec2D, long double>& contCartFrameIn,
                     const string& nameIn = "Dmd2D")
         : DgDiscRF2D (networkIn, contCartFrameIn, nameIn, Diamond, D4,
                 1.0L, 1.0L, M_SQRT3_2, 1.0L)
           { r_ = c(); }

      DgDmdD4Grid2D (const DgDmdD4Grid2D& grd) : DgDiscRF2D (grd) {}

      virtual void setAddVertices (const DgIVec2D& add, DgPolygon& vec) const;
      virtual void setAddNeighbors (const DgIVec2D& add, DgLocVector& vec)
                                                                          const;

      static const long double yOff_;
      static const long double yOff2_;
      static const long double xOff_;
      static const long double xOffComp_;
      static const long double skewFac_;

      virtual DgIVec2D quantify (const DgDVec2D& point) const
         {
            DgDVec2D tmp(point);
            tmp.setX(tmp.x() + skewFac_ * tmp.y());

            return DgIVec2D(dgg::util::lrint(tmp.x()),
                            dgg::util::lrint(tmp.y() / yOff2_));

         } // DgIVec2D DgDmdD4Grid2D::quantify


      virtual DgDVec2D invQuantify (const DgIVec2D& add) const
         {
            DgDVec2D tmp(add.i(), add.j() * yOff2_);
            tmp.setX(tmp.x() - skewFac_ * tmp.y());

            return tmp;

         } // DgDVec2D DgDmdD4Grid2D::invQuantify

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
