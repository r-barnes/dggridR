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
DgOutChildrenFile::DgOutChildrenFile (const std::string& fileName,
                    const DgIDGGBase& dgg, const DgIDGGBase& chdDgg,
                    const DgRFBase* outRF, const DgRFBase* chdOutRF,
                    const std::string& suffix, DgReportLevel failLevel)
   : DgOutputStream (fileName, suffix, failLevel), dgg_ (dgg), chdDgg_ (chdDgg),
                     outRF_ (outRF), chdOutRF_ (chdOutRF)
{

} // DgOutChildrenFile::DgOutChildrenFile

////////////////////////////////////////////////////////////////////////////////
DgOutChildrenFile&
DgOutChildrenFile::insert (const DgLocation& center, DgLocVector& vec)
{
//cout << "@@@@@ DgOutChildrenFile::insert:" << std::endl;
//cout << " dgg: " << dgg << std::endl;
//cout << " center: " << center << std::endl;
//cout << " vec: " << vec << std::endl;

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

   *this << std::endl;

   return *this;

} // DgOutChildrenFile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
