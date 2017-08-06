#include "Rwrapper.hpp"
#include "dglib.hpp"
#include <Rcpp.h>

using namespace Rcpp;

Rcpp::List GlobalGrid(
  const long double pole_lon_deg,
  const long double pole_lat_deg,
  const long double azimuth_deg,
  const unsigned int aperture,
  const int res,
  const std::string topology,
  const std::string projection
){
  dglib::GlobalGridGenerator ggg(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> seqnum;

  while(ggg.good()){
    std::vector<long double> tempx, tempy;
    const auto sn = ggg(tempx,tempy);
    x.insert(x.end(),tempx.begin(),tempx.end());
    y.insert(y.end(),tempy.begin(),tempy.end());
    for(int i=0;i<tempx.size();i++)
      seqnum.push_back(sn);
  }

  return Rcpp::List::create(
    Rcpp::Named("x")      = Rcpp::wrap(x),
    Rcpp::Named("y")      = Rcpp::wrap(y),
    Rcpp::Named("seqnum") = Rcpp::wrap(seqnum)
  );
}

Rcpp::List SeqNumGrid(
  const long double pole_lon_deg,
  const long double pole_lat_deg,
  const long double azimuth_deg,
  const unsigned int aperture,
  const int res,
  const std::string topology,
  const std::string projection,
  Rcpp::NumericVector seqnums
){
  std::vector<uint64_t> cseqnums = Rcpp::as< std::vector<uint64_t> >(seqnums);

  dglib::SeqNumGridGenerator sngg(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection, cseqnums);

  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> seqnum;

  while(sngg.good()){
    std::vector<long double> tempx, tempy;
    const auto sn = sngg(tempx,tempy);
    x.insert(x.end(),tempx.begin(),tempx.end());
    y.insert(y.end(),tempy.begin(),tempy.end());
    for(int i=0;i<tempx.size();i++)
      seqnum.push_back(sn);
  }

  return Rcpp::List::create(
    Rcpp::Named("x")      = Rcpp::wrap(x),
    Rcpp::Named("y")      = Rcpp::wrap(y),
    Rcpp::Named("seqnum") = Rcpp::wrap(seqnum)
  );
}

RCPP_MODULE(gridgens){
  Rcpp::function("GlobalGrid", &GlobalGrid);
  Rcpp::function("SeqNumGrid", &SeqNumGrid);
}

#include "cgen_body.h"
#include "cgen_funcs.h"