////////////////////////////////////////////////////////////////////////////////
//
// DgBoundedHexC2RF2D.h: DgBoundedHexC2RF2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDHEXC2RF2D_H
#define DGBOUNDEDHEXC2RF2D_H

#include <cstdint>

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

      virtual bool validAddress (const DgIVec2D& add) const
          { return add.i() >= lowerLeft().i() && add.i() <= upperRight().i() &&
                   add.j() >= lowerLeft().j() && add.j() <= upperRight().j() &&
                   !((add.i() + add.j()) % 3); }

      virtual std::uint64_t seqNumAddress (const DgIVec2D& add) const;

      virtual DgIVec2D addFromSeqNum (std::uint64_t sNum) const;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
