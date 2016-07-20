////////////////////////////////////////////////////////////////////////////////
//
// DgDmdD8Grid2D.h: DgDmdD8Grid2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDMDGRIDD82D_H 
#define DGDMDGRIDD82D_H

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "DgDmdD4Grid2D.h"

////////////////////////////////////////////////////////////////////////////////
class DgDmdD8Grid2D : public DgDmdD4Grid2D {

   public:

      DgDmdD8Grid2D (DgRFNetwork& networkIn, 
                     const DgRF<DgDVec2D, long double>& contCartFrameIn,
                     const string& nameIn = "Dmd2D")
         : DgDmdD4Grid2D (networkIn, contCartFrameIn, nameIn) { }

      DgDmdD8Grid2D (const DgDmdD8Grid2D& grd) 
         : DgDmdD4Grid2D (grd) {}

      virtual int64_t dist (const DgIVec2D& add1, const DgIVec2D& add2) const
      { 
	uint64_t diffi = abs(add2.i() - add1.i());
	uint64_t diffj = abs(add2.j() - add1.j());
        return (diffi >= diffj) ? diffi : diffj; 
      }

   protected:

      virtual void setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) 
                                                                        const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
