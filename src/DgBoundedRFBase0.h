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
// DgBoundedRFBase0.h: DgBoundedRFBase0 class definitions
//
// Kevin Sahr, 7/20/20
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDRFBASE0_H
#define DGBOUNDEDRFBASE0_H

#include "DgDiscRF.h"

class DgLocation;
class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgBoundedRFBase0 {

   public:
   
      virtual ~DgBoundedRFBase0 (void);
      
      const DgRFBase& rf (void) const { return rf_; }
      
      operator const DgRFBase& (void) const { return rf(); }
      
      virtual bool validLocation (const DgLocation& loc, 
                                  bool convert = true) const = 0;

      virtual DgLocation& incrementLocation (DgLocation& loc, 
                                             bool convert = true) const = 0;
                       
      virtual DgLocation& decrementLocation (DgLocation& loc, 
                                             bool convert = true) const = 0; 
                                     
      const DgLocation& first (void) const { return *first_; }
      const DgLocation& last  (void) const { return *last_; }
      const DgLocation& end   (void) const { return *end_; }

      unsigned long long int size (void) const { return size_; }
      bool validSize (void) const { return validSize_; }

      bool zeroBased (void) const { return zeroBased_; }
      void setZeroBased (bool zBasedIn) { zeroBased_ = zBasedIn; }

      virtual unsigned long long int seqNum (const DgLocation& loc,
                                        bool convert = true) const = 0; 

      virtual bool lessThan (const DgLocation& loc1, 
                     const DgLocation& loc2, bool convert = true) const
                 { return (seqNum(loc1, convert) < seqNum(loc2, convert)); }

      virtual operator string (void) const
      {
         string s = "=== DgBoundedRFBase0: size: " + dgg::util::to_string(size());
         s += " zeroBased: " + dgg::util::to_string(zeroBased());
         s += "\n   -- first: " + first().asString();
         s += "\n   -- last: " + last().asString();

         return s;
      } 

      virtual DgLocation* locFromSeqNum (unsigned long long int sNum) const = 0;
      
      // provide a generic interface to the discrete grid functionality

      virtual string dist2str (const long long int& dist) const = 0;
      virtual long double dist2dbl (const long long int& dist) const = 0;

      virtual unsigned long long int dist2int (const long long int& dist) const = 0;

      virtual void setPoint (const DgLocation& loc, DgLocation& point) 
                                                                     const = 0;

      virtual void setPoint (const DgLocation& loc, const DgRFBase& rf,
                                                  DgLocation& point) const = 0;

      virtual DgLocation* makePoint (const DgLocation& loc) const = 0;

      virtual void setVertices (const DgLocation& loc, DgPolygon& vec) 
                                                                     const = 0;

      virtual void setVertices (const DgLocation& loc, const DgRFBase& rf,
                                                   DgPolygon& vec) const = 0;

      virtual DgPolygon* makeVertices (const DgLocation& loc) const = 0;

      virtual void setNeighbors (const DgLocation& loc, DgLocVector& vec) 
                                                                     const = 0;

      virtual DgLocVector* makeNeighbors (const DgLocation& loc) const = 0;

   protected:

      DgBoundedRFBase0 (const DgRFBase& rfIn, DgLocation* firstIn, 
                       DgLocation* lastIn, DgLocation* endIn,
                       bool zBasedIn = true)
         : size_(0), validSize_(false), rf_ (rfIn), 
	   first_ (firstIn), last_ (lastIn), end_ (endIn),
	   zeroBased_ (zBasedIn) {}
         
   protected:

      unsigned long long int size_;
      bool validSize_;
   
      const DgRFBase& rf_;
      DgLocation *first_;
      DgLocation *last_;
      DgLocation *end_;

      bool zeroBased_; // seq starts with 0 (or 1)?

}; 

////////////////////////////////////////////////////////////////////////////////
template<class B, class DB> inline
ostream& operator<< (ostream& stream, const DgBoundedRFBase0& rfIn)
{ return stream << "Bnd-" << rfIn.rf(); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
