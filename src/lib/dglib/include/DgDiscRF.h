////////////////////////////////////////////////////////////////////////////////
//
// DgDiscRF.h: DgDiscRF class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRF_H
#define DGDISCRF_H

#include <cstdint>

#include "DgRF.h"
#include "DgPolygon.h"

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> 
class DgDiscRF : public DgRF<A, std::int64_t> {

   public:

      class DgQuantConverter : public DgConverter<B, DB, A, std::int64_t> {

         public:

            DgQuantConverter (const DgRF<B, DB>& fromFrame, 
                              const DgDiscRF<A, B, DB>& toFrame)
               : DgConverter<B, DB, A, std::int64_t> 
                       (static_cast< const DgRF<B, DB>& >(fromFrame), 
                        static_cast< const DgRF<A, std::int64_t>& >(toFrame)) { }

            virtual A convertTypedAddress (const B& addIn) const
              { return 
                   static_cast<const DgDiscRF<A, B, DB>&>(
                                  this->toFrame()).quantify(addIn); 
	      }

      };

      class DgInvQuantConverter : public DgConverter<A, std::int64_t, B, DB> {

         public:

            DgInvQuantConverter (const DgDiscRF<A, B, DB>& fromFrame,
                                 const DgRF<B, DB>& toFrame)
               : DgConverter<A, std::int64_t, B, DB> (fromFrame, toFrame) { }

            virtual B convertTypedAddress (const A& addIn) const
             { return 
                  static_cast<const DgDiscRF<A, B, DB>&>(
                                         this->fromFrame()).invQuantify(addIn); 
	     }

      };

      DgDiscRF (DgRFNetwork& networkIn, const DgRF<B, DB>& backFrameIn,
                const string& nameIn = "Disc", long double eIn = 1.0L,
                long double rIn = 1.0L, long double cIn = 1.0L, long double areaIn = 1.0L)
        : DgRF<A, std::int64_t> (networkIn, nameIn), backFrame_ (&backFrameIn),
          e_ (eIn), r_ (rIn), c_ (cIn), area_ (areaIn)
        { new DgQuantConverter(backFrame(), *this);
          new DgInvQuantConverter(*this, backFrame()); }

      DgDiscRF (const DgDiscRF<A, B, DB>& rf) : DgRF<A, std::int64_t> (rf), 
          backFrame_ (&rf.backFrame()), e_ (rf.e()), r_ (rf.r()),
          c_ (rf.c()), area_ (rf.area())
        { new DgQuantConverter(backFrame(), *this);
          new DgInvQuantConverter(*this, backFrame()); 
	}

      DgDiscRF& operator= (const DgDiscRF<A, B, DB>& rf)
          { 
             if (&rf != this)
             {
                DgRF<A, std::int64_t>::operator=(rf); 
                e_ = rf.e();
                r_ = rf.r();
                c_ = rf.c();
                area_ = rf.area();
                backFrame_ = &rf.backFrame();
             }
             return *this; 
          }

      // get methods

      const DgRF<B, DB>& backFrame (void) const { return *backFrame_; }

      long double e    (void) const { return e_; }
      long double r    (void) const { return r_; }
      long double c    (void) const { return c_; }
      long double area (void) const { return area_; }

      // set methods

      void setE    (long double eIn) { e_ = eIn; }
      void setR    (long double rIn) { r_ = rIn; }
      void setC    (long double cIn) { c_ = cIn; }
      void setArea (long double areaIn) { area_ = areaIn; }

      // misc methods
      
      virtual string dist2str (const std::int64_t& dist) const { return dgg::util::to_string(dist); }
      virtual long double dist2dbl (const std::int64_t& dist) const { return (long double) dist; }
      virtual std::uint64_t dist2int (const std::int64_t& dist) const 
                         { return static_cast<std::uint64_t>(dist); }

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

      // remind users of the pure virtual functions remaining from above
      
      virtual string add2str (const A& add) const = 0;
      virtual string add2str (const A& add, char delimiter) const = 0;

      virtual std::int64_t dist (const A& add1, const A& add2) const = 0;

      virtual const char* str2add (A* add, const char* str, char delimiter) 
                                                                      const = 0;

      virtual const A& undefAddress (void) const = 0;

      // new pure virtual functions

      virtual A quantify    (const B& point) const = 0;
      virtual B invQuantify (const A& add)   const = 0;

      virtual void setAddNeighbors (const A& add, DgLocVector& vec) const = 0;
      virtual void setAddVertices  (const A& add, DgPolygon& vec) const = 0;

   protected:

      virtual void setAddPoint (const A& add, DgLocation& pt) const;

      const DgRF<B, DB>* backFrame_;

      long double e_;   // edge length
      long double r_;   // radius (max center-to-vertex)
      long double c_;   // irrational constant
      long double area_; // cell area

};

#include "../lib/DgDiscRF.hpp"

#endif
