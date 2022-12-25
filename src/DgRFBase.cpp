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
// DgRFBase.cpp: DgRFBase class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <list>

using namespace std;

#include "DgBase.h"
#include "DgRFBase.h"
#include "DgLocBase.h"
#include "DgLocation.h"
#include "DgLocVector.h"
#include "DgPolygon.h"
#include "DgConverterBase.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgRFBase::~DgRFBase (void)
{
   delete undefLoc_;

} // DgRFBase::~DgRFBase

////////////////////////////////////////////////////////////////////////////////
DgLocation* 
DgRFBase::convert (DgLocation* loc) const
{
   if (loc->rf_ == 0)
   {
      loc->rf_ = this;
      return loc;
   }
   
   if (network() != loc->rf().network())
   {
      report("DgRFBase::convert() from/to network mismatch",
             DgBase::Fatal);
      return 0;
   }

   if (loc->rf() == *this) return loc;

   if (loc->address() == 0)
   {
      loc->rf_ = this;
      return loc;
   }
   
   // if we're here we need to convert

   const DgConverterBase* conv = network().getConverter(loc->rf(), *this);
   if (!conv)
   {
      report("DgRFBase::convert() getConverter error", DgBase::Fatal);
      return 0;
   }

   return conv->convert(loc);

} // DgLocation* DgRFBase::convert

////////////////////////////////////////////////////////////////////////////////
DgPolygon& 
DgRFBase::convert (DgPolygon& poly) const
{
   if (poly.rf_ == 0) {
      poly.rf_ = this;
      return poly;
   }
   
   if (network() != poly.rf().network()) {
      report("DgRFBase::convert() from/to network mismatch",
             DgBase::Fatal);
      return poly;
   }

   if (poly.rf() == *this) return poly;

   if (poly.size() == 0) {
      poly.rf_ = this;
      return poly;
   }
   
   // if we're here we need to convert

   const DgConverterBase* conv = network().getConverter(poly.rf(), *this);
   if (!conv) {
      report("DgRFBase::convert() getConverter error", DgBase::Fatal);
      return poly;
   }

   convert((DgLocVector&) poly);
   if (poly.hasHoles()) {
      for (unsigned long i = 0; i < poly.holes().size(); i++)
         convert(*poly.holes()[i]);
   }

   return poly;

} // DgPolygon& DgRFBase::convert

////////////////////////////////////////////////////////////////////////////////
DgLocVector& 
DgRFBase::convert (DgLocVector& vec) const
{
   if (vec.rf_ == 0) {
      vec.rf_ = this;
      return vec;
   }
   
   if (network() != vec.rf().network()) {
      report("DgRFBase::convert() from/to network mismatch",
             DgBase::Fatal);
      return vec;
   }

   if (vec.rf() == *this) return vec;

   if (vec.size() == 0) {
      vec.rf_ = this;
      return vec;
   }
   
   // if we're here we need to convert

   const DgConverterBase* conv = network().getConverter(vec.rf(), *this);
   if (!conv) {
      report("DgRFBase::convert() getConverter error", DgBase::Fatal);
      return vec;
   }

   vector<DgAddressBase*>& v = vec.addressVec();
   for (unsigned long i = 0; i < v.size(); i++) {
      if (v[i]) { 
         DgAddressBase* addIn = v[i];
         DgAddressBase* newAdd = conv->createConvertedAddress(*addIn);
         v[i] = newAdd;
         delete addIn;
      }
   }

   vec.rf_ = this;
   vec.tmpLoc_.rf_ = this;

   return vec;

} // DgLocVector& DgRFBase::convert

////////////////////////////////////////////////////////////////////////////////
DgLocation* 
DgRFBase::createLocation (void) const
{
   return new DgLocation(*this, 0);

} // DgLocation* DgRFBase::createLocation

////////////////////////////////////////////////////////////////////////////////
DgLocation* 
DgRFBase::buildLocation (DgAddressBase* addIn) const
{
   return new DgLocation(*this, addIn);

} // DgLocation* DgRFBase::buildLocation

////////////////////////////////////////////////////////////////////////////////
ostream&
DgRFBase::traceToGround (ostream& stream) const
{
   stream << "tracing " << name() << " {";

   if (id() == 0) return stream << " at ground }" << endl;

   if (!connectTo()) return stream << " => NULL }" << endl;

   stream << "\n";

   const DgRFBase* to = connectTo();
   while (true)
   {
      if (!to) return stream << "  => NULL }" << endl;

      // at ground?

      if (to->id() == 0)
      {
         stream << "  => at ground: " << to->name() << "\n";
         break;
      }

      // otherwise continue

      stream << "  => " << to->name() << "\n";
      to = to->connectTo();
   }

   // now go back from ground

   list<const DgRFBase*> rev;
   const DgRFBase* from = connectFrom();
   while (true)
   {
      if (!from) return stream << "  => NULL }" << endl;

      // at ground?

      if (from->id() == 0) break;

      // otherwise continue

      rev.push_front(from);
      from = from->connectFrom();
   }

   list<const DgRFBase*>::iterator it = rev.begin();
   for (; it != rev.end(); it++) stream << "  => " << (*it)->name() << "\n";

   return stream << " => " << name() << " }" << endl;  

} // void DgRFBase::traceToGround

////////////////////////////////////////////////////////////////////////////////
string
DgRFBase::toString (const DgLocBase& lb) const
{
   return lb.asString();

} // string DgRFBase::toString

////////////////////////////////////////////////////////////////////////////////
string
DgRFBase::toAddressString (const DgLocBase& lb) const
{
   return lb.asAddressString();

} // string DgRFBase::toAddressString

////////////////////////////////////////////////////////////////////////////////
string
DgRFBase::toString (const DgLocBase& lb, char delimiter) const
{
   return lb.asString(delimiter);

} // string DgRFBase::toString

////////////////////////////////////////////////////////////////////////////////
string
DgRFBase::toAddressString (const DgLocBase& lb, char delimiter) const
{
   return lb.asAddressString(delimiter);

} // string DgRFBase::toAddressString

////////////////////////////////////////////////////////////////////////////////
void 
DgRFBase::setUndefLoc (DgLocation* undefLoc) 
{
   if (undefLoc_) delete undefLoc_;
   undefLoc_ = undefLoc;
} // void DgRFBase::setUndefLoc

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

