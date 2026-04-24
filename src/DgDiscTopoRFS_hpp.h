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
// DgDiscTopoRFS_hpp.h: DgDiscTopoRFS template class definition.
//
////////////////////////////////////////////////////////////////////////////////

#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB>
const char*
DgDiscTopoRFS<A, B, DB>::str2add (DgResAdd<A>* add, const char* str,
                              char delimiter) const
{
   if (!add) add = new DgResAdd<A>();

   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   char* tok;

   // get the resolution

   tok = strtok(tmpStr, delimStr);
   int res;
   if (sscanf(tok, "%d", &res) != 1)
   {
      ::report("DgDiscTopoRFS<A, B, DB>::str2add() invalid res string " +
               std::string(tok), DgBase::Fatal);
   }

   // now get the address

   const char* tmp = &(str[strlen(tok) + 1]);
   DgLocation tloc(*this->grids()[res]);
   tmp = tloc.fromString(tmp, delimiter);
   const A& subAdd = *(this->grids()[res]->getAddress(tloc));
   *add = DgResAdd<A>(subAdd, res);

   return tmp;

} // const char* DgDiscTopoRFS::str2add

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscTopoRFS<A, B, DB>::setAddNeighbors (const DgResAdd<A>& add,
                                            DgLocVector& vec) const
{
   this->grids()[add.res()]->convert(vec);
   topoRF(add.res()).setAddNeighbors(add.address(), vec);
   this->convert(vec);

} // void DgDiscTopoRFS<A, B, DB>::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscTopoRFS<A, B, DB>::setParents (int res, const DgLocation& loc,
                                                DgLocVector& vec) const
{
   vec.clearAddress();
   this->convert(vec);

   if (res > 0 && res < this->nRes())
   {
      DgLocation tmpLoc(loc);
      this->grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddParents(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscTopoRFS::setParents

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscTopoRFS<A, B, DB>::setInteriorChildren (int res, const DgLocation& loc,
                                   DgLocVector& vec) const
{
   vec.clearAddress();
   this->convert(vec);

   if (res >= 0 && res < (this->nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      this->grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddInteriorChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscTopoRFS::setInteriorChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscTopoRFS<A, B, DB>::setBoundaryChildren (int res, const DgLocation& loc,
                                   DgLocVector& vec) const
{
   vec.clearAddress();
   this->convert(vec);
   if (res >= 0 && res < (this->nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      this->grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddBoundaryChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscTopoRFS::setBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscTopoRFS<A, B, DB>::setBoundary2Children (int res, const DgLocation& loc,
                                   DgLocVector& vec) const
{
   vec.clearAddress();
   this->convert(vec);
   if (res >= 0 && res < (this->nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      this->grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddBoundary2Children(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscTopoRFS::setBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscTopoRFS<A, B, DB>::setAllChildren (int res, const DgLocation& loc,
                              DgLocVector& vec) const
{
   vec.clearAddress();
   this->convert(vec);

   if (res >= 0 && res < (this->nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      this->grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddAllChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscTopoRFS::setAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
