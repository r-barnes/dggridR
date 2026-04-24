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
/** @file DgDiscTopoRFS.h
 * @brief   DgDiscTopoRFS class definitions.
 */

#ifndef DGDISCTOPORFS_H
#define DGDISCTOPORFS_H

#include <vector>

#include "Dg2WayConverter.h"
#include "DgConverter.h"
#include "DgDiscTopoRF.h"
#include "DgDiscRFSGrids.h"
//#include "DgDiscRFS.h"
#include "DgPolygon.h"
//#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
/** @class DgDiscTopoRFS
*  @brief A discrete RFS with pyramid addresses and an arbitrary backframe (e.g., lat/lon or a substrate
*             grid. Specifies methods for forward/inverse quantification, spatial parents/children, and cell center point,
*             vertexes, boundary, and neighbors.
*
*             This is a parallel/alternate to DgDiscRFS, which has a pyramid address backframe and specifies
*             only forward/inverse quantification.
*/
template<class A, class B, class DB> class DgDiscTopoRFS
         : public DgDiscTopoRF<DgResAdd<A>, B, DB>,
           public DgDiscRFSGrids<DgDiscTopoRF, A, B, DB, B> {

   public:

      DgDiscTopoRFS<A, B, DB>& operator= (const DgDiscTopoRFS<A, B, DB>& rf)
      // shallow copy with possible memory leak; don't use if avoidable
          {
             if (*this != rf)
             {
                DgDiscRFSGrids<DgDiscTopoRF, A, B, DB, B>::operator=(rf);
                gridTopo_ = rf.gridTopo();
                gridMetric_ = rf.gridMetric();
                aperture_ = rf.aperture();
                isCongruent_ = rf.isCongruent();
                isAligned_ = rf.isAligned();

                this->grids_ = new std::vector<const DgDiscTopoRF<A, B, DB>*>(rf.nRes(), nullptr);
                for (int i = 0; i < rf.nRes(); i++)
                {
                   // KLUDGE: don't know real type of each grid so can't
                   // easily create them here; opt for shallow copy
                   (*(this->grids_))[i] = rf.grids()[i];
                }
            }

            return *this;
         }

      // abstract functions from DgDiscRF
      // these functions are the same as in DgDiscRFS
      virtual std::string add2str (const DgResAdd<A>& add) const
                         { return std::string(add); }

      virtual std::string add2str (const DgResAdd<A>& add, char delimiter) const
                           { return dgg::util::to_string(add.res()) + delimiter +
                               (*(this->grids_))[add.res()]->add2str(add.address(), delimiter); }

      virtual const char* str2add (DgResAdd<A>* add, const char* str, char delimiter) const;

      unsigned int aperture (void) const { return aperture_; }

      DgGridTopology    gridTopo    (void) const { return gridTopo_; }
      DgGridMetric      gridMetric  (void) const { return gridMetric_; }

      bool isCongruent (void) const { return isCongruent_; }
      bool isAligned   (void) const { return isAligned_; }

      // no bounds checking
      const DgDiscTopoRF<A, B, DB>& topoRF (int res) const
        { return *(static_cast<const DgDiscTopoRF<A, B, DB>*>(this->grids()[res])); }

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
             if (add.res() > 0 && add.res() < this->nRes())
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
             if (add.res() >= 0 && add.res() < (this->nRes() - 1))
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
             if (add.res() >= 0 && add.res() < (this->nRes() - 1))
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

      // second order boundary children (hex aperture 7 only)

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
             if (add.res() >= 0 && add.res() < (this->nRes() - 1)) {
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
             if (add.res() >= 0 && add.res() < (this->nRes() - 1)) {
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

   protected:
      DgDiscTopoRFS (DgRFNetwork& network, const DgRF<B, DB>& backFrame,
                 int nResIn, dgg::topo::DgGridTopology gridTopo = dgg::topo::Hexagon,
                 dgg::topo::DgGridMetric gridMetric = dgg::topo::D6,
                 unsigned int aperture = 3, bool isCongruent = true,
                 bool isAligned = false, const std::string& name = "DiscS")
        : DgDiscTopoRF<DgResAdd<A>, B, DB> (network, backFrame, name),
          DgDiscRFSGrids<DgDiscTopoRF, A, B, DB, B> (backFrame, nResIn),
               //DgDiscRFSGrids<DgDiscTopoRF, A, B, DB> (backFrame, nResIn),
               gridTopo_ (gridTopo), gridMetric_ (gridMetric), aperture_ (aperture),
               isCongruent_ (isCongruent), isAligned_ (isAligned)
        {
          //this->grids_ = new std::vector<const DgDiscTopoRF<A, B, DB>*>(nRes, nullptr);
          if (!this->isAligned() && !this->isCongruent()) {
             report("DgDiscRFS::DgDiscRFS() grid system must be either "
                    "congruent, aligned, or both", DgBase::Fatal);
          }
        }

      DgDiscTopoRFS (const DgDiscTopoRFS<A, B, DB>& rf) // uses dubious operator=
         : DgDiscTopoRF<DgResAdd<A>, B, DB> (rf.network(), rf.backFrame(), rf.name()),
                 DgDiscRFSGrids<DgDiscTopoRF, A, B, DB, B> (rf.backFrame(), rf.nRes())
        { *this = rf; }

      // use default RFS dist
      virtual long long int dist (const DgResAdd<A>& add1,
                               const DgResAdd<A>& add2) const
                     { return this->distRFS(add1, add2); }

      virtual DgResAdd<A> quantify (const B& point) const
                 {
                     // quantify using max res grid
                     int maxRes = this->nRes() - 1;
                     DgLocation* loc = this->backFrame().makeLocation(point);
                     const DgDiscTopoRF<A, B, DB>& grid = *this->grids()[maxRes];
                     grid.convert(loc);
                     DgResAdd<A> add(*grid.getAddress(*loc), maxRes);
                     delete loc;

                     return add;
                  }

       virtual B invQuantify (const DgResAdd<A>& add) const
               {
                   const DgDiscTopoRF<A, B, DB>& grid = *this->grids()[add.res()];
                   DgLocation* loc = grid.makeLocation(add.address());
                   this->backFrame().convert(loc);
                   B newAdd(*(this->backFrame().getAddress(*loc)));
                   delete loc;
                   return newAdd;
               }

       virtual void setAddVertices (const DgResAdd<A>& add,
                                   DgPolygon& vec) const
                    { this->grids()[add.res()]->backFrame().convert(vec);
                        topoRF(add.res()).setVertices(add.address(), vec);
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
      DgGridTopology gridTopo_;
      DgGridMetric gridMetric_;
      int aperture_;
      bool isCongruent_;
      bool isAligned_;

};

////////////////////////////////////////////////////////////////////////////////
/** @class Dg2WayResAddConverter
*  @brief Converts to/from DgResAdd<A> and an A in the correct resolution grid
*
*   Has template parameters:
*
*   DRF - class of each resolution in the grids_ array; should have A addresses
*   A - the DiscRF type to be wrapped in DgResAdd
*   B - background RF type for DgResAdd<A>
*   DB - distance type of background RF
*   BG -  background RF for each resolution grid
*
*   each resolution in DgDiscRFSGrids fromFrame grids_ array is a DRF<A, BG, DB>*
*/

template <class A, class B, class BG, class DB> class Dg2WayResAddConverter
                                               : public Dg2WayConverter {

   public:

      Dg2WayResAddConverter (const DgDiscRF<DgResAdd<A>, B, DB>& fromFrameRFS,
                             const DgDiscRF<A, BG, DB>& toFrameRF, int res)
         : Dg2WayConverter
              (*(new DgResAddConverter<A, B, BG, DB>(fromFrameRFS, toFrameRF, res)),
               *(new DgAddResConverter<A, B, BG, DB>(toFrameRF, fromFrameRFS, res)))
           { }

};

////////////////////////////////////////////////////////////////////////////////
template <class A, class B, class DB> class Dg2WayTopoResAddConverter
                                : public Dg2WayResAddConverter<A, B, B, DB> {

   public:

      Dg2WayTopoResAddConverter (const DgDiscTopoRFS<A, B, DB>& fromFrameRFS,
                                 //DgDiscRF<DgResAdd<A>, B, DB>& fromFrame,
                                 //const DgDiscRFSGrids<DgDiscTopoRF, A, B, DB, B>& fromFrame,
                                 const DgDiscRF<A, B, DB>& toFrameRF, int res)
                                    : Dg2WayResAddConverter<A, B, B, DB> (fromFrameRFS, toFrameRF, res) {  }
};

// JFW: is this really what we mean?
#include "DgDiscTopoRFS_hpp.h"

#endif
