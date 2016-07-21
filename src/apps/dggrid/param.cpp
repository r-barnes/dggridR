////////////////////////////////////////////////////////////////////////////////
//
// param.cpp: param class implementation
//
// Version 6.1  - Kevin Sahr, 5/23/13
// Version 6.2r - Richard Barnes, 2016-07-14
//
////////////////////////////////////////////////////////////////////////////////

#include <limits>

#include "dggrid.h"
#include "DgIDGGS.h"
#include "DgEllipsoidRF.h"
#include "DgSuperfund.h"
#include "DgOutKMLfile.h"

void 
MainParam::determineRes (const DgParamList& plist)
{
   int maxRes = 35;

   // get the parameters

   string resType;
   getParamValue(plist, "dggs_res_specify_type", resType, false);

   if (resType == string("SPECIFIED"))
   {
      getParamValue(plist, "dggs_res_spec", res, false);
   }
   else
   {
      bool area;
      long double value = 0;
      if (resType == string("CELL_AREA"))
      {
         area = true;
         getParamValue(plist, "dggs_res_specify_area", value, false);
      }
      else
      {
         area = false;
         getParamValue(plist, "dggs_res_specify_intercell_distance", value, 
                       false);
      }

      bool roundDown = true;  // round up/down to nearest available cell size
      getParamValue(plist, "dggs_res_specify_rnd_down", roundDown, false);

      // determine the resolution

      DgRFNetwork net0;
      DgGeoSphRF geoRF(net0, "GS0", earthRadius);
      const DgIDGGS *idggs = DgIDGGS::makeRF(net0, geoRF, vert0,
             azimuthDegs, aperture, maxRes, gridTopo, "IDGGS",
             projType, isMixed43, numAp4, isSuperfund);

      long double last = 0.0;
      res = maxRes + 1;
      for (int i = 1; i <= maxRes; i++) 
      {
         const DgGridStats& gs = idggs->idgg(i).gridStats();
         long double next = (area) ? gs.cellAreaKM() : gs.cellDistKM();

         if (value == next)
         {
            res = i;
            break;
         }

         if (value < last && value > next)
         {
            if (roundDown) res = i;
            else res = i - 1;
            break;
         }

         last = next;
      }

      cout << "** choosing grid resolution: " << res << endl;
   }

   if (res > maxRes)
   {
      ::report("MainParam::determineRes() desired resolution exceeds "
               "maximum resolution for this topology", DgBase::Fatal);
   }
   else if (res < 0) res = 0;

} // int MainParam::determineRes

