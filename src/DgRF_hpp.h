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
// DgRF_hpp.h: DgRF template class definition.
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgBase.h"
#include "DgConverter.h"
#include "DgDistance.h"
#include "DgLocation.h"
#include "DgLocVector.h"
// USE_NUCELL is set in MakeIncludes
#ifdef USE_NUCELL
#include <nulib/NuCell.h>
#endif

////////////////////////////////////////////////////////////////////////////////
template<class A, class D>
DgRF<A, D>::~DgRF (void)
{
   // virtual destructor

} // DgRF::~DgRF

////////////////////////////////////////////////////////////////////////////////
template<class A, class D> DgLocation*
DgRF<A, D>::createLocation (const DgLocation& loc, bool conv) const
{
   if (loc.rf() == *this)
   {
      return new DgLocation(loc);
   }
   else if (loc.rf().network() != network())
   {
      report("DgRF<A, D>::getConverter() location not in this network",
             DgBase::Fatal);
      return 0;
   }
   else if (conv)
   {
      DgLocation* newLoc = new DgLocation(loc);
      convert(newLoc);

      return newLoc;
   }
   else
   {
      report("DgRF<A, D>::getConverter() location not from this rf"
             " and conversion not specified",
             DgBase::Fatal);
      return 0;
   }

} // DgLocation* DgRF<A>::createLocation

// USE_NUCELL is set in MakeIncludes
#ifdef USE_NUCELL
////////////////////////////////////////////////////////////////////////////////
template<class A, class D> NuCell*
DgRF<A, D>::createCell (const NuCell& cell, bool conv) const
{
   if (cell.rf() == *this)
   {
      return new NuCell(cell);
   }
   else if (cell.rf().network() != network())
   {
      report("DgRF<A, D>::getConverter() cell not in this network",
             DgBase::Fatal);
      return 0;
   }
   else if (conv)
   {
      NuCell* newCell = new NuCell(cell);
      convert(newCell);

      return newCell;
   }
   else
   {
      report("DgRF<A, D>::getConverter() cell not from this rf"
             " and conversion not specified",
             DgBase::Fatal);
      return 0;
   }

} // NuCell* DgRF<A>::createCell
#endif

////////////////////////////////////////////////////////////////////////////////
template<class A, class D> DgLocation*
DgRF<A, D>::makeLocation (const A& addIn) const
{
   return buildLocation(new DgAddress<A>(addIn));
} // DgLocation* DgRF<A>::makeLocation

////////////////////////////////////////////////////////////////////////////////
template<class A, class D> const A*
DgRF<A, D>::getAddress (const DgLocation& loc) const
{
   if (loc.rf() != *this)
   {
      dgcerr << "ABORTING *this: " << *this << " loc: " << loc << endl;
      report("DgRF<A, D>::getAddress() location not from this rf",
             DgBase::Fatal);
      return 0;
   }

   if (loc.address())
   {
      return (&(static_cast<const DgAddress<A>*>(loc.address()))->address());
   }
   else
   {
      return 0;
   }

} // const A* DgRF<A>::getAddress

////////////////////////////////////////////////////////////////////////////////
// deletes old address
template<class A, class D> void
DgRF<A, D>::forceAddress (DgLocation* loc, const A& addIn) const {
   if (loc->address_)
      loc->clearAddress();
   loc->address_ = new DgAddress<A>(addIn);
}

////////////////////////////////////////////////////////////////////////////////
template<class A, class D> void
DgRF<A, D>::buildLocVector (DgLocVector* vec, const vector<A*> inVec) const
{
   vec->clearAddress();
   vec->resize(inVec.size());
   for (unsigned int i = 0; i < vec->size(); i++)
   {
      vec->vec_[i] = new DgAddress<A>(*inVec[i]);
   }

} // void DgRF<A, D>::buildLocVector

