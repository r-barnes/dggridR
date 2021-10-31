#ifndef _dglib_hpp_
#define _dglib_hpp_

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
#include "DgIDGGS.h"

#include <cstdint>
#include <memory>
#include <string>

namespace dglib {

  const std::string TOPO_HEXAGON  = "HEXAGON";
  const std::string TOPO_DIAMOND  = "DIAMOND";
  const std::string TOPO_TRIANGLE = "TRIANGLE";
  const std::string PROJ_ISEA     = "ISEA";
  const std::string PROJ_FULLER   = "FULLER";

  struct DgParams {
    long double  pole_lon_deg;
    long double  pole_lat_deg;
    long double  azimuth_deg;
    unsigned int aperture;
    int          res;
    std::string  topology = "BAD_TOPOLOGY"; //"HEXAGON", "DIAMOND", "TRIANGLE"
    std::string  projection;  //ISEA/FULLER
  };

  class GridThing {
   private:
    // std::unique_ptr<const DgIDGGS> idggs;
    int myres;
   protected:
    DgRFNetwork net0;
    const DgGeoSphRF *const geoRF;
    const DgIDGGSBase *const idggs;
    const DgIDGGBase& dgg;
    const DgGeoSphDegRF *const deg;
    DgParams *ted;
   public:
    GridThing (const DgParams &dp);
    GridThing (
      long double  pole_lon_deg,
      long double  pole_lat_deg,
      long double  azimuth_deg,
      unsigned int aperture,
      int          res,
      std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
      std::string  projection  //ISEA/FULLER
    );
    // void init (
    //   long double  pole_lon_deg,
    //   long double  pole_lat_deg,
    //   long double  azimuth_deg,
    //   unsigned int aperture,
    //   int          res,
    //   std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
    //   std::string  projection  //ISEA/FULLER
    // );
    double nCells() const;
    double cellAreaKM() const;
    double cellDistKM() const;
    double cls() const;
    double nCells(int res) const;
    double cellAreaKM(int res) const;
    double cellDistKM(int res) const;
    double cls(int res) const;
  };

  class Transformer : public GridThing {
   public:
    Transformer (const DgParams &dp);
    Transformer (
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




  class GlobalGridGenerator : public GridThing {
   private:
    std::unique_ptr<DgLocation> add_loc;
    void init();
   public:
    GlobalGridGenerator (const DgParams &dp);
    GlobalGridGenerator (
      long double  pole_lon_deg,
      long double  pole_lat_deg,
      long double  azimuth_deg,
      unsigned int aperture,
      int          res,
      std::string  topology,   //"HEXAGON", "DIAMOND", "TRIANGLE"
      std::string  projection  //ISEA/FULLER
    );

    bool good() const;
    uint64_t operator()(std::vector<long double> &x, std::vector<long double> &y);
  };

  class SeqNumGridGenerator : public GridThing {
   private:
    std::vector<uint64_t> seqnums;
    size_t i = 0;
    void init(const std::vector<uint64_t> &seqnums0);
   public:
    SeqNumGridGenerator (
      const DgParams &dp,
      const std::vector<uint64_t> &seqnums0
    );
    SeqNumGridGenerator (
      long double  pole_lon_deg,
      long double  pole_lat_deg,
      long double  azimuth_deg,
      unsigned int aperture,
      int          res,
      std::string  topology,    //"HEXAGON", "DIAMOND", "TRIANGLE"
      std::string  projection,  //ISEA/FULLER
      const std::vector<uint64_t> &seqnums0
    );

    bool good() const;
    uint64_t operator()(std::vector<long double> &x, std::vector<long double> &y);
  };

}

#endif
