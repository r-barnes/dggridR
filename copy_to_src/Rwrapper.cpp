#include "Rwrapper.h"
#include "dglib.h"
#include <Rcpp.h>

using namespace Rcpp;

Rcpp::List GlobalGrid(
  const long double pole_lon_deg,
  const long double pole_lat_deg,
  const long double azimuth_deg,
  const unsigned int aperture,
  const int res,
  const std::string topology,
  const std::string projection,
  const bool isMixed43 = false,
  const int numAp4 = 0,
  const int densify = 0
){
  dglib::DgParams dp;
  dp.pole_lon_deg = pole_lon_deg;
  dp.pole_lat_deg = pole_lat_deg;
  dp.azimuth_deg  = azimuth_deg;
  dp.aperture     = aperture;
  dp.res          = res;
  dp.topology     = topology;
  dp.projection   = projection;
  dp.isMixed43    = isMixed43;
  dp.numAp4       = numAp4;
  dp.densify      = densify;

  dglib::GlobalGridGenerator ggg(dp);

  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> seqnum;

  while(ggg.good()){
    std::vector<long double> tempx, tempy;
    const auto sn = ggg(tempx,tempy);
    x.insert(x.end(),tempx.begin(),tempx.end());
    y.insert(y.end(),tempy.begin(),tempy.end());
    for(unsigned int i=0;i<tempx.size();i++)
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
  Rcpp::NumericVector seqnums,
  const bool isMixed43 = false,
  const int numAp4 = 0,
  const int densify = 0
){
  std::vector<uint64_t> cseqnums = Rcpp::as< std::vector<uint64_t> >(seqnums);

  dglib::DgParams dp;
  dp.pole_lon_deg = pole_lon_deg;
  dp.pole_lat_deg = pole_lat_deg;
  dp.azimuth_deg  = azimuth_deg;
  dp.aperture     = aperture;
  dp.res          = res;
  dp.topology     = topology;
  dp.projection   = projection;
  dp.isMixed43    = isMixed43;
  dp.numAp4       = numAp4;
  dp.densify      = densify;

  dglib::SeqNumGridGenerator sngg(dp, cseqnums);

  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> seqnum;

  while(sngg.good()){
    std::vector<long double> tempx, tempy;
    const auto sn = sngg(tempx,tempy);
    x.insert(x.end(),tempx.begin(),tempx.end());
    y.insert(y.end(),tempy.begin(),tempy.end());
    for(unsigned int i=0;i<tempx.size();i++)
      seqnum.push_back(sn);
  }

  return Rcpp::List::create(
    Rcpp::Named("x")      = Rcpp::wrap(x),
    Rcpp::Named("y")      = Rcpp::wrap(y),
    Rcpp::Named("seqnum") = Rcpp::wrap(seqnum)
  );
}


Rcpp::List GetNeighbors(
  const long double pole_lon_deg,
  const long double pole_lat_deg,
  const long double azimuth_deg,
  const unsigned int aperture,
  const int res,
  const std::string topology,
  const std::string projection,
  Rcpp::NumericVector seqnums,
  const bool isMixed43 = false,
  const int numAp4 = 0
){
  dglib::DgParams dp;
  dp.pole_lon_deg = pole_lon_deg;
  dp.pole_lat_deg = pole_lat_deg;
  dp.azimuth_deg  = azimuth_deg;
  dp.aperture     = aperture;
  dp.res          = res;
  dp.topology     = topology;
  dp.projection   = projection;
  dp.isMixed43    = isMixed43;
  dp.numAp4       = numAp4;

  dglib::GridThing gt(dp);

  std::vector<double> center_seqnum;
  std::vector<double> neighbor_seqnum;

  for (int k = 0; k < seqnums.size(); ++k) {
    uint64_t sn = static_cast<uint64_t>(seqnums[k]);
    std::vector<uint64_t> nbrs = gt.getNeighbors(sn);
    for (auto nbr : nbrs) {
      center_seqnum.push_back(static_cast<double>(sn));
      neighbor_seqnum.push_back(static_cast<double>(nbr));
    }
  }

  return Rcpp::List::create(
    Rcpp::Named("seqnum")   = Rcpp::wrap(center_seqnum),
    Rcpp::Named("neighbor") = Rcpp::wrap(neighbor_seqnum)
  );
}


Rcpp::List GetChildren(
  const long double pole_lon_deg,
  const long double pole_lat_deg,
  const long double azimuth_deg,
  const unsigned int aperture,
  const int res,
  const std::string topology,
  const std::string projection,
  Rcpp::NumericVector seqnums,
  const bool isMixed43 = false,
  const int numAp4 = 0
){
  // Build GridThing at res+1 so idggs has nRes = res+2, allowing children at res+1
  dglib::DgParams dp;
  dp.pole_lon_deg = pole_lon_deg;
  dp.pole_lat_deg = pole_lat_deg;
  dp.azimuth_deg  = azimuth_deg;
  dp.aperture     = aperture;
  dp.res          = res + 1;
  dp.topology     = topology;
  dp.projection   = projection;
  dp.isMixed43    = isMixed43;
  dp.numAp4       = numAp4;

  dglib::GridThing gt(dp);

  std::vector<double> parent_seqnum;
  std::vector<double> child_seqnum;

  for (int k = 0; k < seqnums.size(); ++k) {
    uint64_t sn = static_cast<uint64_t>(seqnums[k]);
    std::vector<uint64_t> children = gt.getChildrenAt(sn, res);
    for (auto child : children) {
      parent_seqnum.push_back(static_cast<double>(sn));
      child_seqnum.push_back(static_cast<double>(child));
    }
  }

  return Rcpp::List::create(
    Rcpp::Named("seqnum") = Rcpp::wrap(parent_seqnum),
    Rcpp::Named("child")  = Rcpp::wrap(child_seqnum)
  );
}


Rcpp::List GetParent(
  const long double pole_lon_deg,
  const long double pole_lat_deg,
  const long double azimuth_deg,
  const unsigned int aperture,
  const int res,
  const std::string topology,
  const std::string projection,
  Rcpp::NumericVector seqnums,
  const bool isMixed43 = false,
  const int numAp4 = 0
){
  dglib::DgParams dp;
  dp.pole_lon_deg = pole_lon_deg;
  dp.pole_lat_deg = pole_lat_deg;
  dp.azimuth_deg  = azimuth_deg;
  dp.aperture     = aperture;
  dp.res          = res;
  dp.topology     = topology;
  dp.projection   = projection;
  dp.isMixed43    = isMixed43;
  dp.numAp4       = numAp4;

  dglib::GridThing gt(dp);

  std::vector<double> child_seqnum;
  std::vector<double> parent_seqnum;

  for (int k = 0; k < seqnums.size(); ++k) {
    uint64_t sn = static_cast<uint64_t>(seqnums[k]);
    uint64_t parent = gt.getParentAt(sn, res);
    child_seqnum.push_back(static_cast<double>(sn));
    parent_seqnum.push_back(static_cast<double>(parent));
  }

  return Rcpp::List::create(
    Rcpp::Named("seqnum") = Rcpp::wrap(child_seqnum),
    Rcpp::Named("parent") = Rcpp::wrap(parent_seqnum)
  );
}


double GridStat_nCells    (std::string projection, std::string topology, int aperture, int res) {
  dglib::GridThing gt(0,0,0,aperture,res,topology,projection);
  return gt.nCells(res);
}

double GridStat_cellAreaKM(std::string projection, std::string topology, int aperture, int res) {
  dglib::GridThing gt(0,0,0,aperture,res,topology,projection);
  return gt.cellAreaKM(res);
}

double GridStat_cellDistKM(std::string projection, std::string topology, int aperture, int res) {
  dglib::GridThing gt(0,0,0,aperture,res,topology,projection);
  return gt.cellDistKM(res);
}

double GridStat_cls       (std::string projection, std::string topology, int aperture, int res) {
  dglib::GridThing gt(0,0,0,aperture,res,topology,projection);
  return gt.cls(res);
}




RCPP_MODULE(gridgens){
  Rcpp::function("GlobalGrid",    &GlobalGrid    );
  Rcpp::function("SeqNumGrid",    &SeqNumGrid    );
  Rcpp::function("GetNeighbors",  &GetNeighbors  );
  Rcpp::function("GetChildren",   &GetChildren   );
  Rcpp::function("GetParent",     &GetParent     );
}

RCPP_MODULE(gridstats){
  Rcpp::function("GridStat_nCells",     &GridStat_nCells     );
  Rcpp::function("GridStat_cellAreaKM", &GridStat_cellAreaKM );
  Rcpp::function("GridStat_cellDistKM", &GridStat_cellDistKM );
  Rcpp::function("GridStat_cls",        &GridStat_cls        );
}

#include "cgen_body.h"
#include "cgen_funcs.h"
