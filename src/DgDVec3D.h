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
// DgDVec3D.h: DgDVec3D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDVEC3D_H
#define DGDVEC3D_H

#include "DgDVec2D.h"
#include "DgString.h"

#include <cmath>
#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgDVec3D {

   public:

      static DgDVec3D midPoint (const DgDVec3D& pt1, const DgDVec3D& pt2)
                       { return pt1 * 0.5L + pt2 * 0.5L; }

      static DgDVec3D fracPoint (const DgDVec3D& pt1, const DgDVec3D& pt2,
                                 long double fraction)
                       { return pt2 * fraction + pt1 * (1.0L - fraction); }

      static const DgDVec3D& undefDgDVec3D;

      DgDVec3D (void) { x_ = 0.0L; y_ = 0.0L; z_ = 0.0L; }

      DgDVec3D (long double x, long double y, long double z) { x_ = x; y_ = y; z_ = z; }

      DgDVec3D (const DgDVec3D& pt) : x_ (pt.x()), y_ (pt.y()), z_ (pt.z()) {}

      DgDVec3D (const DgDVec2D& pt) : x_ (pt.x()), y_ (pt.y()), z_ (0.0L) {}

      void setX (long double x) { x_ = x; }
      void setY (long double y) { y_ = y; }
      void setZ (long double z) { z_ = z; }

      long double distance (const DgDVec3D& pt) const
              { return (pt - *this).magnitude(); }

      long double x (void) const { return x_; }
      long double y (void) const { return y_; }
      long double z (void) const { return z_; }

      long double magnitude (void) const
               { return sqrtl(x_ * x_ + y_ * y_ + z_ * z_); }

      inline DgDVec3D& scale (long double xScaleFactor, long double yScaleFactor,
                              long double zScaleFactor);
      //inline DgDVec3D& rotate (long double degrees);

      //inline operator XPoint   (void) const;
      inline operator string (void) const;

      inline DgDVec3D& operator=  (const DgDVec3D& pt);
      inline DgDVec3D& operator=  (const DgDVec2D& pt);
      inline DgDVec3D& operator+= (const DgDVec3D& pt);
      inline DgDVec3D& operator-= (const DgDVec3D& pt);

      inline DgDVec3D& operator*= (long double scaleFactor);
      inline DgDVec3D& operator/= (long double scaleFactor);

      friend DgDVec3D operator*  (const DgDVec3D& pt, long double scaleFactor);
      friend DgDVec3D operator*  (long double scaleFactor, const DgDVec3D& pt);
      friend DgDVec3D operator+  (const DgDVec3D& pt1, const DgDVec3D& pt2);
      friend DgDVec3D operator-  (const DgDVec3D& pt1, const DgDVec3D& pt2);

      friend bool operator== (const DgDVec3D& pt1, const DgDVec3D& pt2);
      friend bool operator!= (const DgDVec3D& pt1, const DgDVec3D& pt2);
      friend bool operator<  (const DgDVec3D& pt1, const DgDVec3D& pt2);
      friend bool operator<= (const DgDVec3D& pt1, const DgDVec3D& pt2);
      friend bool operator>  (const DgDVec3D& pt1, const DgDVec3D& pt2);
      friend bool operator>= (const DgDVec3D& pt1, const DgDVec3D& pt2);

      friend ostream& operator<< (ostream& stream, const DgDVec3D& pt);

   private:

      long double x_;
      long double y_;
      long double z_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D&
DgDVec3D::scale (long double xScaleFactor, long double yScaleFactor, long double zScaleFactor)
//
// Scale me by xScaleFactor in the x direction, yScaleFactor in the y
// direction, and zScaleFactor in the z direction.
//
////////////////////////////////////////////////////////////////////////////////
{
   x_ *= xScaleFactor;
   y_ *= yScaleFactor;
   z_ *= zScaleFactor;

   return *this;

} // void DgDVec3D::scale

