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
// DgHexIDGG.h: DgHexIDGG class definitions
//
// Version 7.0 - Kevin Sahr, 11/16/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHEXIDGG_H 
#define DGHEXIDGG_H

#include "DgIDGG.h"

class DgHexIDGGS;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//  Icosahedral DGG class.
//
class DgHexIDGG : public DgIDGG {

   //using DgDiscRF<DgQ2DICoord, DgGeoCoord, long double>::setVertices;

   public:

      DgHexIDGG (const DgHexIDGGS& dggs, unsigned int aperture = 4,
              int res = 0, const string& name = "HexIDGG", 
              unsigned int precision = DEFAULT_PRECISION);

      DgHexIDGG (const DgHexIDGG& grd);

      virtual ~DgHexIDGG (void);

      const DgHexIDGGS& hexDggs (void) const;

      long double scaleFac (void) const { return scaleFac_; }
      long double rotRads  (void) const { return rotRads_; }
      long double rotDegs  (void) const { return 180.0L * rotRads_ / M_PI; }
 
   protected:

      virtual void initialize (void);

   private:

      long double scaleFac_;
      long double rotRads_;
};

////////////////////////////////////////////////////////////////////////////////
#endif
