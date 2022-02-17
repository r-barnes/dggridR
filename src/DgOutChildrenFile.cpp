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
// DgOutChildrenFile.cpp: DgOutChildrenFile class implementation
//
// Version 7.0 - Kevin Sahr, 12/14/14
//
////////////////////////////////////////////////////////////////////////////////

#include "DgOutChildrenFile.h"
#include "DgIDGGBase.h"
#include "DgBoundedIDGG.h"
#include "DgIDGG.h"
#include "DgIDGGS.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutChildrenFile::DgOutChildrenFile (const string& fileName, 
         const string& suffix, DgReportLevel failLevel)
   : DgOutputStream (fileName, suffix, failLevel)
{

} // DgOutChildrenFile::DgOutChildrenFile

////////////////////////////////////////////////////////////////////////////////
DgOutChildrenFile& 
DgOutChildrenFile::insert (const DgIDGGBase& dgg, const DgLocation& center,
           DgLocVector& vec)
{
//cout << "@@@@@ DgOutChildrenFile::insert:" << endl;
//cout << " dgg: " << dgg << endl;
//cout << " center: " << center << endl;
//cout << " vec: " << vec << endl;
   const DgIDGGSBase& dggs = *(dgg.dggs());
   const DgIDGGBase& dggr = dggs.idggBase(dgg.res() + 1);

   unsigned long long int sn = dgg.bndRF().seqNum(center);
   *this << sn;
   for (int i = 0; i < vec.size(); i++) {
      DgLocation tmpLoc(vec[i]);
      dggr.convert(&tmpLoc);
      *this << " " << dggr.bndRF().seqNum(tmpLoc);
   }

   *this << endl;

   return *this;

} // DgOutChildrenFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
