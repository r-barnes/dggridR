/* determine constant small m */
#include <cmath>
	long double
pj_msfn(long double sinphi, long double cosphi, long double es) {
	return (cosphi / std::sqrt (1. - es * sinphi * sinphi));
}
