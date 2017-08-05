#ifndef _dglib_hpp_
#define _dglib_hpp_

#include <memory>
#include <cstdint>

#include "DgConstants.h"
#include "dggrid.h"
//#include "DgProjGnomonicRF.h"
//#include "DgGeoProjConverter.h"

#include "DgIDGG.h"
#include "DgBoundedIDGG.h"
#include "DgInputStream.h"
#include "DgOutputStream.h"
#include "DgCell.h"
#include "DgOutAIGenFile.h"
#include "DgProjGnomonicRF.h"
#include "DgGeoProjConverter.h"

namespace dglib {

  class DgParams {
   public:
    long double  pole_lon_deg;
    long double  pole_lat_deg;
    long double  azimuth_deg;
    unsigned int aperture;
    int          res;
    std::string  topology;    //"HEXAGON", "DIAMOND", "TRIANGLE"
    std::string  projection;  //ISEA/FULLER
  };

  class DgTransformer {
   private:
    DgRFNetwork   net0;
    DgGeoSphRF    geoRF;
    DgIDGG        dgg;
    DgGeoSphDegRF deg;
   public:
    static const std::string TOPO_HEXAGON;
    static const std::string TOPO_DIAMOND;
    static const std::string TOPO_TRIANGLE;
    static const std::string PROJ_ISEA;
    static const std::string PROJ_FULLER;
    DgTransformer (const DgParams &dp);
    DgTransformer (
      long double  pole_lon_deg,
      long double  pole_lat_deg,
      long double  azimuth_deg,
      unsigned int aperture,
      int          res,
      std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
      std::string  projection  //ISEA/FULLER
    );

    std::shared_ptr<DgLocation> inGEO    (long double lon_deg, long double lat_deg);
    std::shared_ptr<DgLocation> inPROJTRI(uint64_t tnum, long double x, long double y);
    std::shared_ptr<DgLocation> inQ2DD   (uint64_t quad, long double x, long double y);
    std::shared_ptr<DgLocation> inQ2DI   (uint64_t quad, long double i, long double j);
    std::shared_ptr<DgLocation> inSEQNUM (uint64_t seqnum);

    void outGEO    (std::shared_ptr<DgLocation> loc, long double &lon_deg, long double &lat_deg);
    void outPROJTRI(std::shared_ptr<DgLocation> loc, uint64_t &tnum, long double &x, long double &y);
    void outQ2DD   (std::shared_ptr<DgLocation> loc, uint64_t &quad, long double &x, long double &y);
    void outQ2DI   (std::shared_ptr<DgLocation> loc, uint64_t &quad, long double &i, long double &j);
    void outSEQNUM (std::shared_ptr<DgLocation> loc, uint64_t &seqnum);
    void outPLANE  (std::shared_ptr<DgLocation> loc, long double &x, long double &y);
  };




  // [[Rcpp::export]]
  void GEO_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, long double *const out_lon_deg, long double *const out_lat_deg);
  // [[Rcpp::export]]
  void GEO_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty);
  // [[Rcpp::export]]
  void GEO_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy);
  // [[Rcpp::export]]
  void GEO_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, uint64_t *const out_quad, long double *const out_i, long double *const out_j);
  // [[Rcpp::export]]
  void GEO_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, uint64_t *const out_seqnum);
  // [[Rcpp::export]]
  void GEO_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, long double *const out_px, long double *const out_py);
  // [[Rcpp::export]]
  void PROJTRI_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, long double *const out_lon_deg, long double *const out_lat_deg);
  // [[Rcpp::export]]
  void PROJTRI_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty);
  // [[Rcpp::export]]
  void PROJTRI_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy);
  // [[Rcpp::export]]
  void PROJTRI_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, uint64_t *const out_quad, long double *const out_i, long double *const out_j);
  // [[Rcpp::export]]
  void PROJTRI_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, uint64_t *const out_seqnum);
  // [[Rcpp::export]]
  void PROJTRI_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, long double *const out_px, long double *const out_py);
  // [[Rcpp::export]]
  void Q2DD_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, long double *const out_lon_deg, long double *const out_lat_deg);
  // [[Rcpp::export]]
  void Q2DD_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty);
  // [[Rcpp::export]]
  void Q2DD_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy);
  // [[Rcpp::export]]
  void Q2DD_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, uint64_t *const out_quad, long double *const out_i, long double *const out_j);
  // [[Rcpp::export]]
  void Q2DD_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, uint64_t *const out_seqnum);
  // [[Rcpp::export]]
  void Q2DD_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, long double *const out_px, long double *const out_py);
  // [[Rcpp::export]]
  void Q2DI_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, long double *const out_lon_deg, long double *const out_lat_deg);
  // [[Rcpp::export]]
  void Q2DI_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty);
  // [[Rcpp::export]]
  void Q2DI_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy);
  // [[Rcpp::export]]
  void Q2DI_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, uint64_t *const out_quad, long double *const out_i, long double *const out_j);
  // [[Rcpp::export]]
  void Q2DI_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, uint64_t *const out_seqnum);
  // [[Rcpp::export]]
  void Q2DI_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, long double *const out_px, long double *const out_py);
  // [[Rcpp::export]]
  void SEQNUM_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, long double *const out_lon_deg, long double *const out_lat_deg);
  // [[Rcpp::export]]
  void SEQNUM_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty);
  // [[Rcpp::export]]
  void SEQNUM_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy);
  // [[Rcpp::export]]
  void SEQNUM_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, uint64_t *const out_quad, long double *const out_i, long double *const out_j);
  // [[Rcpp::export]]
  void SEQNUM_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, uint64_t *const out_seqnum);
  // [[Rcpp::export]]
  void SEQNUM_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, long double *const out_px, long double *const out_py);



}

#endif
