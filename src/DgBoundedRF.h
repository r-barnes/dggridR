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
// DgBoundedRF.h: DgBoundedRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBOUNDEDRF_H
#define DGBOUNDEDRF_H

#include "DgBoundedRFBase.h"
#include "DgDiscRF.h"
#include "DgPolygon.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> class DgBoundedRF
                                         : public DgBoundedRFBase<B, DB> {

   public:

      DgBoundedRF<A, B, DB> (const DgDiscRF<A, B, DB>& rfIn,
                             const A& firstAddIn,
                             const A& lastAddIn, const A& endAddIn,
                             bool zBasedIn = true);

      const DgDiscRF<A, B, DB>& discRF (void) const { return discRF_; }

      virtual bool validLocation (const DgLocation& loc,
                                  bool convert = true) const;

      virtual DgLocation& incrementLocation (DgLocation& loc,
                                             bool convert = true) const;

      virtual DgLocation& decrementLocation (DgLocation& loc,
                                             bool convert = true) const;

      virtual unsigned long long int seqNum (const DgLocation& loc,
                                        bool convert = true) const;

      virtual DgLocation* locFromSeqNum (unsigned long long int sNum) const;

      const A& firstAdd (void) const { return firstAdd_; }
      const A& lastAdd  (void) const { return lastAdd_; }
      const A& endAdd   (void) const { return endAdd_; }

      // provide a generic interface to the discrete grid functionality

      virtual const DgRF<B, DB>& backFrame (void) const
                     { return discRF().backFrame(); }

      virtual string dist2str (const long long int& dist) const
                     { return discRF().dist2str(dist); }

      virtual long double dist2dbl (const long long int& dist) const
                     { return discRF().dist2dbl(dist); }

      virtual unsigned long long int dist2int (const long long int& dist) const
                     { return discRF().dist2int(dist); }

      virtual void setPoint (const DgLocation& loc, DgLocation& point) const
                   { discRF().setPoint(loc, point); }

      virtual void setPoint (const DgLocation& loc, const DgRFBase& rf,
                                                       DgLocation& point) const
                   { discRF().setPoint(loc, rf, point); }

      virtual void setPoint (const A& add, const DgRFBase& rf,
                                                       DgLocation& point) const
                   { discRF().setPoint(add, rf, point); }

      virtual void setPoint (const A& add, DgLocation& pt) const
                   { discRF().setPoint(add, pt); }

      virtual void setVertices (const DgLocation& loc, DgPolygon& vec) const
                   { discRF().setVertices(loc, vec); }

      virtual void setVertices (const DgLocation& loc, const DgRFBase& rf,
                                                        DgPolygon& vec) const
                   { discRF().setVertices(loc, rf, vec); }

      virtual void setVertices (const A& add, const DgRFBase& rf,
                                                        DgPolygon& vec) const
                   { discRF().setVertices(add, rf, vec); }

      virtual void setNeighbors (const DgLocation& loc, DgLocVector& vec) const
                   { discRF().setNeighbors(loc, vec); }

      virtual void setNeighbors (const A& add, const DgRFBase& rf,
                                                        DgLocVector& vec) const
                   { discRF().setNeighbors(add, rf, vec); }

      virtual void setNeighbors (const A& add, DgLocVector& vec) const
                   { discRF().setNeighbors(add, vec); }

      virtual void setVertices  (const A& add, DgPolygon& vec) const
                   { discRF().setVertices(add, vec); }

      virtual DgLocation* makePoint (const DgLocation& loc) const
                   { return discRF().makePoint(loc); }

      virtual DgPolygon* makeVertices (const DgLocation& loc) const
                   { return discRF().makeVertices(loc); }

      virtual DgLocVector* makeNeighbors (const DgLocation& loc) const
                   { return discRF().makeNeighbors(loc); }

      // pure virtual functions that must be defined by sub-classes

      virtual bool validAddress (const A& add) const = 0;

      virtual A& incrementAddress (A& add) const = 0;
      virtual A& decrementAddress (A& add) const = 0;

      virtual unsigned long long int seqNumAddress (const A& loc) const = 0;

      virtual A addFromSeqNum (unsigned long long int sNum) const = 0;

      virtual bool lessThanAddress (const A& a1, const A& a2) const
                      { return (seqNumAddress(a1) < seqNumAddress(a2)); }

   protected:

      void setFirstAdd (const A& firstAddIn) 
         { firstAdd_ = firstAddIn;
           discRF().forceAddress(DgBoundedRFBase0::first_, firstAddIn); }

      void setLastAdd  (const A& lastAddIn)  { lastAdd_ = lastAddIn; }
      void setEndAdd   (const A& endAddIn)   { endAdd_ = endAddIn; }

   private:

      const DgDiscRF<A, B, DB>& discRF_;

      A firstAdd_;
      A lastAdd_;
      A endAdd_;

};

#include "DgBoundedRF_hpp.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
