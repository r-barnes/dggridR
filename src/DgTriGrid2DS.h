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
// DgTriGrid2DS.h: DgTriGrid2DS class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGTRIGRID2DS_H
#define DGTRIGRID2DS_H

#include "DgDiscRFS2D.h"
#include "DgIVec2D.h"
#include "DgLocVector.h"
#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgTriGrid2DS : public DgDiscRFS2D {

   public:

      static const DgTriGrid2DS* makeRF (DgRFNetwork& networkIn,
                    const DgRF<DgDVec2D, long double>& backFrameIn,
                    int nRes = 1, unsigned int apertureIn = 4,
                    bool isCongruentIn = true, bool isAlignedIn = false,
                    const string& nameIn = "3T4")
         { return new DgTriGrid2DS (networkIn, backFrameIn, nRes, apertureIn,
                    isCongruentIn, isAlignedIn, nameIn); }

     ~DgTriGrid2DS (void);

      DgTriGrid2DS& operator= (const DgTriGrid2DS& rf);

      int radix (void) const { return radix_; }

   protected:

      DgTriGrid2DS (DgRFNetwork& networkIn,
                    const DgRF<DgDVec2D, long double>& backFrameIn,
                    int nRes = 1, unsigned int apertureIn = 4,
                    bool isCongruentIn = true, bool isAlignedIn = false,
                    const string& nameIn = "3T4");

      DgTriGrid2DS (const DgTriGrid2DS& rf);

      int radix_;

      // pure virtual functions from DgDiscRFS

      virtual void setAddParents (const DgResAdd<DgIVec2D>& add,
                                  DgLocVector& vec) const;

      virtual void setAddInteriorChildren (const DgResAdd<DgIVec2D>& add,
                                           DgLocVector& vec) const;

      virtual void setAddBoundaryChildren (const DgResAdd<DgIVec2D>& add,
                                           DgLocVector& vec) const;

      virtual void setAddAllChildren (const DgResAdd<DgIVec2D>& add,
                                      DgLocVector& vec) const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
