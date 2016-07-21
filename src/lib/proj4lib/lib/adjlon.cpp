/* reduce argument to range +/- PI */
#ifndef lint
//static const char SCCSID[]="@(#)adjlon.c	4.3	93/06/12	GIE	REL";
#endif
#include "proj4_constants.h"
#include <cmath>
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
