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
// DgIVec3D.h: DgIVec3D class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIVEC3D_H
#define DGIVEC3D_H

#include "DgDVec2D.h"
#include "DgIVec2D.h"
#include "DgUtil.h"

#include <cmath>
#include <string>

////////////////////////////////////////////////////////////////////////////////
class DgIVec3D {

   public:

      static const DgIVec3D& undefDgIVec3D;

      DgIVec3D (long long int i = 0, long long int j = 0,
                long long int k = 0)
       : i_(i), j_(j), k_(k)
      {}

      DgIVec3D (const DgIVec3D& pt)
       : i_ (pt.i_), j_ (pt.j_), k_ (pt.k_)
      {}

      DgIVec3D (const DgDVec2D&  pt)
       : i_ ((int) dgg::util::lrint(pt.x())),
         j_ ((int) dgg::util::lrint(pt.y())),
         k_ (0)
      {}

      void setI (long long int i) { i_ = i; }
      void setJ (long long int j) { j_ = j; }
      void setK (long long int k) { k_ = k; }

      long double distance (const DgIVec3D& pt) const
              { return (pt - *this).magnitude(); }

      long long int i (void) const { return i_; }
      long long int j (void) const { return j_; }
      long long int k (void) const { return k_; }

      long double magnitude (void) const
              { return sqrtl((long double) (i_ * i_ + j_ * j_) + k_ * k_); }

      DgIVec3D diffVec (const DgIVec3D& pt0) const
                          { return DgIVec3D(*this - pt0); }

      DgIVec3D absDiffVec (const DgIVec3D& pt0) const
                          { return DgIVec3D(abs(i_ - pt0.i()),
                                            abs(j_ - pt0.j()),
                                            abs(k_ - pt0.k())); }

      const char* fromString (const char* str, char delimiter);

      inline DgIVec3D& scale (long double iScaleFactor, long double jScaleFactor,
                 long double zScaleFacgtor);

      inline operator string (void) const;
      inline operator DgIVec2D  (void) const;

      inline DgIVec3D& operator=  (const DgIVec3D& pt);
      inline DgIVec3D& operator+= (const DgIVec3D& pt);
      inline DgIVec3D& operator-= (const DgIVec3D& pt);

      inline DgIVec3D& operator*= (long double scaleFactor);

      friend DgIVec3D operator*  (const DgIVec3D& pt, long double scaleFactor);
      friend DgIVec3D operator*  (long double scaleFactor, const DgIVec3D& pt);
      friend DgIVec3D operator+  (const DgIVec3D& pt1, const DgIVec3D& pt2);
      friend DgIVec3D operator-  (const DgIVec3D& pt1, const DgIVec3D& pt2);

      friend bool operator== (const DgIVec3D& pt1, const DgIVec3D& pt2);
      friend bool operator!= (const DgIVec3D& pt1, const DgIVec3D& pt2);
      friend bool operator<  (const DgIVec3D& pt1, const DgIVec3D& pt2);
      friend bool operator<= (const DgIVec3D& pt1, const DgIVec3D& pt2);
      friend bool operator>  (const DgIVec3D& pt1, const DgIVec3D& pt2);
      friend bool operator>= (const DgIVec3D& pt1, const DgIVec3D& pt2);

      friend ostream& operator<< (ostream& stream, const DgIVec3D& pt);

   private:

