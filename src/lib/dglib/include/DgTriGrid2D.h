////////////////////////////////////////////////////////////////////////////////
//
// DgTriGrid2D.h: DgTriGrid2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGTRIGRID2D_H 
#define DGTRIGRID2D_H

#include <cmath>

#include "DgDiscRF2D.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"
#include "DgConstants.h"

class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgTriGrid2D : public DgDiscRF2D {

   public:

      DgTriGrid2D (DgRFNetwork& networkIn, 
                   const DgRF<DgDVec2D, long double>& ccFrameIn,
                   const string& nameIn = "TriC12D")
         : DgDiscRF2D (networkIn, ccFrameIn, nameIn, M_SQRT3, 1.0L,
                       M_SQRT3_2, 3.0L) 
           { }

      DgTriGrid2D (const DgTriGrid2D& grd) : DgDiscRF2D (grd) {}

      DgTriGrid2D& operator= (const DgTriGrid2D& grd)
           { DgDiscRF2D::operator=(grd); return *this; }

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const;

      static bool isUp (const DgIVec2D& add) { return !(add.j() % 2); }

   protected:

      static const long double sin60_;

      virtual void setAddVertices (const DgIVec2D& add, DgPolygon& vec) const;
      virtual void setAddNeighbors 
                                  (const DgIVec2D& add, DgLocVector& vec) const;

      virtual DgIVec2D quantify (const DgDVec2D& point) const;

      virtual DgDVec2D invQuantify (const DgIVec2D& add) const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
