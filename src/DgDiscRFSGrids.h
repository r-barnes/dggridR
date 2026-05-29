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
// DgDiscRFSGrids.h: DgDiscRFSGrids class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDISCRFSGRIDS_H
#define DGDISCRFSGRIDS_H

#include <vector>

#include "Dg2WayConverter.h"
#include "DgConverter.h"
//#include "DgDiscRF.h"
#include "DgPolygon.h"
#include "DgRF.h"

//class DgDiscTopoRFS;

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

      operator std::string (void) const
                { return std::string("[") + dgg::util::to_string((int) res()) + ", " +
                         std::string(address()) + std::string("]"); }

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
template<class A> std::ostream& operator<< (std::ostream& stream, const DgResAdd<A>& add)
{
   stream << std::string(add);
   return stream;

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class BG, class DB> class DgDiscRFSGridsBase {

   public:

      virtual ~DgDiscRFSGridsBase (void) { }

      // sub-classes must define
      virtual const DgDiscRF<A, BG, DB>& discRF (int res) const = 0;

      int nRes (void) const { return nRes_; }

      virtual operator std::string (void) const
      {
         std::string s = "*** DgDiscRFSGridsBase\nnRes: " + dgg::util::to_string(nRes()) + "\n";
         for (int i = 0; i < nRes(); i++)
            s += " >>> " + dgg::util::to_string(i) + ": " +
                   std::string(discRF(i)) + "\n";

         return s;
      }

   protected:

      DgDiscRFSGridsBase (const DgRF<B, DB>& backFrameIn, int nResIn)
         : backFrameLocal_ (backFrameIn), nRes_ (nResIn)
        {
          if (nRes() < 0) {
             report("DgDiscRFSGridsBase<A, B, DB>::DgDiscRFSGridsBase() nRed < 0",
                    DgBase::Fatal);
          }
        }

/*
      DgDiscRFSGrids (const DgDiscRFSGrids<DRF, A, B, DB, BG>& rf) // uses dubious operator=
        //: DgDiscRF<DgResAdd<A>, B, DB> (rf)
        { *this = rf; }
*/
      // hokey temporary notion of distance
      virtual long long int distRFS (const DgResAdd<A>& add1,
                      const DgResAdd<A>& add2) const
            { return std::abs(add2.res() - add1.res()); }
    /*
      virtual DgResAdd<A> quantifyRFS (const B& point) const
            {
               // quantify using max res grid
               int maxRes = nRes() - 1;
               DgLocation* loc = backFrameLocal_.makeLocation(point);
               const DRF<A, B, DB>& grid = *grids()[maxRes];
               grid.convert(loc);
               DgResAdd<A> add(*grid.getAddress(*loc), maxRes);

               delete loc;

               return add;
             }

      virtual B invQuantifyRFS (const DgResAdd<A>& add) const
             {
               const DRF<A, B, DB>& grid = *grids()[add.res()];
               DgLocation* loc = grid.makeLocation(add.address());
               backFrameLocal_.convert(loc);
               B newAdd(*(backFrameLocal_.getAddress(*loc)));
               delete loc;
               return newAdd;
             }
     */

      // state data
      const DgRF<B, DB>& backFrameLocal_;
      int nRes_;
};

////////////////////////////////////////////////////////////////////////////////
/** @class DgDiscRFSGrids
 *  @brief A multi-resolution array of pointers to DgDiscRF's
 *
 *   Has template parameters:
 *
 *   DRF - class of each resolution in the grids_ array; should have A addresses
 *   A - the DiscRF type to be wrapped in DgResAdd
 *   B - background RF type for DgResAdd<A>
 *   DB - distance type of background RF
 *   BG -  background RF for each resolution grid
 *
 *   each resolution in the grids_ array is a DRF<A, BG, DB>*
 */

template<template <class, class, class> class DRF, class A, class B, class DB, class BG> class DgDiscRFSGrids
               : public DgDiscRFSGridsBase<A, B, BG, DB> {

   public:

      virtual ~DgDiscRFSGrids (void) { delete grids_; }
/*
      virtual std::string add2str (const DgResAdd<A>& add) const
                                              { return std::string(add); }

      virtual std::string add2str (const DgResAdd<A>& add, char delimiter) const
                  { return dgg::util::to_string(add.res()) + delimiter +
                        grids()[add.res()]->add2str(add.address(), delimiter); }

      virtual const char* str2add (DgResAdd<A>* add, const char* str,
                                   char delimiter) const;
 */

      DgDiscRFSGrids<DRF, A, B, DB, BG>& operator=
                      (const DgDiscRFSGrids<DRF, A, B, DB, BG>& rf)
      // shallow copy with possible memory leak; don't use if avoidable
          {
             if (*this != rf)
             {
                //DRF<DgResAdd<A>, B, DB>::operator=(rf);

                this->nRes_ = rf.nRes();
                delete grids_; // the DgRFNetwork will delete the grids themselves

                grids_ = new std::vector<const DRF<A, BG, DB>*>(rf.nRes());
                for (int i = 0; i < rf.nRes(); i++)
                {
                   // KLUDGE: don't know real type of each grid so can't
                   // easily create them here; opt for shallow copy

                   (*grids_)[i] = rf.grids()[i];
                }
            }

            return *this;
         }

      const std::vector<const DRF<A, BG, DB>*>& grids (void) const { return *grids_; }

      // no bounds checking
      const DRF<A, BG, DB>& operator[] (int res) const
                           { return *((*grids_)[res]); }

      // pure virtual from above
      virtual const DgDiscRF<A, BG, DB>& discRF (int res) const
                           { return *((*grids_)[res]); }

   protected:

      DgDiscRFSGrids (const DgRF<B, DB>& backFrameIn, int nResIn)
         : DgDiscRFSGridsBase<A, B, BG, DB>(backFrameIn, nResIn), grids_ (nullptr)
        {
          this->grids_ = new std::vector<const DRF<A, BG, DB>*>(this->nRes(), nullptr);
        }

      DgDiscRFSGrids (const DgDiscRFSGrids<DRF, A, B, DB, BG>& rf) // uses dubious operator=
        //: DgDiscRF<DgResAdd<A>, B, DB> (rf)
        { *this = rf; }

      std::vector<const DRF<A, BG, DB>*>& gridsMutable (void) const { return *grids_; }

    /*
      virtual DgResAdd<A> quantifyRFS (const B& point) const
            {
               // quantify using max res grid
               int maxRes = nRes() - 1;
               DgLocation* loc = backFrameLocal_.makeLocation(point);
               const DRF<A, B, DB>& grid = *grids()[maxRes];
               grid.convert(loc);
               DgResAdd<A> add(*grid.getAddress(*loc), maxRes);

               delete loc;

               return add;
             }

      virtual B invQuantifyRFS (const DgResAdd<A>& add) const
             {
               const DRF<A, B, DB>& grid = *grids()[add.res()];
               DgLocation* loc = grid.makeLocation(add.address());
               backFrameLocal_.convert(loc);
               B newAdd(*(backFrameLocal_.getAddress(*loc)));
               delete loc;
               return newAdd;
             }
     */

      // state data
      std::vector<const DRF<A, BG, DB>*>* grids_;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <class A, class B, class BG, class DB> class DgResAddConverter :
    public DgConverter<DgResAdd<A>, long long int, A, long long int> {

   public:

      DgResAddConverter (const DgDiscRF<DgResAdd<A>, B, DB>& fromFrameRFS,
                         const DgDiscRF<A, BG, DB>& toFrameRF, int resIn)
         : DgConverter<DgResAdd<A>, long long int, A,
                 long long int> (fromFrameRFS, toFrameRF),
           res_ (resIn), discRFS_ (nullptr), discRF_ (toFrameRF)
           {
              discRFS_ = dynamic_cast<const DgDiscRFSGridsBase<A, B, BG, DB>*>(&fromFrameRFS);
              if (!discRFS_) {
                 report("DgResAddConverter<A, B, BG, DB>::DgResAddConverter() "
                        "from frame is not a DgDiscRFSGridsBase", DgBase::Fatal);
              }

              if (res() < 0 ||
                  res() >= discRFS().nRes() + 1)
              {
                 report("DgResAddConverter<A, B, BG, DB>::DgResAddConverter() "
                        "invalid resolution", DgBase::Fatal);
              }

              if (this->discRFS().discRF(res()) != discRF())
              {
                 report("DgDgResAddConverter<A, B, BG, DB>::DgResAddConverter() "
                        "grid mismatch", DgBase::Fatal);
              }
           }

      DgResAddConverter (const DgResAddConverter& con)
         : DgConverter<DgResAdd<A>, long long int, A, long long int> (con) { }

      int res (void) const { return res_; }

      const DgDiscRFSGridsBase<A, B, BG, DB>& discRFS (void) const { return *discRFS_; }

      const DgDiscRF<A, BG, DB>& discRF (void) const { return discRF_; }

      virtual A convertTypedAddress (const DgResAdd<A>& add) const
        {
           if (add.res() == res()) return add.address();

           DgLocation* tmpLoc =
             discRFS().discRF(add.res()).makeLocation(add.address());
           discRF().convert(tmpLoc);
           A newAdd = *(discRF().getAddress(*tmpLoc));
           delete tmpLoc;
           return newAdd;
        }

   protected:

      int res_;
      const DgDiscRFSGridsBase<A, B, BG, DB>* discRFS_;
      const DgDiscRF<A, BG, DB>& discRF_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <class A, class B, class BG, class DB> class DgAddResConverter :
    public DgConverter<A, long long int, DgResAdd<A>, long long int> {

   public:

      DgAddResConverter (const DgDiscRF<A, BG, DB>& fromFrameRF,
                         const DgDiscRF<DgResAdd<A>, B, DB>& toFrameRFS, int resIn)
         : DgConverter<A, long long int, DgResAdd<A>,
                 long long int> (fromFrameRF, toFrameRFS),
           res_ (resIn), discRFS_ (nullptr), discRF_ (fromFrameRF)
           {
              discRFS_ = dynamic_cast<const DgDiscRFSGridsBase<A, B, BG, DB>*>(&toFrameRFS);
              if (!discRFS_) {
                 report("DgAddResConverter<A, B, DB>::DgAddResConverter() "
                        "from frame is not a DgDiscRFSGridsBase", DgBase::Fatal);
              }

              if (res() < 0 ||
                  res() >= discRFS().nRes() + 1)
              {
                 report("DgAddResConverter<A, B, DB>::DgAddResConverter() "
                        "invalid resolution", DgBase::Fatal);
              }

              if (discRFS().discRF(res()) != discRF())
              {
                 report("DgDgAddResConverter<A, B, DB>::DgAddResConverter() "
                        "grid mismatch", DgBase::Fatal);
              }
           }

      DgAddResConverter (const DgAddResConverter& con)
         : DgConverter<DgResAdd<A>, long long int, A, long long int> (con) { }

      int res (void) const { return res_; }

      const DgDiscRFSGridsBase<A, B, BG, DB>& discRFS (void) const { return *discRFS_; }

      const DgDiscRF<A, BG, DB>& discRF (void) const { return discRF_; }

      virtual DgResAdd<A> convertTypedAddress (const A& add) const
        {
           return DgResAdd<A>(add, res());
        }

   protected:

      int res_;
      const DgDiscRFSGridsBase<A, B, BG, DB>* discRFS_;
      const DgDiscRF<A, BG, DB>& discRF_;

};

#endif


