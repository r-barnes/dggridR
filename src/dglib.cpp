////////////////////////////////////////////////////////////////////////////////
//
// dglib.cpp: DGGRID library interface
//
// Version 6.1 - Richard Barnes, 8/2/17
//
////////////////////////////////////////////////////////////////////////////////

#include "dglib.h"
#include <cassert>
#include <fstream>

#include "dggrid.h"
#include "DgIVec2D.h"
#include "DgIDGG.h"
#include "DgBoundedIDGG.h"
#include "DgGeoSphRF.h"
#include "DgBoundedRF2D.h"
#include "DgParamList.h"
#include "DgProjGnomonicRF.h"
#include "DgGeoProjConverter.h"
#include "DgCell.h"
//#include "DgLocList.h"
//#include "DgDmdD4Grid2D.h"
//#include "DgDmdD4Grid2DS.h"
#include "DgTriGrid2D.h"
#include "DgIDGGS.h"


namespace dglib {

  GridThing::GridThing (
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
    init(dp.pole_lon_deg,dp.pole_lat_deg,dp.azimuth_deg,dp.aperture,dp.res,dp.topology,dp.projection);
  }

  GridThing::GridThing (
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
    init(pole_lon_deg,pole_lat_deg,azimuth_deg,aperture,res,topology,projection);
  }

  void GridThing::init (
    long double  pole_lon_deg,
    long double  pole_lat_deg,
    long double  azimuth_deg,
    unsigned int aperture,
    int          res,
    std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
    std::string  projection  //ISEA/FULLER
  ){
    DgGeoCoord pole(pole_lon_deg,pole_lat_deg,false);
    idggs.reset(DgIDGGS::makeRF(net0, geoRF, pole, azimuth_deg, aperture, res+1,
      topology, "IDGGS", projection, false, 0, false));
    myres = res;
  }

  double GridThing::nCells()     const { return nCells(myres);     }
  double GridThing::cellAreaKM() const { return cellAreaKM(myres); }
  double GridThing::cellDistKM() const { return cellDistKM(myres); }
  double GridThing::cls()        const { return cls(myres);        }

  double GridThing::nCells(int res) const {
    const DgGridStats& gs = idggs->idgg(res).gridStats();
    return gs.nCells();
  }

  double GridThing::cellAreaKM(int res) const {
    const DgGridStats& gs = idggs->idgg(res).gridStats();
    return gs.cellAreaKM();
  }

  double GridThing::cellDistKM(int res) const {
    const DgGridStats& gs = idggs->idgg(res).gridStats();
    return gs.cellDistKM();
  }

  double GridThing::cls(int res) const {
    const DgGridStats& gs = idggs->idgg(res).gridStats();
    return gs.cls();
  }



  Transformer::Transformer (const DgParams &dp) : GridThing(dp) {}

  Transformer::Transformer (
    long double  pole_lon_deg,
    long double  pole_lat_deg,
    long double  azimuth_deg,
    unsigned int aperture,
    int          res,
    std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
    std::string  projection  //ISEA/FULLER
  ) : GridThing(pole_lon_deg,pole_lat_deg,azimuth_deg,aperture,res,topology,projection) {}

  std::shared_ptr<DgLocation> Transformer::inGEO    (long double lon_deg, long double lat_deg){
    const DgGeoCoord dgc(lon_deg,lat_deg,false); //DgGeoCoord (long double lon, long double lat, bool rads = true) 
    DgAddressBase *dab = new DgAddress<DgGeoCoord>(dgc);
    return std::make_shared<DgLocation>(geoRF,dab);
    delete dab;
  }

  std::shared_ptr<DgLocation> Transformer::inPROJTRI(uint64_t tnum, long double x, long double y){
    const DgProjTriCoord dptc(tnum, DgDVec2D(x,y));
    DgAddressBase *dab = new DgAddress<DgProjTriCoord>(dptc);
    return std::make_shared<DgLocation>(dgg.projTriRF(),dab);
    delete dab;
  }

  std::shared_ptr<DgLocation> Transformer::inQ2DD   (uint64_t quad, long double x, long double y){
    const DgQ2DDCoord dgq(quad, DgDVec2D(x,y));
    DgAddressBase *dab = new DgAddress<DgQ2DDCoord>(dgq);
    return std::make_shared<DgLocation>(dgg.q2ddRF(),dab);
    delete dab;
  }

  std::shared_ptr<DgLocation> Transformer::inQ2DI   (uint64_t quad, long double i, long double j){
    const DgQ2DICoord dgq(quad, DgDVec2D(i,j));
    DgAddressBase *dab = new DgAddress<DgQ2DICoord>(dgq);
    return std::make_shared<DgLocation>(dgg,dab);
    delete dab;
  }

  std::shared_ptr<DgLocation> Transformer::inSEQNUM (uint64_t seqnum){
    DgLocation *loc = static_cast<const DgIDGG&>(dgg).bndRF().locFromSeqNum(seqnum);
    return std::shared_ptr<DgLocation>(loc);
  }

  void Transformer::outGEO    (std::shared_ptr<DgLocation> loc, long double &lon_deg, long double &lat_deg){
    geoRF.convert(loc.get());
    const DgGeoCoord *add = geoRF.getAddress(*loc);
    lon_deg = add->lonDegs();
    lat_deg = add->latDegs();
  }

  void Transformer::outPROJTRI(std::shared_ptr<DgLocation> loc, uint64_t &tnum, long double &x, long double &y){
    dgg.projTriRF().convert(loc.get());
    const DgRFBase* pOutRF = &dgg.projTriRF();
    const DgProjTriCoord *add = static_cast<const DgRF<DgProjTriCoord, long double>*>(pOutRF)->getAddress(*loc);
    tnum = add->triNum();
    x    = add->coord().x();
    y    = add->coord().y();
  }

