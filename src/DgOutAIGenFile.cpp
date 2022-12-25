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
// DgOutAIGenFile.cpp: DgOutAIGenFile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <sstream>

#include "DgOutAIGenFile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutAIGenFile::DgOutAIGenFile (const DgRFBase& rfIn, const string& fileNameIn,
                          int precision, bool isPointFile,
                          DgReportLevel failLevel)
   : DgOutLocTextFile (fileNameIn, rfIn, isPointFile, "gen", precision,
                       failLevel)
{
   // test for override of vecAddress
   DgAddressBase* dummy = rfIn.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy)
      DgOutputStream::report("DgOutAIGenFile::DgOutAIGenFile(): RF " + rfIn.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   delete dummy;

   setFormatStr();

} // DgOutAIGenFile::DgOutAIGenFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutAIGenFile::insert (const DgDVec2D& pt)
//
// Put the point pt.
//
////////////////////////////////////////////////////////////////////////////////
{
   const int maxBuffSize = 200;
   char buff[maxBuffSize];

   snprintf(buff, maxBuffSize, formatStr(), pt.x(), pt.y());

   *this << buff;

   return *this;

} // DgOutLocFile& DgOutAIGenFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutAIGenFile::insert (DgLocation& loc, const string* label)
//
// Put the point loc.
//
////////////////////////////////////////////////////////////////////////////////
{
   rf().convert(&loc);

   if (label)
     *this << *label << " ";
   else
     *this << "0 ";

   this->insert(rf().getVecLocation(loc));

   return *this;

} // DgOutLocFile& DgOutAIGenFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutAIGenFile::insert (DgLocVector& vec, const string* label,
                     const DgLocation* cent)
//
// Put the polyline vec.
//
////////////////////////////////////////////////////////////////////////////////
{
   rf().convert(vec);

   // output the header line
   if (label)
     *this << *label;
   else
     *this << "0";

   if (cent)
   {
      *this << " ";
      this->insert(rf().getVecLocation(*cent));
   }
   else
      *this << "\n";

   // output the vertices
   const vector<DgAddressBase*>& v = vec.addressVec();
   for (unsigned long i = 0; i < v.size(); i++)
      this->insert(rf().getVecAddress(*v[i]));

   *this << "END" << endl;

   return *this;

} // DgOutLocFile& DgOutAIGenFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutAIGenFile::insert (DgPolygon& poly, const string* label,
                     const DgLocation* cent)
//
// Put the polygon poly.
//
////////////////////////////////////////////////////////////////////////////////
{
   rf().convert(poly);

   // output the header line
   if (label)
     *this << *label;
   else
     *this << "0";

   if (cent)
   {
      *this << " ";
      this->insert(rf().getVecLocation(*cent));
   }
   else
      *this << "\n";

   // output the vertices in reverse order (clockwise winding)
   const vector<DgAddressBase*>& v = poly.addressVec();
   for (int i = (int) (v.size() - 1); i >= 0; i--)
      this->insert(rf().getVecAddress(*v[i]));
 
   // rewrite the first vertex
   this->insert(rf().getVecAddress(*v[v.size() - 1]));

   *this << "END" << endl;

   return *this;

} // DgOutLocFile& DgOutAIGenFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
