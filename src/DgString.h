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
// DgString.h: DgString class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGSTRING_H
#define DGSTRING_H

#include "DgUtil.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// string comparison operators

inline bool operator== (const string& s1, const string& s2)
{ return (s1.compare(s2) == 0); }

inline bool operator!= (const string& s1, const string& s2)
{ return (s1.compare(s2) != 0); }

inline bool operator< (const string& s1, const string& s2)
{ return (s1.compare(s2) < 0); }

inline bool operator<= (const string& s1, const string& s2)
{ return (s1.compare(s2) <= 0); }

inline bool operator> (const string& s1, const string& s2)
{ return (s1.compare(s2) > 0); }

inline bool operator>= (const string& s1, const string& s2)
{ return (s1.compare(s2) >= 0); }

inline bool operator== (const string& s1, const char* s2)
{ return (s1 == string(s2)); }

inline bool operator!= (const string& s1, const char* s2)
{ return (s1 != string(s2)); }

inline bool operator< (const string& s1, const char* s2)
{ return (s1 < string(s2)); }

inline bool operator<= (const string& s1, const char* s2)
{ return (s1 <= string(s2)); }

inline bool operator> (const string& s1, const char* s2)
{ return (s1 > string(s2)); }

inline bool operator>= (const string& s1, const char* s2)
{ return (s1 >= string(s2)); }

inline bool operator== (const char* s1, const string& s2)
{ return (string(s1) == s2); }

inline bool operator!= (const char* s1, const string& s2)
{ return (string(s1) != s2); }

inline bool operator< (const char* s1, const string& s2)
{ return (string(s1) < s2); }

inline bool operator<= (const char* s1, const string& s2)
{ return (string(s1) <= s2); }

inline bool operator> (const char* s1, const string& s2)
{ return (string(s1) > s2); }

inline bool operator>= (const char* s1, const string& s2)
{ return (string(s1) >= s2); }

////////////////////////////////////////////////////////////////////////////////
inline string toLower (const string& strIn)
{
   std::string ret(strIn);
   std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
   return ret;
}

////////////////////////////////////////////////////////////////////////////////
namespace dgg { namespace util {

////////////////////////////////////////////////////////////////////////////////
inline string addCommas (unsigned long long int num)
{
    std::stringstream ss;
    ss << num;
    string s = ss.str();

    string newS = "";
    if (s.length() <= 3)
       newS = s;
    else
    {
       int offset = s.length() % 3; // adjust for different # of digits

       for (unsigned int i = 0; i < s.length(); i++)
       {
          if (i && !(i % 3 - offset))
             newS += ",";
          newS += s[i];
       }
   }

   return newS;
}

////////////////////////////////////////////////////////////////////////////////
inline string addCommas (long double num, unsigned int precision)
{
   unsigned long long int intPart = num;

   string newS = addCommas(intPart);

   std::stringstream ss;
   ss << std::fixed << setprecision(precision) << num;
   string s = ss.str();
   size_t ptNdx = s.find_first_of('.');

   if (ptNdx != string::npos)
      newS += s.substr(ptNdx, s.length() - ptNdx);

   return newS;
}

/* Strip away trailing and leading \r\n (no perfect forwarding in C++03!): */
inline void trim(std::string& line, const std::string& candidates = "\n\r")
{
 // Tail:
 size_t pos;
 while(std::string::npos != (pos = candidates.find(line[line.size() - 1])))
  line.erase(line.size() - 1);

 size_t i = 0;

 // Shift ahead to the first non-match:
 for(; std::string::npos != (pos = candidates.find(line[i])); ++i)
  ;

 line.erase(0, i);	// ...and, erase to there.
}

inline void trim(char *line, const std::string& candidates = "\n\r")
{
 // Again, this could be made more efficient, when there's a need:

 size_t len	= std::strlen(line),
        pos	= 0,
        i	= 0;

 while(std::string::npos != (pos = candidates.find(line[len - 1])))
  {
	line[len - 1] = 0;
	--len;
  }

 /* Here, we find the first non-candidate character in the string, starting at the head. Then,
 we shift the remainder of the string left: */
 while(std::string::npos != candidates.find(line[i]))
  ++i;

 // Shift left:
 memcpy(static_cast<void *>(line), static_cast<void *>(&line[i]), len - i);

 line[len - i] = 0;	// add NULL
}

template <class TargetT>
inline static TargetT from_string(const std::string& source)
{
 TargetT result;

 std::istringstream os(source);

 os >> result;

 return result;
}

#define GCC_VERSION (__GNUC__ * 10000 \
                               + __GNUC_MINOR__ * 100 \
                               + __GNUC_PATCHLEVEL__)

// JFW: clean up (gcc specific)
template <>
inline
/* Test for GCC < 4.3.0 */
#if !defined  __APPLE__ && !defined __clang__ && GCC_VERSION < 40300
/* Template specializations are not allowed to have their own storage
   classes, but older g++ didn't know that. Without this, you'll get
   linker errors. */
static
#endif
long long int from_string(const std::string& source)
{
 long long int n;
 sscanf(source.c_str(), "%lld", &n);
 return n;
}

// JFW: clean up (gcc specific)
template <>
inline
#if !defined  __APPLE__ && !defined __clang__ && GCC_VERSION < 40300
static
#endif
unsigned long long int from_string(const std::string& source)
{
 unsigned long long int n;
 sscanf(source.c_str(), "%llu", &n);
 return n;
}

template <class SourceT>
static std::string to_string(const SourceT& source)
{
 std::ostringstream os;
 os << source;
 return os.str();
}

// JFW: We should tidy all of this up later:
inline std::string to_string(long double val, const char *formatStr)
{
   const unsigned int maxBuffer = 100;
   char buffer[maxBuffer];

   snprintf(buffer, maxBuffer, formatStr, val);

   return string(buffer);
}

inline std::string to_string(int val, int padWidth)
{
 std::ostringstream os;
 os << std::setw(padWidth) << std::setfill('0') << val;
 return os.str();
}

}} // namespace dgg::util

////////////////////////////////////////////////////////////////////////////////

#endif
