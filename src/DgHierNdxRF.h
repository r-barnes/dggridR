#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

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
// DgHierNdxRF.h: DgHierNdxRF header file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHIERNDXRF_H
#define DGHIERNDXRF_H

#include <climits>
#include <ostream>

#include "DgDiscRF.h"
#include "DgIDGGS.h"
#include "DgIDGG.h"
#include "DgHierNdxSystemRFBase.h"

////////////////////////////////////////////////////////////////////////////////
template <class T> class DgHierNdxCoord  {

   public:

      //static const DgHierNdxCoord<T> undefDgHierNdxCoord<T>;

      DgHierNdxCoord (void) : value_ (0) { }

      DgHierNdxCoord (T valIn) : value_ (valIn) { }

      DgHierNdxCoord (const DgHierNdxCoord<T>& coord)
              { value_ = coord.value(); }

      void setValue (T value) { value_ = value; }

      T value (void) const { return value_; }

      operator std::string (void) const { return valString(); }

      bool operator== (const DgHierNdxCoord<T>& c) const
          { return value() == c.value(); }

      bool operator!= (const DgHierNdxCoord<T>& c) const
          { return !(*this == c); }

      DgHierNdxCoord<T>& operator= (const DgHierNdxCoord<T>& add)
          {
             if (add != *this) setValue(add.value());

             return *this;
          }

      // sub-classes must define
      virtual std::string valString (void) const = 0;

   private:

      T value_;
};

////////////////////////////////////////////////////////////////////////////////
template <class T> inline std::ostream&
operator<< (std::ostream& stream, const DgHierNdxCoord<T>& coord)
{ return stream << std::string(coord); }

////////////////////////////////////////////////////////////////////////////////
// class C is a DgHierNdxCoord<T> for some T
template <class C> class DgHierNdxRF :
                       public DgDiscRF<C, DgQ2DICoord, long long int>
{
   public:

      // this should be initialized in the instantiated class
      //static const C undefCoord;

      // sub-classes should create a factory method
      const DgHierNdxSystemRFBase& system (void) const { return sys_; }

      const DgIDGGSBase& dggs (void) const { return sys_.dggs(); }
      const DgIDGGBase& dgg  (void) const { return *sys_.dgg(); }

      int res      (void) const { return sys_.res(); }
      int aperture (void) const { return sys_.aperture(); }
      int effRes   (void) const { return effRes_; }
      int effRadix (void) const { return effRadix_; }

      // indexes don't typically use delimiters
      virtual std::string add2str (const C& add, char delimiter) const
                       { return add2str(add); }

      //virtual const C& undefAddress (void) const { return undefCoord; }
      virtual const C& undefAddress (void) const = 0;

      // these need to be defined by specializations
      // given dummy definitions for now so the class isn't abstract
      virtual C quantify (const DgQ2DICoord& point) const
                { return undefAddress(); }
      virtual DgQ2DICoord invQuantify (const C& add) const
                { return DgQ2DICoord::undefDgQ2DICoord; }
      virtual std::string add2str (const C& add) const { return dgg::util::to_string(add); }
      virtual const char* str2add (C* c, const char* str, char delimiter) const
                      { return str; }

      // these should use the associated dgg
      virtual long long int dist (const C& add1, const C& add2) const
                       { return 0; }
      virtual std::string dist2str (const long long int& dist) const
                       { return dgg::util::to_string(dist); }
      virtual long double dist2dbl (const long long int& dist) const
                       { return dist; }
      virtual unsigned long long int dist2int (const long long int& dist) const
                       { return dist; }
      virtual void setAddNeighbors (const C& add, DgLocVector& vec) const { }
      virtual void setAddVertices  (const C& add, DgPolygon& vec) const { }

   protected:

      DgHierNdxRF (const DgHierNdxSystemRFBase& sysIn, int resIn, const std::string& nameIn)
         : DgDiscRF<C, DgQ2DICoord, long long int>(sysIn.dggs().network(),
                       sysIn.dggs().idggBase(resIn), nameIn),
           sys_ (sysIn) { }
/*
      void initSystem (const DgIDGGS& dggsIn, int resIn, const std::string& nameIn)
         {
           sys_ = new DgHierNdxSystemRF(dggsIn, resIn, nameIn + "Sys");
         }
*/
      const DgHierNdxSystemRFBase& sys_;
      int effRes_;
      int effRadix_;
//      bool ownSysMemory;
};

// the actual value should be defined by the specializations
//template<typename C> const C DgHierNdxRF<C>::undefCoord;

////////////////////////////////////////////////////////////////////////////////
#endif
