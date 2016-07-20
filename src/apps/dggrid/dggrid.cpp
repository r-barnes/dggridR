////////////////////////////////////////////////////////////////////////////////
//
// dggrid.cpp: DGGRID main program 
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#define __STDC_LIMIT_MACROS
#include <stdint.h>

using namespace std;

#include "DgConstants.h"
#include "dggrid.h"
#include "DgProjGnomonicRF.h"
#include "DgGeoProjConverter.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[])
{
   //////// setup /////////////////

   //// parse the command line parameters ////

   DgBase::testArgEqual(argc, argv, 1, string("metaFileName"));
   string metaFileName(argv[1]);

   //// build and load the parameter list ////

   cout << "** executing DGGRID version " << DGGRID_VERSION << " **\n";
   cout << "type sizes: big int: " << sizeof(int64_t) * 8 << " bits / ";
   cout << "big double: " << sizeof(long double) * 8 << " bits\n";

   cout << "\n** loading meta file " << metaFileName << "..." << endl;

   // first parse the meta file 
   DgGridPList plist; // builds the parameter list
   plist.loadParams(metaFileName);

   // now build our "global parameters" structure
   string tmp;
   getParamValue(plist, "dggrid_operation", tmp, false);
   MainParam* pdp = 0;
   if (tmp == "GENERATE_GRID")
      pdp = new GridGenParam(plist);
   else if (tmp == "OUTPUT_STATS")   
      pdp = new MainParam(plist);
   else if (tmp == "BIN_POINT_VALS")     
      pdp = new BinValsParam(plist);
   else if (tmp == "BIN_POINT_PRESENCE")
      pdp = new BinPresenceParam(plist);
   else if (tmp == "TRANSFORM_POINTS")   
      pdp = new TransformParam(plist);

   // echo the parameter list
   cout << "* using parameter values:\n";
   cout << plist << endl;

   // execute the operation
   if (tmp == "GENERATE_GRID")
      doGridGen(static_cast<GridGenParam&>(*pdp), plist);
   else if (tmp == "OUTPUT_STATS")   
      doTable(*pdp, plist);
   else if (tmp == "BIN_POINT_VALS")     
      doBinVals(static_cast<BinValsParam&>(*pdp), plist);
   else if (tmp == "BIN_POINT_PRESENCE")
      doBinPresence(static_cast<BinPresenceParam&>(*pdp), plist);
   else if (tmp == "TRANSFORM_POINTS")   
      doTransforms(static_cast<TransformParam&>(*pdp), plist);

   delete pdp;
   
   ////// do the grid /////////

   return 0;

} // main 

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
