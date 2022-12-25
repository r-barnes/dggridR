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
// DgLocVector.cpp: DgLocVector class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgLocVector.h"

////////////////////////////////////////////////////////////////////////////////
DgLocVector::DgLocVector (const DgRFBase& rfIn, int sizeIn)
   : DgLocBase (rfIn)
{
   vec_.resize(sizeIn);

   for (long long int i = 0; i < size(); i++) 
     vec_[i] = rf().createAddress();

} // DgLocVector::DgLocVector

////////////////////////////////////////////////////////////////////////////////
void
DgLocVector::clearAddress (void)
{
   tmpLoc_.address_ = 0;

   for (long long int i = 0; i < size(); i++) {
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
DgLocVector::setLoc (int ndx, const DgLocation& loc, bool /* conv */)
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
DgLocVector::push_back (const DgLocation& loc, bool /* conv */)
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
