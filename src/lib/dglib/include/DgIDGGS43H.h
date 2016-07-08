////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS43H.h: DgIDGGS43H class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGS43H_H 
#define DGIDGGS43H_H

#include "DgRF.h"
#include "DgLocVector.h"
#include "DgIDGGS.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgIDGGS43H : public DgIDGGS {

   public:

      DgIDGGS43H (DgRFNetwork& networkIn, const DgGeoSphRF& backFrameIn,
             const DgGeoCoord& vert0, long double azDegs, int nResIn = 1,
             const string& nameIn = "ISEA43H", const string& projType = "ISEA",
             int numAp4 = 0, bool isSuperfund = false)
         : DgIDGGS (networkIn, backFrameIn, vert0, azDegs, 3, nResIn,
                "HEXAGON", nameIn, projType, true, numAp4, isSuperfund)
         { frequency_ = sqrt((long double) aperture()); }

      DgIDGGS43H (const DgIDGGS43H& rf);

     ~DgIDGGS43H (void);

      DgIDGGS43H& operator= (const DgIDGGS43H& rf);

      long double frequency   (void) const { return frequency_; }

   protected:

      long double frequency_;

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
