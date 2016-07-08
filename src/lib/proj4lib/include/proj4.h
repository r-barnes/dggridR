//
// this is an adapted version of the projects.h file in Gerald Evenden's 
// USGS PROJ4 projection system
// Version 6.1 - Kevin Sahr, 5/23/13
////////////////////////////////////////////////////////////////////////////////

#ifndef PROJ4_H
#define PROJ4_H

#include <stdlib.h>
#include "DgUtil.h"
/*
#ifdef __cplusplus
extern "C" {
#endif
*/

/* prototype hypot for systems where absent */

extern long double usgs_hypot(long double, long double);

/* some useful constants */

#define FORTPI     M_PI_4
#define PI         M_PI
#define TWOPI      6.28318530717958647692528676655900576L
#define RAD_TO_DEG M_180_PI
#define DEG_TO_RAD M_PI_180

typedef union { long double f; int i; char *s; } PVALUE;

/* procedure prototypes */

long double adjlon(long double);
long double *pj_enfn(long double);
long double pj_mlfn(long double, long double, long double, long double *);
long double pj_inv_mlfn(long double, long double, long double *);
long double pj_qsfn(long double, long double, long double);
long double pj_tsfn(long double, long double, long double);
long double pj_msfn(long double, long double, long double);
long double pj_phi2(long double, long double);
long double *pj_authset(long double);
long double pj_authlat(long double, long double *);

/*
#ifdef __cplusplus
}
#endif
*/

#endif

