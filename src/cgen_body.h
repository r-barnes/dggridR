
void GEO_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const long double tin_lon_deg = in_lon_deg[i];
    const long double tin_lat_deg = in_lat_deg[i];
    long double tout_lon_deg = out_lon_deg[i];
    long double tout_lat_deg = out_lat_deg[i];
    auto in = dgt.inGEO(tin_lon_deg, tin_lat_deg);
    dgt.outGEO(in, tout_lon_deg, tout_lat_deg);
    out_lon_deg[i] = tout_lon_deg;
    out_lat_deg[i] = tout_lat_deg;
  }
}

void GEO_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const long double tin_lon_deg = in_lon_deg[i];
    const long double tin_lat_deg = in_lat_deg[i];
    uint64_t tout_tnum = out_tnum[i];
    long double tout_tx = out_tx[i];
    long double tout_ty = out_ty[i];
    auto in = dgt.inGEO(tin_lon_deg, tin_lat_deg);
    dgt.outPROJTRI(in, tout_tnum, tout_tx, tout_ty);
    out_tnum[i] = tout_tnum;
    out_tx[i] = tout_tx;
    out_ty[i] = tout_ty;
  }
}

void GEO_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const long double tin_lon_deg = in_lon_deg[i];
    const long double tin_lat_deg = in_lat_deg[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_qx = out_qx[i];
    long double tout_qy = out_qy[i];
    auto in = dgt.inGEO(tin_lon_deg, tin_lat_deg);
    dgt.outQ2DD(in, tout_quad, tout_qx, tout_qy);
    out_quad[i] = tout_quad;
    out_qx[i] = tout_qx;
    out_qy[i] = tout_qy;
  }
}

void GEO_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const long double tin_lon_deg = in_lon_deg[i];
    const long double tin_lat_deg = in_lat_deg[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_i = out_i[i];
    long double tout_j = out_j[i];
    auto in = dgt.inGEO(tin_lon_deg, tin_lat_deg);
    dgt.outQ2DI(in, tout_quad, tout_i, tout_j);
    out_quad[i] = tout_quad;
    out_i[i] = tout_i;
    out_j[i] = tout_j;
  }
}

void GEO_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_seqnum){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const long double tin_lon_deg = in_lon_deg[i];
    const long double tin_lat_deg = in_lat_deg[i];
    uint64_t tout_seqnum = out_seqnum[i];
    auto in = dgt.inGEO(tin_lon_deg, tin_lat_deg);
    dgt.outSEQNUM(in, tout_seqnum);
    out_seqnum[i] = tout_seqnum;
  }
}

void GEO_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_lon_deg, Rcpp::NumericVector in_lat_deg, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const long double tin_lon_deg = in_lon_deg[i];
    const long double tin_lat_deg = in_lat_deg[i];
    long double tout_px = out_px[i];
    long double tout_py = out_py[i];
    auto in = dgt.inGEO(tin_lon_deg, tin_lat_deg);
    dgt.outPLANE(in, tout_px, tout_py);
    out_px[i] = tout_px;
    out_py[i] = tout_py;
  }
}

void PROJTRI_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_tnum = in_tnum[i];
    const long double tin_tx = in_tx[i];
    const long double tin_ty = in_ty[i];
    long double tout_lon_deg = out_lon_deg[i];
    long double tout_lat_deg = out_lat_deg[i];
    auto in = dgt.inPROJTRI(tin_tnum, tin_tx, tin_ty);
    dgt.outGEO(in, tout_lon_deg, tout_lat_deg);
    out_lon_deg[i] = tout_lon_deg;
    out_lat_deg[i] = tout_lat_deg;
  }
}

void PROJTRI_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_tnum = in_tnum[i];
    const long double tin_tx = in_tx[i];
    const long double tin_ty = in_ty[i];
    uint64_t tout_tnum = out_tnum[i];
    long double tout_tx = out_tx[i];
    long double tout_ty = out_ty[i];
    auto in = dgt.inPROJTRI(tin_tnum, tin_tx, tin_ty);
    dgt.outPROJTRI(in, tout_tnum, tout_tx, tout_ty);
    out_tnum[i] = tout_tnum;
    out_tx[i] = tout_tx;
    out_ty[i] = tout_ty;
  }
}

void PROJTRI_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_tnum = in_tnum[i];
    const long double tin_tx = in_tx[i];
    const long double tin_ty = in_ty[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_qx = out_qx[i];
    long double tout_qy = out_qy[i];
    auto in = dgt.inPROJTRI(tin_tnum, tin_tx, tin_ty);
    dgt.outQ2DD(in, tout_quad, tout_qx, tout_qy);
    out_quad[i] = tout_quad;
    out_qx[i] = tout_qx;
    out_qy[i] = tout_qy;
  }
}

void PROJTRI_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_tnum = in_tnum[i];
    const long double tin_tx = in_tx[i];
    const long double tin_ty = in_ty[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_i = out_i[i];
    long double tout_j = out_j[i];
    auto in = dgt.inPROJTRI(tin_tnum, tin_tx, tin_ty);
    dgt.outQ2DI(in, tout_quad, tout_i, tout_j);
    out_quad[i] = tout_quad;
    out_i[i] = tout_i;
    out_j[i] = tout_j;
  }
}

