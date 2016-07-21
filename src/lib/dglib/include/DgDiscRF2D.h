////////////////////////////////////////////////////////////////////////////////
//
// DgDiscRF2D.h: DgDiscRF2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRF2D_H 
#define DGDISCRF2D_H

#include <cmath>
#include <cstdint>

#include "DgDiscRF.h"
#include "DgDVec2D.h"
#include "DgIVec2D.h"

class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgDiscRF2D : public DgDiscRF<DgIVec2D, DgDVec2D, long double> {

   public:

      DgDiscRF2D (DgRFNetwork& networkIn, 
                  const DgRF<DgDVec2D, long double>& ccFrameIn,
                  const string& nameIn = "DiscRF2D", long double eIn = 1.0L,
                  long double rIn = 1.0L, long double cIn = 1.0L, long double areaIn = 1.0L)
         : DgDiscRF<DgIVec2D, DgDVec2D, long double> 
                       (networkIn, ccFrameIn, nameIn, eIn, rIn, cIn, areaIn)
           { undefLoc_ = makeLocation(undefAddress()); }

      DgDiscRF2D (const DgDiscRF2D& grd) 
           : DgDiscRF<DgIVec2D, DgDVec2D, long double> (grd)
           { undefLoc_ = makeLocation(undefAddress()); }

      DgDiscRF2D& operator= (const DgDiscRF2D& grd)
           { DgDiscRF<DgIVec2D, DgDVec2D, long double>::operator=(grd); 
             return *this; }

      virtual string add2str (const DgIVec2D& add) const { return string(add); }

      virtual string add2str (const DgIVec2D& add, char delimiter) const 
               { return dgg::util::to_string(add.i()) + delimiter + dgg::util::to_string(add.j()); }

      virtual const char* str2add (DgIVec2D* add, const char* str, 
                                   char delimiter) const
                  {    if (!add) add = new DgIVec2D();
                       return add->fromString(str, delimiter); }

      virtual const DgIVec2D& undefAddress (void) const 
                            { return DgIVec2D::undefDgIVec2D; }

      // remind users of the pure virtual functions remaining from above

      virtual std::int64_t dist (const DgIVec2D& add1, const DgIVec2D& add2) const = 0;

      virtual DgIVec2D quantify    (const DgDVec2D& point) const = 0;
      virtual DgDVec2D invQuantify (const DgIVec2D& add)   const = 0;

      virtual void setAddNeighbors (const DgIVec2D& add, 
                                    DgLocVector& vec) const = 0;

      virtual void setAddVertices  (const DgIVec2D& add, 
                                    DgPolygon& vec) const = 0;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