////////////////////////////////////////////////////////////////////////////////
template<class A, class D> DgDistanceBase*
DgRF<A, D>::distance (const DgLocation& loc1, const DgLocation& loc2,
                            bool conv) const
{
   if (loc1.rf() == *this && loc2.rf() == *this)
   {
      const A* add1 = getAddress(loc1);
      const A* add2 = getAddress(loc2);
      if (!add1 || !add2)
      {
         report("DgRF<A, D>::distance() null address", DgBase::Fatal);
         return 0;
      }

      return new DgDistance<D>(*this, dist(*add1, *add2));
   }
   else if (loc1.rf().network() != network() ||
            loc2.rf().network() != network())
   {
      report("DgRF<A, D>::distance() location not in this network",
             DgBase::Fatal);
      return 0;
   }
   else if (conv)
   {
      A add1;
      A add2;

      if (loc1.rf() != *this)
      {
         DgLocation* pLoc1 = createLocation(loc1);
         convert(pLoc1);
         const A* pAdd1 = getAddress(*pLoc1);
         if (!pAdd1)
         {
            report("DgRF<A, D>::distance() null address", DgBase::Fatal);
            return 0;
         }
         add1 = *pAdd1;
         delete pLoc1;
      }

      if (loc2.rf() != *this)
      {
         DgLocation* pLoc2 = createLocation(loc2);
         convert(pLoc2);
         const A* pAdd2 = getAddress(*pLoc2);
         if (!pAdd2)
         {
            report("DgRF<A, D>::distance() null address", DgBase::Fatal);
            return 0;
         }
         add2 = *pAdd2;
         delete pLoc2;
      }

      return new DgDistance<D>(*this, dist(add1, add2));
   }
   else
   {
      report("DgRF<A, D>::distance() location not from this rf"
             " and conversion not specified", DgBase::Fatal);
      return 0;
   }

} // DgDistanceBase* DgRF<A, D>::distance

