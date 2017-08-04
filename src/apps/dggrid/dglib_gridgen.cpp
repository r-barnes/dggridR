////////////////////////////////////////////////////////////////////////////////
//
// gridgen.cpp: gridgen class implementation
//
// Version 6.1  - Kevin Sahr, 5/23/13
// Version 6.2r - Richard Barnes, 2016-07-14
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>

using namespace std;

#include "dggrid.h"
#include "DgIVec2D.h"
#include "DgIDGG.h"
#include "DgBoundedIDGG.h"
#include "DgGeoSphRF.h"
#include "DgBoundedRF2D.h"
#include "DgParamList.h"
#include "DgProjGnomonicRF.h"
#include "DgGeoProjConverter.h"
#include "DgRandom.h"
#include "DgCell.h"
#include "DgLocList.h"
#include "DgDmdD4Grid2D.h"
#include "DgDmdD4Grid2DS.h"
#include "DgTriGrid2D.h"

void outputCellAdd2D (
  const DgIDGG& dgg,
  const DgLocation& add2D,
  const DgPolygon& verts,
  const DgContCartRF& deg,
  std::vector<double> out1,
  std::vector<double> out2
){
  std::uint64_t sn = dgg.bndRF().seqNum(add2D);
  string label = dgg::util::to_string(sn);

  DgLocation* tmpLoc = new DgLocation(add2D);
  DgCell cell(dgg.geoRF(), label, *tmpLoc, new DgPolygon(verts));

  delete tmpLoc;

  const DgPolygon& reg = cell.region();
  const auto &grf = dgg.geoRF();
  for(unsigned int i=0;i<reg.size();i++){
    const DgGeoCoord *add = grf.getAddress(reg[i]);
    out1.push_back(add->lonDegs());
    out2.push_back(add->latDegs());
  }
}

void GeneratorGlobalGrid (
  long double  pole_lon_deg,
  long double  pole_lat_deg,
  long double  azimuth_deg,
  unsigned int aperture,
  int          res,
  std::string topology, //"HEXAGON", "DIAMOND", "TRIANGLE"
  std::string projection, //ISEA/FULLER
  bool ismixed43,
  int  numap4,
  std::vector<double> out1,
  std::vector<double> out2
){
  DgRFNetwork net0;
  DgGeoSphRF geoRF(net0); //, dp.datum, dp.earthRadius);
  DgGeoCoord pole(pole_lon_deg,pole_lat_deg,false);  

  DgIDGG dgg(geoRF, pole, azimuth_deg, aperture, res, "DDG", topology, 
    projection, ismixed43, numap4, false, 0, 7);

  DgGeoSphDegRF deg(geoRF, geoRF.name() + "Deg");

  DgLocation* addLoc = new DgLocation(dgg.bndRF().first());
  while(dgg.bndRF().validLocation(*addLoc)){
    DgPolygon verts(dgg);
    dgg.setVertices(*addLoc, verts, 0); //dp.npDensify

    outputCellAdd2D(dgg, *addLoc, verts, deg, out1, out2);

    dgg.bndRF().incrementLocation(*addLoc);
  }

  delete addLoc;
}



void GeneratorGridFromSeqNums (
  long double  pole_lon_deg,
  long double  pole_lat_deg,
  long double  azimuth_deg,
  unsigned int aperture,
  int          res,
  std::string topology, //"HEXAGON", "DIAMOND", "TRIANGLE"
  std::string projection, //ISEA/FULLER
  bool ismixed43,
  int  numap4,
  const std::vector<unsigned long int> seqnums_to_print,
  std::vector<double> out1,
  std::vector<double> out2
){
  DgRFNetwork net0;
  DgGeoSphRF geoRF (net0); //, dp.datum, dp.earthRadius);
  DgGeoCoord pole(pole_lon_deg,pole_lat_deg,false);  
  DgIDGG dgg(geoRF, pole, azimuth_deg, aperture, res, "DDG", topology, 
    projection, ismixed43, numap4, false, 0, 7);

  DgGeoSphDegRF deg(geoRF, geoRF.name() + "Deg");

  for(const auto i: seqnums_to_print){
    DgLocation* loc = static_cast<const DgIDGG&>(dgg).bndRF().locFromSeqNum(i);
    if (!dgg.bndRF().validLocation(*loc)){
      std::cerr<<"doTransform(): SEQNUM " << i << " not a valid location"<<std::endl;
      ::report("doTransform(): Invalid SEQNUM found.", DgBase::Warning);
    }

    DgPolygon verts(dgg);
    dgg.setVertices(*loc, verts, 0); //dp.npDensify

    outputCellAdd2D(dgg, *loc, verts, deg, out1, out2);

    delete loc;
  }
}
