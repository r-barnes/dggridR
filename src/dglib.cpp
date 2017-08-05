////////////////////////////////////////////////////////////////////////////////
//
// dglib.cpp: DGGRID library interface
//
// Version 6.1 - Richard Barnes, 8/2/17
//
////////////////////////////////////////////////////////////////////////////////

#include "dglib.hpp"
#include <cassert>

void orientGrid (MainParam& dp, DgGridPList& plist)
//
// Set the orientation parameters if not specified
//
{
   if (dp.placeRandom) // randomize grid orientation
   {
      dp.vert0 = dp.orientRand->nextGeo();

      dp.azimuthDegs = dp.orientRand->randInRange(0.0, 360.0);

      // set the paramlist to match so we can print it back out

      plist.setParam("dggs_orient_specify_type", "SPECIFIED");
      plist.setParam("dggs_num_placements", dgg::util::to_string(1));
      plist.setParam("dggs_vert0_lon", dgg::util::to_string(dp.vert0.lonDegs()));
      plist.setParam("dggs_vert0_lat", dgg::util::to_string(dp.vert0.latDegs()));
      plist.setParam("dggs_vert0_azimuth", dgg::util::to_string(dp.azimuthDegs));

      cout << "Grid " << dp.curGrid <<
           " #####################################################" << endl;
      cout << "grid #" << dp.curGrid << " orientation randomized to: " << endl;
      cout << plist << endl;
   }
   else if (dp.orientCenter && dp.curGrid == 1)
   {
      DgRFNetwork netc;
      DgGeoSphRF geoRF(netc, "GS0", dp.earthRadius);

      long double lonc = 0.0, latc = 0.0;
      getParamValue(plist, "region_center_lon", lonc, false);
      getParamValue(plist, "region_center_lat", latc, false);

      DgProjGnomonicRF gnomc(netc, "cgnom", DgGeoCoord(lonc, latc, false)); 
      Dg2WayGeoProjConverter(geoRF, gnomc);

      DgLocation* gloc = gnomc.makeLocation(DgDVec2D(-7289214.618283, 
                                                      7289214.618283));
      geoRF.convert(gloc);
   
      DgGeoCoord p0 = *geoRF.getAddress(*gloc);
      delete gloc;

      gloc = gnomc.makeLocation(DgDVec2D(2784232.232959, 2784232.232959));
      geoRF.convert(gloc);
      DgGeoCoord p1 = *geoRF.getAddress(*gloc);
      delete gloc;

      dp.vert0 = p0;
      dp.azimuthDegs = DgGeoSphRF::azimuth(p0, p1, false);
   }

} // void orientGrid

namespace dglib {

  const std::string DgTransformer::TOPO_HEXAGON  = "HEXAGON";
  const std::string DgTransformer::TOPO_DIAMOND  = "DIAMOND";
  const std::string DgTransformer::TOPO_TRIANGLE = "TRIANGLE";
  const std::string DgTransformer::PROJ_ISEA     = "ISEA";
  const std::string DgTransformer::PROJ_FULLER   = "FULLER";

  DgTransformer::DgTransformer (
    const DgParams &dp
  ) : geoRF(DgGeoSphRF(net0)),
      dgg(
        geoRF,       //geoRF:
        DgGeoCoord(dp.pole_lon_deg,dp.pole_lat_deg,false),        //vert0:
        dp.azimuth_deg, //azDegs:
        dp.aperture,    //aperture:
        dp.res,         //res:
        "DDG",       //const string& name ??
        dp.topology,    //gridTopo (HEXAGON, DIAMOND, TRIANGLE):
        dp.projection,  //projType (ISEA, FULLER):
        false,       //isMixed43 (true, false):
        0,           //numAp4: number of leading aperture 4 resolutions in a mixed grid
        false,       //isSuperfund (true, false): Is a superfund grid
        0,           //sfRes: Superfund resolution
        7            //Precision of printed output. NOTE: Should be unused here.
      ),
      deg(geoRF, geoRF.name() + "Deg")
   {
    //DgGeoSphRF (DgRFNetwork& networkIn, const string& nameIn = "GeodeticSph", long double earthRadiusKMin = DEFAULT_RADIUS_KM)
    //DgGeoSphRF geoRF(net0, dp.datum, dp.earthRadius);
    //DgGeoCoord (long double lon, long double lat, bool rads = true) 
    //DgIDGG(const DgGeoSphRF& geoRF, const DgGeoCoord& vert0, long double azDegs, unsigned int aperture, int res, const string& name, const string& gridTopo, const string& projType, bool isMixed43, int numAp4, bool isSuperfund, int sfRes, unsigned int precision)
    // set-up to convert to degrees
    //deg = DgGeoSphDegRF(geoRF, geoRF.name() + "Deg");
    //cout << "Res " << dgg.outputRes() << " " << dgg.gridStats() << endl;
  }

