////////////////////////////////////////////////////////////////////////////////
//
// DgIcosaProj.h: DgIcosaProj class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGICOSA_PROJ_H
#define DGICOSA_PROJ_H

#include "DgConverter.h"
#include "Dg2WayConverter.h"
#include "DgGeoSphRF.h"
#include "DgProjTriRF.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgIcosaProj : public Dg2WayConverter {

   protected:

      DgIcosaProj (const DgConverter<DgGeoCoord, long double, 
                            DgProjTriCoord, long double>& forward,
                   const DgConverter<DgProjTriCoord, long double, 
                            DgGeoCoord, long double>& inverse)
         : Dg2WayConverter (forward, inverse) {}
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
