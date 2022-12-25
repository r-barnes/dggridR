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
// DgOutRandPtsText.cpp: DgOutRandPtsText class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <sstream>

#include "DgOutRandPtsText.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutRandPtsText::DgOutRandPtsText (const DgRFBase& rfIn,
        const string& fileNameIn, int precision, DgReportLevel failLevel)
   : DgOutLocTextFile (fileNameIn, rfIn, true, "txt", precision, failLevel)
{
   // test for override of vecAddress
   DgAddressBase* dummy = rfIn.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy)
      DgOutputStream::report("DgOutRandPtsText::DgOutRandPtsText(): RF " + rfIn.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   delete dummy;

   setFormatStr();

} // DgOutRandPtsText::DgOutRandPtsText

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutRandPtsText::insert (const DgDVec2D& pt)
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

} // DgOutLocFile& DgOutRandPtsText::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutRandPtsText::insert (DgLocation& loc, const string* label)
//
// Put the point loc.
//
////////////////////////////////////////////////////////////////////////////////
{
   rf().convert(&loc);

   if (label)
     *this << *label << ",";
   else
     *this << "0,";

   this->insert(rf().getVecLocation(loc));

   return *this;

} // DgOutLocFile& DgOutRandPtsText::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutRandPtsText::insert (DgLocVector&, const string*, const DgLocation*)
//
// Put the polyline vec.
//
////////////////////////////////////////////////////////////////////////////////
{
   DgOutputStream::report("DgOutRandPtsText::insert(DgLocVector): not defined.", DgBase::Fatal);
   return *this;

} // DgOutLocFile& DgOutRandPtsText::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutRandPtsText::insert (DgPolygon&, const string*, const DgLocation*)
//
// Put the polygon poly.
//
////////////////////////////////////////////////////////////////////////////////
{
   DgOutputStream::report("DgOutRandPtsText::insert(DgPolygon): not defined.", DgBase::Fatal);

   return *this;

} // DgOutLocFile& DgOutRandPtsText::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
