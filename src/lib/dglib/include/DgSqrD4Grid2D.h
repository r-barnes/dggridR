////////////////////////////////////////////////////////////////////////////////
//
// DgSqrD4Grid2D.h: DgSqrD4Grid2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGSQRD4GRID2D_H 
#define DGSQRD4GRID2D_H

#include <cmath>
#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "DgDiscRF2D.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgSqrD4Grid2D : public DgDiscRF2D {

   public:

      DgSqrD4Grid2D (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& contCartFrameIn,
                     const string& nameIn = "Sqr2D")
         : DgDiscRF2D (networkIn, contCartFrameIn, nameIn, 1.0, 1.0,
                       sqrt(2.0), 1.0) 
           { r_ = 1.0 / c(); }

      DgSqrD4Grid2D (const DgSqrD4Grid2D& grd) : DgDiscRF2D (grd) {}

      DgSqrD4Grid2D& operator= (const DgSqrD4Grid2D& grd)
           { DgDiscRF2D::operator=(grd); return *this; }

      virtual int64_t dist (const DgIVec2D& add1, const DgIVec2D& add2) const
           { return abs(add2.i() - add1.i()) + abs(add2.j() - add1.j()); }

   protected:

      virtual void setAddVertices (const DgIVec2D& add, DgPolygon& vec) const;
      virtual void setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) 
                                                                          const;

      virtual DgIVec2D quantify (const DgDVec2D& point) const
           { return DgIVec2D((int64_t) floor(point.x() + 0.5), 
                             (int64_t) floor(point.y() + 0.5)); }

      virtual DgDVec2D invQuantify (const DgIVec2D& add) const
           { return DgDVec2D(add.i(), add.j()); }

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
