////////////////////////////////////////////////////////////////////////////////
//
// dglib.cpp: DGGRID library interface
//
// Version 6.1 - Richard Barnes, 8/2/17
//
////////////////////////////////////////////////////////////////////////////////

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

  ////////////////////////////////////////////////////////////////////////////////
  void DgTransform (
     long double  pole_lon_deg,
     long double  pole_lat_deg,
     long double  azimuth_deg,
     unsigned int aperture,
     int          res,
     std::string topology, //"HEXAGON", "DIAMOND", "TRIANGLE"
     std::string projection, //ISEA/FULLER
     InType       in_type,
     const double *const in1,
     const double *const in2,
     const double *const in3,
     OutType      out_type,
     double       *const out1,
     double       *const out2,
     double       *const out3,
     const unsigned int N
  ){
    DgRFNetwork net0;
    //DgGeoSphRF (DgRFNetwork& networkIn, const string& nameIn = "GeodeticSph", long double earthRadiusKMin = DEFAULT_RADIUS_KM)
    //DgGeoSphRF geoRF(net0, dp.datum, dp.earthRadius);
    DgGeoSphRF geoRF(net0);

    //DgGeoCoord (long double lon, long double lat, bool rads = true) 
    DgGeoCoord pole(pole_lon_deg,pole_lat_deg,false);  

    DgIDGG dgg(
      geoRF,       //geoRF:
      pole,        //vert0:
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
    );

    //DgIDGG(const DgGeoSphRF& geoRF, const DgGeoCoord& vert0, long double azDegs, unsigned int aperture, int res, const string& name, const string& gridTopo, const string& projType, bool isMixed43, int numAp4, bool isSuperfund, int sfRes, unsigned int precision)

    //cout << "Res " << dgg.outputRes() << " " << dgg.gridStats() << endl;

     // set-up to convert to degrees
    DgGeoSphDegRF deg(geoRF, geoRF.name() + "Deg");

    switch(in_type){
      case InType::SEQNUM:
        assert(in1!=nullptr);
        break;
      case InType::GEO:
        assert(in1!=nullptr && in2!=nullptr);
        break;
      case InType::PROJTRI:
      case InType::Q2DD:
      case InType::Q2DI:
        assert(in1!=nullptr && in2!=nullptr && in3!=nullptr);
        break;
      default:
        throw std::runtime_error("Unknown input coordinate system!");
    }

    switch(out_type){
      case OutType::SEQNUM:
        assert(out1!=nullptr);
        break;
      case OutType::GEO:
      case OutType::PLANE:
        assert(out1!=nullptr && out2!=nullptr);
        break;
      case OutType::PROJTRI:
      case OutType::Q2DD:
      case OutType::Q2DI:
        assert(out1!=nullptr && out2!=nullptr && out3!=nullptr);
        break;
      default:
        throw std::runtime_error("Unknown output coordinate system!");
    }

    //Setup coordinate converter
    const DgRFBase* pInRF = NULL;
    switch(in_type){
      case InType::GEO:         pInRF = &geoRF;            break;
      case InType::PROJTRI:     pInRF = &dgg.projTriRF();  break;
      //case InType::VERTEX2DD:   pInRF = &dgg.vertexRF();   break;
      case InType::Q2DD:        pInRF = &dgg.q2ddRF();     break;
      case InType::Q2DI:        pInRF = &dgg;              break;
      case InType::SEQNUM:      pInRF = &dgg;              break;
      default:
        throw std::runtime_error("Unknown input coordinate system!");
    }
    const DgRFBase& inRF = *pInRF;

    const DgRFBase* pOutRF = NULL;
    switch(out_type){
      case OutType::GEO:        pOutRF = &geoRF;           break;
      case OutType::PROJTRI:    pOutRF = &dgg.projTriRF(); break;
      //case OutType::VERTEX2DD:  pOutRF = &dgg.vertexRF();  break;
      case OutType::Q2DD:       pOutRF = &dgg.q2ddRF();    break;
      //case OutType::INTERLEAVE: pOutRF = &dgg.intRF();     break;
      case OutType::PLANE:      pOutRF = &dgg.planeRF();   break;
      case OutType::Q2DI:       pOutRF = &dgg;             break;
      case OutType::SEQNUM:     pOutRF = &dgg;             break;
      default:
        throw std::runtime_error("Unknown input coordinate system!");
    }
    const DgRFBase& outRF = *pOutRF;

    for(unsigned int i=0;i<N;i++){
      DgLocation    *loc = NULL; 
      DgAddressBase *dab = NULL;

      if(in_type==InType::GEO) {
        const DgGeoCoord dgc(in1[i],in2[i],false); //DgGeoCoord (long double lon, long double lat, bool rads = true) 
        DgAddressBase *dab = new DgAddress<DgGeoCoord>(dgc);
        loc = new DgLocation(inRF,dab);
      } else if (in_type==InType::PROJTRI) {
        const DgProjTriCoord dptc(in1[i], DgDVec2D(in2[i],in3[i]));
        dab = new DgAddress<DgProjTriCoord>(dptc);
        loc = new DgLocation(inRF,dab);
      } else if (in_type==InType::Q2DD) {
        const DgQ2DDCoord dgq(in1[i], DgDVec2D(in2[i],in3[i]));
        dab = new DgAddress<DgQ2DDCoord>(dgq);
        loc = new DgLocation(inRF,dab);
      } else if (in_type==InType::Q2DI) {
        const DgQ2DICoord dgq(in1[i], DgDVec2D(in2[i],in3[i]));
        dab = new DgAddress<DgQ2DICoord>(dgq);
        loc = new DgLocation(inRF,dab);
      } else if (in_type==InType::SEQNUM){
        loc = static_cast<const DgIDGG&>(inRF).bndRF().locFromSeqNum(in1[i]);
      } else {
        throw std::runtime_error("Unknown input coordinate system!");
      }

      delete dab;

      outRF.convert(loc);       // convert the address

      if(out_type==OutType::GEO){
        const DgGeoCoord *add = geoRF.getAddress(*loc);
        out1[i] = add->lonDegs();
        out2[i] = add->latDegs();
      } else if(out_type==OutType::PROJTRI){
        const DgProjTriCoord *add = static_cast<const DgRF<DgProjTriCoord, long double>&>(outRF).getAddress(*loc);
        out1[i] = add->triNum();
        out2[i] = add->coord().x();
        out3[i] = add->coord().y();
      } else if(out_type==OutType::Q2DD){
        const DgQ2DDCoord *add = static_cast<const DgRF<DgQ2DDCoord, long double>&>(outRF).getAddress(*loc);
        out1[i] = add->quadNum();
        out2[i] = add->coord().x();
        out3[i] = add->coord().y();
      } else if(out_type==OutType::PLANE){
        const DgDVec2D *add = static_cast<const DgRF<DgDVec2D, long double>&>(outRF).getAddress(*loc);
        out1[i] = add->x();
        out2[i] = add->y();
      } else if(out_type==OutType::Q2DI){
        const DgQ2DICoord *add = static_cast<const DgRF<DgQ2DICoord, long double>&>(outRF).getAddress(*loc);
        out1[i] = add->quadNum();
        out2[i] = add->coord().i();
        out3[i] = add->coord().j();
      } else if(out_type==OutType::SEQNUM){
        out1[i] = static_cast<const DgIDGG&>(outRF).bndRF().seqNum(*loc);
      } else {
        throw std::runtime_error("Unknown output coordinate system!");
      }

      delete loc;
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
