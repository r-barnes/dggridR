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
// DgHierNdxIntRF.cpp: DgHierNdxIntRF class implementation
//
////////////////////////////////////////////////////////////////////////////////

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <cmath>
#include <climits>
#include <cstdint>
#include <cfloat>
#include <string.h>

#include "DgHierNdxIntRF.h"

////////////////////////////////////////////////////////////////////////////////
const DgHierNdxIntCoord DgHierNdxIntRF::undefCoord(UINT64_MAX);

////////////////////////////////////////////////////////////////////////////////
DgHierNdxIntCoord::DgHierNdxIntCoord (void)
  : DgHierNdxCoord<HIERNDX_INT_TYPE>(DgHierNdxIntRF::undefCoord.value())
{ }

////////////////////////////////////////////////////////////////////////////////
std::string
DgHierNdxIntCoord::valString (void) const {
   const int maxStrSize = 17; // max 16 digits plus 1 for the null terminator
   char str[maxStrSize];
   snprintf(str, maxStrSize, "%016" PRIx64, value());

   return std::string(str);
}

////////////////////////////////////////////////////////////////////////////////
// assumes hexadecimal
const char*
DgHierNdxIntRF::str2add (DgHierNdxIntCoord* c, const char* str,
                    char delimiter) const
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
      report("DgHierNdxIntRF::str2add(): invalid index", DgBase::Fatal);

   if (!c) c = new DgHierNdxIntCoord();
   c->setValue(val);

   unsigned long offset = strlen(tok) + 1;
   delete[] tmpStr;
   if (offset >= strlen(str)) return 0;
   else return &str[offset];

} // const char* DgHierNdxIntRF::str2add

////////////////////////////////////////////////////////////////////////////////
