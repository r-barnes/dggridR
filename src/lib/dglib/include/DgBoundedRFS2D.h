////////////////////////////////////////////////////////////////////////////////
//
// DgBoundedRFS2D.h: DgBoundedRFS2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDRFS2D_H
#define DGBOUNDEDRFS2D_H

#include "DgDiscRFS2D.h"
#include "DgBoundedRF2D.h"
#include "DgIVec2D.h"

class DgLocation;
class DgBoundedRFS2D;

////////////////////////////////////////////////////////////////////////////////
class DgBoundedRFS2D 
        : public DgBoundedRF<DgResAdd<DgIVec2D>, DgDVec2D, long double > {

   public:

      DgBoundedRFS2D (const DgDiscRFS2D& rf, 
                      const DgIVec2D& lowerLeft0, const DgIVec2D& upperRight0);
                    
      virtual DgResAdd<DgIVec2D>& incrementAddress 
                                      (DgResAdd<DgIVec2D>& add) const;

      virtual DgResAdd<DgIVec2D>& decrementAddress 
                                      (DgResAdd<DgIVec2D>& add) const;

      virtual bool validAddress (const DgResAdd<DgIVec2D>& add) const
          { return add == endAdd() || (add.res() >= 0 && 
                   add.res() < discRFS().nRes() && 
                   grids()[add.res()]->validAddress(add.address())); }

      const DgResAdd<DgIVec2D>& invalidAdd (void) const 
              { return discRFS().undefAddress(); }

      const DgDiscRFS2D& discRFS (void) const { return discRFS_; }

      const vector<DgBoundedRF2D*>& grids (void) const { return *grids_; }

      virtual unsigned long long int seqNumAddress (const DgResAdd<DgIVec2D>& add) 
                                                                        const;

      virtual DgResAdd<DgIVec2D> addFromSeqNum (unsigned long long int sNum) const;

   private:

      const DgDiscRFS2D& discRFS_;

      vector<DgBoundedRF2D*>* grids_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
