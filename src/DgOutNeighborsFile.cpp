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
// DgOutNeighborsFile.cpp: DgOutNeighborsFile class implementation
//
// Version 7.0 - Kevin Sahr, 12/14/14
//
////////////////////////////////////////////////////////////////////////////////

#include "DgOutNeighborsFile.h"
#include "DgIDGGBase.h"
#include "DgBoundedIDGG.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutNeighborsFile::DgOutNeighborsFile (const std::string& fileName,
         const DgIDGGBase& dgg, const DgRFBase* outRF, const std::string& suffix,
         DgReportLevel failLevel)
   : DgOutputStream (fileName, suffix, failLevel), dgg_ (dgg), outRF_ (outRF)
{

} // DgOutNeighborsFile::DgOutNeighborsFile

////////////////////////////////////////////////////////////////////////////////
DgOutNeighborsFile&
DgOutNeighborsFile::insert (const DgLocation& center, DgLocVector& vec)
{
   if (!outRF_) { // indicates seqnum output
      unsigned long long int sn = dgg_.bndRF().seqNum(center);
      *this << sn;
      for (int i = 0; i < vec.size(); i++)
         *this << " " << dgg_.bndRF().seqNum(vec[i]);
   } else {
      DgLocation tmpLoc(center);
      outRF_->convert(&tmpLoc);
      *this << tmpLoc.asString(' ');
      outRF_->convert(&vec);
      for (int i = 0; i < vec.size(); i++)
         *this << " " << vec[i].asString(' ');
   }

   *this << std::endl;

   return *this;

} // DgOutNeighborsFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
