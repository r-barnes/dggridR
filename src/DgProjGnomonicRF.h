////////////////////////////////////////////////////////////////////////////////
//
// DgProjGnomonicRF.h: DgProjGnomonicRF class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPROJGNOMONICRF_H
#define DGPROJGNOMONICRF_H

#include <iostream>

#include "DgConstants.h"
#include "DgGeoProjRF.h"
#include "DgDVec2D.h"

using namespace std;

class DgEllipsoidRF;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgProjGnomonicRF : public DgGeoProjRF {

   public:

      DgProjGnomonicRF (DgRFNetwork& networkIn, 
                      const string& nameIn = "GnomonicRF",
                      const DgGeoCoord& proj0In = DgGeoCoord(M_ZERO, M_ZERO),
                      long double x0In = M_ZERO, long double y0In = M_ZERO,
                      long double k0In = M_ONE, long double to_meterIn = M_ONE,
                      long double fr_meterIn = M_ONE);

      // pure virtual methods

      virtual DgDVec2D   projForward (const DgGeoCoord& addIn,
                                      const DgEllipsoidRF& e) const;

      virtual DgGeoCoord projInverse (const DgDVec2D& addIn,
                                      const DgEllipsoidRF& e) const;

   protected:

      long double sinph0_;
      long double cosph0_;
      int mode_;

}; // class DgProjGnomonicRF

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
