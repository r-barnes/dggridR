////////////////////////////////////////////////////////////////////////////////
//
// DgUtil.h: DgUtil class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGUTIL_H
#define DGUTIL_H

#include <vector>
#include <algorithm>

// make sure we have the necessary C99 support in our gcc version

#define DGGS_GCC_VERSION (__GNUC__ * 10000 \
                              + __GNUC_MINOR__ * 100 \
                              + __GNUC_PATCHLEVEL__)

#if !defined __clang__ && DGGS_GCC_VERSION >= 40200

#include <tr1/cmath>
#include <tr1/cfloat>
#include <tr1/cstdint>
#include <tr1/climits>

#else	

#include <cmath>
#include <cfloat>
#include <cstdint>
#include <climits>

// hard-code the TR1 limits (these are GPL)

#ifndef LLONG_MIN
#define LLONG_MIN -__LONG_LONG_MAX__ - 1
#endif

#ifndef LLONG_MAX
#define LLONG_MAX __LONG_LONG_MAX__
#endif

#ifndef ULLONG_MAX
#define ULLONG_MAX __LONG_LONG_MAX__ * 2ULL + 1
#endif

#endif

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
#if DGGS_GCC_VERSION >= 40401		// try the C++0x TR1 version:
 return std::tr1::lrintl(x);
#else					// try the C99 version:
 return llrintl(x);
#endif
}

}} // namespace dgg::util

#endif