////////////////////////////////////////////////////////////////////////////////
template<class A, class D> D
DgRF<A, D>::getDistance (const DgDistanceBase& dist) const
{
   D d;
   if (dist.rf() != *this)
   {
      report("DgRF<A, D>::getDistance() distance not from this rf",
             DgBase::Fatal);
   }

   return ((static_cast<const DgDistance<D>&>(dist)).distance());

} // D DgRF<A, D>::getDistance

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toString (const DgLocation& loc) const
{
   string str;

   if (loc.rf() != *this)
   {
      report("DgRF<A, D>::toString(" + loc.asString() +
             ") location not from this rf", DgBase::Fatal);
      return str;
   }

   str = name() + "{";

   const A* add = getAddress(loc);
   if (add)
   {
      str += add2str(*add) + "}";
   }
   else
   {
      str += "NULL}";
   }

   return str;

} // string DgRF<A, D>::toString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toString (const DgLocation& loc, char delimiter) const
{
   string str;

   if (loc.rf() != *this)
   {
      report("DgRF<A, D>::toString(" + loc.asString() +
             ") location not from this rf", DgBase::Fatal);
      return str;
   }

   const A* add = getAddress(loc);
   if (add)
   {
      str = add2str(*add, delimiter);
   }
   else
   {
      str = "NULL";
   }

   return str;

} // string DgRF<A, D>::toString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> const char*
DgRF<A, D>::fromString (DgLocation& loc, const char* str, char delimiter) const
{
   A add;
   const char* tmp = str2add(&add, str, delimiter);
   if (add == undefAddress())
   {
      ::report("DgRF<A, D>::fromString() invalid address string " + string(str),
               DgBase::Fatal);
   }

   DgLocation* tloc = makeLocation(add);
   loc = *tloc;
   delete tloc;

   return tmp;

} // const char* DgRF<A, D>::fromString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toString (const DgLocVector& locVec) const
{
   string str;

   if (locVec.rf() != *this)
   {
      report("DgRF<A, D>::toString(" + locVec.asString() +
             ") location vector not from this rf", DgBase::Fatal);
      return str;
   }

   str = name() + "{\n";
   const vector<DgAddressBase*>& v = locVec.addressVec();
   for (unsigned int i = 0; i < v.size(); i++)
   {
      const A& add = (static_cast<const DgAddress<A>*>(v[i]))->address();
      str += add2str(add) + "\n";
   }
   str += "}";

   return str;

} // string DgRF<A, D>::toString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toString (const DgLocVector& locVec, char delimiter) const
{
   string str;

   if (locVec.rf() != *this)
   {
      report("DgRF<A, D>::toString(" + locVec.asString() +
             ") location vector not from this rf", DgBase::Fatal);
      return str;
   }

   const vector<DgAddressBase*>& v = locVec.addressVec();
   for (unsigned int i = 0; i < v.size(); i++)
   {
      const A& add = (static_cast<const DgAddress<A>*>(v[i]))->address();
      str += add2str(add, delimiter) + "\n";
   }

   return str;

} // string DgRF<A, D>::toString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toAddressString (const DgLocation& loc) const
{
   string str;

   if (loc.rf() != *this)
   {
      report("DgRF<A, D>::toAddressString(" + loc.asString() +
             ") location not from this rf", DgBase::Fatal);
      return str;
   }

   const A* add = getAddress(loc);
   if (add)
   {
      str = add2str(*add);
   }
   else
   {
      str = "(NULL)";
   }

   return str;

} // string DgRF<A, D>::toAddressString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toAddressString (const DgLocation& loc, char delimiter) const
{
   string str;

   if (loc.rf() != *this)
   {
      report("DgRF<A, D>::toAddressString(" + loc.asString() +
             ") location not from this rf", DgBase::Fatal);
      return str;
   }

   const A* add = getAddress(loc);
   if (add)
   {
      str = add2str(*add, delimiter);
   }
   else
   {
      str = "NULL";
   }

   return str;

} // string DgRF<A, D>::toAddressString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toAddressString (const DgLocVector& locVec) const
{
   string str;

   if (locVec.rf() != *this)
   {
      report("DgRF<A, D>::toAddressString(" + locVec.asString() +
             ") location vector not from this rf", DgBase::Fatal);
      return str;
   }

   str = "{\n";
   const vector<DgAddressBase*>& v = locVec.addressVec();
   for (unsigned int i = 0; i < v.size(); i++)
   {
      const A& add = (static_cast<const DgAddress<A>*>(v[i]))->address();
      str += add2str(add) + "\n";
   }
   str += "}";

   return str;

} // string DgRF<A, D>::toAddressString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toAddressString (const DgLocVector& locVec, char delimiter) const
{
   string str;

   if (locVec.rf() != *this)
   {
      report("DgRF<A, D>::toAddressString(" + locVec.asString() +
             ") location vector not from this rf", DgBase::Fatal);
      return str;
   }

   const vector<DgAddressBase*>& v = locVec.addressVec();
   for (unsigned int i = 0; i < v.size(); i++)
   {
      const A& add = (static_cast<const DgAddress<A>*>(v[i]))->address();
      str += add2str(add, delimiter) + "\n";
   }

   return str;

} // string DgRF<A, D>::toAddressString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> string
DgRF<A, D>::toString (const DgDistanceBase& dist) const
{
   string str;

   if (dist.rf() != *this)
   {
      report("DgRF<A, D>::toString(" + dist.asString() +
             ") distance not from this rf", DgBase::Fatal);
      return str;
   }

   str = name() + "{" +
         dist2str((static_cast<const DgDistance<D>&>(dist)).distance()) + "}";

   return str;

} // string DgRF<A, D>::toString

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> long double
DgRF<A, D>::toDouble (const DgDistanceBase& dist) const
{
   if (dist.rf() != *this)
   {
      report("DgRF<A, D>::toDouble(" + dist.asString() +
             ") distance not from this rf", DgBase::Fatal);
      return -1;
   }

   return dist2dbl((static_cast<const DgDistance<D>&>(dist)).distance());

} // long double DgRF<A, D>::toDouble

////////////////////////////////////////////////////////////////////////////////
template <class A, class D>
unsigned long long int
DgRF<A, D>::toInt (const DgDistanceBase& dist) const
{
   if (dist.rf() != *this)
   {
      report("DgRF<A, D>::toInt(" + dist.asString() +
             ") distance not from this rf", DgBase::Fatal);
      return 0;
   }

   return dist2int((static_cast<const DgDistance<D>&>(dist)).distance());

} // unsigned long long int DgRF<A, D>::toInt

////////////////////////////////////////////////////////////////////////////////
template <class A, class D> void
DgRF<A, D>::jumpRF (DgLocation* loc)
{
   const DgRF<A, D>* newRF = dynamic_cast<const DgRF<A, D>*>(&loc->rf());
   if (!newRF)
   {
      report("DgRF<A, D>::jumpTo(" + loc->asString() +
             ") does not match type of " + name(), DgBase::Fatal);
   }

   forceRF(loc);

} // void DgRF<A, D>::jumpRF

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
