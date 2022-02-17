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
// DgIVec2D.h: DgIVec2D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIVEC2D_H
#define DGIVEC2D_H

#include "DgConstants.h"
#include "DgDVec2D.h"
#include "DgUtil.h"

#include <cmath>
#include <string>

////////////////////////////////////////////////////////////////////////////////
class DgIVec2D {

   public:

      static DgIVec2D midPoint (const DgIVec2D& pt1, const DgIVec2D& pt2)
                       { return pt1 * 0.5L + pt2 * 0.5L; }

      static DgIVec2D fracPoint (const DgIVec2D& pt1, const DgIVec2D& pt2,
                                long double fraction)
                       { return pt2 * fraction + pt1 * (1 - fraction); }

      static const DgIVec2D& undefDgIVec2D;

      DgIVec2D (long long int i = 0, long long int j = 0)
       : i_(i), j_(j)
      {}

      DgIVec2D (const DgIVec2D& pt)
       : i_ (pt.i_), j_ (pt.j_)
      {}

      DgIVec2D (const DgDVec2D&  pt)
       : i_ (dgg::util::lrint(pt.x())),
         j_ (dgg::util::lrint(pt.y()))
      {}

      void setI (long long int i) { i_ = i; }
      void setJ (long long int j) { j_ = j; }

      long double distance (const DgIVec2D& pt) const
              { return (pt - *this).magnitude(); }

      long long int i (void) const { return i_; }
      long long int j (void) const { return j_; }

      long double magnitude (void) const
              { return sqrtl((long double) (i_ * i_ + j_ * j_)); }

      DgIVec2D diffVec (const DgIVec2D& pt0) const
                          { return DgIVec2D(*this - pt0); }

      DgIVec2D absDiffVec (const DgIVec2D& pt0) const
                          { return DgIVec2D(abs(i_ - pt0.i()),
                                            abs(j_ - pt0.j())); }

      const char* fromString (const char* str, char delimiter);

      inline DgIVec2D& scale (long double xScaleFactor, long double yScaleFactor);
      inline DgIVec2D& rotate (long double degrees);

      //inline operator XPoint   (void) const;
      inline operator string (void) const;
      inline operator DgDVec2D  (void) const;

      inline DgIVec2D& operator=  (const DgIVec2D& pt);
      inline DgIVec2D& operator+= (const DgIVec2D& pt);
      inline DgIVec2D& operator-= (const DgIVec2D& pt);

      inline DgIVec2D& operator*= (long double scaleFactor);

      friend DgIVec2D operator*  (const DgIVec2D& pt, long double scaleFactor);
      friend DgIVec2D operator*  (long double scaleFactor, const DgIVec2D& pt);
      friend DgIVec2D operator%  (const DgIVec2D& pt, int modulo);
      friend DgIVec2D operator+  (const DgIVec2D& pt1, const DgIVec2D& pt2);
      friend DgIVec2D operator-  (const DgIVec2D& pt1, const DgIVec2D& pt2);

      friend bool operator== (const DgIVec2D& pt1, const DgIVec2D& pt2);
      friend bool operator!= (const DgIVec2D& pt1, const DgIVec2D& pt2);
      friend bool operator<  (const DgIVec2D& pt1, const DgIVec2D& pt2);
      friend bool operator<= (const DgIVec2D& pt1, const DgIVec2D& pt2);
      friend bool operator>  (const DgIVec2D& pt1, const DgIVec2D& pt2);
      friend bool operator>= (const DgIVec2D& pt1, const DgIVec2D& pt2);

      friend ostream& operator<< (ostream& stream, const DgIVec2D& pt);

   private:

