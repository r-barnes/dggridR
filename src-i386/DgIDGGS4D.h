////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS4D.h: DgIDGGS4D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGS4D_H 
#define DGIDGGS4D_H

#include "DgRF.h"
#include "DgLocVector.h"
#include "DgIDGGS.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgIDGGS4D : public DgIDGGS {

   public:

      DgIDGGS4D (DgRFNetwork& networkIn, const DgGeoSphRF& backFrameIn,
               const DgGeoCoord& vert0, long double azDegs, int nResIn = 1,
               const string& nameIn = "ISEA4D", const string& projType = "ISEA")
         : DgIDGGS (networkIn, backFrameIn, vert0, azDegs, 4, nResIn,
                "DIAMOND", nameIn, projType, false, 0, false) 
           { radix_ = (int) sqrt((float) aperture()); }

      DgIDGGS4D (const DgIDGGS4D& rf);

     ~DgIDGGS4D (void);

      DgIDGGS4D& operator= (const DgIDGGS4D& rf);

      int radix (void) const { return radix_; }

   protected:

      int radix_;

      // pure virtual functions from DgDiscRFS

      virtual void setAddParents (const DgResAdd<DgQ2DICoord>& add,
                                  DgLocVector& vec) const;

      virtual void setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const;

      virtual void setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const;

      virtual void setAddAllChildren (const DgResAdd<DgQ2DICoord>& add,
                                      DgLocVector& vec) const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
