////////////////////////////////////////////////////////////////////////////////
//
// DgIVec3D.h: DgIVec3D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIVEC3D_H
#define DGIVEC3D_H

#include <cmath>
#include <string>
#include <stdint.h>

#include "DgUtil.h"
#include "DgIVec2D.h"
#include "DgDVec2D.h"

////////////////////////////////////////////////////////////////////////////////
class DgIVec3D {

   public:

      static const DgIVec3D& undefDgIVec3D;

      DgIVec3D (int64_t i = 0, int64_t j = 0,
                int64_t k = 0) 
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

      void setI (int64_t i) { i_ = i; }
      void setJ (int64_t j) { j_ = j; }
      void setK (int64_t k) { k_ = k; }

      long double distance (const DgIVec3D& pt) const
              { return (pt - *this).magnitude(); }

      int64_t i (void) const { return i_; }
      int64_t j (void) const { return j_; }
      int64_t k (void) const { return k_; }
      
      long double magnitude (void) const 
              { return sqrt((long double) (i_ * i_ + j_ * j_) + k_ * k_); }

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

      int64_t i_;
      int64_t j_;
      int64_t k_;

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
   i_ = (int64_t) dgg::util::lrint(i_ * iScaleFactor);
   j_ = (int64_t) dgg::util::lrint(j_ * jScaleFactor);
   k_ = (int64_t) dgg::util::lrint(k_ * kScaleFactor);

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