  DgTransformer::DgTransformer (
    long double  pole_lon_deg,
    long double  pole_lat_deg,
    long double  azimuth_deg,
    unsigned int aperture,
    int          res,
    std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
    std::string  projection  //ISEA/FULLER
  ) : geoRF(DgGeoSphRF(net0)),
      dgg(
        geoRF,       //geoRF:
        DgGeoCoord(pole_lon_deg,pole_lat_deg,false),        //vert0:
        azimuth_deg, //azDegs:
        aperture,    //aperture:
        res,         //res:
        "DDG",       //const string& name ??
        topology,    //gridTopo (HEXAGON, DIAMOND, TRIANGLE):
        projection,  //projType (ISEA, FULLER):
        false,       //isMixed43 (true, false):
        0,           //numAp4: number of leading aperture 4 resolutions in a mixed grid
        false,       //isSuperfund (true, false): Is a superfund grid
        0,           //sfRes: Superfund resolution
        7            //Precision of printed output. NOTE: Should be unused here.
      ),
      deg(geoRF, geoRF.name() + "Deg")
   {
    //DgGeoSphRF (DgRFNetwork& networkIn, const string& nameIn = "GeodeticSph", long double earthRadiusKMin = DEFAULT_RADIUS_KM)
    //DgGeoSphRF geoRF(net0, dp.datum, dp.earthRadius);
    //DgGeoCoord (long double lon, long double lat, bool rads = true) 
    //DgIDGG(const DgGeoSphRF& geoRF, const DgGeoCoord& vert0, long double azDegs, unsigned int aperture, int res, const string& name, const string& gridTopo, const string& projType, bool isMixed43, int numAp4, bool isSuperfund, int sfRes, unsigned int precision)
    // set-up to convert to degrees
    //deg = DgGeoSphDegRF(geoRF, geoRF.name() + "Deg");
    //cout << "Res " << dgg.outputRes() << " " << dgg.gridStats() << endl;
  }

  std::shared_ptr<DgLocation> DgTransformer::inGEO    (long double lon_deg, long double lat_deg){
    const DgGeoCoord dgc(lon_deg,lat_deg,false); //DgGeoCoord (long double lon, long double lat, bool rads = true) 
    DgAddressBase *dab = new DgAddress<DgGeoCoord>(dgc);
    return std::make_shared<DgLocation>(geoRF,dab);
    delete dab;
  }

  std::shared_ptr<DgLocation> DgTransformer::inPROJTRI(uint64_t tnum, long double x, long double y){
    const DgProjTriCoord dptc(tnum, DgDVec2D(x,y));
    DgAddressBase *dab = new DgAddress<DgProjTriCoord>(dptc);
    return std::make_shared<DgLocation>(dgg.projTriRF(),dab);
    delete dab;
  }

  std::shared_ptr<DgLocation> DgTransformer::inQ2DD   (uint64_t quad, long double x, long double y){
    const DgQ2DDCoord dgq(quad, DgDVec2D(x,y));
    DgAddressBase *dab = new DgAddress<DgQ2DDCoord>(dgq);
    return std::make_shared<DgLocation>(dgg.q2ddRF(),dab);
    delete dab;
  }

  std::shared_ptr<DgLocation> DgTransformer::inQ2DI   (uint64_t quad, long double i, long double j){
    const DgQ2DICoord dgq(quad, DgDVec2D(i,j));
    DgAddressBase *dab = new DgAddress<DgQ2DICoord>(dgq);
    return std::make_shared<DgLocation>(dgg,dab);
    delete dab;
  }

  std::shared_ptr<DgLocation> DgTransformer::inSEQNUM (uint64_t seqnum){
    DgLocation *loc = static_cast<const DgIDGG&>(dgg).bndRF().locFromSeqNum(seqnum);
    return std::shared_ptr<DgLocation>(loc);
  }

  void DgTransformer::outGEO    (std::shared_ptr<DgLocation> loc, long double &lon_deg, long double &lat_deg){
    geoRF.convert(loc.get());
    const DgGeoCoord *add = geoRF.getAddress(*loc);
    lon_deg = add->lonDegs();
    lat_deg = add->latDegs();
  }

  void DgTransformer::outPROJTRI(std::shared_ptr<DgLocation> loc, uint64_t &tnum, long double &x, long double &y){
    dgg.projTriRF().convert(loc.get());
    const DgRFBase* pOutRF = &dgg.projTriRF();
    const DgProjTriCoord *add = static_cast<const DgRF<DgProjTriCoord, long double>*>(pOutRF)->getAddress(*loc);
    tnum = add->triNum();
    x    = add->coord().x();
    y    = add->coord().y();
  }

