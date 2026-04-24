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
// DgUtil.cpp: DgUtil class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <iostream>
#include <string.h>

#include "DgUtil.h"

namespace dgg { namespace util {

long rint(const float& x)
{
 return static_cast<long>(std::ceil(x));
}

void ssplit(const std::string& src, std::vector<std::string>& dest, const char *delim)
{
 for(char *name = strtok(const_cast<char *>(src.c_str()), delim); 0 != name; name = strtok(0, delim))
  dest.push_back(name);
}

void ssplit(const char *src, std::vector<std::string>& dest, const char *delim)
{
 std::string s(src);
 return ssplit(s, dest, delim);
}

std::string stripQuotes (const std::string& str, char quote) {
   if (str[0] == quote && str.back() == quote && str.length() > 2)
      return str.substr(1, str.length() - 2);
   else
      return str;
}

std::string baseName (const std::string& path) {
  return path.substr(path.find_last_of("/\\") + 1);
}

std::string toUpper (const std::string& str) {
    std::string result = str;
    for (auto & c: result) c = toupper(c);
    return result;
}

////////////////////////////////////////////////////////////////////////////////

}} // namespace dgg::util
