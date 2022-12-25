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
// DgOutPRPtsFile.cpp: DgOutPRPtsFile class implementation
//
// Version 7.0 - Kevin Sahr, 11/25/14
//
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <sstream>

#include "DgOutPRPtsFile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutPRPtsFile::DgOutPRPtsFile (const DgRFBase& rfIn,
        const string& fileNameIn, int precision, DgReportLevel failLevel)
   : DgOutLocTextFile (fileNameIn, rfIn, true, "pts", precision, failLevel)
{
   // test for override of vecAddress
   DgAddressBase* dummy = rfIn.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy)
      DgOutputStream::report("DgOutPRPtsFile::DgOutPRPtsFile(): RF " + rfIn.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   delete dummy;

   setFormatStr();

} // DgOutPRPtsFile::DgOutPRPtsFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutPRPtsFile::insert (const DgDVec2D& pt)
//
// Put the point pt.
//
////////////////////////////////////////////////////////////////////////////////
{
   const int maxBuffSize = 200;
   char buff[maxBuffSize];

   // want latitude, longitude
   snprintf(buff, maxBuffSize, formatStr(), pt.y(), pt.x());

   *this << buff;

   return *this;

} // DgOutLocFile& DgOutPRPtsFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutPRPtsFile::insert (DgLocation& loc, const string* label)
//
// Put the point loc.
//
////////////////////////////////////////////////////////////////////////////////
{
   return insert(loc, "H", label);

} // DgOutPRPtsFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutPRPtsFile::insert (DgLocation& loc, const string& type,
                 const string* label)
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

   *this << type << " ";

   this->insert(rf().getVecLocation(loc));

   return *this;

} // DgOutLocFile& DgOutPRPtsFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutPRPtsFile::insert (DgLocVector&, const string*, const DgLocation*)
//
// Put the polyline vec.
//
////////////////////////////////////////////////////////////////////////////////
{
   DgOutputStream::report("DgOutPRPtsFile::insert(DgLocVector): not defined.", DgBase::Fatal);
   return *this;

} // DgOutLocFile& DgOutPRPtsFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutPRPtsFile::insert (DgPolygon&, const string*, const DgLocation*)
//
// Put the polygon poly.
//
////////////////////////////////////////////////////////////////////////////////
{
   DgOutputStream::report("DgOutPRPtsFile::insert(DgPolygon): not defined.", DgBase::Fatal);

   return *this;

} // DgOutLocFile& DgOutPRPtsFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