  void DgTransformer::outQ2DD   (std::shared_ptr<DgLocation> loc, uint64_t &quad, long double &x, long double &y){
    dgg.q2ddRF().convert(loc.get());
    const DgRFBase* pOutRF = &dgg.q2ddRF();
    const DgQ2DDCoord *add = static_cast<const DgRF<DgQ2DDCoord, long double>*>(pOutRF)->getAddress(*loc);
    quad = add->quadNum();
    x    = add->coord().x();
    y    = add->coord().y();
  }

  void DgTransformer::outPLANE  (std::shared_ptr<DgLocation> loc, long double &x, long double &y){
    dgg.planeRF().convert(loc.get());
    const DgRFBase* pOutRF = &dgg;
    const DgDVec2D *add = static_cast<const DgRF<DgDVec2D, long double>*>(pOutRF)->getAddress(*loc);
    x = add->x();
    y = add->y();
  }

  void DgTransformer::outQ2DI   (std::shared_ptr<DgLocation> loc, uint64_t &quad, long double &i, long double &j){
    dgg.convert(loc.get());
    const DgRFBase* pOutRF = &dgg;
    const DgQ2DICoord *add = static_cast<const DgRF<DgQ2DICoord, long double>*>(pOutRF)->getAddress(*loc);
    quad = add->quadNum();
    i    = add->coord().i();
    j    = add->coord().j();
  }

  void DgTransformer::outSEQNUM (std::shared_ptr<DgLocation> loc, uint64_t &seqnum){
    dgg.convert(loc.get());
    seqnum = static_cast<const DgIDGG&>(dgg).bndRF().seqNum(*loc);
  }







