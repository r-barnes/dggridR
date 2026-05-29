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
// DgZOrderRF.cpp: DgZOrderRF class implementation
//
////////////////////////////////////////////////////////////////////////////////

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <cmath>
#include <climits>
#include <cfloat>
#include <string.h>

#include "DgZOrderRF.h"
#include "DgZOrderSystem.h"

const DgHierNdxIntCoord DgZOrderRF::undefZOrderIntCoord(0xffffffffffffffff);

////////////////////////////////////////////////////////////////////////////////
DgZOrderRF::DgZOrderRF (const DgHierNdxSystemRFBase& sysIn, int resIn, const std::string& nameIn)
    : DgHierNdxIntRF(sysIn, resIn, nameIn)
{
}

////////////////////////////////////////////////////////////////////////////////
const char*
DgZOrderRF::str2add (DgHierNdxIntCoord* add, const char* str, char delimiter) const
{
    char delimStr[2];
    delimStr[0] = delimiter;
    delimStr[1] = '\0';

    char* tmpStr = new char[strlen(str) + 1];
    strcpy(tmpStr, str);
    char* tok = strtok(tmpStr, delimStr);

    // convert to a unit64_t
    uint64_t val = 0;
    if (!tok || !sscanf(tok, "%" PRIx64, &val))
       report("DgZOrderRF::str2add(): invalid ZORDER index", DgBase::Fatal);

    if (!add) add = new DgHierNdxIntCoord();
    add->setValue(val);

    unsigned long offset = strlen(tok) + 1;
    delete[] tmpStr;
    if (offset >= strlen(str)) return 0;
    else return &str[offset];

} // const char* DgZOrderRF::str2add

////////////////////////////////////////////////////////////////////////////////
DgHierNdxIntCoord
DgZOrderRF::quantify (const DgQ2DICoord& point) const
{
   // use string form to quantify
   DgHierNdxStringCoord strC = system().pStrRF()->quantify(point);
   DgHierNdxIntCoord c = system().toIntCoord(strC);

   return c;
}

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord
DgZOrderRF::invQuantify (const DgHierNdxIntCoord& add) const
{
   // via string form
   DgHierNdxStringCoord strC = system().toStringCoord(add);
   DgQ2DICoord q = system().pStrRF()->invQuantify(strC);
   return q;
}

////////////////////////////////////////////////////////////////////////////////
