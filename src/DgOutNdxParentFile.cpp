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
// DgOutNdxParentFile.cpp: DgOutNdxParentFile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgOutNdxParentFile.h"
#include "DgIDGGBase.h"
#include "DgBoundedIDGG.h"
#include "DgIDGG.h"
#include "DgIDGGS.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
DgOutNdxParentFile::DgOutNdxParentFile (const std::string& fileName,
                        const DgIDGGBase& dgg,
                        const DgIDGGBase& ndxPrtDgg,
                        const DgRFBase* outRF,
                        const DgRFBase* ndxPrtOutRF,
                        const std::string& suffix,
                        DgReportLevel failLevel)
   : DgOutputStream (fileName, suffix, failLevel), dgg_ (dgg), ndxPrtDgg_ (ndxPrtDgg),
outRF_ (outRF), ndxPrtOutRF_ (ndxPrtOutRF)
{

} // DgOutNdxParentFile::DgOutNdxParentFile

////////////////////////////////////////////////////////////////////////////////
DgOutNdxParentFile&
DgOutNdxParentFile::insert (const DgLocation& center, DgLocation& parent)
{
//cout << "@@@@@ DgOutNdxParentFile::insert:" << std::endl;
//cout << " dgg: " << dgg << std::endl;
//cout << " center: " << center << std::endl;
//cout << " parent: " << parent << std::endl;
/*
 if (!outRF_) { // indicates seqnum output
    unsigned long long int sn = dgg_.bndRF().seqNum(center);
    *this << sn;
    for (int i = 0; i < vec.size(); i++) {
       DgLocation tmpLoc(vec[i]);
       chdDgg_.convert(&tmpLoc);
       *this << " " << chdDgg_.bndRF().seqNum(tmpLoc);
    }
 } else {
    DgLocation tmpLoc(center);
    outRF_->convert(&tmpLoc);
    *this << tmpLoc.asString(' ');
    chdOutRF_->convert(&vec);
    for (int i = 0; i < vec.size(); i++)
       *this << " " << vec[i].asString(' ');
 }
 */
   if (!outRF_) { // indicates seqnum output
      unsigned long long int sn = dgg_.bndRF().seqNum(center);
      *this << sn;
      DgLocation tmpLoc(parent);
       ndxPrtDgg_.convert(&tmpLoc);
       *this << " " << ndxPrtDgg_.bndRF().seqNum(tmpLoc);
   } else {
      DgLocation tmpLoc(center);
      outRF_->convert(&tmpLoc);
      *this << tmpLoc.asString(' ');
      ndxPrtOutRF_->convert(&parent);
      *this << " " << parent.asString(' ');
   }

   *this << std::endl;

   return *this;

} // DgOutNdxParentFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
