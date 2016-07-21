/* determine small q */
#ifndef lint
//static const char SCCSID[]="@(#)pj_qsfn.c	4.3	93/06/12	GIE	REL";
#endif
#include <cmath>
#include "proj4_constants.h"

# define EPSILON 1.0e-7
	long double
pj_qsfn(long double sinphi, long double e, long double one_es) {
	long double con;

	if (e >= EPSILON) {
		con = e * sinphi;
		return (one_es * (sinphi / (1. - con * con) -
		   (.5 / e) * std::log ((1. - con) / (1. + con))));
	} else
		return (sinphi + sinphi);
}
