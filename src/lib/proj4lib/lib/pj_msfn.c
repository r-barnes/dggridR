/* determine constant small m */
#ifndef lint
//static const char SCCSID[]="@(#)pj_msfn.c	4.3	93/06/12	GIE	REL";
#endif
#include <cmath>
	long double
pj_msfn(long double sinphi, long double cosphi, long double es) {
	return (cosphi / sqrt (1. - es * sinphi * sinphi));
}
