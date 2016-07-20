////////////////////////////////////////////////////////////////////////////////
//
// DgBoundedIDGG.h: DgBoundedIDGG class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDIDGG_H 
#define DGBOUNDEDIDGG_H

#include <stdint.h>

#include "DgIDGG.h"
#include "DgGeoSphRF.h"
#include "DgBoundedRF.h"

using namespace std;

class DgBoundedRF2D;

////////////////////////////////////////////////////////////////////////////////
class DgBoundedIDGG : public DgBoundedRF<DgQ2DICoord, DgGeoCoord, long double> {

   public:

      DgBoundedIDGG (const DgIDGG& IDGGin);

      virtual DgQ2DICoord& incrementAddress (DgQ2DICoord& add) const;
      virtual DgQ2DICoord& decrementAddress (DgQ2DICoord& add) const;

      virtual bool validAddress (const DgQ2DICoord& add) const;

      const DgQ2DICoord& invalidAdd (void) const
                         { return idgg().undefAddress(); }

      const DgIDGG& idgg (void) const { return IDGG_; }

      unsigned long long int offsetPerQuad (void) const { return offsetPerQuad_; }

      virtual unsigned long long int seqNumAddress (const DgQ2DICoord& add) const;

      virtual DgQ2DICoord addFromSeqNum (unsigned long long int sNum) const;

   protected:

      const DgBoundedRF2D& bnd2D (void) const { return *bnd2D_; }

   private:

      const DgIDGG& IDGG_;
      DgBoundedRF2D* bnd2D_;
      unsigned long long int offsetPerQuad_;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