  void GEO_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, long double *const out_lon_deg, long double *const out_lat_deg){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inGEO(in_lon_deg[i], in_lat_deg[i]);
      dgt.outGEO(in, out_lon_deg[i], out_lat_deg[i]);
    }
  }

  void GEO_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inGEO(in_lon_deg[i], in_lat_deg[i]);
      dgt.outPROJTRI(in, out_tnum[i], out_tx[i], out_ty[i]);
    }
  }

  void GEO_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inGEO(in_lon_deg[i], in_lat_deg[i]);
      dgt.outQ2DD(in, out_quad[i], out_qx[i], out_qy[i]);
    }
  }

  void GEO_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, uint64_t *const out_quad, long double *const out_i, long double *const out_j){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inGEO(in_lon_deg[i], in_lat_deg[i]);
      dgt.outQ2DI(in, out_quad[i], out_i[i], out_j[i]);
    }
  }

  void GEO_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, uint64_t *const out_seqnum){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inGEO(in_lon_deg[i], in_lat_deg[i]);
      dgt.outSEQNUM(in, out_seqnum[i]);
    }
  }

  void GEO_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const long double *const in_lon_deg, const long double *const in_lat_deg, long double *const out_px, long double *const out_py){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inGEO(in_lon_deg[i], in_lat_deg[i]);
      dgt.outPLANE(in, out_px[i], out_py[i]);
    }
  }

  void PROJTRI_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, long double *const out_lon_deg, long double *const out_lat_deg){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inPROJTRI(in_tnum[i], in_tx[i], in_ty[i]);
      dgt.outGEO(in, out_lon_deg[i], out_lat_deg[i]);
    }
  }

  void PROJTRI_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inPROJTRI(in_tnum[i], in_tx[i], in_ty[i]);
      dgt.outPROJTRI(in, out_tnum[i], out_tx[i], out_ty[i]);
    }
  }

  void PROJTRI_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inPROJTRI(in_tnum[i], in_tx[i], in_ty[i]);
      dgt.outQ2DD(in, out_quad[i], out_qx[i], out_qy[i]);
    }
  }

  void PROJTRI_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, uint64_t *const out_quad, long double *const out_i, long double *const out_j){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inPROJTRI(in_tnum[i], in_tx[i], in_ty[i]);
      dgt.outQ2DI(in, out_quad[i], out_i[i], out_j[i]);
    }
  }

  void PROJTRI_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, uint64_t *const out_seqnum){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inPROJTRI(in_tnum[i], in_tx[i], in_ty[i]);
      dgt.outSEQNUM(in, out_seqnum[i]);
    }
  }

  void PROJTRI_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_tnum, const long double *const in_tx, const long double *const in_ty, long double *const out_px, long double *const out_py){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inPROJTRI(in_tnum[i], in_tx[i], in_ty[i]);
      dgt.outPLANE(in, out_px[i], out_py[i]);
    }
  }

  void Q2DD_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, long double *const out_lon_deg, long double *const out_lat_deg){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DD(in_quad[i], in_qx[i], in_qy[i]);
      dgt.outGEO(in, out_lon_deg[i], out_lat_deg[i]);
    }
  }

  void Q2DD_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DD(in_quad[i], in_qx[i], in_qy[i]);
      dgt.outPROJTRI(in, out_tnum[i], out_tx[i], out_ty[i]);
    }
  }

  void Q2DD_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DD(in_quad[i], in_qx[i], in_qy[i]);
      dgt.outQ2DD(in, out_quad[i], out_qx[i], out_qy[i]);
    }
  }

  void Q2DD_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, uint64_t *const out_quad, long double *const out_i, long double *const out_j){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DD(in_quad[i], in_qx[i], in_qy[i]);
      dgt.outQ2DI(in, out_quad[i], out_i[i], out_j[i]);
    }
  }

  void Q2DD_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, uint64_t *const out_seqnum){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DD(in_quad[i], in_qx[i], in_qy[i]);
      dgt.outSEQNUM(in, out_seqnum[i]);
    }
  }

  void Q2DD_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_qx, const long double *const in_qy, long double *const out_px, long double *const out_py){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DD(in_quad[i], in_qx[i], in_qy[i]);
      dgt.outPLANE(in, out_px[i], out_py[i]);
    }
  }

  void Q2DI_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, long double *const out_lon_deg, long double *const out_lat_deg){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DI(in_quad[i], in_i[i], in_j[i]);
      dgt.outGEO(in, out_lon_deg[i], out_lat_deg[i]);
    }
  }

  void Q2DI_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DI(in_quad[i], in_i[i], in_j[i]);
      dgt.outPROJTRI(in, out_tnum[i], out_tx[i], out_ty[i]);
    }
  }

  void Q2DI_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DI(in_quad[i], in_i[i], in_j[i]);
      dgt.outQ2DD(in, out_quad[i], out_qx[i], out_qy[i]);
    }
  }

  void Q2DI_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, uint64_t *const out_quad, long double *const out_i, long double *const out_j){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DI(in_quad[i], in_i[i], in_j[i]);
      dgt.outQ2DI(in, out_quad[i], out_i[i], out_j[i]);
    }
  }

  void Q2DI_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, uint64_t *const out_seqnum){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DI(in_quad[i], in_i[i], in_j[i]);
      dgt.outSEQNUM(in, out_seqnum[i]);
    }
  }

  void Q2DI_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_quad, const long double *const in_i, const long double *const in_j, long double *const out_px, long double *const out_py){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inQ2DI(in_quad[i], in_i[i], in_j[i]);
      dgt.outPLANE(in, out_px[i], out_py[i]);
    }
  }

  void SEQNUM_to_GEO(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, long double *const out_lon_deg, long double *const out_lat_deg){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inSEQNUM(in_seqnum[i]);
      dgt.outGEO(in, out_lon_deg[i], out_lat_deg[i]);
    }
  }

  void SEQNUM_to_PROJTRI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, uint64_t *const out_tnum, long double *const out_tx, long double *const out_ty){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inSEQNUM(in_seqnum[i]);
      dgt.outPROJTRI(in, out_tnum[i], out_tx[i], out_ty[i]);
    }
  }

  void SEQNUM_to_Q2DD(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, uint64_t *const out_quad, long double *const out_qx, long double *const out_qy){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inSEQNUM(in_seqnum[i]);
      dgt.outQ2DD(in, out_quad[i], out_qx[i], out_qy[i]);
    }
  }

  void SEQNUM_to_Q2DI(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, uint64_t *const out_quad, long double *const out_i, long double *const out_j){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inSEQNUM(in_seqnum[i]);
      dgt.outQ2DI(in, out_quad[i], out_i[i], out_j[i]);
    }
  }

  void SEQNUM_to_SEQNUM(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, uint64_t *const out_seqnum){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inSEQNUM(in_seqnum[i]);
      dgt.outSEQNUM(in, out_seqnum[i]);
    }
  }

  void SEQNUM_to_PLANE(long double pole_lon_deg, long double pole_lat_deg, long double azimuth_deg, unsigned int aperture, int res, std::string topology, std::string projection, unsigned int N, const uint64_t *const in_seqnum, long double *const out_px, long double *const out_py){
    dglib::DgTransformer dgt(pole_lon_deg, pole_lat_deg, azimuth_deg, aperture, res, topology, projection);

    for(unsigned int i=0;i<N;i++){
      auto in = dgt.inSEQNUM(in_seqnum[i]);
      dgt.outPLANE(in, out_px[i], out_py[i]);
    }
  }


}



/*   
   if (tmp == "GENERATE_GRID")
      pdp = new GridGenParam(plist);
   else if (tmp == "OUTPUT_STATS")   
      pdp = new MainParam(plist);

   if (tmp == "GENERATE_GRID")
      doGridGen(static_cast<GridGenParam&>(*pdp), plist);
   else if (tmp == "OUTPUT_STATS")   
      doTable(*pdp, plist);
*/
