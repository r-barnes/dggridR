/* determine small t */
#include <cmath>
#include "proj4.h"
	long double
pj_tsfn(long double phi, long double sinphi, long double e) {
	sinphi *= e;
	return (std::tan (.5 * (projM_PI_2 - phi)) /
	   std::pow((1. - sinphi) / (1. + sinphi), .5 * e));
}
