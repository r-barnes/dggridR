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
// DgOutLocTextFile.cpp: DgOutLocTextFile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgOutLocTextFile.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocTextFile::DgOutLocTextFile (const string& fileName, const DgRFBase& rf, 
                    bool isPointFile,  const string& suffix, int precision,
                    DgReportLevel failLevel)
   : DgOutputStream (fileName, suffix, failLevel),
     DgOutLocFile (fileName, rf, isPointFile, failLevel), 
     precision_ (precision)
{
} // DgOutLocTextFile::DgOutLocTextFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
