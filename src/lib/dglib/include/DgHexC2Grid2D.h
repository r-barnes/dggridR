////////////////////////////////////////////////////////////////////////////////
//
// DgHexC2Grid2D.h: DgHexC2Grid2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHEXC2GRID2D_H 
#define DGHEXC2GRID2D_H

#include <cmath>
#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "DgDiscRF2D.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

class DgHexC1Grid2D;
class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgHexC2Grid2D : public DgDiscRF2D {

   public:

      DgHexC2Grid2D (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& ccFrameIn,
                     const string& nameIn = "HexC12D");

      DgHexC2Grid2D (const DgHexC2Grid2D& grd) : DgDiscRF2D (grd) {}

      DgHexC2Grid2D& operator= (const DgHexC2Grid2D& grd)
           { DgDiscRF2D::operator=(grd); return *this; }

      virtual int64_t dist (const DgIVec2D& add1, const DgIVec2D& add2) const;

      const DgHexC1Grid2D& surrogate (void) const { return *surrogate_; }
      const DgHexC1Grid2D& substrate (void) const { return *substrate_; }

   protected:

      static const long double sin60_;

      DgHexC1Grid2D* surrogate_;
      DgHexC1Grid2D* substrate_;

      virtual void setAddVertices (const DgIVec2D& add, DgPolygon& vec) const;

      virtual void setAddNeighbors 
                                  (const DgIVec2D& add, DgLocVector& vec) const;

      virtual DgIVec2D quantify (const DgDVec2D& point) const;

      virtual DgDVec2D invQuantify (const DgIVec2D& add) const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
