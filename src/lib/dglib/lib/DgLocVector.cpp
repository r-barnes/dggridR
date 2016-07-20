////////////////////////////////////////////////////////////////////////////////
//
// DgLocVector.cpp: DgLocVector class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#include "DgLocVector.h"

////////////////////////////////////////////////////////////////////////////////
DgLocVector::DgLocVector (const DgRFBase& rfIn, int sizeIn)
   : DgLocBase (rfIn)
{
   vec_.resize(sizeIn);

   for (int64_t i = 0; i < size(); i++) 
     vec_[i] = rf().createAddress();

} // DgLocVector::DgLocVector

////////////////////////////////////////////////////////////////////////////////
void
DgLocVector::clearAddress (void)
{
   tmpLoc_.address_ = 0;

   for (int64_t i = 0; i < size(); i++)
   {
      delete vec_[i];
      vec_[i] = 0;
   }

   resize(0);

} // void DgLocVector::clearAddress

////////////////////////////////////////////////////////////////////////////////
bool
DgLocVector::operator== (const DgLocVector& vec) const
{ 
   if (rf() != vec.rf() || vec.size() != size()) 
    return false;

   for (int i = 0; i < size(); i++)
   {
      if (!rf().equalAddress(*vec_[i], *vec[i].address())) 
       return false;
   }

   return true;

} // bool DgLocVector::operator==

////////////////////////////////////////////////////////////////////////////////
void 
DgLocVector::setLoc (int ndx, const DgLocation& loc, bool conv)
{
   if (rf() == loc.rf()) 
   {
      rf().copyAddress(*loc.address(), vec_[ndx]);
      return;
   }

   // otherwise need to convert

   DgLocation newLoc(loc);
   rf().convert(&newLoc);
   rf().copyAddress(*newLoc.address(), vec_[ndx]);

} // void DgLocVector::setLoc

////////////////////////////////////////////////////////////////////////////////
void 
DgLocVector::push_back (const DgLocation& loc, bool conv)
{
   if (rf() == loc.rf()) 
   {
      DgAddressBase* add = rf().createAddress(*loc.address());
      vec_.push_back(add);
      return;
   }

   // otherwise need to convert

   DgLocation newLoc(loc);
   rf().convert(&newLoc);
   DgAddressBase* add = rf().createAddress(*newLoc.address());
   vec_.push_back(add);

} // void DgLocVector::push_back

////////////////////////////////////////////////////////////////////////////////
const char*
DgLocVector::fromString (const char* str, char delimiter)
{
   clearAddress();

   DgLocation tloc(rf());
   const char* tmp = str;
   while (*tmp)
   {
      tmp = tloc.fromString(tmp, delimiter);
      push_back(tloc);  
      if (*tmp == delimiter) 
       tmp++;
   }

   return tmp;

} // DgLocVector::fromString

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
