////////////////////////////////////////////////////////////////////////////////
//
// DgPolygon.h: DgPolygon class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPOLYGON_H
#define DGPOLYGON_H

#include "DgLocVector.h"

////////////////////////////////////////////////////////////////////////////////
class DgPolygon : public DgLocVector {

   public:

      DgPolygon (void) { }

      DgPolygon (const DgLocVector& vec)    // deep copy
         : DgLocVector (vec) { }

      DgPolygon (const DgPolygon& vec)    // deep copy
         : DgLocVector (vec) { }

      DgPolygon (const DgRFBase& rfIn, int sizeIn = 0)
         : DgLocVector (rfIn, sizeIn) { }

      const DgPolygon& operator= (const DgPolygon& vec) // deep copy
         { return reinterpret_cast<DgPolygon&>(DgLocVector::operator=(vec)); }

      bool operator== (const DgPolygon& vec) const
         { return DgLocVector::operator==(vec); }

      bool operator!= (const DgPolygon& vec) const
            { return !operator==(vec); }

      void densify (int ptsPerEdge);

};

////////////////////////////////////////////////////////////////////////////////

#endif
