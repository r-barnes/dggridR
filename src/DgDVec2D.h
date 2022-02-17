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
// DgDVec2D.h: DgDVec2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDVEC2D_H
#define DGDVEC2D_H

#include "DgConstants.h"
#include "DgString.h"
#include "DgUtil.h"

#include <cfloat>
#include <cmath>
#include <iostream>

using namespace std;

class DgDVec3D;

////////////////////////////////////////////////////////////////////////////////
class DgDVec2D {

   public:

      static DgDVec2D midPoint (const DgDVec2D& pt1, const DgDVec2D& pt2)
                       { return pt1 * M_HALF + pt2 * M_HALF; }

      static DgDVec2D fracPoint (const DgDVec2D& pt1, const DgDVec2D& pt2,
                                long double fraction)
                       { return pt2 * fraction + pt1 * (M_ONE - fraction); }

      static const DgDVec2D& undefDgDVec2D;

      DgDVec2D (void) { x_ = M_ZERO; y_ = M_ZERO; }

      DgDVec2D (long double x, long double y) { x_ = x; y_ = y; }

      DgDVec2D (const DgDVec2D& pt) : x_ (pt.x_), y_ (pt.y_) {}
      DgDVec2D (const DgDVec3D& pt);
      //DgDVec2D (const XPoint&  pt) : x_ (pt.x),  y_ (pt.y)  {}

      void setX (long double x) { x_ = x; }
      void setY (long double y) { y_ = y; }

      long double distance (const DgDVec2D& pt) const
              { return (pt - *this).magnitude(); }

      long double x (void) const { return x_; }
      long double y (void) const { return y_; }

      long double magnitude (void) const { return sqrtl(x_ * x_ + y_ * y_); }

      long double dotProd (const DgDVec2D& pt) const
                { return x_ * pt.x() + y_ * pt.y(); }

      long double angle (const DgDVec2D& pt) const  // angle between in radians
                { return acosl(dotProd(pt) / (magnitude() * pt.magnitude())); }

      const char* fromString (const char* str, char delimiter);

      inline DgDVec2D& scale (long double xScaleFactor, long double yScaleFactor);
      inline DgDVec2D& rotate (long double degrees);

      //inline operator XPoint   (void) const;
      inline operator string (void) const;

      inline DgDVec2D& operator=  (const DgDVec2D& pt);
      DgDVec2D& operator=  (const DgDVec3D& pt);
      inline DgDVec2D& operator+= (const DgDVec2D& pt);
      inline DgDVec2D& operator-= (const DgDVec2D& pt);

      inline DgDVec2D& operator*= (long double scaleFactor);
      inline DgDVec2D& operator/= (long double scaleFactor);

      friend DgDVec2D operator*  (const DgDVec2D& pt, long double scaleFactor);
      friend DgDVec2D operator*  (long double scaleFactor, const DgDVec2D& pt);
      friend DgDVec2D operator+  (const DgDVec2D& pt1, const DgDVec2D& pt2);
      friend DgDVec2D operator-  (const DgDVec2D& pt1, const DgDVec2D& pt2);

      friend bool operator== (const DgDVec2D& pt1, const DgDVec2D& pt2);
      friend bool operator!= (const DgDVec2D& pt1, const DgDVec2D& pt2);
      friend bool operator<  (const DgDVec2D& pt1, const DgDVec2D& pt2);
      friend bool operator<= (const DgDVec2D& pt1, const DgDVec2D& pt2);
      friend bool operator>  (const DgDVec2D& pt1, const DgDVec2D& pt2);
      friend bool operator>= (const DgDVec2D& pt1, const DgDVec2D& pt2);

      friend ostream& operator<< (ostream& stream, const DgDVec2D& pt);

   private:

