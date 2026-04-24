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
// DgDiscTopoRF.h: DgDiscTopoRF class definitions
//
// A discrete location system with a floating point backFrame, cell
// geometry, and topology.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCTOPORF_H
#define DGDISCTOPORF_H

#include "DgDiscRF.h"
#include "DgGridTopo.h"
#include "DgPolygon.h"

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB>
class DgDiscTopoRF : public DgDiscRF<A, B, DB> {

   public:

      DgDiscTopoRF& operator= (const DgDiscTopoRF<A, B, DB>& rf)
          {
             if (&rf != this)
             {
                DgDiscRF<A, B, DB>::operator=(rf);
                e_ = rf.e();
                r_ = rf.r();
                c_ = rf.c();
                area_ = rf.area();
                gridTopo_ = rf.gridTopo();
                gridMetric_ = rf.gridMetric();
             }
             return *this;
          }

      // get methods

      long double e    (void) const { return e_; }
      long double r    (void) const { return r_; }
      long double c    (void) const { return c_; }
      long double area (void) const { return area_; }

      DgGridTopology gridTopo    (void) const { return gridTopo_; }
      DgGridMetric   gridMetric  (void) const { return gridMetric_; }

      // set methods

      void setE    (long double eIn) { e_ = eIn; }
      void setR    (long double rIn) { r_ = rIn; }
      void setC    (long double cIn) { c_ = cIn; }
      void setArea (long double areaIn) { area_ = areaIn; }

      void setGridTopology (DgGridTopology t) { gridTopo_ = t; }
      void setGridMetric   (DgGridMetric m)   { gridMetric_ = m; }

      // misc methods
      virtual void setPoint (const DgLocation& loc, DgLocation& point) const;

      virtual void setPoint (const DgLocation& loc, const DgRFBase& rf,
                                                   DgLocation& point) const;
      virtual void setPoint (const A& add, const DgRFBase& rf,
                             DgLocation& point) const
               { setAddPoint(add, point); rf.convert(&point); }

      virtual void setPoint (const A& add, DgLocation& pt) const
                    { pt.clearAddress(); this->backFrame().convert(&pt);
                      setAddPoint(add, pt); }

      virtual void setVertices (const DgLocation& loc, DgPolygon& vec) const;

      virtual void setVertices (const DgLocation& loc, const DgRFBase& rf,
                                                       DgPolygon& vec) const;

      virtual void setVertices (const A& add, const DgRFBase& rf,
                                                        DgPolygon& vec) const
               { setAddVertices(add, vec); rf.convert(vec); }

      virtual void setVertices  (const A& add, DgPolygon& vec) const
               { vec.clearAddress(); this->backFrame().convert(vec);
                 setAddVertices(add, vec); }

      virtual void setNeighbors (const DgLocation& loc, DgLocVector& vec) const;

      virtual void setNeighbors (const A& add, const DgRFBase& rf,
                                                        DgLocVector& vec) const
               { setAddNeighbors(add, vec); rf.convert(vec); }

      virtual void setNeighbors (const A& add, DgLocVector& vec) const
               { vec.clearAddress(); this->convert(vec); setAddNeighbors(add, vec); }

      virtual DgLocVector* makeNeighbors (const DgLocation& loc) const
               { DgLocVector* vec = new DgLocVector(*this);
                 setNeighbors(loc, *vec);  return vec; }

      virtual DgPolygon* makeVertices (const DgLocation& loc) const
               { DgPolygon* vec = new DgPolygon(this->backFrame());
                 setVertices(loc, *vec);  return vec; }

      virtual DgLocation* makePoint (const DgLocation& loc) const
               { DgLocation* pt = new DgLocation(this->backFrame());
                 setPoint(loc, *pt);  return pt; }

      virtual DgLocVector* makeNeighbors (const A& add) const
               { DgLocVector* vec = new DgLocVector(*this);
                 setNeighbors(add, *vec);  return vec; }

      virtual DgPolygon* makeVertices (const A& add) const
               { DgPolygon* vec = new DgPolygon(this->backFrame());
                 setVertices(add, *vec);  return vec; }

      virtual DgLocation* makePoint (const A& add) const
               { DgLocation* pt = new DgLocation(this->backFrame());
                 setPoint(add, *pt);  return pt; }

      // second order boundary neighbors (for aperture 7 hex grids only)
      virtual void setNeighborsBdry2 (const DgLocation& loc, DgLocVector& vec) const;

      virtual void setNeighborsBdry2 (const A& add, const DgRFBase& rf,
                                                        DgLocVector& vec) const
               { setAddNeighborsBdry2(add, vec); rf.convert(vec); }

      virtual void setNeighborsBdry2 (const A& add, DgLocVector& vec) const
               { vec.clearAddress(); this->convert(vec); setAddNeighborsBdry2(add, vec); }

      virtual DgLocVector* makeNeighborsBdry2 (const DgLocation& loc) const
               { DgLocVector* vec = new DgLocVector(*this);
                 setNeighborsBdry2(loc, *vec);  return vec; }

      virtual void setAddNeighborsBdry2 (const A&, DgLocVector&) const { }

      virtual operator std::string (void) const {
         std::string s = "*** DgDiscRF " + DgRFBase::name();
         return s;
      }

      // remind users of the pure virtual functions remaining from above

      virtual std::string add2str (const A& add) const = 0;
      virtual std::string add2str (const A& add, char delimiter) const = 0;

      virtual long long int dist (const A& add1, const A& add2) const = 0;

      virtual const char* str2add (A* add, const char* str, char delimiter)
                                                                      const = 0;

      virtual const A& undefAddress (void) const = 0;

      // new pure virtual functions

      virtual A quantify    (const B& point) const = 0;
      virtual B invQuantify (const A& add)   const = 0;

      virtual void setAddNeighbors (const A& add, DgLocVector& vec) const = 0;
      virtual void setAddVertices  (const A& add, DgPolygon& vec) const = 0;

   protected:

      DgDiscTopoRF (DgRFNetwork& networkIn, const DgRF<B, DB>& backFrameIn,
                const std::string& nameIn = "DiscTopo",
                DgGridTopology gridTopoIn = Hexagon,
                DgGridMetric gridMetricIn = D6,
                long double eIn = 1.0L,
                long double rIn = 1.0L, long double cIn = 1.0L, long double areaIn = 1.0L)
        : DgDiscRF<A, B, DB> (networkIn, backFrameIn, nameIn),
          e_ (eIn), r_ (rIn), c_ (cIn), area_ (areaIn), gridTopo_ (gridTopoIn),
          gridMetric_ (gridMetricIn)
        { }

      DgDiscTopoRF (const DgDiscTopoRF<A, B, DB>& rf) : DgDiscRF<A, B, DB> (rf),
          e_ (rf.e()), r_ (rf.r()), c_ (rf.c()), area_ (rf.area()), gridTopo_ (rf.gridTopo()),
          gridMetric_ (rf.gridMetric())
        { }

      virtual void setAddPoint (const A& add, DgLocation& pt) const;

      long double e_;   // edge length
      long double r_;   // radius (max center-to-vertex)
      long double c_;   // irrational constant
      long double area_; // cell area

      DgGridTopology gridTopo_;
      DgGridMetric gridMetric_;

};

////////////////////////////////////////////////////////////////////////////////

#include "DgDiscTopoRF_hpp.h"

#endif
