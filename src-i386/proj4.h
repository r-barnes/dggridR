//
// this is an adapted version of the projects.h file in Gerald Evenden's 
// USGS PROJ4 projection system
// Version 6.1 - Kevin Sahr, 5/23/13
////////////////////////////////////////////////////////////////////////////////

#ifndef PROJ4_H
#define PROJ4_H

//#include "projUtil.h"
/*
#ifdef __cplusplus
extern "C" {
#endif
*/

#ifndef _proj4_constants_
#define _proj4_constants_

/* some useful constants */

#define projM_PI_180   0.017453292519943295769236907684886L
#define projM_PI_4     0.785398163397448309615660845819875721L
#define projM_PI_2     1.570796326794896619231321691639751442L
#define projM_PI       3.141592653589793238462643383279502884L
#define projTWOPI      6.28318530717958647692528676655900576L

#endif


/* prototype hypot for systems where absent */

extern long double usgs_hypot(long double, long double);

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

