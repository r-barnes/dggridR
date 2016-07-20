////////////////////////////////////////////////////////////////////////////////
//
// DgBoundedRF2D.h: DgBoundedRF2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDRF2D_H
#define DGBOUNDEDRF2D_H

#include <stdint.h>

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

      virtual bool validAddress (const DgIVec2D& add) const
          { return add.i() >= lowerLeft().i() && add.i() <= upperRight().i() &&
                   add.j() >= lowerLeft().j() && add.j() <= upperRight().j(); }

      const DgIVec2D& lowerLeft  (void) const { return lowerLeft_; }
      const DgIVec2D& upperRight (void) const { return upperRight_; }

      int64_t numI (void) const { return numI_; }
      int64_t numJ (void) const { return numJ_; }
      
      const DgIVec2D& invalidAdd (void) const 
                         { return discRF().undefAddress(); }

      const DgDiscRF<DgIVec2D, DgDVec2D, long double>& discRF (void) const 
               { return discRF_; }

      virtual uint64_t seqNumAddress (const DgIVec2D& add) const;

      virtual DgIVec2D addFromSeqNum (uint64_t sNum) const;

   private:

      const DgDiscRF<DgIVec2D, DgDVec2D, long double>& discRF_;
      
      DgIVec2D lowerLeft_;
      DgIVec2D upperRight_;
      int64_t numI_, numJ_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
