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
// DgDiscRFS.h: DgDiscRFS class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRFS_H
#define DGDISCRFS_H

#include <vector>

#include "Dg2WayConverter.h"
#include "DgConverter.h"
#include "DgDiscRF.h"
#include "DgPolygon.h"
#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
template<class A> class DgResAdd {

   public:

      DgResAdd (void) : res_ (0) { }

      DgResAdd (const DgResAdd& add) { *this = add; }

      DgResAdd (const A& add, int res = 0)
         : address_ (add), res_ (res) { }

      const A& address (void) const { return address_; }

      void setAddress (const A& add) { address_ = add; }

      int res (void) const { return res_; }

      void setRes (int res) { res_ = res; }

      operator string (void) const
                { return  string("[") + dgg::util::to_string((int) res()) + ", " +
                          string(address()) + string("]"); }

      DgResAdd<A>& operator= (const DgResAdd<A>& add)
                {
                   if (add != *this)
                   {
                      setRes(add.res());
                      setAddress(add.address());
                   }

                   return *this;
                }

      bool operator== (const DgResAdd<A>& add) const
             { return res() == add.res() && address() == add.address(); }

      bool operator!= (const DgResAdd<A>& add) const
             { return !operator==(add); }

/*
      // remind users of the pure virtual functions remaining from above

       virtual const DgResAdd<A>& undefAddress (void) const = 0;
*/

   private:

      A address_{};
      int res_ = -1;
};

