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
// DgHierNdxSystemRFS.h: DgHierNdxSystemRFS class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHIERNDXSYSTEMRFS_H
#define DGHIERNDXSYSTEMRFS_H

#include <vector>
#include "DgIDGGSBase.h"
#include "DgHierNdxSystemRFSBase.h"
#include "DgHierNdxSystemRF.h"

////////////////////////////////////////////////////////////////////////////////
template <class TINT, class TSTR> class DgHierNdxSystemRFS :
                                     public DgHierNdxSystemRFSBase {

   public:

   protected:

     DgHierNdxSystemRFS (const DgIDGGSBase& dggsIn, bool extModeIntIn = true,
            const std::string& nameIn = "HierNdxIDGGS")
        : DgHierNdxSystemRFSBase(dggsIn, extModeIntIn, nameIn)
     {
         // all the grids need to be created before we can set the parent/child grids
         // RFs are deallocated by the RFNetwork
         std::vector<DgHierNdxSystemRFBase*> rfGrids(nRes(), nullptr);
         for (int r = 0; r < nRes(); r++)
             rfGrids[r] = new DgHierNdxSystemRF<TINT, TSTR>(*this, r,
                                            nameIn + dgg::util::to_string(r));

         // initializ
         for (int r = 0; r < nRes(); r++)
             rfGrids[r]->initialize(rfGrids);

         // move into our grids_
         for (int r = 0; r < nRes(); r++)
             (*grids_)[r] =
                  static_cast<const DgDiscRF<DgHierNdx, DgQ2DICoord, long long int>*>(rfGrids[r]);
     };

     virtual void initNdxFromInt (DgResAdd<DgHierNdx>& ndx, int res, HIERNDX_INT_TYPE val) const {
         DgHierNdx newAdd (extModeInt());
         DgHierNdxIntCoord ic(val);
         newAdd.setIntNdx(ic);
         this->sysRF(res).setStringFromIntCoord(newAdd);
         ndx.setRes(res);
         ndx.setAddress(newAdd);
     }

     virtual void initNdxFromString (DgResAdd<DgHierNdx>& ndx, int res, const std::string& val) const {
         DgHierNdx newAdd (extModeInt());
         DgHierNdxStringCoord sc(val);
         newAdd.setStrNdx(sc);
         this->sysRF(res).setIntFromStringCoord(newAdd);
         ndx.setRes(res);
         ndx.setAddress(newAdd);
      }

     // pure virtual functions passed down from above
     virtual DgHierNdxIntCoord toIntCoord (const DgHierNdxStringCoord& c,
                                           int gridRes) const = 0;
     virtual DgHierNdxStringCoord toStringCoord (const DgHierNdxIntCoord& c,
                                                 int gridRes) const = 0;

     virtual void setAddNdxParent (const DgResAdd<DgHierNdx>& add,
                                   DgLocation& parent) const = 0;
     virtual void setAddNdxChildren (const DgResAdd<DgHierNdx>& add,
                                     DgLocVector& children) const = 0;
};

#endif
