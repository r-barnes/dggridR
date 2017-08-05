/* reduce argument to range +/- PI */
#include <cmath>
#include "proj4.h"
/* note: PI adjusted high
** approx. true val:	3.14159265358979323844
*/
#define SPI		3.14159265359
	long double
adjlon (long double lon) {
	while ( std::fabs(lon) > SPI )
		lon += lon < 0. ? projTWOPI : -projTWOPI;
	return( lon );
}
