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
// DgDiscRF2D.h: DgDiscRF2D class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRF2D_H
#define DGDISCRF2D_H

#include <cmath>

#include "DgDiscRF.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgDiscRF2D : public DgDiscRF<DgIVec2D, DgDVec2D, long double> {

   public:

/* TODO: necessary?
      static const DgDiscRF2D* makeRF (DgRFNetwork& networkIn,
          const DgRF<DgDVec2D, long double>& ccFrameIn,
          const string& nameIn = "DiscRF2D",
          dgg::topo::DgGridTopology gridTopoIn = dgg::topo::Hexagon,
          dgg::topo::DgGridMetric gridMetricIn = dgg::topo::D6,
          long double eIn = 1.0L, long double rIn = 1.0L,
          long double cIn = 1.0L, long double areaIn = 1.0L)
         {
            return new DgDiscRF2D (networkIn, ccFrameIn, nameIn, gridTopoIn,
                        gridMetricIn, eIn, rIn, cIn, areaIn);
         }
*/

      virtual string add2str (const DgIVec2D& add) const { return string(add); }

      virtual string add2str (const DgIVec2D& add, char delimiter) const
               { return dgg::util::to_string(add.i()) + delimiter + dgg::util::to_string(add.j()); }

      virtual const char* str2add (DgIVec2D* add, const char* str,
                                   char delimiter) const
                  {    if (!add) add = new DgIVec2D();
                       return add->fromString(str, delimiter); }

      virtual const DgIVec2D& undefAddress (void) const
                            { return DgIVec2D::undefDgIVec2D; }

      // this one only applies to aperture 7 hex grids
      virtual void setAddNeighborsBdry2 (const DgIVec2D&, DgLocVector&) const { }

      // remind users of the pure virtual functions remaining from above

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const = 0;

      virtual DgIVec2D quantify    (const DgDVec2D& point) const = 0;
      virtual DgDVec2D invQuantify (const DgIVec2D& add)   const = 0;

      virtual void setAddNeighbors (const DgIVec2D& add,
                                    DgLocVector& vec) const = 0;

      virtual void setAddVertices  (const DgIVec2D& add,
                                    DgPolygon& vec) const = 0;

   protected:

      DgDiscRF2D (DgRFNetwork& networkIn,
          const DgRF<DgDVec2D, long double>& ccFrameIn,
          const string& nameIn = "DiscRF2D",
          dgg::topo::DgGridTopology gridTopoIn = dgg::topo::Hexagon,
          dgg::topo::DgGridMetric gridMetricIn = dgg::topo::D6,
          long double eIn = 1.0L, long double rIn = 1.0L,
          long double cIn = 1.0L, long double areaIn = 1.0L)
         : DgDiscRF<DgIVec2D, DgDVec2D, long double>
               (networkIn, ccFrameIn, nameIn, gridTopoIn, gridMetricIn,
                eIn, rIn, cIn, areaIn)
           { setUndefLoc(makeLocation(undefAddress())); }

      DgDiscRF2D (const DgDiscRF2D& grd)
           : DgDiscRF<DgIVec2D, DgDVec2D, long double> (grd)
           { setUndefLoc(makeLocation(undefAddress())); }

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
