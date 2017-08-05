////////////////////////////////////////////////////////////////////////////////
//
// util.h: util class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
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

