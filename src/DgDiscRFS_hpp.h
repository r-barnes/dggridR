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
// DgDiscRFS_hpp.h: DgDiscRFS template class definition.
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setAddNeighbors (const DgResAdd<A>& add, 
                                            DgLocVector& vec) const
{
   grids()[add.res()]->convert(vec);
   grids()[add.res()]->setAddNeighbors(add.address(), vec);
   this->convert(vec);

} // void DgDiscRFS<A, B, DB>::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setParents (int res, const DgLocation& loc, 
                                                DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);

   if (res > 0 && res < nRes())
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddParents(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setParents

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setInteriorChildren (int res, const DgLocation& loc, 
                                   DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);

   if (res >= 0 && res < (nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddInteriorChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setInteriorChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setBoundaryChildren (int res, const DgLocation& loc, 
                                   DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec); 
   if (res >= 0 && res < (nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddBoundaryChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setBoundary2Children (int res, const DgLocation& loc, 
                                   DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec); 
   if (res >= 0 && res < (nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddBoundary2Children(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setAllChildren (int res, const DgLocation& loc, 
                              DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);

   if (res >= 0 && res < (nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddAllChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> const char*
DgDiscRFS<A, B, DB>::str2add (DgResAdd<A>* add, const char* str,
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
      ::report("DgDiscRFS<A, B, DB>::str2add() invalid res string " +
               string(tok), DgBase::Fatal);
   }

   // now get the address

   const char* tmp = &(str[strlen(tok) + 1]);
   DgLocation tloc(*grids()[res]);
   tmp = tloc.fromString(tmp, delimiter);
   const A& subAdd = *grids()[res]->getAddress(tloc);
   *add = DgResAdd<A>(subAdd, res);

   return tmp;

} // const char* DgDiscRFS::str2add

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
