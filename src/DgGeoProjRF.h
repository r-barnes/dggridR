#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//
// DgGeoProjRF.h: DgGeoProjRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGGEOPROJRF_H
#define DGGEOPROJRF_H

#include "DgConstants.h"
#include "DgDVec2D.h"
#include "DgEllipsoidRF.h"
#include "DgGeoDatumRF.h"

#include <iostream>

using namespace std;

class DgGeoCoord;
class DgEllipsoidRF;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgGeoProjRF : public DgGeoDatumRF<DgDVec2D, long double> {

   public:

      DgGeoProjRF (DgRFNetwork& networkIn, const string& nameIn,
                   const DgGeoCoord& proj0In = DgGeoCoord(M_ZERO, M_ZERO),
                   long double x0In = M_ZERO, long double y0In = M_ZERO,
                   long double k0In = M_ONE, long double to_meterIn = M_ONE,
                   long double fr_meterIn = M_ONE)
               : DgGeoDatumRF<DgDVec2D, long double> (networkIn, nameIn),
                 proj0_ (proj0In), x0_ (x0In), y0_ (y0In),
                 k0_ (k0In), to_meter_ (to_meterIn), fr_meter_ (fr_meterIn)
               { }

      // pure virtual methods

      virtual DgDVec2D   projForward (const DgGeoCoord& addIn,
                                      const DgEllipsoidRF& e) const = 0;

      virtual DgGeoCoord projInverse (const DgDVec2D& addIn,
                                      const DgEllipsoidRF& e) const = 0;

      // get methods

      const DgGeoCoord& proj0 (void) const { return proj0_; }

      long double lam0 (void) const { return proj0_.lon(); }
      long double phi0 (void) const { return proj0_.lat(); }
      long double x0 (void) const { return x0_; }
      long double y0 (void) const { return y0_; }
      long double k0 (void) const { return k0_; }

      int over (void) const { return over_; }
      int geoc (void) const { return geoc_; }

      long double to_meter (void) const { return to_meter_; }
      long double fr_meter (void) const { return fr_meter_; }

      // set methods

      void setProj0 (const DgGeoCoord& proj0In) { proj0_ = proj0In; }

      void setX0 (long double x0In) { x0_ = x0In; }
      void setY0 (long double y0In) { y0_ = y0In; }

      void setK0 (long double k0In) { k0_ = k0In; }
      void setOver (int overIn) { over_ = overIn; }
      void setGeoc (int geocIn) { geoc_ = geocIn; }
      void setTo_meter (long double to_meterIn) { to_meter_ = to_meterIn; }
      void setFr_meter (long double fr_meterIn) { fr_meter_ = fr_meterIn; }

      // inherited from above

      virtual const DgDVec2D& undefAddress (void) const
               { return DgDVec2D::undefDgDVec2D; }

      virtual long double dist (const DgDVec2D& add1, const DgDVec2D& add2) const
         { return add1.distance(add2); }

      virtual string add2str (const DgDVec2D& add) const
                       { return string(add); }

      virtual string add2str (const DgDVec2D& add, char delimiter) const
                  { return dgg::util::to_string(add.x(), formatStr()) + delimiter +
                           dgg::util::to_string(add.y(), formatStr()); }

      virtual const char* str2add (DgDVec2D* add, const char* str,
                                   char delimiter) const
                  {    if (!add) add = new DgDVec2D();
                       return add->fromString(str, delimiter); }

      virtual string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual unsigned long long int dist2int (const long double& dist) const
                       { return static_cast<unsigned long long int>(dist); }

      // the following routines are "back-doors" included for speed;
      // use with care!

      virtual DgAddressBase* vecAddress (const DgDVec2D& v) const
                    { return new DgAddress<DgDVec2D>(v); }

      virtual DgLocation* vecLocation (const DgDVec2D& v) const
                    { return makeLocation(v); }

      virtual DgDVec2D getVecAddress (const DgAddressBase& add) const
         { return static_cast< const DgAddress<DgDVec2D> &>(add).address(); }

      virtual DgDVec2D getVecLocation (const DgLocation& loc) const
                    { return *getAddress(loc); }

   protected:

      DgGeoCoord proj0_; // central lon/lat in radians

      long double x0_;   // easting; default 0.0
      long double y0_;   // northing; default 0.0
      long double k0_;   // general scaling factor; default 1.0

      // not currently used; kept for PROJ4 compatability

      long double to_meter_; // cartesian scaling
      long double fr_meter_; // cartesian scaling
      int over_;        // over-range flag
      int geoc_;        // geocentric latitude flag

}; // class DgGeoProjRF

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgGeoProjRF& obj)
{
   return stream << obj.name() << ": " << obj.proj0()
                 << " " << obj.x0() << " " << obj.y0() << " " << obj.k0();

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
