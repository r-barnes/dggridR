////////////////////////////////////////////////////////////////////////////////
//
// DgDmdD8Grid2D.h: DgDmdD8Grid2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDMDGRIDD82D_H 
#define DGDMDGRIDD82D_H

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

      virtual long long int dist (const DgIVec2D& add1, const DgIVec2D& add2) const
      { 
	unsigned long long int diffi = abs(add2.i() - add1.i());
	unsigned long long int diffj = abs(add2.j() - add1.j());
        return (diffi >= diffj) ? diffi : diffj; 
      }

   protected:

      virtual void setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) 
                                                                        const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
