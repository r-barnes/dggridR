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
// DgDataFieldBase.cpp: DgDataFieldBase class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgBase.h"
#include "DgDataFieldBase.h"

////////////////////////////////////////////////////////////////////////////////
DgDataFieldBase::~DgDataFieldBase (void)
{
   // virtual destructor

} // DgDataFieldBase::~DgDataFieldBase

////////////////////////////////////////////////////////////////////////////////
#ifdef USE_GDAL
void
DgDataFieldBase::createField (OGRLayer* oLayer) const
{
   OGRFieldDefn *fldDfn = new OGRFieldDefn(name().c_str(), fieldType());
   fldDfn->SetWidth(32);
   if (oLayer->CreateField(fldDfn) != OGRERR_NONE)
      ::report("Creating name field failed.", DgBase::Fatal );
   delete fldDfn;
   fldDfn = NULL;

} // void DgDataFieldBase::createField
#endif
////////////////////////////////////////////////////////////////////////////////