/* JFW: I have a feeling that the new-ed pointers here are never freed... */
MainParam::MainParam (DgParamList& plist)
      : aperture (4), projType ("ISEA"), res (5), actualRes (5), 
        placeRandom (false), orientCenter (false), orientRand(0),
        numGrids (1), curGrid (1), lastGrid (false), azimuthDegs (0.0),
        precision (DEFAULT_PRECISION), verbosity (0), megaVerbose (false), 
        isMixed43 (false), isSuperfund (false)
{ 
   /////// fill state variables from the parameter list //////////

   getParamValue(plist, "dggrid_operation", operation, false);

   string dummy;
   getParamValue(plist, "rng_type", dummy, false);

   if (dummy == "MOTHER") useMother = true;
   else useMother = false;

   // setup preset DGGS (if any)
   string tmp;
   getParamValue(plist, "dggs_type", tmp, false);
   string tmplc = toLower(tmp);
   if (tmplc != "custom")
   {
      // these params are common to all presets
      plist.setPresetParam("dggs_base_poly", "ICOSAHEDRON");
      plist.setPresetParam("dggs_orient_specify_type", "SPECIFIED");
      plist.setPresetParam("dggs_num_placements", "1");
      plist.setPresetParam("dggs_vert0_lon", "11.25");
      plist.setPresetParam("dggs_vert0_lat", "58.28252559");
      plist.setPresetParam("dggs_vert0_azimuth", "0.0");
      plist.setPresetParam("dggs_res_specify_type", "SPECIFIED");
      plist.setPresetParam("dggs_res_spec", "9");

      if (tmplc == "superfund")
      {
         plist.setPresetParam("dggs_topology", "HEXAGON");
         plist.setPresetParam("dggs_proj", "FULLER");
         plist.setPresetParam("dggs_num_aperture_4_res", "2");
         plist.setPresetParam("dggs_aperture_type", "MIXED43");
         plist.setPresetParam("output_cell_label_type", "SUPERFUND");
      }
      else
      {
         // get the topology
         char topo = tmplc[tmplc.length() - 1];
         switch (topo)
         {
            case 'h':
               plist.setPresetParam("dggs_topology", "HEXAGON");
               break;
            case 't':
               plist.setPresetParam("dggs_topology", "TRIANGLE");
               break;
            case 'd':
               plist.setPresetParam("dggs_topology", "DIAMOND");
               break;
         }

         // get the projection
         int projLen;

         if (!tmplc.compare(0, 4, "isea"))
         {
            plist.setPresetParam("dggs_proj", "ISEA");
            projLen = string("isea").length();
         }
         else // must be FULLER
         {
            plist.setPresetParam("dggs_proj", "FULLER");
            projLen = string("fuller").length();
         }

         // get the aperture
         tmplc = tmplc.substr(projLen, tmplc.length() - projLen - 1);
         if (tmplc == "43")
         {
            plist.setPresetParam("dggs_aperture_type", "MIXED43");
         }
         else
         {
            plist.setPresetParam("dggs_aperture_type", "PURE");
            if (tmplc == "4")
               plist.setPresetParam("dggs_aperture", "4");
            else // must be 3
               plist.setPresetParam("dggs_aperture", "3");
         }
      }
   }

   getParamValue(plist, "dggs_topology", gridTopo, false);
   getParamValue(plist, "dggs_aperture_type", apertureType, false);
   if (apertureType == "MIXED43") 
      isMixed43 = true;

   if (!isMixed43)
      getParamValue(plist, "dggs_aperture", aperture, false);
   else
      getParamValue(plist, "dggs_num_aperture_4_res", numAp4, false);

   getParamValue(plist, "dggs_num_placements", numGrids, false);
   getParamValue(plist, "dggs_proj", projType, false);
   getParamValue(plist, "dggs_vert0_azimuth", azimuthDegs, false);

   long double lon0, lat0;
   getParamValue(plist, "dggs_vert0_lon", lon0, false);
   getParamValue(plist, "dggs_vert0_lat", lat0, false);
   vert0 = DgGeoCoord(lon0, lat0, false);

   getParamValue(plist, "proj_datum", datum, false);
   if (datum == "WGS84_AUTHALIC_SPHERE")
      earthRadius = 6371.007180918475L;
   else if (datum == "WGS84_MEAN_SPHERE")
      earthRadius = 6371.0087714L;
   else // datum must be CUSTOM_SPHERE
      getParamValue(plist, "proj_datum_radius", earthRadius, false);

   if (tmp == "SUPERFUND")
   {
      isSuperfund = true;

      if (!isMixed43)
         ::report("MainParam::MainParam() Superfund grid requires "
             "dggs_aperture_type of MIXED43", DgBase::Fatal);

      if (numAp4 != 2)
         ::report("MainParam::MainParam() Superfund grid requires "
             "dggs_num_aperture_4_res of 2", DgBase::Fatal);

      string resType;
      getParamValue(plist, "dggs_res_specify_type", resType, false);

      if (resType != string("SPECIFIED"))
         ::report("MainParam::MainParam() Superfund grid requires "
             "dggs_res_specify_type of SPECIFIED", DgBase::Fatal);

      getParamValue(plist, "dggs_res_spec", res, false);
      sfRes = res;
      actualRes = res = sfRes2actualRes(sfRes);
   }
   else // not superfund
   {
      sfRes = 0;
      determineRes(plist);
      actualRes = res;
   }

   getParamValue(plist, "precision", precision, false);

   getParamValue(plist, "verbosity", verbosity, false);
   if (verbosity > 2) 
   {
      megaVerbose = true;
      DgConverterBase::setTraceOn(true);
   }

   getParamValue(plist, "dggs_orient_output_file_name", metaOutFileNameBase, 
                 false);
   getParamValue(plist, "dggs_orient_specify_type", dummy, false);
   if (dummy == string("SPECIFIED"))
      placeRandom = false;
   else if (dummy == string("REGION_CENTER")) 
   {
      placeRandom = false;
      orientCenter = true;
   }
   else 
   {
      placeRandom = true;

      unsigned long int ranSeed = 0;
      getParamValue(plist, "dggs_orient_rand_seed", ranSeed, false);
      if (useMother)
      {
         orientRand = new DgRandMother(ranSeed);
      }
      else
      {
         orientRand = new DgRand(ranSeed);
      }
   }

} // MainParam::MainParam

