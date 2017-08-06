#ifndef _Rwrapper_hpp_
#define _Rwrapper_hpp_

#include "dglib.hpp"
#include <Rcpp.h>

class DgParamsR : public dglib::DgParams {
 public:
  DgParamsR() = default;
  DgParamsR(SEXPREC*){}
};

/*extern "C" {
  #include "cgen_head.h"
}*/

#endif