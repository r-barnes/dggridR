////////////////////////////////////////////////////////////////////////////////
//
// DgLocList.cpp: DgLocList class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
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
      list<DgLocBase*>::iterator it;
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
   list<DgLocBase*>::iterator it;
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
   list<DgLocBase*>::iterator it;
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
   list<DgLocBase*>::const_iterator it;
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
string 
DgLocList::asString (void) const
{
   string str;
   str = "[[\n";
   list<DgLocBase*>::const_iterator it;
   for (it = begin(); it != end(); it++) str += (*it)->asString();
   str += "]]\n";

   return str;

} // DgLocList::asString

////////////////////////////////////////////////////////////////////////////////
string 
DgLocList::asString (char delimiter) const
{
   string str;
   list<DgLocBase*>::const_iterator it;
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
string 
DgLocList::asAddressString (void) const
{
   string str;
   str = "[[\n";

   list<DgLocBase*>::const_iterator it;
   for (it = begin(); it != end(); it++) 
    str += (*it)->asAddressString();

   str += "]]\n";

   return str;
} // DgLocList::asAddressString

////////////////////////////////////////////////////////////////////////////////
string 
DgLocList::asAddressString (char delimiter) const
{
   string str;

   list<DgLocBase*>::const_iterator it;
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
      list<DgLocBase*>::iterator it;
      for (it = begin(); it != end(); it++) (*it)->convertTo(rfIn);
   }

} // DgLocList::convertTo

////////////////////////////////////////////////////////////////////////////////
void
DgLocList::push_back (DgLocBase* loc)
{ 
   rf().convert(loc); 
   list<DgLocBase*>::push_back(loc); 
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ostream&
operator<< (ostream& stream, const DgLocList& list)
{
   //stream << "[[\n";
   DgLocList::const_iterator it;
   for (it = list.begin(); it != list.end(); it++) stream << **it << "\n";
   stream << "]]\n";

   return stream;

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