////////////////////////////////////////////////////////////////////////////////
template<class A> ostream& operator<< (ostream& stream, const DgResAdd<A>& add)
{
   stream << string(add);
   return stream;

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> class DgDiscRFS
                               : public DgDiscRF<DgResAdd<A>, B, DB> {

   public:

     ~DgDiscRFS (void)
        {
/* the grids themselves are deleted by the DgRFNetwork
           for (unsigned int i = 0; i < grids().size(); i++)
              delete (*grids_)[i];
*/

           delete grids_;
        }


      virtual string add2str (const DgResAdd<A>& add) const
                                              { return string(add); }

      virtual string add2str (const DgResAdd<A>& add, char delimiter) const
                  { return dgg::util::to_string(add.res()) + delimiter +
                        grids()[add.res()]->add2str(add.address(), delimiter); }

      virtual const char* str2add (DgResAdd<A>* add, const char* str,
                                   char delimiter) const;

      DgDiscRFS<A, B, DB>& operator= (const DgDiscRFS<A, B, DB>& rf)
      // shallow copy with possible memory leak; don't use if avoidable
          {
             if (*this != rf)
             {
                DgDiscRF<DgResAdd<A>, B, DB>::operator=(rf);

                aperture_ = rf.aperture();
                nRes_ = rf.nRes();
                isCongruent_ = rf.isCongruent();
                isAligned_ = rf.isAligned();

                delete grids_; // the DgRFNetwork deleteds the grids themselves

                grids_ = new vector<const DgDiscRF<A, B, DB>*>(rf.nRes());
                for (int i = 0; i < nRes(); i++)
                {
                   // KLUDGE: don't know real type of each grid so can't
                   // easily create them here; opt for shallow copy

                   (*grids_)[i] = rf.grids()[i];
                }
            }

            return *this;
         }

      const vector<const DgDiscRF<A, B, DB>*>& grids (void) const { return *grids_; }

      unsigned int aperture (void) const { return aperture_; }

      int nRes (void) const { return nRes_; }

      bool isCongruent (void) const { return isCongruent_; }
      bool isAligned   (void) const { return isAligned_; }

      // no bounds checking

      const DgDiscRF<A, B, DB>& operator[] (int res) const
                           { return *((*grids_)[res]); }

      // hokey temporary notion of distance

      virtual long long int dist (const DgResAdd<A>& add1,
                        const DgResAdd<A>& add2) const
           { return abs(add2.res() - add1.res()); }

      // parents

      // only the DgLocation version performs checking on the input

      virtual void setParents (int res,
                    const DgLocation& loc, DgLocVector& vec) const;

      virtual void setParents (const DgResAdd<A>& add, const DgRFBase& rf,
                               DgLocVector& vec) const
           {
             setParents(add, vec);
             rf.convert(vec);
           }

      virtual void setParents (const DgResAdd<A>& add, DgLocVector& vec) const
           {
             vec.clearAddress();
             this->convert(vec);
             if (add.res() > 0 && add.res() < nRes())
	      setAddParents(add, vec);
           }

      virtual DgLocVector* makeParents (int res, const DgLocation& loc) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setParents(res, loc, *vec);

             return vec;
           }

      virtual DgLocVector* makeParents (const DgResAdd<A>& add) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setParents(add, *vec);
             return vec;
           }

      // interior children

      // only the DgLocation version performs checking on the input

      virtual void setInteriorChildren (int res,
                    const DgLocation& loc, DgLocVector& vec) const;

      virtual void setInteriorChildren (const DgResAdd<A>& add,
                                        const DgRFBase& rf,
                                        DgLocVector& vec) const
           {
              setInteriorChildren(add, vec);
              rf.convert(vec);
           }

      virtual void setInteriorChildren (const DgResAdd<A>& add,
                                        DgLocVector& vec) const
           {
             vec.clearAddress();
             this->convert(vec);
             if (add.res() >= 0 && add.res() < (nRes() - 1))
             {
                setAddInteriorChildren(add, vec);
             }
           }

      virtual DgLocVector* makeInteriorChildren (int res,
                                                 const DgLocation& loc) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setInteriorChildren(res, loc, *vec);

             return vec;
           }

      virtual DgLocVector* makeInteriorChildren (const DgResAdd<A>& add) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setInteriorChildren(add, *vec);

             return vec;
           }

      // boundary children

      // only the DgLocation version performs checking on the input

      virtual void setBoundaryChildren (int res,
                    const DgLocation& loc, DgLocVector& vec) const;

      virtual void setBoundaryChildren (const DgResAdd<A>& add,
                                        const DgRFBase& rf,
                                        DgLocVector& vec) const
           {
              setBoundaryChildren(add, vec);
              rf.convert(vec);
           }

      virtual void setBoundaryChildren (const DgResAdd<A>& add,
                                        DgLocVector& vec) const
           {
             vec.clearAddress();
             this->convert(vec);
             if (add.res() >= 0 && add.res() < (nRes() - 1))
             {
                setAddBoundaryChildren(add, vec);
             }
           }

      virtual DgLocVector* makeBoundaryChildren (int res,
                                                 const DgLocation& loc) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setBoundaryChildren(res, loc, *vec);

             return vec;
           }

      virtual DgLocVector* makeBoundaryChildren (const DgResAdd<A>& add) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setBoundaryChildren(add, *vec);

             return vec;
           }

      // second order boundary children (aperture 7 only)

      // only the DgLocation version performs checking on the input

      virtual void setBoundary2Children (int res,
                    const DgLocation& loc, DgLocVector& vec) const;

      virtual void setBoundary2Children (const DgResAdd<A>& add,
                                        const DgRFBase& rf,
                                        DgLocVector& vec) const
           {
              setBoundary2Children(add, vec);
              rf.convert(vec);
           }

      virtual void setBoundary2Children (const DgResAdd<A>& add,
                                        DgLocVector& vec) const
           {
             vec.clearAddress();
             this->convert(vec);
             if (add.res() >= 0 && add.res() < (nRes() - 1))
             {
                setAddBoundary2Children(add, vec);
             }
           }

      virtual DgLocVector* makeBoundary2Children (int res,
                                                 const DgLocation& loc) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setBoundary2Children(res, loc, *vec);

             return vec;
           }

      virtual DgLocVector* makeBoundary2Children (const DgResAdd<A>& add) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setBoundary2Children(add, *vec);

             return vec;
           }

      // all children (interior and boundary)

      // only the DgLocation version performs checking on the input

      virtual void setAllChildren (int res,
                    const DgLocation& loc, DgLocVector& vec) const;

      virtual void setAllChildren (const DgResAdd<A>& add,
                                        const DgRFBase& rf,
                                        DgLocVector& vec) const
           {
              setAllChildren(add, vec);
              rf.convert(vec);
           }

      virtual void setAllChildren (const DgResAdd<A>& add,
                                        DgLocVector& vec) const
           {
             vec.clearAddress();
             this->convert(vec);
             if (add.res() >= 0 && add.res() < (nRes() - 1))
             {
                setAddAllChildren(add, vec);
             }
           }

      virtual DgLocVector* makeAllChildren (int res,
                                                 const DgLocation& loc) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setAllChildren(res, loc, *vec);

             return vec;
           }

      virtual DgLocVector* makeAllChildren (const DgResAdd<A>& add) const
           {
             DgLocVector* vec = new DgLocVector(*this);
             setAllChildren(add, *vec);

             return vec;
           }

      virtual operator string (void) const
      {
         string s = "*** DgDiscRFS " + DgRFBase::name() +
               "\nap: " + dgg::util::to_string(aperture()) +
               "\nnRes: " + dgg::util::to_string(nRes()) +
               "\nisCongruent: " + dgg::util::to_string(isCongruent()) +
               "\nisAligned: " + dgg::util::to_string(isAligned()) + "\n";
         for (int i = 0; i < nRes(); i++)
            s += " >>> " + dgg::util::to_string(i) + ": " +
                   string(*(*grids_)[i]) + "\n";

         return s;
      }

   protected:

      DgDiscRFS (DgRFNetwork& network, const DgRF<B, DB>& backFrame,
                 int nResIn, unsigned int aperture,
                 dgg::topo::DgGridTopology gridTopo = dgg::topo::Hexagon,
                 dgg::topo::DgGridMetric gridMetric = dgg::topo::D6,
                 bool isCongruent = true, bool isAligned = false,
                 const string& name = "DiscS")
        : DgDiscRF<DgResAdd<A>, B, DB>
                      (network, backFrame, name, gridTopo, gridMetric),
          aperture_ (aperture), grids_ (new vector<const DgDiscRF<A, B, DB>*>()),
          nRes_ (nResIn), isCongruent_ (isCongruent),
          isAligned_ (isAligned)
        {
          if (nRes() < 0)
          {
             report("DgDiscRFS<A, B, DB>::DgDiscRF() nRes < 0",
                    DgBase::Fatal);
          }

          if (!this->isAligned() && !this->isCongruent())
          {
             report("DgDiscRFS::DgDiscRFS() grid system must be either "
                    "congruent, aligned, or both", DgBase::Fatal);
          }

          grids_->resize(nRes());
        }

      DgDiscRFS (const DgDiscRFS<A, B, DB>& rf) // uses dubious operator=
        : DgDiscRF<DgResAdd<A>, B, DB> (rf)
        { *this = rf; }

      virtual DgResAdd<A> quantify (const B& point) const
            {
               // quantify using max res grid

               int maxRes = nRes() - 1;
               DgLocation* loc = this->backFrame().makeLocation(point);
               const DgDiscRF<A, B, DB>& grid = *grids()[maxRes];
               grid.convert(loc);
               DgResAdd<A> add(*grid.getAddress(*loc), maxRes);

               delete loc;

               return add;
             }

      virtual B invQuantify (const DgResAdd<A>& add) const
             {
               const DgDiscRF<A, B, DB>& grid = *grids()[add.res()];
               DgLocation* loc = grid.makeLocation(add.address());
               this->backFrame().convert(loc);
               B newAdd(*(this->backFrame().getAddress(*loc)));
               delete loc;
               return newAdd;
             }

      virtual void setAddVertices (const DgResAdd<A>& add,
                                   DgPolygon& vec) const
                    { grids()[add.res()]->backFrame().convert(vec);
                      grids()[add.res()]->setVertices(add.address(), vec);
                      this->backFrame().convert(vec);
		    }

      virtual void setAddNeighbors (const DgResAdd<A>& add,
                                    DgLocVector& vec) const;

      // second order boundary children; aperture 7 hex only
      virtual void setAddBoundary2Children (const DgResAdd<A>&, DgLocVector&) const { }

      // new pure virtual functions

      virtual void setAddParents (const DgResAdd<A>& add,
                                  DgLocVector& vec) const = 0;

      virtual void setAddInteriorChildren (const DgResAdd<A>& add,
                                           DgLocVector& vec) const = 0;

      virtual void setAddBoundaryChildren (const DgResAdd<A>& add,
                                           DgLocVector& vec) const = 0;

      virtual void setAddAllChildren (const DgResAdd<A>& add,
                                      DgLocVector& vec) const = 0;

      // state data

      const DgRF<B, DB>* backFrame_;

      unsigned int aperture_;

      vector<const DgDiscRF<A, B, DB>*>* grids_;

      int nRes_;
      bool isCongruent_;
      bool isAligned_;

};

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> ostream& operator<< (ostream& stream,
          const DgDiscRFS<A, B, DB>& g)
{
   stream << string(g) << endl;

   return stream;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <class A, class B, class DB> class DgResAddConverter :
    public DgConverter<DgResAdd<A>, long long int, A, long long int> {

   public:

      DgResAddConverter (const DgDiscRFS<A, B, DB>& fromFrame,
                         const DgDiscRF<A, B, DB>& toFrame, int resIn)
         : DgConverter<DgResAdd<A>, long long int, A,
                 long long int> (fromFrame, toFrame),
           res_ (resIn), discRFS_ (fromFrame), discRF_ (toFrame)
           {
	      // JFW: second clause will never be > (int vs long):
              if (res() < 0 ||
                  static_cast<unsigned long>(res()) >= discRFS().grids().size())
              {
                 report("DgResAddConverter<A, B, DB>::DgResAddConverter() "
                        "invalid resolution", DgBase::Fatal);
              }

              if (*(discRFS().grids()[res()]) != discRF())
              {
                 report("DgDgResAddConverter<A, B, DB>::DgResAddConverter() "
                        "grid mismatch", DgBase::Fatal);
              }
           }

      DgResAddConverter (const DgResAddConverter& con)
         : DgConverter<DgResAdd<A>, long long int, A, long long int> (con) { }

      int res (void) const { return res_; }

      const DgDiscRFS<A, B, DB>& discRFS (void) const { return discRFS_; }

      const DgDiscRF<A, B, DB>& discRF (void) const { return discRF_; }

      virtual A convertTypedAddress (const DgResAdd<A>& add) const
        {
           if (add.res() == res()) return add.address();

           DgLocation* tmpLoc =
             discRFS().grids()[add.res()]->makeLocation(add.address());
           discRF().convert(tmpLoc);
           A newAdd = *(discRF().getAddress(*tmpLoc));
           delete tmpLoc;
           return newAdd;
        }

   protected:

      int res_;
      const DgDiscRFS<A, B, DB>& discRFS_;
      const DgDiscRF<A, B, DB>& discRF_;

};

////////////////////////////////////////////////////////////////////////////////
template <class A, class B, class DB> class DgAddResConverter :
    public DgConverter<A, long long int, DgResAdd<A>, long long int> {

   public:

      DgAddResConverter (const DgDiscRF<A, B, DB>& fromFrame,
                         const DgDiscRFS<A, B, DB>& toFrame, int resIn)
         : DgConverter<A, long long int, DgResAdd<A>,
                       long long int> (fromFrame, toFrame),
           res_ (resIn), discRFS_ (toFrame), discRF_ (fromFrame)
           {
              // JFW: Note that int res() > long size() can never occur:
              if (res() < 0 ||
                static_cast<unsigned long>(res()) >= discRFS().grids().size())
              {
                 report("DgDgAddResConverter<A, B, DB>::DgAddResConverter() "
                        "invalid resolution", DgBase::Fatal);
              }

              if (*(discRFS().grids()[res()]) != discRF())
              {
                 report("DgAddResConverter<A, B, DB>::DgAddResConverter() "
                        "grid mismatch", DgBase::Fatal);
              }
           }

      DgAddResConverter (const DgAddResConverter& con)
         : DgConverter<A, long long int, DgResAdd<A>, long long int> (con) { }

      int res (void) const { return res_; }

      const DgDiscRFS<A, B, DB>& discRFS (void) const { return discRFS_; }

      const DgDiscRF<A, B, DB>& discRF (void) const { return discRF_; }

      virtual DgResAdd<A> convertTypedAddress (const A& add) const
        {
           return DgResAdd<A>(add, res());
        }

   protected:

      int res_;
      const DgDiscRFS<A, B, DB>& discRFS_;
      const DgDiscRF<A, B, DB>& discRF_;

};

////////////////////////////////////////////////////////////////////////////////
template <class A, class B, class DB> class Dg2WayResAddConverter
                                               : public Dg2WayConverter {

   public:

      Dg2WayResAddConverter (const DgDiscRFS<A, B, DB>& fromFrame,
                             const DgDiscRF<A, B, DB>& toFrame, int res)
         : Dg2WayConverter
              (*(new DgResAddConverter<A, B, DB>(fromFrame, toFrame, res)),
               *(new DgAddResConverter<A, B, DB>(toFrame, fromFrame, res)))
           { }

};

// JFW: is this really what we mean?
#include "DgDiscRFS_hpp.h"

#endif
