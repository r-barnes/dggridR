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
// DgHexC2Grid2D.h: DgHexC2Grid2D class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHEXC2GRID2D_H 
#define DGHEXC2GRID2D_H

#include <cmath>

#include "DgDiscRF2D.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

class DgPolygon;
  
using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgHexC2Grid2D : public DgDiscRF2D {

   public:

      static const DgHexC2Grid2D* makeRF (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& ccFrameIn,
                     const string& nameIn = "HexC2D")
         { return new DgHexC2Grid2D (networkIn, ccFrameIn, nameIn); }

      DgHexC2Grid2D& operator= (const DgHexC2Grid2D& grd)
           { DgDiscRF2D::operator=(grd); return *this; }

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const;

      const DgDiscRF2D& surrogate (void) const { return *surrogate_; }
      const DgDiscRF2D& substrate (void) const { return *substrate_; }

      virtual operator string (void) const
        {
           string s = DgDiscRF::operator string() + ": DgHexC2Grid2D\n";
           s += "   -- surrogate: " + string(*surrogate_);
           s += "\n   -- substrate: " + string(*substrate_);
           
           return s;
        }  

   protected:

      DgHexC2Grid2D (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& ccFrameIn,
                     const string& nameIn = "HexC2D");

      DgHexC2Grid2D (const DgHexC2Grid2D& grd) : DgDiscRF2D (grd) {}

      static const long double sin60_;

      const DgDiscRF2D* surrogate_;
      const DgDiscRF2D* substrate_;

      virtual void setAddVertices (const DgIVec2D& add, DgPolygon& vec) const;

      virtual void setAddNeighbors 
                                  (const DgIVec2D& add, DgLocVector& vec) const;
      virtual void setAddNeighborsBdry2 
                                  (const DgIVec2D& add, DgLocVector& vec) const;

      virtual DgIVec2D quantify (const DgDVec2D& point) const;

      virtual DgDVec2D invQuantify (const DgIVec2D& add) const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
