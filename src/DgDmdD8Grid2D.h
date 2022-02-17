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
// DgDmdD8Grid2D.h: DgDmdD8Grid2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDMDGRIDD82D_H 
#define DGDMDGRIDD82D_H

#include "DgDmdD4Grid2D.h"

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
class DgDmdD8Grid2D : public DgDmdD4Grid2D {

   public:

      static const DgDmdD8Grid2D* makeRF (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& contCartFrameIn,
                     const string& nameIn = "Dmd2D")
         { return new DgDmdD8Grid2D (networkIn, contCartFrameIn, nameIn); }

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const
      { 
	unsigned long long int diffi = abs(add2.i() - add1.i());
	unsigned long long int diffj = abs(add2.j() - add1.j());
        return (diffi >= diffj) ? diffi : diffj; 
      }

   protected:

      DgDmdD8Grid2D (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& contCartFrameIn,
                     const string& nameIn = "Dmd2D")
         : DgDmdD4Grid2D (networkIn, contCartFrameIn, nameIn) 
              { setGridMetric(D8); }

      DgDmdD8Grid2D (const DgDmdD8Grid2D& grd) 
         : DgDmdD4Grid2D (grd) { setGridMetric(D8); }

      virtual void setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) 
                                                                        const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
