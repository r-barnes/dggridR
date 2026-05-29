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
// DgLocList.cpp: DgLocList class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>

#include "DgLocation.h"
#include "DgLocList.h"
#include "DgRFBase.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgLocList::~DgLocList (void)
{
   if (isOwner()) destroy();
   clear();

} // DgLocList::~DgLocList

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgLocList::destroy (void)
{
   if (isOwner())
   {
      std::list<DgLocBase*>::iterator it;
      for (it = begin(); it != end(); it++)
      {
         (*it)->clearAddress();
         delete (*it);
      }
   }

   clear();

} // DgLocList::destroy

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgLocList::clearAddress (void)
{
   std::list<DgLocBase*>::iterator it;
   for (it = begin(); it != end(); it++)
   {
      (*it)->clearAddress();
   }
   clear();

} // DgLocList::clearAddress

/*
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
DgLocList::thin (float dMin, bool clear)
//
// Apply the appropriate line reduction algorithms to all the drawables.
// Returns true if some portion of this object still remains after the thinning
// and false if the entire object has been thinned away.
//
////////////////////////////////////////////////////////////////////////////////
{
   std::list<DgLocBase*>::iterator it;
   for (it = begin(); it != end(); it++)
   {
      (*it)->thin(dMin, clear);
   }

   return true;

} // bool DgLocList::thin
*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int
DgLocList::cardinality (void) const
//
// Return the total number of elements in this tree.
//
////////////////////////////////////////////////////////////////////////////////
{
   int card = 0;
   std::list<DgLocBase*>::const_iterator it;
   for (it = begin(); it != end(); it++) card += (*it)->cardinality();

   return card;

} // int DgLocList::cardinality

////////////////////////////////////////////////////////////////////////////////
DgLocList&
DgLocList::operator= (const DgLocList& list)
{
   if (&list != this)
   {
      clearAddress();

      rf_ = &list.rf();
      copy(list.begin(), list.end(), this->begin());
   }

   return *this;

} // DgLocList& DgLocList::operator=

////////////////////////////////////////////////////////////////////////////////
/*
bool
DgLocList::operator== (const DgLocList& list) const
{
   if (this == &list) return true;

} // DgLocList::operator==
*/

////////////////////////////////////////////////////////////////////////////////
std::string
DgLocList::asString (void) const
{
   std::string str;
   str = "[[\n";
   std::list<DgLocBase*>::const_iterator it;
   for (it = begin(); it != end(); it++) str += (*it)->asString();
   str += "]]\n";

   return str;

} // DgLocList::asString

////////////////////////////////////////////////////////////////////////////////
std::string
DgLocList::asString (char delimiter) const
{
   std::string str;
   std::list<DgLocBase*>::const_iterator it;
   for (it = begin(); it != end(); it++) str += (*it)->asString(delimiter);

   return str;

} // DgLocList::asString

////////////////////////////////////////////////////////////////////////////////
const char*
DgLocList::fromString (const char* str, char delimiter)
{
   destroy();
   setIsOwner(true);

   const char* tmp = str;
   while (*tmp)
   {
      DgLocation* tloc = new DgLocation(rf());
      tmp = tloc->fromString(tmp, delimiter);
      push_back(tloc);
      if (*tmp == delimiter) tmp++;
   }

   return tmp;

} // DgLocList::fromString

////////////////////////////////////////////////////////////////////////////////
std::string
DgLocList::asAddressString (void) const
{
   std::string str;
   str = "[[\n";

   std::list<DgLocBase*>::const_iterator it;
   for (it = begin(); it != end(); it++)
    str += (*it)->asAddressString();

   str += "]]\n";

   return str;
} // DgLocList::asAddressString

////////////////////////////////////////////////////////////////////////////////
std::string
DgLocList::asAddressString (char delimiter) const
{
   std::string str;

   std::list<DgLocBase*>::const_iterator it;
   for (it = begin(); it != end(); it++)
      str += (*it)->asAddressString(delimiter);

   return str;
} // DgLocList::asAddressString

////////////////////////////////////////////////////////////////////////////////
void
DgLocList::convertTo (const DgRFBase& rfIn)
{
   if (!rf_ || rf() != rfIn)
   {
      rf_ = &rfIn;
      std::list<DgLocBase*>::iterator it;
      for (it = begin(); it != end(); it++) (*it)->convertTo(rfIn);
   }

} // DgLocList::convertTo

////////////////////////////////////////////////////////////////////////////////
void
DgLocList::push_back (DgLocBase* loc)
{
   rf().convert(loc);
   std::list<DgLocBase*>::push_back(loc);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::ostream&
operator<< (std::ostream& stream, const DgLocList& list)
{
   //stream << "[[\n";
   DgLocList::const_iterator it;
   for (it = list.begin(); it != list.end(); it++) stream << **it << "\n";
   stream << "]]\n";

   return stream;

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