  void Transformer::outQ2DD   (std::shared_ptr<DgLocation> loc, uint64_t &quad, long double &x, long double &y){
    dgg.q2ddRF().convert(loc.get());
    const DgRFBase* pOutRF = &dgg.q2ddRF();
    const DgQ2DDCoord *add = static_cast<const DgRF<DgQ2DDCoord, long double>*>(pOutRF)->getAddress(*loc);
    quad = add->quadNum();
    x    = add->coord().x();
    y    = add->coord().y();
  }

  void Transformer::outPLANE  (std::shared_ptr<DgLocation> loc, long double &x, long double &y){
    dgg.planeRF().convert(loc.get());
    const DgRFBase* pOutRF = &dgg;
    const DgDVec2D *add = static_cast<const DgRF<DgDVec2D, long double>*>(pOutRF)->getAddress(*loc);
    x = add->x();
    y = add->y();
  }

  void Transformer::outQ2DI   (std::shared_ptr<DgLocation> loc, uint64_t &quad, long double &i, long double &j){
    dgg.convert(loc.get());
    const DgRFBase* pOutRF = &dgg;
    const DgQ2DICoord *add = static_cast<const DgRF<DgQ2DICoord, long double>*>(pOutRF)->getAddress(*loc);
    quad = add->quadNum();
    i    = add->coord().i();
    j    = add->coord().j();
  }

  void Transformer::outSEQNUM (std::shared_ptr<DgLocation> loc, uint64_t &seqnum){
    dgg.convert(loc.get());
    seqnum = static_cast<const DgIDGG&>(dgg).bndRF().seqNum(*loc);
  }



























  static void outputCellAdd2D (
    const DgIDGG& dgg,
    const DgLocation& add2D,
    const DgPolygon& verts,
    const DgContCartRF& deg,
    std::vector<long double> &x,
    std::vector<long double> &y
  ){
    std::uint64_t sn = dgg.bndRF().seqNum(add2D);
    string label = dgg::util::to_string(sn);

    std::unique_ptr<DgLocation> tmpLoc(new DgLocation(add2D));
    DgCell cell(dgg.geoRF(), label, *tmpLoc, new DgPolygon(verts));

    const DgPolygon& reg = cell.region();

    x.resize(reg.size());
    y.resize(reg.size());

    const auto &grf = dgg.geoRF();
    for(unsigned int i=0;i<reg.size();i++){
      const DgGeoCoord *add = grf.getAddress(reg[i]);
      x.at(i) = add->lonDegs();
      y.at(i) = add->latDegs();
    }
  }

  GlobalGridGenerator::GlobalGridGenerator (const DgParams &dp) : GridThing(dp) {
    init();
  }

  GlobalGridGenerator::GlobalGridGenerator (
    long double  pole_lon_deg,
    long double  pole_lat_deg,
    long double  azimuth_deg,
    unsigned int aperture,
    int          res,
    std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
    std::string  projection  //ISEA/FULLER
  ) : GridThing(pole_lon_deg,pole_lat_deg,azimuth_deg,aperture,res,topology,projection) {
    init();
  }

  void GlobalGridGenerator::init(){
    add_loc.reset(new DgLocation(dgg.bndRF().first()));
  }

  bool GlobalGridGenerator::good() const {
    return dgg.bndRF().validLocation(*add_loc);
  }

  uint64_t GlobalGridGenerator::operator()(std::vector<long double> &x, std::vector<long double> &y) {
    if(!good())
      throw std::runtime_error("GlobalGridGenerator is no longer good!");

    const auto sn = dgg.bndRF().seqNum(*add_loc); 

    DgPolygon verts(dgg);
    dgg.setVertices(*add_loc, verts, 0); //dp.npDensify

    outputCellAdd2D(dgg, *add_loc, verts, deg, x, y);

    dgg.bndRF().incrementLocation(*add_loc);

    return sn;
  }

  

  SeqNumGridGenerator::SeqNumGridGenerator (
    const DgParams &dp,
    const std::vector<uint64_t> &seqnums0
  ) : GridThing(dp) {
    init(seqnums0);
  }

  SeqNumGridGenerator::SeqNumGridGenerator (
    long double  pole_lon_deg,
    long double  pole_lat_deg,
    long double  azimuth_deg,
    unsigned int aperture,
    int          res,
    std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
    std::string  projection,  //ISEA/FULLER
    const std::vector<uint64_t> &seqnums0
  ) : GridThing(pole_lon_deg,pole_lat_deg,azimuth_deg,aperture,res,topology,projection) {
    init(seqnums0);
  }

  void SeqNumGridGenerator::init(const std::vector<uint64_t> &seqnums0){
    seqnums = seqnums0;
  }

  bool SeqNumGridGenerator::good() const {
    return i<seqnums.size();
  }

  uint64_t SeqNumGridGenerator::operator()(std::vector<long double> &x, std::vector<long double> &y) {
    if(!good())
      throw std::runtime_error("SeqNumGridGenerator is no longer good!");

    const auto sn = seqnums.at(i);

    std::unique_ptr<DgLocation> loc(static_cast<const DgIDGG&>(dgg).bndRF().locFromSeqNum(sn));
    if (!dgg.bndRF().validLocation(*loc)){
      //std::cerr<<"doTransform(): SEQNUM " << i << " not a valid location"<<std::endl;
      ::report("SeqNumGridGenerator: Invalid SEQNUM found.", DgBase::Warning);
    }

    DgPolygon verts(dgg);
    dgg.setVertices(*loc, verts, 0); //dp.npDensify

    outputCellAdd2D(dgg, *loc, verts, deg, x, y);

    i++;

    return sn;
  }

}
