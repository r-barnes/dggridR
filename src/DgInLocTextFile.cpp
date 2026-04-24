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
// DgInLocTextFile.cpp: DgInLocTextFile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgInLocTextFile.h"
#include "DgLocation.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgContCartRF.h"
#include "DgEllipsoidRF.h"
#include "DgGeoSphRF.h"
#include "DgDataList.h"
#include "DgDataField.h"

#include <sstream>

////////////////////////////////////////////////////////////////////////////////
DgInLocTextFile::DgInLocTextFile (const DgRFBase& rfIn, const std::string* fileNameIn,
                              DgReportLevel failLevel)
    : DgInLocStreamFile (rfIn, fileNameIn, false, failLevel),
      forcePolyLine_ (false), forceCells_ (false)
{
   // create lat/lon rf (may be NULL)
   degRF_ = dynamic_cast<const DgGeoSphDegRF*>(&rfIn);

} // DgInLocTextFile::DgInLocTextFile

////////////////////////////////////////////////////////////////////////////////
DgInLocTextFile::~DgInLocTextFile (void)
{

} // DgInLocTextFile::~DgInLocTextFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInLocTextFile::extract (DgCell& cell)
//
// Get the next cell from me and put it in cell.
//
////////////////////////////////////////////////////////////////////////////////
{
   // cell label and region will be empty
   // need to check for name field below and use for label
   cell.setLabel("");
   cell.setRegion(nullptr);

   // get the cell node
   DgLocation point;
   //extractPointGeometry(point); // should set oFeature_
   cell.setNode(point);

   // get any other data fields
   DgDataList* data = nullptr;

      // need to grab the data fields
      data = new DgDataList();
/*
          if (cellFld)
             data->list_.push_back(cellFld);
*/
   cell.setDataList(data);

   return *this;

} // DgInLocTextFile& DgInLocTextFile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInLocTextFile::extractPointGeometry (DgLocation& point) {

    return *this;

} // DgInLocFile& DgInLocTextFile::extractPointGeometry

////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInLocTextFile::extract (DgLocation& point)
//
// Get the next point DgLocation. The input location must be a lat/lon coordinate.
//
////////////////////////////////////////////////////////////////////////////////
{
   extractPointGeometry(point);
   return *this;

} // DgInLocFile& DgInLocTextFile::extract

////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInLocTextFile::extract (DgLocationData& point)
//
// Get the next point DgLocation with possible data fields. The input location
// must be a lat/lon coordinate.
//
////////////////////////////////////////////////////////////////////////////////
{
   extractPointGeometry(point);
   return *this;

} // DgInLocFile& DgInLocTextFile::extract

////////////////////////////////////////////////////////////////////////////////
