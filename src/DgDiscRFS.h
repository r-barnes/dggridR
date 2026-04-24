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
/** @file DgDiscRFS.h
 * @brief   DgDiscRFS class definitions.
 */

#ifndef DGDISCRFS_H
#define DGDISCRFS_H

#include <vector>

#include "DgDiscRF.h"
#include "DgPolygon.h"
#include "DgDiscRFSGrids.h"

////////////////////////////////////////////////////////////////////////////////
/** @class DgDiscRFS
*  @brief A discrete RFS with pyramid addresses and a backframe that also has discfrete
*             pyramid addresses. The only operations specified are forward and inverse quantization.
*/
template<class A, class B, class DB> class DgDiscRFS
         : public DgDiscRF<DgResAdd<A>, DgResAdd<B>, DB>,
           public DgDiscRFSGrids<DgDiscRF, A, DgResAdd<B>, DB, B> {

   public:

      DgDiscRFS (DgRFNetwork& network, const DgRF<DgResAdd<B>, DB>& backFrame,
                 int nResIn, const std::string& name = "DiscS")
        : DgDiscRF<DgResAdd<A>, DgResAdd<B>, DB> (network, backFrame, name),
          DgDiscRFSGrids<DgDiscRF, A, DgResAdd<B>, DB, B> (backFrame, nResIn)
        {
        }

      DgDiscRFS (const DgDiscRFS<A, B, DB>& rf) // uses dubious operator=
        : DgDiscRF<DgResAdd<A>, DgResAdd<B>, DB> (rf)
        { *this = rf; }

      // abstract functions from DgDiscRF
      // these functions are the same as in DgDiscRFS

      virtual std::string add2str (const DgResAdd<A>& add) const
                         { return std::string(add); }

      virtual std::string add2str (const DgResAdd<A>& add, char delimiter) const
                         { return dgg::util::to_string(add.res()) + delimiter +
                           this->grids()[add.res()]->add2str(add.address(), delimiter); }

      virtual const char* str2add (DgResAdd<A>* add, const char* str, char delimiter) const;

      //std::vector<const DgDiscRF<A, B, DB>*>& gridsMutable (void) const { return *grids_; }

      protected:

         virtual long long int dist (const DgResAdd<A>& add1,
                                        const DgResAdd<A>& add2) const
                              { return this->distRFS(add1, add2); }

         virtual DgResAdd<A> quantify (const DgResAdd<B>& point) const
            {
                A newAdd = (this->grids()[point.res()])->quantify(point.address());
                DgResAdd<A> add(newAdd, point.res());
                return add;
             }

         virtual DgResAdd<B> invQuantify (const DgResAdd<A>& add) const
             {
                 B newPt = (this->grids()[add.res()])->invQuantify(add.address());
                 DgResAdd<B> point(newPt, add.res());
                 return point;
             }
/*
      // state data

      const DgRF<B, DB>* backFrame_;
      std::vector<const DgDiscRF<A, B, DB>*>* grids_;
      int nRes_;
*/

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// JFW: is this really what we mean?
#include "DgDiscRFS_hpp.h"

#endif
