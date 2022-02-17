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
// DgHexC3Grid2D.h: DgHexC3Grid2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHEXC3GRID2D_H 
#define DGHEXC3GRID2D_H

#include <cmath>

#include "DgDiscRF2D.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

class DgPolygon;

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgHexC3Grid2D : public DgDiscRF2D {

   public:

      static const DgHexC3Grid2D* makeRF (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& ccFrameIn,
                     bool isClassI = true, const string& nameIn = "HexC3D")
         { return new DgHexC3Grid2D (networkIn, ccFrameIn, isClassI, nameIn); }

      DgHexC3Grid2D (const DgHexC3Grid2D& grd) : DgDiscRF2D (grd) {}

      DgHexC3Grid2D& operator= (const DgHexC3Grid2D& grd)
           { DgDiscRF2D::operator=(grd); return *this; }

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const;

      bool isClassI (void) const { return isClassI_; }

      const DgDiscRF2D& surrogate (void) const { return *surrogate_; }
      const DgDiscRF2D& substrate (void) const { return *substrate_; }

      virtual operator string (void) const
        {
           string s = DgDiscRF::operator string() + ": DgHexC3Grid2D\n";
           s += "   -- isClassI: " + dgg::util::to_string(isClassI());
           s += "\n   -- surrogate: " + string(*surrogate_);
           s += "\n   -- substrate: " + string(*substrate_);
           
           return s;
        }  

   protected:

      DgHexC3Grid2D (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& ccFrameIn,
                     bool isClassI = true, const string& nameIn = "HexC3D");

      static const long double sin60_;

      bool isClassI_;

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