MainParam::~MainParam()
{
 delete orientRand; 
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void MainParam::dump (void)
{
   cout << "BEGIN MAIN PARAMETER DUMP" << endl;
   
   cout << " dggsType: " << dggsType << endl;
   cout << " curGrid: " << curGrid << endl;
   cout << " lastGrid: " << lastGrid << endl;
   cout << " numGrids: " << numGrids << endl;
   cout << " gridTopo: " << gridTopo << endl;
   cout << " aperture: " << aperture << endl;
   cout << " projType: " << projType << endl;
   cout << " res: " << res << endl;
   cout << " superfund res: " << sfRes << endl;
   cout << " numAp4: " << numAp4 << endl;
   cout << " actualRes: " << actualRes << endl;
   cout << " placeRandom: " << placeRandom << endl;
   cout << " orientCenter: " << orientCenter << endl;
   cout << " vert0: " << vert0 << endl;
   cout << " azimuthDegs: " << azimuthDegs << endl;
   cout << " earthRadius: " << earthRadius << endl;
   cout << " precision: " << precision << endl;
   cout << " verbosity: " << verbosity << endl;
   cout << " megaVerbose: " << megaVerbose << endl;

   cout << " *orientRand: ";
   if (orientRand)
      cout << *orientRand << endl;
   else
      cout << "null" << endl; 

   cout << " metaOutFileNameBase: " << metaOutFileNameBase << endl;

   cout << "END MAIN PARAMETER DUMP" << endl;

} // void MainParam::dump

/* JFW: We should check to see if the new-ed pointers here are freed:
	(I doubt we really need a vector of pointers to strings.) */
DgGridPList::DgGridPList (void)
{
   vector<string*> choices;

   // dggrid_operation <GENERATE_GRID | BIN_POINT_VALS | BIN_POINT_PRESENCE |
   //                   TRANSFORM_POINTS | OUTPUT_STATS>
   choices.push_back(new string("GENERATE_GRID"));
   choices.push_back(new string("BIN_POINT_VALS"));
   choices.push_back(new string("BIN_POINT_PRESENCE"));
   choices.push_back(new string("TRANSFORM_POINTS"));
   choices.push_back(new string("OUTPUT_STATS"));
   insertParam(new DgStringChoiceParam("dggrid_operation", "GENERATE_GRID", 
               &choices));
   dgg::util::release(choices);

   // dggs_type <CUSTOM | SUPERFUND | 
   //            ISEA3H | ISEA4H | ISEA43H | ISEA4T | ISEA4D |
   //            FULLER3H | FULLER4H | FULLER43H | FULLER4T | FULLER4D>
   choices.push_back(new string("CUSTOM"));
   choices.push_back(new string("SUPERFUND"));
   choices.push_back(new string("ISEA3H")); 
   choices.push_back(new string("ISEA4H")); 
   choices.push_back(new string("ISEA43H")); 
   choices.push_back(new string("ISEA4T")); 
   choices.push_back(new string("ISEA4D")); 
   choices.push_back(new string("FULLER3H")); 
   choices.push_back(new string("FULLER4H")); 
   choices.push_back(new string("FULLER43H")); 
   choices.push_back(new string("FULLER4T")); 
   choices.push_back(new string("FULLER4D"));
   insertParam(new DgStringChoiceParam("dggs_type", "CUSTOM", &choices));
   dgg::util::release(choices);

   // dggs_base_poly <ICOSAHEDRON>
   choices.push_back(new string("ICOSAHEDRON"));
   insertParam(new DgStringChoiceParam("dggs_base_poly", "ICOSAHEDRON", 
               &choices));
   dgg::util::release(choices);

   // dggs_topology <HEXAGON | TRIANGLE | DIAMOND>
   choices.push_back(new string("HEXAGON"));
   choices.push_back(new string("TRIANGLE"));
   choices.push_back(new string("DIAMOND"));
   insertParam(new DgStringChoiceParam("dggs_topology", "HEXAGON", &choices));
   dgg::util::release(choices);

   // dggs_proj <ISEA | FULLER | GNOMONIC>
   choices.push_back(new string("ISEA"));
   choices.push_back(new string("FULLER"));
   //choices.push_back(new string("GNOMONIC"));
   insertParam(new DgStringChoiceParam("dggs_proj", "ISEA", &choices));
   dgg::util::release(choices);

   // dggs_aperture_type:
   choices.push_back(new string("PURE"));
   choices.push_back(new string("MIXED43"));
   insertParam(new DgStringChoiceParam("dggs_aperture_type", "PURE", 
             &choices));
   dgg::util::release(choices);

   // dggs_aperture <int>
   insertParam(new DgIntParam("dggs_aperture", 4, 3, 4));

   // dggs_num_aperture_4_res
   insertParam(new DgIntParam("dggs_num_aperture_4_res", 0, 0, 35));

   // proj_datum <WGS84_AUTHALIC_SPHERE WGS84_MEAN_SPHERE CUSTOM_SPHERE>
   choices.push_back(new string("WGS84_AUTHALIC_SPHERE"));
   choices.push_back(new string("WGS84_MEAN_SPHERE"));
   choices.push_back(new string("CUSTOM_SPHERE"));
   insertParam(new DgStringChoiceParam("proj_datum", 
               "WGS84_AUTHALIC_SPHERE", &choices));
   dgg::util::release(choices);

   // proj_datum_radius <long double: km> (1.0 <= v <= 10000.0)
   insertParam(new DgDoubleParam("proj_datum_radius", DEFAULT_RADIUS_KM,
               1.0, 10000.0));

   //// specify the position and orientation

   // dggs_orient_specify_type <RANDOM | SPECIFIED | REGION_CENTER>
   choices.push_back(new string("RANDOM"));
   choices.push_back(new string("SPECIFIED"));
   choices.push_back(new string("REGION_CENTER"));
   insertParam(new DgStringChoiceParam("dggs_orient_specify_type", "SPECIFIED", 
               &choices));
   dgg::util::release(choices);

   // dggs_num_placements <int> (v >= 1)
   insertParam(new DgIntParam("dggs_num_placements", 1, 1, std::numeric_limits<int>::max(), true));

   // dggs_orient_output_file_name <whatever.meta>
   insertParam(new DgStringParam("dggs_orient_output_file_name", 
                     "grid.meta"));

   // dggs_orient_rand_seed <unsigned long int int>
   insertParam(new DgULIntParam("dggs_orient_rand_seed", 77316727, 0,
                     std::numeric_limits<unsigned long>::max(), true));

   // dggs_vert0_lon <long double: decimal degrees> (-180.0 <= v <= 180.0)
   insertParam(new DgDoubleParam("dggs_vert0_lon", 11.25, -180.0, 180.0));

   // dggs_vert0_lat <long double: decimal degrees> (-90.0 <= v <= 90.0)
   insertParam(new DgDoubleParam("dggs_vert0_lat", 58.28252559, -90.0, 90.0));

   // dggs_vert0_azimuth <long double: decimal degrees> (0.0 <= v < 360.0)
   insertParam(new DgDoubleParam("dggs_vert0_azimuth", 0.0, 0.0, 360.0));

   // region_center_lon <long double: decimal degrees> (-180.0 <= v <= 180.0)
   insertParam(new DgDoubleParam("region_center_lon", 0.0, -180.0, 180.0));

   // region_center_lat <long double: decimal degrees> (-90.0 <= v <= 90.0)
   insertParam(new DgDoubleParam("region_center_lat", 0.0, -90.0, 90.0));

   // dggs_res_specify_type <SPECIFIED | CELL_AREA | INTERCELL_DISTANCE>
   choices.push_back(new string("SPECIFIED"));
   choices.push_back(new string("CELL_AREA"));
   choices.push_back(new string("INTERCELL_DISTANCE"));
   insertParam(new DgStringChoiceParam("dggs_res_specify_type", "SPECIFIED", 
               &choices));
   dgg::util::release(choices);

   // dggs_res_specify_area <long double: km^2> (v > 0.0)
   insertParam(new DgDoubleParam("dggs_res_specify_area", 100.0, 
                                 0.0, 4.0 * dgM_PI * 6500.0 * 6500.0));

   // dggs_res_specify_intercell_distance <long double: km> (v > 0.0)
   insertParam(new DgDoubleParam("dggs_res_specify_intercell_distance", 100.0, 
                                 0.0, 2.0 * dgM_PI * 6500.0));

   // dggs_res_specify_rnd_down <TRUE ! FALSE> (true indicates round down, false up)
   insertParam(new DgBoolParam("dggs_res_specify_rnd_down", true));
 
   // dggs_res_spec <int> (0 <= v <= 35)
   insertParam(new DgIntParam("dggs_res_spec", 9, 0, 35));

   // rng_type <RAND | MOTHER>
   choices.push_back(new string("RAND"));
   choices.push_back(new string("MOTHER"));
   insertParam(new DgStringChoiceParam("rng_type", "RAND", &choices));
   dgg::util::release(choices);

   // geodetic_densify <long double: decimal degrees> (v >= 0.0)
   insertParam(new DgDoubleParam("geodetic_densify", 0.0, 0.0, 360.0));

   // clip_subset_type <WHOLE_EARTH | AIGEN | SHAPEFILE>
   choices.push_back(new string("WHOLE_EARTH"));
   choices.push_back(new string("AIGEN"));
   choices.push_back(new string("SHAPEFILE"));
   choices.push_back(new string("SEQTOPOLY"));
   insertParam(new DgStringChoiceParam("clip_subset_type", "WHOLE_EARTH", 
               &choices));
   dgg::util::release(choices);

   // clip_region_files <fileName1 fileName2 ... fileNameN>
   insertParam(new DgStringParam("clip_region_files", "test.gen"));

   // clip_type <POLY_INTERSECT>
   choices.push_back(new string("POLY_INTERSECT"));
   insertParam(new DgStringChoiceParam("clip_type", "POLY_INTERSECT", 
               &choices));
   dgg::util::release(choices);

   init2();

} // DgGridPList::DgGridPList

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgGridPList::init2 (void)
//
// This method was created solely because gcc optimiziation choked when 
// everything was in the constructor.
//
{
   vector<string*> choices;

   // densification <int> (0 <= v <= 500)
   insertParam(new DgIntParam("densification", 0, 0, 500));

   // precision <int> (0 <= v <= 30)
   insertParam(new DgIntParam("precision", DEFAULT_PRECISION, 0, std::numeric_limits<int>::max()));

   // output_cell_label_type <GLOBAL_SEQUENCE | ENUMERATION | SUPERFUND>
   choices.push_back(new string("GLOBAL_SEQUENCE"));
   choices.push_back(new string("ENUMERATION"));
   choices.push_back(new string("SUPERFUND"));
   insertParam(new DgStringChoiceParam("output_cell_label_type", "GLOBAL_SEQUENCE", 
               &choices));
   dgg::util::release(choices);

   ////// output parameters //////

   // cell_output_type <NONE | AIGEN | KML | GEOJSON | SHAPEFILE>
   choices.push_back(new string("NONE"));
   choices.push_back(new string("AIGEN"));
   choices.push_back(new string("KML"));
   choices.push_back(new string("GEOJSON"));
   choices.push_back(new string("SHAPEFILE"));
   insertParam(new DgStringChoiceParam("cell_output_type", "AIGEN", 
               &choices));
   dgg::util::release(choices);

   // point_output_type <NONE | AIGEN | KML | GEOJSON | SHAPEFILE | TEXT>
   choices.push_back(new string("NONE"));
   choices.push_back(new string("AIGEN"));
   choices.push_back(new string("KML"));
   choices.push_back(new string("GEOJSON"));
   choices.push_back(new string("SHAPEFILE"));
   choices.push_back(new string("TEXT"));
   insertParam(new DgStringChoiceParam("point_output_type", "NONE", 
               &choices));
   dgg::util::release(choices);

   // randpts_output_type <NONE | AIGEN | KML | GEOJSON | SHAPEFILE | TEXT>
   choices.push_back(new string("NONE"));
   choices.push_back(new string("AIGEN"));
   choices.push_back(new string("KML"));
   choices.push_back(new string("SHAPEFILE"));
   choices.push_back(new string("GEOJSON"));
   choices.push_back(new string("TEXT"));
   insertParam(new DgStringChoiceParam("randpts_output_type", "NONE", 
               &choices));
   dgg::util::release(choices);

   // cell_output_file_name <outputFileName>
   insertParam(new DgStringParam("cell_output_file_name", "cells"));

   // point_output_file_name <outputFileName>
   insertParam(new DgStringParam("point_output_file_name", "centers"));

   // randpts_output_file_name <outputFileName>
   insertParam(new DgStringParam("randpts_output_file_name", "randPts"));

   // shapefile_id_field_length <int> (1 <= v <= 50)
   insertParam(new DgIntParam("shapefile_id_field_length", 11, 1, 50));

   // kml_default_width <int> (1 <= v <= 100)
   insertParam(new DgIntParam("kml_default_width", 
               DgOutLocFile::defaultKMLWidth, 1, 100));

   // kml_default_color <string> <hexadecmial color>
   insertParam(new DgStringParam("kml_default_color", 
               DgOutLocFile::defaultKMLColor));

   // kml_name <string> <string name>
   insertParam(new DgStringParam("kml_name", 
               DgOutLocFile::defaultKMLName));

   // kml_description <string> <string description>
   insertParam(new DgStringParam("kml_description", 
               DgOutLocFile::defaultKMLDescription));

   ///// additional random points parameters /////

   // randpts_concatenate_output <TRUE ! FALSE> 
   insertParam(new DgBoolParam("randpts_concatenate_output", true));

   // randpts_num_per_cell <int> (v >= 0)
   insertParam(new DgIntParam("randpts_num_per_cell", 0, 0, std::numeric_limits<int>::max()));

   // randpts_seed <unsigned long int>
   insertParam(new DgULIntParam("randpts_seed", 77316727, 0, std::numeric_limits<unsigned long>::max(), true));

/*
   // clip_randpts <TRUE | FALSE>
   insertParam(new DgBoolParam("clip_randpts", true));

   // randpts_clipped_file_name <outputFileName>
   insertParam(new DgStringParam("randpts_clipped_file_name", "clippedRandPts"));
*/

/* // quad_bndry_nudge <long double> (0.0 <= v < 1.0) 
   insertParam(new DgDoubleParam("quad_bndry_nudge", 0.001, 0.0, 1.0));
*/
/*** shapefile attribute stuff
   ///// shapefile attribute parameters
   // build_shapefile_attributes <TRUE | FALSE>
   insertParam(new DgBoolParam("build_shapefile_attributes", false));

   // shapefile_attribute_default_int <int>
   insertParam(new DgIntParam("shapefile_attribute_default_int", 0, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));

   // shapefile_attribute_default_double <double>
   insertParam(new DgDoubleParam("shapefile_attribute_default_double", 0.0, -DBL_MIN, DBL_MAX));

   // shapefile_attribute_default_string <defaultString>
   insertParam(new DgStringParam("shapefile_attribute_default_string", "X"));

   // build_shapefile_attributes_source <NONE | CLIP_FILES | SHAPEFILES>
   choices.push_back(new string("NONE"));
   choices.push_back(new string("CLIP_FILES"));
   choices.push_back(new string("SHAPEFILES"));
   insertParam(new DgStringChoiceParam("build_shapefile_attributes_source", 
               "NONE", &choices));
   dgg::util::release(choices);

   // attribute_files <fileName1 fileName2 ... fileNameN>
   insertParam(new DgStringParam("attribute_files", "test.shp"));
****/

   //  update_frequency <int> (v >= 0)
   insertParam(new DgULIntParam("update_frequency", 100000, 0, std::numeric_limits<unsigned long>::max(), true));

   //  max_cells_per_output_file <int> (v >= 0)
   insertParam(new DgULIntParam("max_cells_per_output_file", 0, 0, std::numeric_limits<unsigned long>::max(), true));

   //  verbosity <int> (0 <= v <= 3)
   insertParam(new DgIntParam("verbosity", 0, 0, 3));

   ///// binvals parameters /////

   // bin_method <ARITHMETIC_MEAN>
   choices.push_back(new string("ARITHMETIC_MEAN"));
   insertParam(new DgStringChoiceParam("bin_method", "ARITHMETIC_MEAN", 
                                       &choices));
   dgg::util::release(choices);

   // bin_coverage <GLOBAL | PARTIAL>
   choices.push_back(new string("GLOBAL"));
   choices.push_back(new string("PARTIAL"));
   insertParam(new DgStringChoiceParam("bin_coverage", "GLOBAL", &choices));
   dgg::util::release(choices);

   // input_files <fileName1 fileName2 ... fileNameN>
   insertParam(new DgStringParam("input_files", "vals.txt"));

   // input_delimiter <v is any character in long double quotes>
   insertParam(new DgStringParam("input_delimiter", " "));

   // output_file_name <fileName>

   insertParam(new DgStringParam("output_file_name", "valsout.txt"));

   // output_address_type <GEO | Q2DI | SEQNUM | INTERLEAVE | PLANE | Q2DD | 
   //                      PROJTRI | VERTEX2DD | AIGEN >
   choices.push_back(new string("GEO")); 
   choices.push_back(new string("Q2DI")); 
   choices.push_back(new string("SEQNUM")); 
   //choices.push_back(new string("INTERLEAVE")); 
   choices.push_back(new string("PLANE")); 
   choices.push_back(new string("Q2DD")); 
   choices.push_back(new string("PROJTRI")); 
   choices.push_back(new string("VERTEX2DD"));
   choices.push_back(new string("AIGEN"));
   insertParam(new DgStringChoiceParam("output_address_type", 
               "SEQNUM", &choices));
   dgg::util::release(choices);

   // output_delimiter <v is any character in double quotes>
   insertParam(new DgStringParam("output_delimiter", " "));

   // cell_output_control <OUTPUT_ALL | OUTPUT_OCCUPIED>
   choices.push_back(new string("OUTPUT_ALL")); 
   choices.push_back(new string("OUTPUT_OCCUPIED"));
   insertParam(new DgStringChoiceParam("cell_output_control", "OUTPUT_ALL", 
                                       &choices));
   dgg::util::release(choices);

   ///// transform parameters (that haven't already been inserted) /////

   // input_file_name <fileName>
   insertParam(new DgStringParam("input_file_name", "valsin.txt"));

   // input_address_type <GEO | Q2DI | SEQNUM | INTERLEAVE | PLANE | Q2DD | 
   //                      PROJTRI | VERTEX2DD>
   choices.push_back(new string("GEO")); 
   choices.push_back(new string("Q2DI")); 
   choices.push_back(new string("SEQNUM")); 
   //choices.push_back(new string("INTERLEAVE")); // NOT IMPLEMENTED
   //choices.push_back(new string("PLANE")); 
   choices.push_back(new string("Q2DD")); 
   choices.push_back(new string("PROJTRI")); 
   choices.push_back(new string("VERTEX2DD"));
   insertParam(new DgStringChoiceParam("input_address_type", 
               "SEQNUM", &choices));
   dgg::util::release(choices);

   // output_count <TRUE | FALSE> 
   insertParam(new DgBoolParam("output_count", true));
   
} // DgGridPList::init2