      long long int i_;
      long long int j_;
      long long int k_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D&
DgIVec3D::scale (long double iScaleFactor, long double jScaleFactor,
                 long double kScaleFactor)
//
// Scale me by each factor in the corresponding ijk direction.
//
////////////////////////////////////////////////////////////////////////////////
{
   i_ = (long long int) dgg::util::lrint(i_ * iScaleFactor);
   j_ = (long long int) dgg::util::lrint(j_ * jScaleFactor);
   k_ = (long long int) dgg::util::lrint(k_ * kScaleFactor);

   return *this;

} // void DgIVec3D::scale

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D::operator DgIVec2D (void) const
//
// DgIVec2D conversion function.
//
////////////////////////////////////////////////////////////////////////////////
{
   return DgIVec2D(i_ - k_, j_ - k_);

} // DgIVec3D::operator DgIVec2D

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D::operator string (void) const
{
   return "(" + dgg::util::to_string(i_) + ", "
              + dgg::util::to_string(j_) + ", "
              + dgg::util::to_string(k_) + ")";

} // DgIVec3D::operator string

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D&
DgIVec3D::operator= (const DgIVec3D& pt)
{
   if (&pt != this)
   {
      i_ = pt.i_;
      j_ = pt.j_;
      k_ = pt.k_;
   }
   return *this;

} // DgIVec3D& DgIVec3D::operator=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D&
DgIVec3D::operator*= (long double scaleFactor)
{
   scale(scaleFactor, scaleFactor, scaleFactor);

   return *this;

} // DgIVec3D& DgIVec3D::operator*=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D&
DgIVec3D::operator+= (const DgIVec3D& pt)
{
   i_ += pt.i_;
   j_ += pt.j_;
   k_ += pt.k_;

   return *this;

} // DgIVec3D& DgIVec3D::operator+=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D&
DgIVec3D::operator-= (const DgIVec3D& pt)
{
   i_ -= pt.i_;
   j_ -= pt.j_;
   k_ -= pt.k_;

   return *this;

} // DgIVec3D& DgIVec3D::operator-=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D
operator* (const DgIVec3D& pt, long double scaleFactor)
{
   DgIVec3D result(pt);
   result.scale(scaleFactor, scaleFactor, scaleFactor);

   return result;

} // bool operator*

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D
operator* (long double scaleFactor, const DgIVec3D& pt)
{
   DgIVec3D result(pt);
   result.scale(scaleFactor, scaleFactor, scaleFactor);

   return result;

} // bool operator*

////////////////////////////////////////////////////////////////////////////////
inline bool
operator== (const DgIVec3D& pt1, const DgIVec3D& pt2)
{
   return (pt1.i_ == pt2.i_ && pt1.j_ == pt2.j_ && pt1.k_ == pt2.k_);

} // bool operator==

////////////////////////////////////////////////////////////////////////////////
inline bool
operator!= (const DgIVec3D& pt1, const DgIVec3D& pt2)
{
   return !operator==(pt1, pt2);

} // bool operator!=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator> (const DgIVec3D& pt1, const DgIVec3D& pt2)
{
   return (pt1.i_ > pt2.i_ && pt1.j_ > pt2.j_ && pt1.k_ > pt2.k_);

} // bool operator>

////////////////////////////////////////////////////////////////////////////////
inline bool
operator>= (const DgIVec3D& pt1, const DgIVec3D& pt2)
{
   return (pt1.i_ >= pt2.i_ && pt1.j_ >= pt2.j_ && pt1.k_ >= pt2.k_);

} // bool operator>=

////////////////////////////////////////////////////////////////////////////////
inline bool
operator< (const DgIVec3D& pt1, const DgIVec3D& pt2)
{
   return (pt1.i_ < pt2.i_ && pt1.j_ < pt2.j_ && pt1.k_ < pt2.k_);

} // bool operator<

////////////////////////////////////////////////////////////////////////////////
inline bool
operator<= (const DgIVec3D& pt1, const DgIVec3D& pt2)
{
   return (pt1.i_ <= pt2.i_ && pt1.j_ <= pt2.j_ && pt1.k_ <= pt2.k_);

} // bool operator<=

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D
operator+ (const DgIVec3D& pt1, const DgIVec3D& pt2)
{
   DgIVec3D result(pt1);
   result += pt2;

   return result;

} // bool operator+

////////////////////////////////////////////////////////////////////////////////
inline DgIVec3D
operator- (const DgIVec3D& pt1, const DgIVec3D& pt2)
{
   DgIVec3D result(pt1);
   result -= pt2;

   return result;

} // bool operator-

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgIVec3D& pt)
{
   return stream << string(pt);

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
