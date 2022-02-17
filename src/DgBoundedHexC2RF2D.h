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
// DgBoundedHexC2RF2D.h: DgBoundedHexC2RF2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDHEXC2RF2D_H
#define DGBOUNDEDHEXC2RF2D_H

#include "DgBoundedRF2D.h"
#include "DgIVec2D.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
class DgBoundedHexC2RF2D : public DgBoundedRF2D {

   public:

      DgBoundedHexC2RF2D (const DgDiscRF<DgIVec2D, DgDVec2D, long double>& rf, 
                const DgIVec2D& lowerLeft, const DgIVec2D& upperRight);
                    
      virtual DgIVec2D& incrementAddress (DgIVec2D& add) const;
      virtual DgIVec2D& decrementAddress (DgIVec2D& add) const;

      virtual bool validAddressPattern (const DgIVec2D& add) const
          { return !((add.i() + add.j()) % 3); }

      virtual unsigned long long int seqNumAddress (const DgIVec2D& add) const;

      virtual DgIVec2D addFromSeqNum (unsigned long long int sNum) const;

      virtual operator string (void) const
      {
         string s = "=== DgBoundedHexC2RF2D: " + DgBoundedRF2D::operator string();
         return s;
      }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
