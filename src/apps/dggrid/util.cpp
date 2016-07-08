////////////////////////////////////////////////////////////////////////////////
//
// util.cpp: util class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////
#include "util.h"

#include <iostream>

namespace dgg { namespace util {

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


}} // namespace dgg::util
