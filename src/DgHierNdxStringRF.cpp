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
// DgHierNdxStringRF.cpp: DgHierNdxStringRF class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgHierNdxStringRF.h"

////////////////////////////////////////////////////////////////////////////////
const DgHierNdxStringCoord DgHierNdxStringRF::undefCoord("99");

DgHierNdxStringCoord::DgHierNdxStringCoord (void)
  : DgHierNdxCoord<std::string>(DgHierNdxStringRF::undefCoord.value())
{ }

////////////////////////////////////////////////////////////////////////////////
const char*
DgHierNdxStringRF::str2add (DgHierNdxStringCoord* add, const char* str,
                                   char delimiter) const
{
    if (!add) return str;

    char delimStr[2];
    delimStr[0] = delimiter;
    delimStr[1] = '\0';

    char* tmpStr = new char[strlen(str) + 1];
    strcpy(tmpStr, str);
    char* tok = strtok(tmpStr, delimStr);

    add->setValue(tok);

    unsigned long offset = strlen(tok) + 1;
    delete[] tmpStr;

    if (offset >= strlen(str)) return 0;
    else return &str[offset];

} // const char* DgHierNdxStringRF::str2add
////////////////////////////////////////////////////////////////////////////////