void PROJTRI_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_seqnum){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_tnum = in_tnum[i];
    const long double tin_tx = in_tx[i];
    const long double tin_ty = in_ty[i];
    uint64_t tout_seqnum = out_seqnum[i];
    auto in = dgt.inPROJTRI(tin_tnum, tin_tx, tin_ty);
    dgt.outSEQNUM(in, tout_seqnum);
    out_seqnum[i] = tout_seqnum;
  }
}

void PROJTRI_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_tnum, Rcpp::NumericVector in_tx, Rcpp::NumericVector in_ty, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_tnum = in_tnum[i];
    const long double tin_tx = in_tx[i];
    const long double tin_ty = in_ty[i];
    long double tout_px = out_px[i];
    long double tout_py = out_py[i];
    auto in = dgt.inPROJTRI(tin_tnum, tin_tx, tin_ty);
    dgt.outPLANE(in, tout_px, tout_py);
    out_px[i] = tout_px;
    out_py[i] = tout_py;
  }
}

void Q2DD_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_qx = in_qx[i];
    const long double tin_qy = in_qy[i];
    long double tout_lon_deg = out_lon_deg[i];
    long double tout_lat_deg = out_lat_deg[i];
    auto in = dgt.inQ2DD(tin_quad, tin_qx, tin_qy);
    dgt.outGEO(in, tout_lon_deg, tout_lat_deg);
    out_lon_deg[i] = tout_lon_deg;
    out_lat_deg[i] = tout_lat_deg;
  }
}

void Q2DD_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_qx = in_qx[i];
    const long double tin_qy = in_qy[i];
    uint64_t tout_tnum = out_tnum[i];
    long double tout_tx = out_tx[i];
    long double tout_ty = out_ty[i];
    auto in = dgt.inQ2DD(tin_quad, tin_qx, tin_qy);
    dgt.outPROJTRI(in, tout_tnum, tout_tx, tout_ty);
    out_tnum[i] = tout_tnum;
    out_tx[i] = tout_tx;
    out_ty[i] = tout_ty;
  }
}

void Q2DD_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_qx = in_qx[i];
    const long double tin_qy = in_qy[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_qx = out_qx[i];
    long double tout_qy = out_qy[i];
    auto in = dgt.inQ2DD(tin_quad, tin_qx, tin_qy);
    dgt.outQ2DD(in, tout_quad, tout_qx, tout_qy);
    out_quad[i] = tout_quad;
    out_qx[i] = tout_qx;
    out_qy[i] = tout_qy;
  }
}

void Q2DD_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_qx = in_qx[i];
    const long double tin_qy = in_qy[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_i = out_i[i];
    long double tout_j = out_j[i];
    auto in = dgt.inQ2DD(tin_quad, tin_qx, tin_qy);
    dgt.outQ2DI(in, tout_quad, tout_i, tout_j);
    out_quad[i] = tout_quad;
    out_i[i] = tout_i;
    out_j[i] = tout_j;
  }
}

void Q2DD_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_seqnum){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_qx = in_qx[i];
    const long double tin_qy = in_qy[i];
    uint64_t tout_seqnum = out_seqnum[i];
    auto in = dgt.inQ2DD(tin_quad, tin_qx, tin_qy);
    dgt.outSEQNUM(in, tout_seqnum);
    out_seqnum[i] = tout_seqnum;
  }
}

void Q2DD_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_qx, Rcpp::NumericVector in_qy, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_qx = in_qx[i];
    const long double tin_qy = in_qy[i];
    long double tout_px = out_px[i];
    long double tout_py = out_py[i];
    auto in = dgt.inQ2DD(tin_quad, tin_qx, tin_qy);
    dgt.outPLANE(in, tout_px, tout_py);
    out_px[i] = tout_px;
    out_py[i] = tout_py;
  }
}

void Q2DI_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_i = in_i[i];
    const long double tin_j = in_j[i];
    long double tout_lon_deg = out_lon_deg[i];
    long double tout_lat_deg = out_lat_deg[i];
    auto in = dgt.inQ2DI(tin_quad, tin_i, tin_j);
    dgt.outGEO(in, tout_lon_deg, tout_lat_deg);
    out_lon_deg[i] = tout_lon_deg;
    out_lat_deg[i] = tout_lat_deg;
  }
}

void Q2DI_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_i = in_i[i];
    const long double tin_j = in_j[i];
    uint64_t tout_tnum = out_tnum[i];
    long double tout_tx = out_tx[i];
    long double tout_ty = out_ty[i];
    auto in = dgt.inQ2DI(tin_quad, tin_i, tin_j);
    dgt.outPROJTRI(in, tout_tnum, tout_tx, tout_ty);
    out_tnum[i] = tout_tnum;
    out_tx[i] = tout_tx;
    out_ty[i] = tout_ty;
  }
}

