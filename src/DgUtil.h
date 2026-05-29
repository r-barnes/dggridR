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
// DgUtil.h: DgUtil class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGUTIL_H
#define DGUTIL_H

#include <algorithm>
#include <vector>
#include <string>

// C++11 provides all the math/integer headers we need in the standard locations
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdint>

namespace dgg { namespace util {

// Some older compilers may have an easier time matching this:
template <class T>
void release(T x)
{
 delete x;
}

// Delete a pointer parameter, and set to zero:
template <class T>
void release(T *& x)
{
 delete x;
 x = 0;		// a little pedantic, but there we have it
}

/* Delete each pointed-at object in a vector, then release the pointers: */
template <class T>
void release(std::vector<T>& v)
{
 for(typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
  dgg::util::release(*i);

 /*
  Note: g++ 4.4.1 is ok with the straightforward:
	std::for_each(v.begin(), v.end(), dgg::util::release<T>);
 */

 v.resize(0);
}

// Round a long double into a long:
inline long lrint(const long double& x)
{
 return std::lrintl(x);
}

void ssplit(const char *& src, std::vector<std::string>& dest, const char *delim = " ");
void ssplit(const std::string& src, std::vector<std::string>& dest, const char *delim = " ");
std::string stripQuotes (const std::string& str, char quote = '"');
std::string baseName (const std::string& path);
std::string toUpper (const std::string& str);

}} // namespace dgg::util

#endif
