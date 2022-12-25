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
// DgBoundedRF_hpp.h: DgBoundedRF template class definition.
//
////////////////////////////////////////////////////////////////////////////////

template<class A, class B, class DB>
DgBoundedRF<A, B, DB>::DgBoundedRF (const DgDiscRF<A, B, DB>& rfIn,
      const A& firstAddIn, const A& lastAddIn, const A& endAddIn, bool zBasedIn)
   : DgBoundedRFBase<B, DB> (rfIn, rfIn.makeLocation(firstAddIn),
     rfIn.makeLocation(lastAddIn), rfIn.makeLocation(endAddIn), zBasedIn),
     discRF_(rfIn),
     firstAdd_(firstAddIn), lastAdd_(lastAddIn), endAdd_(endAddIn)
{

} // DgBoundedRF<A, B, DB>::DgBoundedRF

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> bool
DgBoundedRF<A, B, DB>::validLocation (const DgLocation& loc, bool /* convert */) const
{
   return validAddress(*discRF().getAddress(loc));

} // bool DgBoundedRF<A, B, DB>::validLocation

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> DgLocation&
DgBoundedRF<A, B, DB>::incrementLocation (DgLocation& loc, bool /* convert */) const
{
   const A* add = discRF().getAddress(loc);

   incrementAddress(const_cast<A&>(*add)); // cheat for speed

   return loc;

} // DgLocation& DgBoundedRF<A, B, DB>::incrementLocation

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> DgLocation&
DgBoundedRF<A, B, DB>::decrementLocation (DgLocation& loc, bool /* convert */) const
{
   const A* add = discRF().getAddress(loc);

   decrementAddress(const_cast<A&>(*add)); // cheat for speed

   return loc;

} // DgLocation& DgBoundedRF<A, B, DB>::decrementLocation

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> unsigned long long int
DgBoundedRF<A, B, DB>::seqNum (const DgLocation& loc, bool /* convert */) const
{
   const A* add = discRF().getAddress(loc);

   return seqNumAddress(*add);

} // unsigned long long int DgBoundedRF<A, B, DB>::seqNum

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> DgLocation*
DgBoundedRF<A, B, DB>::locFromSeqNum (unsigned long long int sNum) const
{
   A add = addFromSeqNum(sNum);

   return discRF().makeLocation(add);

} // DgLocation* DgBoundedRF<A, B, DB>::locFromSeqNum

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
