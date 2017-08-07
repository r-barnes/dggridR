void GEO_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg);
void GEO_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty);
void GEO_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy);
void GEO_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j);
void GEO_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_seqnum);
void GEO_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py);
void PROJTRI_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg);
void PROJTRI_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty);
void PROJTRI_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy);
void PROJTRI_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j);
void PROJTRI_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_seqnum);
void PROJTRI_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py);
void Q2DD_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg);
void Q2DD_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty);
void Q2DD_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy);
void Q2DD_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j);
void Q2DD_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_seqnum);
void Q2DD_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py);
void Q2DI_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg);
void Q2DI_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty);
void Q2DI_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy);
void Q2DI_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j);
void Q2DI_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_seqnum);
void Q2DI_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py);
void SEQNUM_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg);
void SEQNUM_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty);
void SEQNUM_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy);
void SEQNUM_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j);
void SEQNUM_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_seqnum);
void SEQNUM_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py);