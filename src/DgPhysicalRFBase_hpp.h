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
// DgPhysicalRFBase_hpp.h: DgPhysicalRFBase template class definition.
//
////////////////////////////////////////////////////////////////////////////////

template<class C>
DgPhysicalRFBase<C>::~DgPhysicalRFBase (void)
{
   // virtual destructor

} // DgPhysicalRFBase<C>::~DgPhysicalRFBase<C>

////////////////////////////////////////////////////////////////////////////////
template<class C> void
DgPhysicalRFBase<C>::initAllCells (void)
{
   for (DgLocation loc = boundedRFBase().first(); loc != boundedRFBase().end();
           boundedRFBase().incrementLocation(loc))
   {
      C* c = getContents(loc, false, true);
      if (c) initCell(c, loc);
   }

} // void DgPhysicalRFBase<C>::initAllCells

////////////////////////////////////////////////////////////////////////////////
template<class C> ostream& operator<< (ostream& stream,
                                       const DgPhysicalRFBase<C>& rfIn)
{ 
   return stream << "Phys-" << rfIn.boundedRFBase(); 

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

