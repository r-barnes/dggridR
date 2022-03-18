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
// DgDiscRF_hpp.h: DgDiscRF template class definition.
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setVertices (const DgLocation& loc, DgPolygon& vec) const
{
   vec.clearAddress(); 
   backFrame().convert(vec);
   
   if (loc.rf() == *this)
   {
      setAddVertices(*(this->getAddress(loc)), vec);
   }
   else
   {
      DgLocation tmpLoc(loc);
      this->convert(&tmpLoc);
      setAddVertices(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRF::setVertices

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setVertices (const DgLocation& loc, const DgRFBase& rf,
                                                     DgPolygon& vec) const
{
   setVertices(loc, vec);
   if (rf != backFrame()) backFrame().convert(vec);

} // const DgLocVector& DgDiscRF::setVertices

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setNeighbors (const DgLocation& loc, DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);
   
   if (loc.rf() == *this)
   {
      setAddNeighbors(*(this->getAddress(loc)), vec);
   }
   else
   {
      DgLocation tmpLoc(loc);
      this->convert(&tmpLoc);
      setAddNeighbors(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRF::setNeighbors

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setNeighborsBdry2 (const DgLocation& loc, DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);
   
   if (loc.rf() == *this)
   {
      setAddNeighborsBdry2(*(this->getAddress(loc)), vec);
   }
   else
   {
      DgLocation tmpLoc(loc);
      this->convert(&tmpLoc);
      setAddNeighborsBdry2(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRF::setNeighborsBdry2

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setPoint (const DgLocation& loc, DgLocation& point) const
{
   if (loc.rf() == *this)
   {
      setAddPoint(*(this->getAddress(loc)), point);
   }
   else
   {
      DgLocation tmpLoc(loc);
      this->convert(&tmpLoc);
      setAddPoint(*(this->getAddress(tmpLoc)), point);
   }

} // void DgDiscRF::setPoint

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setPoint (const DgLocation& loc, const DgRFBase& rf,
                                                     DgLocation& point) const
{
   setPoint(loc, point);
   if (rf != backFrame()) backFrame().convert(&point);

} // void DgDiscRF::setPoint

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setAddPoint (const A& add, DgLocation& pt) const
{
   DgLocation* tmpLoc = backFrame().makeLocation(invQuantify(add));

   pt = *tmpLoc;

   delete tmpLoc;

} // void DgDiscRF<A, B, DB>::setPoint

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