/*
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D&
DgDVec3D::rotate (long double degrees)
//
// Rotate me about the origin degrees degrees in a counter-clockwise
// direction.
//
////////////////////////////////////////////////////////////////////////////////
{
   if (fabsl(degrees - 0.0L) < M_EPSILON) return *this;

   long double rotAng = degrees * M_PI_180;
   long double cosAng = cosl(rotAng);
   long double sinAng = sinl(rotAng);
   long double x = x_;
   long double y = y_;

   x_ = x * cosAng - y * sinAng;
   y_ = x * sinAng + y * cosAng;

   return *this;

} // void DgDVec3D::rotate
*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D::operator string (void) const
{
   return "(" + dgg::util::to_string(x_) + ", " + dgg::util::to_string(y_) + ", " + dgg::util::to_string(z_) + ")";

} // DgDVec3D::operator string

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D&
DgDVec3D::operator= (const DgDVec3D& pt)
{
   if (&pt != this)
   {
      x_ = pt.x_;
      y_ = pt.y_;
      z_ = pt.z_;
   }
   return *this;

} // DgDVec3D& DgDVec3D::operator=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D&
DgDVec3D::operator= (const DgDVec2D& pt)
{
   x_ = pt.x();
   y_ = pt.y();
   z_ = 0.0L;

   return *this;

} // DgDVec3D& DgDVec3D::operator=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D&
DgDVec3D::operator*= (long double scaleFactor)
{
   scale(scaleFactor, scaleFactor, scaleFactor);

   return *this;

} // DgDVec3D& DgDVec3D::operator*=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D&
DgDVec3D::operator/= (long double scaleFactor)
{
   x_ /= scaleFactor;
   y_ /= scaleFactor;
   z_ /= scaleFactor;

   return *this;

} // DgDVec3D& DgDVec3D::operator/=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D&
DgDVec3D::operator+= (const DgDVec3D& pt)
{
   x_ += pt.x_;
   y_ += pt.y_;
   z_ += pt.z_;

   return *this;

} // DgDVec3D& DgDVec3D::operator+=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D&
DgDVec3D::operator-= (const DgDVec3D& pt)
{
   x_ -= pt.x_;
   y_ -= pt.y_;
   z_ -= pt.z_;

   return *this;

} // DgDVec3D& DgDVec3D::operator-=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D
operator* (const DgDVec3D& pt, long double scaleFactor)
{
   DgDVec3D result(pt);
   result.scale(scaleFactor, scaleFactor, scaleFactor);

   return result;

} // DgDVec3D operator*

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D
operator* (long double scaleFactor, const DgDVec3D& pt)
{
   DgDVec3D result(pt);
   result.scale(scaleFactor, scaleFactor, scaleFactor);

   return result;

} // DgDVec3D operator*

////////////////////////////////////////////////////////////////////////////////
inline bool
operator== (const DgDVec3D& pt1, const DgDVec3D& pt2)
{
   return (pt1.x_ == pt2.x_ && pt1.y_ == pt2.y_ && pt1.z_ == pt2.z_);

} // bool operator==

////////////////////////////////////////////////////////////////////////////////
inline bool
operator!= (const DgDVec3D& pt1, const DgDVec3D& pt2)
{
   return !operator==(pt1, pt2);

} // bool operator!=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator> (const DgDVec3D& pt1, const DgDVec3D& pt2)
{
   return (pt1.x_ > pt2.x_ && pt1.y_ > pt2.y_ && pt1.z_ > pt2.z_);

} // bool operator>

////////////////////////////////////////////////////////////////////////////////
inline bool
operator>= (const DgDVec3D& pt1, const DgDVec3D& pt2)
{
   return (pt1.x_ >= pt2.x_ && pt1.y_ >= pt2.y_ && pt1.z_ >= pt2.z_);

} // bool operator>=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator< (const DgDVec3D& pt1, const DgDVec3D& pt2)
{
   return (pt1.x_ < pt2.x_ && pt1.y_ < pt2.y_ && pt1.z_ < pt2.z_);

} // bool operator<

////////////////////////////////////////////////////////////////////////////////
inline bool
operator<= (const DgDVec3D& pt1, const DgDVec3D& pt2)
{
   return (pt1.x_ <= pt2.x_ && pt1.y_ <= pt2.y_ && pt1.z_ <= pt2.z_);

} // bool operator<=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D
operator+ (const DgDVec3D& pt1, const DgDVec3D& pt2)
{
   DgDVec3D result(pt1);
   result += pt2;

   return result;

} // DgDVec3D operator+

////////////////////////////////////////////////////////////////////////////////
inline DgDVec3D
operator- (const DgDVec3D& pt1, const DgDVec3D& pt2)
{
   DgDVec3D result(pt1);
   result -= pt2;

   return result;

} // DgDVec3D operator-

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgDVec3D& pt)
{
   return stream << string(pt);

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
