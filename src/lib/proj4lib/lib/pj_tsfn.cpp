/* determine small t */
#ifndef lint
//static const char SCCSID[]="@(#)pj_tsfn.c	4.3	93/06/12	GIE	REL";
#endif
#include <cmath>
#include "proj4_constants.h"
	long double
pj_tsfn(long double phi, long double sinphi, long double e) {
	sinphi *= e;
	return (std::tan (.5 * (projM_PI_2 - phi)) /
	   std::pow((1. - sinphi) / (1. + sinphi), .5 * e));
}
