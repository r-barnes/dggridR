
#ifndef PJ_GNOM_H
#define PJ_GNOM_H

#ifdef __cplusplus
extern "C" {
#endif

#define EPS10 1.e-10L
#define N_POLE  0
#define S_POLE 1
#define EQUIT   2
#define OBLIQ   3

/* projection-specific parameters */

typedef struct PJ_GNOM {

   long double sinph0;
   long double cosph0;
   int mode;

} PJ_GNOM;

/* function prototypes */

void allocGnom (PJ* P);

#ifdef __cplusplus
}
#endif

#endif

