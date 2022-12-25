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
// DgDiscRF.h: DgDiscRF class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRF_H
#define DGDISCRF_H

#include "DgGridTopo.h"
#include "DgPolygon.h"
#include "DgRF.h"

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB>
class DgDiscRF : public DgRF<A, long long int> {

   public:

      class DgQuantConverter : public DgConverter<B, DB, A, long long int> {

         public:

            DgQuantConverter (const DgRF<B, DB>& fromFrame,
                              const DgDiscRF<A, B, DB>& toFrame)
               : DgConverter<B, DB, A, long long int>
                       (static_cast< const DgRF<B, DB>& >(fromFrame),
                        static_cast< const DgRF<A, long long int>& >(toFrame)) { }

            virtual A convertTypedAddress (const B& addIn) const
              { return
                   static_cast<const DgDiscRF<A, B, DB>&>(
                                  this->toFrame()).quantify(addIn);
	      }

      };

      class DgInvQuantConverter : public DgConverter<A, long long int, B, DB> {

         public:

            DgInvQuantConverter (const DgDiscRF<A, B, DB>& fromFrame,
                                 const DgRF<B, DB>& toFrame)
               : DgConverter<A, long long int, B, DB> (fromFrame, toFrame) { }

            virtual B convertTypedAddress (const A& addIn) const
             { return
                  static_cast<const DgDiscRF<A, B, DB>&>(
                                         this->fromFrame()).invQuantify(addIn);
	     }

      };

      DgDiscRF& operator= (const DgDiscRF<A, B, DB>& rf)
          {
             if (&rf != this)
             {
                DgRF<A, long long int>::operator=(rf);
                e_ = rf.e();
                r_ = rf.r();
                c_ = rf.c();
                area_ = rf.area();
                backFrame_ = &rf.backFrame();
                gridTopo_ = rf.gridTopo();
                gridMetric_ = rf.gridMetric();
             }
             return *this;
          }

      // get methods

      const DgRF<B, DB>& backFrame (void) const { return *backFrame_; }

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

      virtual string dist2str (const long long int& dist) const { return to_string(dist); }
      virtual long double dist2dbl (const long long int& dist) const { return (long double) dist; }
      virtual unsigned long long int dist2int (const long long int& dist) const
                         { return static_cast<unsigned long long int>(dist); }

      virtual void setPoint (const DgLocation& loc, DgLocation& point) const;

      virtual void setPoint (const DgLocation& loc, const DgRFBase& rf,
                                                   DgLocation& point) const;
      virtual void setPoint (const A& add, const DgRFBase& rf,
                             DgLocation& point) const
               { setAddPoint(add, point); rf.convert(&point); }

      virtual void setPoint (const A& add, DgLocation& pt) const
                    { pt.clearAddress(); backFrame().convert(&pt);
                      setAddPoint(add, pt); }

      virtual void setVertices (const DgLocation& loc, DgPolygon& vec) const;

      virtual void setVertices (const DgLocation& loc, const DgRFBase& rf,
                                                       DgPolygon& vec) const;

      virtual void setVertices (const A& add, const DgRFBase& rf,
                                                        DgPolygon& vec) const
               { setAddVertices(add, vec); rf.convert(vec); }

      virtual void setVertices  (const A& add, DgPolygon& vec) const
               { vec.clearAddress(); backFrame().convert(vec);
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
               { DgPolygon* vec = new DgPolygon(backFrame());
                 setVertices(loc, *vec);  return vec; }

      virtual DgLocation* makePoint (const DgLocation& loc) const
               { DgLocation* pt = new DgLocation(backFrame());
                 setPoint(loc, *pt);  return pt; }

      virtual DgLocVector* makeNeighbors (const A& add) const
               { DgLocVector* vec = new DgLocVector(*this);
                 setNeighbors(add, *vec);  return vec; }

      virtual DgPolygon* makeVertices (const A& add) const
               { DgPolygon* vec = new DgPolygon(backFrame());
                 setVertices(add, *vec);  return vec; }

      virtual DgLocation* makePoint (const A& add) const
               { DgLocation* pt = new DgLocation(backFrame());
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

      virtual operator string (void) const {
         string s = "*** DgDiscRF " + DgRFBase::name();
         return s;
      }

      // remind users of the pure virtual functions remaining from above

      virtual string add2str (const A& add) const = 0;
      virtual string add2str (const A& add, char delimiter) const = 0;

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

      DgDiscRF (DgRFNetwork& networkIn, const DgRF<B, DB>& backFrameIn,
                const string& nameIn = "Disc",
                DgGridTopology gridTopoIn = Hexagon,
                DgGridMetric gridMetricIn = D6,
                long double eIn = 1.0L,
                long double rIn = 1.0L, long double cIn = 1.0L, long double areaIn = 1.0L)
        : DgRF<A, long long int> (networkIn, nameIn), backFrame_ (&backFrameIn),
          e_ (eIn), r_ (rIn), c_ (cIn), area_ (areaIn), gridTopo_ (gridTopoIn),
          gridMetric_ (gridMetricIn)
        { new DgQuantConverter(backFrame(), *this);
          new DgInvQuantConverter(*this, backFrame()); }

      DgDiscRF (const DgDiscRF<A, B, DB>& rf) : DgRF<A, long long int> (rf),
          backFrame_ (&rf.backFrame()), e_ (rf.e()), r_ (rf.r()),
          c_ (rf.c()), area_ (rf.area()), gridTopo_ (rf.gridTopo()),
          gridMetric_ (rf.gridMetric())
        { new DgQuantConverter(backFrame(), *this);
          new DgInvQuantConverter(*this, backFrame()); }

      virtual void setAddPoint (const A& add, DgLocation& pt) const;

      const DgRF<B, DB>* backFrame_;

      long double e_;   // edge length
      long double r_;   // radius (max center-to-vertex)
      long double c_;   // irrational constant
      long double area_; // cell area

      DgGridTopology gridTopo_;
      DgGridMetric gridMetric_;

};

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> ostream& operator<< (ostream& stream,
          const DgDiscRF<A, B, DB>& g)
{
   stream << string(g) << endl;

   return stream;
}

#include "DgDiscRF_hpp.h"

#endif
