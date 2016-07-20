////////////////////////////////////////////////////////////////////////////////
//
// DgBoundedIDGGS.h: DgBoundedIDGGS class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDIDGGS_H
#define DGBOUNDEDIDGGS_H

#include <stdint.h>

#include "DgIDGGS.h"
#include "DgBoundedIDGG.h"
#include "DgIDGG.h"

class DgLocation;
class DgBoundedIDGGS;

////////////////////////////////////////////////////////////////////////////////
class DgBoundedIDGGS 
        : public DgBoundedRF<DgResAdd<DgQ2DICoord>, DgGeoCoord, long double > {

   public:

      DgBoundedIDGGS (const DgIDGGS& rf);
                    
      virtual DgResAdd<DgQ2DICoord>& incrementAddress 
                                      (DgResAdd<DgQ2DICoord>& add) const;

      virtual DgResAdd<DgQ2DICoord>& decrementAddress 
                                      (DgResAdd<DgQ2DICoord>& add) const;

      virtual bool validAddress (const DgResAdd<DgQ2DICoord>& add) const
          { return add == endAdd() || (add.res() >= 0 && 
                   add.res() < IDGGS().nRes() && 
                   grids()[add.res()]->validAddress(add.address())); }

      const DgResAdd<DgQ2DICoord>& invalidAdd (void) const 
              { return IDGGS().undefAddress(); }

      const DgIDGGS& IDGGS (void) const { return IDGGS_; }

      const vector<DgBoundedIDGG*>& grids (void) const { return *grids_; }

      virtual uint64_t seqNumAddress 
                                  (const DgResAdd<DgQ2DICoord>& add) const;

      virtual DgResAdd<DgQ2DICoord> addFromSeqNum 
                                  (uint64_t sNum) const;

   private:

      const DgIDGGS& IDGGS_;

      vector<DgBoundedIDGG*>* grids_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
