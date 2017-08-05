////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS4H.h: DgIDGGS4H class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGS4H_H 
#define DGIDGGS4H_H

#include "DgRF.h"
#include "DgLocVector.h"
#include "DgIDGGS.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgIDGGS4H : public DgIDGGS {

   public:

      DgIDGGS4H (DgRFNetwork& networkIn, const DgGeoSphRF& backFrameIn,
               const DgGeoCoord& vert0, long double azDegs, int nResIn = 1,
               const string& nameIn = "ISEA4H", const string& projType = "ISEA")
         : DgIDGGS (networkIn, backFrameIn, vert0, azDegs, 4, nResIn,
                "HEXAGON", nameIn, projType, false, 0, false) 
           { frequency_ = sqrt((long double) aperture()); }

      DgIDGGS4H (const DgIDGGS4H& rf);

     ~DgIDGGS4H (void);

      DgIDGGS4H& operator= (const DgIDGGS4H& rf);

      long double frequency (void) const { return frequency_; }

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
