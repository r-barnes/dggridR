////////////////////////////////////////////////////////////////////////////////
//
// DgIVec2D.h: DgIVec2D class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIVEC2D_H
#define DGIVEC2D_H

#include <cmath>
#include <string>
#include <cstdint>

#include "DgUtil.h"
#include "DgConstants.h"
#include "DgDVec2D.h"

////////////////////////////////////////////////////////////////////////////////
class DgIVec2D {

   public:

      static DgIVec2D midPoint (const DgIVec2D& pt1, const DgIVec2D& pt2)
                       { return pt1 * 0.5L + pt2 * 0.5L; }

      static DgIVec2D fracPoint (const DgIVec2D& pt1, const DgIVec2D& pt2, 
                                long double fraction)
                       { return pt2 * fraction + pt1 * (1 - fraction); }

      static const DgIVec2D& undefDgIVec2D;

      DgIVec2D (std::int64_t i = 0, std::int64_t j = 0) 
       : i_(i), j_(j)
      {}

      DgIVec2D (const DgIVec2D& pt) 
       : i_ (pt.i_), j_ (pt.j_) 
      {}

      DgIVec2D (const DgDVec2D&  pt) 
       : i_ (dgg::util::lrint(pt.x())),  
         j_ (dgg::util::lrint(pt.y())) 
      {}

      void setI (std::int64_t i) { i_ = i; }
      void setJ (std::int64_t j) { j_ = j; }

      long double distance (const DgIVec2D& pt) const
              { return (pt - *this).magnitude(); }

      std::int64_t i (void) const { return i_; }
      std::int64_t j (void) const { return j_; }
      
      long double magnitude (void) const 
              { return sqrt((long double) (i_ * i_ + j_ * j_)); }

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

      std::int64_t i_;
      std::int64_t j_;

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
   i_ = (std::int64_t) dgg::util::lrint(i_ * xScaleFactor);
   j_ = (std::int64_t) dgg::util::lrint(j_ * yScaleFactor);

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
   long double rotAng = degrees * dgM_PI_180;
   long double cosAng = cos(rotAng);
   long double sinAng = sin(rotAng);
   std::int64_t i = i_;
   std::int64_t j = j_;

   i_ = (std::int64_t) dgg::util::lrint(i * cosAng - j * sinAng);
   j_ = (std::int64_t) dgg::util::lrint(i * sinAng + j * cosAng);

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
