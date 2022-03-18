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
// DgPhysicalRF_hpp.h: DgPhysicalRF template class definition.
//
////////////////////////////////////////////////////////////////////////////////

template<class A, class C> void
DgPhysicalRF<A, C>::deleteContents (const DgLocation& loc, bool convert)
{
   DgLocation* pLoc = discRF().createLocation(loc, convert);
   const A* add = discRF().getAddress(*pLoc);

   if (!boundedRF().validAddress(*add))
   {
      report("DgPhysicalRF<A, C>::deleteContents() invalid address",
             DgBase::Fatal);
   }

   deleteAddContents(*add);

   delete pLoc;

} // void DgPhysicalRF<A, C>::deleteContents

////////////////////////////////////////////////////////////////////////////////
template<class A, class C> C*
DgPhysicalRF<A, C>::getContents (const DgLocation& loc, bool convert,
                                       bool allocate) const
{
   DgLocation* pLoc = discRF().createLocation(loc, convert);
   const A* add = discRF().getAddress(*pLoc);

   if (!boundedRF().validAddress(*add)) return (C*) 0;
/*
   {
      report("DgPhysicalRF<A, C>::getContents() invalid address",
             DgBase::Fatal);
   }
*/

   C* cont = getAddContents(*add, allocate);

   delete pLoc;

   return cont;

} // C* DgPhysicalRF<A, C>::getContents

////////////////////////////////////////////////////////////////////////////////
template<class A, class C> void
DgPhysicalRF<A, C>::replaceContents (const DgLocation& loc, C* cont, 
                                        bool convert)
{
   DgLocation* pLoc = discRF().createLocation(loc, convert);
   const A* add = discRF().getAddress(*pLoc);

   if (!boundedRF().validAddress(*add))
   {
      report("DgPhysicalRF<A, C>::replaceContents() invalid address",
             DgBase::Fatal);
   }

   replaceAddContents(*add, cont);

   delete pLoc;

} // void DgPhysicalRF<A, C>::setContents

////////////////////////////////////////////////////////////////////////////////
template<class A, class C> void
DgPhysicalRF<A, C>::setContents (const DgLocation& loc, const C& cont, 
                                    bool convert)
{
   DgLocation* pLoc = discRF().createLocation(loc, convert);
   const A* add = discRF().getAddress(*pLoc);

   if (!boundedRF().validAddress(*add))
   {
      report("DgPhysicalRF<A, C>::setContents() invalid address",
             DgBase::Fatal);
   }

   setAddContents(*add, cont);

   delete pLoc;

} // void DgPhysicalRF<A, C>::setContents

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