void Q2DI_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_i = in_i[i];
    const long double tin_j = in_j[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_qx = out_qx[i];
    long double tout_qy = out_qy[i];
    auto in = dgt.inQ2DI(tin_quad, tin_i, tin_j);
    dgt.outQ2DD(in, tout_quad, tout_qx, tout_qy);
    out_quad[i] = tout_quad;
    out_qx[i] = tout_qx;
    out_qy[i] = tout_qy;
  }
}

void Q2DI_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_i = in_i[i];
    const long double tin_j = in_j[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_i = out_i[i];
    long double tout_j = out_j[i];
    auto in = dgt.inQ2DI(tin_quad, tin_i, tin_j);
    dgt.outQ2DI(in, tout_quad, tout_i, tout_j);
    out_quad[i] = tout_quad;
    out_i[i] = tout_i;
    out_j[i] = tout_j;
  }
}

void Q2DI_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_seqnum){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_i = in_i[i];
    const long double tin_j = in_j[i];
    uint64_t tout_seqnum = out_seqnum[i];
    auto in = dgt.inQ2DI(tin_quad, tin_i, tin_j);
    dgt.outSEQNUM(in, tout_seqnum);
    out_seqnum[i] = tout_seqnum;
  }
}

void Q2DI_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_quad, Rcpp::NumericVector in_i, Rcpp::NumericVector in_j, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_quad = in_quad[i];
    const long double tin_i = in_i[i];
    const long double tin_j = in_j[i];
    long double tout_px = out_px[i];
    long double tout_py = out_py[i];
    auto in = dgt.inQ2DI(tin_quad, tin_i, tin_j);
    dgt.outPLANE(in, tout_px, tout_py);
    out_px[i] = tout_px;
    out_py[i] = tout_py;
  }
}

void SEQNUM_to_GEO(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_lon_deg, Rcpp::NumericVector out_lat_deg){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_seqnum = in_seqnum[i];
    long double tout_lon_deg = out_lon_deg[i];
    long double tout_lat_deg = out_lat_deg[i];
    auto in = dgt.inSEQNUM(tin_seqnum);
    dgt.outGEO(in, tout_lon_deg, tout_lat_deg);
    out_lon_deg[i] = tout_lon_deg;
    out_lat_deg[i] = tout_lat_deg;
  }
}

void SEQNUM_to_PROJTRI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_tnum, Rcpp::NumericVector out_tx, Rcpp::NumericVector out_ty){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_seqnum = in_seqnum[i];
    uint64_t tout_tnum = out_tnum[i];
    long double tout_tx = out_tx[i];
    long double tout_ty = out_ty[i];
    auto in = dgt.inSEQNUM(tin_seqnum);
    dgt.outPROJTRI(in, tout_tnum, tout_tx, tout_ty);
    out_tnum[i] = tout_tnum;
    out_tx[i] = tout_tx;
    out_ty[i] = tout_ty;
  }
}

void SEQNUM_to_Q2DD(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_qx, Rcpp::NumericVector out_qy){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_seqnum = in_seqnum[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_qx = out_qx[i];
    long double tout_qy = out_qy[i];
    auto in = dgt.inSEQNUM(tin_seqnum);
    dgt.outQ2DD(in, tout_quad, tout_qx, tout_qy);
    out_quad[i] = tout_quad;
    out_qx[i] = tout_qx;
    out_qy[i] = tout_qy;
  }
}

void SEQNUM_to_Q2DI(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_quad, Rcpp::NumericVector out_i, Rcpp::NumericVector out_j){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_seqnum = in_seqnum[i];
    uint64_t tout_quad = out_quad[i];
    long double tout_i = out_i[i];
    long double tout_j = out_j[i];
    auto in = dgt.inSEQNUM(tin_seqnum);
    dgt.outQ2DI(in, tout_quad, tout_i, tout_j);
    out_quad[i] = tout_quad;
    out_i[i] = tout_i;
    out_j[i] = tout_j;
  }
}

void SEQNUM_to_SEQNUM(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_seqnum){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_seqnum = in_seqnum[i];
    uint64_t tout_seqnum = out_seqnum[i];
    auto in = dgt.inSEQNUM(tin_seqnum);
    dgt.outSEQNUM(in, tout_seqnum);
    out_seqnum[i] = tout_seqnum;
  }
}

void SEQNUM_to_PLANE(const long double pole_lon_deg, const long double pole_lat_deg, const long double azimuth_deg, const unsigned int aperture, const int res, const std::string topology, const std::string projection, unsigned int N, Rcpp::NumericVector in_seqnum, Rcpp::NumericVector out_px, Rcpp::NumericVector out_py){
  dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

  for(unsigned int i=0;i<N;i++){
    const uint64_t tin_seqnum = in_seqnum[i];
    long double tout_px = out_px[i];
    long double tout_py = out_py[i];
    auto in = dgt.inSEQNUM(tin_seqnum);
    dgt.outPLANE(in, tout_px, tout_py);
    out_px[i] = tout_px;
    out_py[i] = tout_py;
  }
}
