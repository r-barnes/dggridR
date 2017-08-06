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
    std::string  topology = "cheese";    //"HEXAGON", "DIAMOND", "TRIANGLE"
    std::string  projection;  //ISEA/FULLER
    void print() const;
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

}

#endif
