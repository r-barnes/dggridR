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
// DgTriIDGG.h: DgTriIDGG class definitions
//
// Kevin Sahr, 8/11/20
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGTRIIDGG_H 
#define DGTRIIDGG_H

#include "DgIDGGBase.h"

class DgIDGGS4T;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//  Icosahedral DGG class.
//
class DgTriIDGG : public DgIDGGBase {

   public:

      DgTriIDGG (const DgIDGGS4T& dggs, unsigned int aperture = 4,
              int res = 0, const string& name = "TriIDGG", 
              unsigned int precision = DEFAULT_PRECISION);

      DgTriIDGG (const DgTriIDGG& grd);

      virtual ~DgTriIDGG (void);

      const DgIDGGS4T& triDggs (void) const;

      long double scaleFac (void) const { return scaleFac_; }
 
   protected:

      void initialize (void);

   private:

      long double scaleFac_;
};

////////////////////////////////////////////////////////////////////////////////
#endif
