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
// DgDiscRF.h: DgDiscRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRF_H
#define DGDISCRF_H

#include "DgRF.h"

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
             { return static_cast<const DgDiscRF<A, B, DB>&>(
                                         this->fromFrame()).invQuantify(addIn);
	     }

      };

      virtual ~DgDiscRF (void) { }

      DgDiscRF& operator= (const DgDiscRF<A, B, DB>& rf)
          {
             if (&rf != this) {
                DgRF<A, long long int>::operator=(rf);
                backFrame_ = &rf.backFrame();
             }
             return *this;
          }

      // get methods
      const DgRF<B, DB>& backFrame (void) const { return *backFrame_; }

      // misc methods from above
      virtual std::string dist2str (const long long int& dist) const { return dgg::util::to_string(dist); }
      virtual long double dist2dbl (const long long int& dist) const { return (long double) dist; }
      virtual unsigned long long int dist2int (const long long int& dist) const
                         { return static_cast<unsigned long long int>(dist); }

      virtual operator std::string (void) const {
         std::string s = "*** DgDiscRF " + DgRFBase::name();
         return s;
      }

      // give this a default definition for RF's that don't have a well defined
      // concept of distance
      virtual long long int dist (const A& add1, const A& add2) const
                  { return -1; }

      // remind users of the pure virtual functions remaining from above

      virtual std::string add2str (const A& add) const = 0;
      virtual std::string add2str (const A& add, char delimiter) const = 0;

      virtual const char* str2add (A* add, const char* str, char delimiter)
                                                                      const = 0;

      virtual const A& undefAddress (void) const = 0;

      // new pure virtual functions

      virtual A quantify    (const B& point) const = 0;
      virtual B invQuantify (const A& add)   const = 0;

   protected:

      DgDiscRF (DgRFNetwork& networkIn, const DgRF<B, DB>& backFrameIn,
                const std::string& nameIn = "Disc")
        : DgRF<A, long long int> (networkIn, nameIn), backFrame_ (&backFrameIn)
        { new DgQuantConverter(backFrame(), *this);
          new DgInvQuantConverter(*this, backFrame()); }

      DgDiscRF (const DgDiscRF<A, B, DB>& rf) : DgRF<A, long long int> (rf),
          backFrame_ (&rf.backFrame())
        { new DgQuantConverter(backFrame(), *this);
          new DgInvQuantConverter(*this, backFrame()); }

      int abc;
      const DgRF<B, DB>* backFrame_;
};

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> std::ostream& operator<< (std::ostream& stream,
          const DgDiscRF<A, B, DB>& g)
{
   stream << std::string(g) << std::endl;
   return stream;
}

#include "DgDiscRF_hpp.h"

#endif
