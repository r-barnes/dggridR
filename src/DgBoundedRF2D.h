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
// DgBoundedRF2D.h: DgBoundedRF2D class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDRF2D_H
#define DGBOUNDEDRF2D_H

#include "DgBoundedRF.h"
#include "DgIVec2D.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
class DgBoundedRF2D : public DgBoundedRF<DgIVec2D, DgDVec2D, long double> {

   public:

      DgBoundedRF2D (const DgDiscRF<DgIVec2D, DgDVec2D, long double>& rf, 
                     const DgIVec2D& lowerLeft, const DgIVec2D& upperRight);
                    
      virtual DgIVec2D& incrementAddress (DgIVec2D& add) const;
      virtual DgIVec2D& decrementAddress (DgIVec2D& add) const;

      virtual bool validAddressPattern (const DgIVec2D& /* add */) const
                     { return true; }

      virtual bool validAddress (const DgIVec2D& add) const
          { return add.i() >= lowerLeft().i() && add.i() <= upperRight().i() &&
                   add.j() >= lowerLeft().j() && add.j() <= upperRight().j()
                   && validAddressPattern(add); }

      const DgIVec2D& lowerLeft  (void) const { return lowerLeft_; }
      const DgIVec2D& upperRight (void) const { return upperRight_; }

      long long int numI (void) const { return numI_; }
      long long int numJ (void) const { return numJ_; }
      
      const DgIVec2D& invalidAdd (void) const 
                         { return discRF().undefAddress(); }

      const DgDiscRF<DgIVec2D, DgDVec2D, long double>& discRF (void) const 
               { return discRF_; }

      virtual unsigned long long int seqNumAddress (const DgIVec2D& add) const;

      virtual DgIVec2D addFromSeqNum (unsigned long long int sNum) const;

      virtual operator string (void) const
      {
         string s = "=== DgBoundedRF2D: " + DgBoundedRF::operator string();
         s += "\n lowerLeft: " + string(lowerLeft());
         s += "\n upperRight: " + string(upperRight());
         s += "\n numI: " + dgg::util::to_string(numI());
         s += "\n numJ: " + dgg::util::to_string(numJ());
         s += "\n firstAdd: " + string(firstAdd());
         s += "\n lastAdd: " + string(lastAdd());
         s += "\n endAdd: " + string(endAdd());

         return s;
      }

   private:

      const DgDiscRF<DgIVec2D, DgDVec2D, long double>& discRF_;
      
      DgIVec2D lowerLeft_;
      DgIVec2D upperRight_;
      long long int numI_, numJ_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
