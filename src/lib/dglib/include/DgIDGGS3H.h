////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS3H.h: DgIDGGS3H class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGS3H_H 
#define DGIDGGS3H_H

#include "DgRF.h"
#include "DgLocVector.h"
#include "DgIDGGS.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgIDGGS3H : public DgIDGGS {

   public:

      DgIDGGS3H (DgRFNetwork& networkIn, const DgGeoSphRF& backFrameIn,
               const DgGeoCoord& vert0, long double azDegs, int nResIn = 1,
               const string& nameIn = "ISEA3H", const string& projType = "ISEA")
         : DgIDGGS (networkIn, backFrameIn, vert0, azDegs, 3, nResIn,
                "HEXAGON", nameIn, projType, false, 0, false) 
           { frequency_ = sqrt((long double) aperture()); }

      DgIDGGS3H (const DgIDGGS3H& rf);

     ~DgIDGGS3H (void);

      DgIDGGS3H& operator= (const DgIDGGS3H& rf);

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