      long double x_;
      long double y_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D&
DgDVec2D::scale (long double xScaleFactor, long double yScaleFactor)
//
// Scale me by xScaleFactor in the x direction and yScaleFactor in the y
// direction.
//
////////////////////////////////////////////////////////////////////////////////
{
   x_ *= xScaleFactor;
   y_ *= yScaleFactor;

   return *this;

} // void DgDVec2D::scale

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D&
DgDVec2D::rotate (long double degrees)
//
// Rotate me about the origin degrees degrees in a counter-clockwise
// direction.
//
////////////////////////////////////////////////////////////////////////////////
{
   while (degrees < M_ZERO) degrees += 360.0L;
   while (degrees >= 360.0L) degrees -= 360.0L;
   if (fabsl(degrees - M_ZERO) < M_EPSILON) return *this;

   long double rotAng = degrees * M_PI_180;
   long double cosAng = cosl(rotAng);
   long double sinAng = sinl(rotAng);
   long double x = x_;
   long double y = y_;

   x_ = x * cosAng - y * sinAng;
   y_ = x * sinAng + y * cosAng;

   return *this;

} // void DgDVec2D::rotate

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D::operator string (void) const
{
   const char* fmtStr = "%.9LF";
   return "(" + dgg::util::to_string(x_, fmtStr) + ", "
          + dgg::util::to_string(y_, fmtStr) + ")";

} // DgDVec2D::operator string

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D&
DgDVec2D::operator= (const DgDVec2D& pt)
{
   if (&pt != this)
   {
      x_ = pt.x_;
      y_ = pt.y_;
   }
   return *this;

} // DgDVec2D& DgDVec2D::operator=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D&
DgDVec2D::operator*= (long double scaleFactor)
{
   scale(scaleFactor, scaleFactor);

   return *this;

} // DgDVec2D& DgDVec2D::operator*=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D&
DgDVec2D::operator/= (long double scaleFactor)
{
   x_ /= scaleFactor;
   y_ /= scaleFactor;

   return *this;

} // DgDVec2D& DgDVec2D::operator/=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D&
DgDVec2D::operator+= (const DgDVec2D& pt)
{
   x_ += pt.x_;
   y_ += pt.y_;

   return *this;

} // DgDVec2D& DgDVec2D::operator+=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D&
DgDVec2D::operator-= (const DgDVec2D& pt)
{
   x_ -= pt.x_;
   y_ -= pt.y_;

   return *this;

} // DgDVec2D& DgDVec2D::operator-=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D
operator* (const DgDVec2D& pt, long double scaleFactor)
{
   DgDVec2D result(pt);
   result.scale(scaleFactor, scaleFactor);

   return result;

} // DgDVec2D operator*

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D
operator* (long double scaleFactor, const DgDVec2D& pt)
{
   DgDVec2D result(pt);
   result.scale(scaleFactor, scaleFactor);

   return result;

} // DgDVec2D operator*

////////////////////////////////////////////////////////////////////////////////
inline bool
operator== (const DgDVec2D& pt1, const DgDVec2D& pt2)
{
   return (pt1.x_ == pt2.x_ && pt1.y_ == pt2.y_);

} // bool operator==

////////////////////////////////////////////////////////////////////////////////
inline bool
operator!= (const DgDVec2D& pt1, const DgDVec2D& pt2)
{
   return !operator==(pt1, pt2);

} // bool operator!=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator> (const DgDVec2D& pt1, const DgDVec2D& pt2)
{
   return (pt1.x_ > pt2.x_ && pt1.y_ > pt2.y_);

} // bool operator>

////////////////////////////////////////////////////////////////////////////////
inline bool
operator>= (const DgDVec2D& pt1, const DgDVec2D& pt2)
{
   return (pt1.x_ >= pt2.x_ && pt1.y_ >= pt2.y_);

} // bool operator>=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator< (const DgDVec2D& pt1, const DgDVec2D& pt2)
{
   return (pt1.x_ < pt2.x_ && pt1.y_ < pt2.y_);

} // bool operator<

////////////////////////////////////////////////////////////////////////////////
inline bool
operator<= (const DgDVec2D& pt1, const DgDVec2D& pt2)
{
   return (pt1.x_ <= pt2.x_ && pt1.y_ <= pt2.y_);

} // bool operator<=

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D
operator+ (const DgDVec2D& pt1, const DgDVec2D& pt2)
{
   DgDVec2D result(pt1);
   result += pt2;

   return result;

} // DgDVec2D operator+

////////////////////////////////////////////////////////////////////////////////
inline DgDVec2D
operator- (const DgDVec2D& pt1, const DgDVec2D& pt2)
{
   DgDVec2D result(pt1);
   result -= pt2;

   return result;

} // DgDVec2D operator-

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgDVec2D& pt)
{
   return stream << string(pt);

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
