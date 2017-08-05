#include "Rwrapper.hpp"
#include "dglib.hpp"
#include <Rcpp.h>

using namespace Rcpp;

RCPP_MODULE(dgparams){
  Rcpp::class_<dglib::DgParams>("dgparams")
  
  .constructor()
  
  .field("pole_lon_deg", &dglib::DgParams::pole_lon_deg)
  .field("pole_lat_deg", &dglib::DgParams::pole_lat_deg)
  .field("azimuth_deg",  &dglib::DgParams::azimuth_deg)
  .field("aperture",     &dglib::DgParams::aperture)
  .field("res",          &dglib::DgParams::res)
  .field("topology",     &dglib::DgParams::topology)
  .field("projection",   &dglib::DgParams::projection)
  ;
}