      long long int i_;
      long long int j_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D&
DgIVec2D::scale (long double xScaleFactor, long double yScaleFactor)
//
// Scale me by xScaleFactor in the x direction and yScaleFactor in the y
// direction.
//
////////////////////////////////////////////////////////////////////////////////
{
   i_ = (long long int) dgg::util::lrint(i_ * xScaleFactor);
   j_ = (long long int) dgg::util::lrint(j_ * yScaleFactor);

   return *this;

} // void DgIVec2D::scale

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D&
DgIVec2D::rotate (long double degrees)
//
// Rotate me about the origin degrees degrees in a counter-clockwise
// direction.
//
////////////////////////////////////////////////////////////////////////////////
{
   long double rotAng = degrees * M_PI_180;
   long double cosAng = cosl(rotAng);
   long double sinAng = sinl(rotAng);
   long long int i = i_;
   long long int j = j_;

   i_ = (long long int) dgg::util::lrint(i * cosAng - j * sinAng);
   j_ = (long long int) dgg::util::lrint(i * sinAng + j * cosAng);

   return *this;

} // void DgIVec2D::rotate

/*
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D::operator XPoint (void) const
//
// XPoint conversion function. Note that the values may be truncated.
//
////////////////////////////////////////////////////////////////////////////////
{
   XPoint Xpt;

   Xpt.x = (short) i_;
   Xpt.y = (short) j_;

   return Xpt;

} // DgIVec2D::operator XPoint
*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D::operator DgDVec2D (void) const
//
// DgDVec2D conversion function.
//
////////////////////////////////////////////////////////////////////////////////
{
   return DgDVec2D((long double) i_, (long double) j_);

} // DgIVec2D::operator DgDVec2D

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D::operator string (void) const
{
   return "(" + dgg::util::to_string(i_) + ", " + dgg::util::to_string(j_) + ")";

} // DgIVec2D::operator string

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D&
DgIVec2D::operator= (const DgIVec2D& pt)
{
   if (&pt != this)
   {
      i_ = pt.i_;
      j_ = pt.j_;
   }
   return *this;

} // DgIVec2D& DgIVec2D::operator=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D&
DgIVec2D::operator*= (long double scaleFactor)
{
   scale(scaleFactor, scaleFactor);

   return *this;

} // DgIVec2D& DgIVec2D::operator*=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D&
DgIVec2D::operator+= (const DgIVec2D& pt)
{
   i_ += pt.i_;
   j_ += pt.j_;

   return *this;

} // DgIVec2D& DgIVec2D::operator+=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D&
DgIVec2D::operator-= (const DgIVec2D& pt)
{
   i_ -= pt.i_;
   j_ -= pt.j_;

   return *this;

} // DgIVec2D& DgIVec2D::operator-=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D
operator* (const DgIVec2D& pt, long double scaleFactor)
{
   DgIVec2D result(pt);
   result.scale(scaleFactor, scaleFactor);

   return result;

} // bool operator*

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D
operator* (long double scaleFactor, const DgIVec2D& pt)
{
   DgIVec2D result(pt);
   result.scale(scaleFactor, scaleFactor);

   return result;

} // bool operator*

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D
operator% (const DgIVec2D& pt, int modulo)
{
   return DgIVec2D(pt.i() % modulo, pt.j() % modulo);

} // bool operator%

////////////////////////////////////////////////////////////////////////////////
inline bool
operator== (const DgIVec2D& pt1, const DgIVec2D& pt2)
{
   return (pt1.i_ == pt2.i_ && pt1.j_ == pt2.j_);

} // bool operator==

////////////////////////////////////////////////////////////////////////////////
inline bool
operator!= (const DgIVec2D& pt1, const DgIVec2D& pt2)
{
   return !operator==(pt1, pt2);

} // bool operator!=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator> (const DgIVec2D& pt1, const DgIVec2D& pt2)
{
   if (pt1.i_ != pt2.i_)
      return (pt1.i_ > pt2.i_);
   else // i's are equal
      return (pt1.j_ > pt2.j_);

} // bool operator>

////////////////////////////////////////////////////////////////////////////////
inline bool
operator>= (const DgIVec2D& pt1, const DgIVec2D& pt2)
{
   return (pt1 > pt2 || pt1 == pt2);

} // bool operator>=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator< (const DgIVec2D& pt1, const DgIVec2D& pt2)
{
   if (pt1.i_ != pt2.i_)
      return (pt1.i_ < pt2.i_);
   else // i's are equal
      return (pt1.j_ < pt2.j_);

} // bool operator<

////////////////////////////////////////////////////////////////////////////////
inline bool
operator<= (const DgIVec2D& pt1, const DgIVec2D& pt2)
{
   return (pt1 < pt2 || pt1 == pt2);

} // bool operator<=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D
operator+ (const DgIVec2D& pt1, const DgIVec2D& pt2)
{
   DgIVec2D result(pt1);
   result += pt2;

   return result;

} // bool operator+

////////////////////////////////////////////////////////////////////////////////
inline DgIVec2D
operator- (const DgIVec2D& pt1, const DgIVec2D& pt2)
{
   DgIVec2D result(pt1);
   result -= pt2;

   return result;

} // bool operator-

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgIVec2D& pt)
{
   return stream << string(pt);

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
