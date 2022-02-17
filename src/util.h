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
// util.h: util class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DGG_UTIL_HPP
 #define __DGG_UTIL_HPP

#include <string>
#include <vector>
#include <cstring>

namespace dgg { namespace util {

void ssplit(const char *& src, std::vector<std::string>& dest, const char *delim = " ");
void ssplit(const std::string& src, std::vector<std::string>& dest, const char *delim = " ");

}} // namespace dgg::util

#endif

