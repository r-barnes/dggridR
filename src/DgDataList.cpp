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
// DgDataList.cpp: DgDataList class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgDataList.h"

////////////////////////////////////////////////////////////////////////////////
DgDataList::~DgDataList (void)
{
   clearList();

} // DgDataList::~DgDataList

////////////////////////////////////////////////////////////////////////////////
void
DgDataList::clearList (void) {

   for (auto& it : list_)
      delete it;

   list_.clear();
   map_.clear();

} // void DgDataList::clearList

////////////////////////////////////////////////////////////////////////////////
#ifdef USE_GDAL
void
DgDataList::createFields (OGRLayer* oLayer) const
{
   for (auto& it : list_)
      it->createField(oLayer);

} // void DgDataList::createFields

////////////////////////////////////////////////////////////////////////////////
void
DgDataList::setFields (OGRFeature* feature) const
{
   for (auto& it : list_)
      it->setField(feature);

} // void DgDataList::setFields
#endif
////////////////////////////////////////////////////////////////////////////////
