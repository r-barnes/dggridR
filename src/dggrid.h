#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//
// dggrid.h: this file defines the classes (effectively structures) which hold
//           the parameter info
//
// Version 6.1 - Kevin Sahr, 5/23/13
// Version 6.2 - Kevin Sahr, 10/25/14
// Version 7.0 - Kevin Sahr, 9/1/19
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGGRID_H
#define DGGRID_H

#include "DgParamList.h"
#include "DgEllipsoidRF.h"
#include "DgRandom.h"
#include "DgOutLocFile.h"
#include "DgIDGG.h"
#include "DgInShapefileAtt.h"
#include "DgApSeq.h"
#include "util.h"
#include "DgRunningStats.h"
#include "DgGridTopo.h"

#include <iostream>
#include <set>

using namespace std;

class DggridParam;
class DgEvalData;
class DgOutShapefile;
class DgOutPRCellsFile;
class DgOutPRPtsFile;
class DgOutNeighborsFile;
class DgOutChildrenFile;
class DgOutputStream;

// constants
#define MAX_DGG_RES 35

////////////////////////////////////////////////////////////////////////////////
//// function prototypes ////

// gridgen.cpp

class MainParam;
class DgGridPList;
void orientGrid (MainParam& dp, DgGridPList& plist);

// gridgen.cpp

class GridGenParam;
class DgGridPList;
void outputStatus (const GridGenParam& dp, bool force = false);
void doGridGen (GridGenParam& dp, DgGridPList& plist);

// binvals.cpp

class BinValsParam;
void doBinVals (BinValsParam& dp, DgGridPList& plist);

// binpres.cpp

class BinPresenceParam;
void doBinPresence (BinPresenceParam& dp, DgGridPList& plist);

// transform.cpp

class TransformParam;
void doTransforms (TransformParam& dp, DgGridPList& plist);

// table.cpp

class MainParam;
void doTable (MainParam& dp, DgGridPList& plist);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//// the parameter structures ////
// no attempt to be subtle here; these contain the "global" parameters for
// the specified dggrid operation

// common parameters

class MainParam {

   public:

      MainParam (DgParamList& plist);

      virtual ~MainParam();

      void dump (void);   // output to cout

      // the parameters

      string operation;             // operation for dggrid to perform
      string dggsType;              // preset DGGS type
      dgg::topo::DgGridTopology gridTopo;      // Diamond/Hexagon/Triangle
      dgg::topo::DgGridMetric   gridMetric;    // D4/D8
      int aperture;      // aperture
      string projType;              // projection type
      int res;           // resolution (may be adjusted)
      int actualRes;     // original, actual resolution
      bool placeRandom;             // random grid placement?
      bool orientCenter;            // grid placement based on quad center point?
      DgRandom* orientRand;             // RNG for generating random orientation
      int numGrids;      // number of grids to generate
      int curGrid;       // grid counter
      bool lastGrid;                    // last grid?
      DgGeoCoord vert0;                 // placement vert
      long double azimuthDegs;               // orientation azimuth
      long double earthRadius;               // earth radius in km
      string datum;               // datum used to determine the earthRadius
      int precision;     // number of digits after decimal pt to output
      int verbosity;     // debugging info verbosity
      bool megaVerbose;                 //
      bool pauseOnStart;
      bool pauseBeforeExit;
      bool useMother;                   // use Mother RNG?
      string metaOutFileNameBase;
      string metaOutFileName;
      string apertureType;		// "PURE", "MIXED43", "SUPERFUND", or "SEQUENCE"
      bool   isMixed43;		// are we using mixed43 aperture?
      int numAp4;      // # of leading ap 4 resolutions in a mixed grid
      bool   isSuperfund;
      bool   isApSeq;		// are we using an aperture sequence?
      DgApSeq apSeq;
      int   sfRes; // superfund digit resolution

   protected:

      void determineRes (const DgParamList& plist);
      DgGridStats genStats (int res);

};

////////////////////////////////////////////////////////////////////////////////
// grid generation parameter structure

class GridGenParam : public MainParam {

   public:

      GridGenParam (DgParamList& plist);
      ~GridGenParam ();

      void dump (void);   // output to cout

      // the parameters

      bool wholeEarth;       // generate entire grid?
      bool regionClip;       // whether user wants to generate using regions
      bool seqToPoly;        // whether user wants polys from seqnum
      bool pointClip;        // whether user wants to generate using points
      bool cellClip;         // whether user wants to generate using coarse cells
      bool useGDAL;          // use GDAL for either input or output
      bool clipAIGen;        // clip using AIGen files (or Shapefiles)
      bool clipGDAL;         // clip using GDAL files
      bool clipShape;        // clip using Shapefiles
      vector<string> regionFiles;
      int clipCellRes;       // resolution of the clipping cell indexes
      set<unsigned long int> clipSeqNums; // coarse clipping cells
      int nClipCellDensify;  // number of points-per-edge of densification for clipping cells
      int nRandPts;          // # of random pts generated for each hex
      bool clipRandPts;      // clip randpts to polys
      int nDensify;          // number of points-per-edge of densification
      DgGeoSphRF::DgLonWrapMode lonWrapMode; 
                  // how to handle outputting cells that straddle the anti-meridian
      bool unwrapPts;        // unwrap point longitude to match cell boundary
      long double nudge;     // adjustment for quad intersection consistency
      DgRandom* ptsRand;     // RNG for generating random points

      string cellOutType;
      string gdalCellDriver;
      string pointOutType;
      string gdalPointDriver;
      string randPtsOutType;
      string gdalCollectDriver;

      string neighborsOutType;
      string childrenOutType;
      string neighborsOutFileNameBase;
      string neighborsOutFileName;
      string childrenOutFileNameBase;
      string childrenOutFileName;

      string cellOutFileNameBase;
      string cellOutFileName;
      string ptOutFileNameBase;
      string ptOutFileName;
      string collectOutFileNameBase;
      string collectOutFileName;
      string randPtsOutFileNameBase;
      string randPtsOutFileName;
      int    shapefileIdLen;  // global_id string field length

      string kmlColor;
      int    kmlWidth;
      string kmlName;
      string kmlDescription;

      bool doPointInPoly;             // perform pt-in-poly intersection
      bool doPolyIntersect;           // perform poly intersection
      long long int sampleCount; // last sample point sequence number
      unsigned long long int nSamplePts;
      bool doRandPts;                 // generate random points for the cells
      unsigned long int clipperFactor; // clipper scaling factor
      long double invClipperFactor; // 1.0L / clipper scaling factor
      bool useHoles; // handle holes in clipping polygons

      DgOutLocFile *cellOut, *ptOut, *collectOut, *randPtsOut;
      DgOutShapefile *cellOutShp, *ptOutShp;
      DgOutPRCellsFile *prCellOut;
      DgOutPRPtsFile *prPtOut;
      DgOutNeighborsFile *nbrOut;
      DgOutChildrenFile *chdOut;

      DgRunningStats runStats;

      bool concatPtOut;
      char formatStr[50];
      bool useEnumLbl;
      unsigned long long int nCellsTested;
      unsigned long long int nCellsAccepted;
      unsigned long long int nCellsOutputToFile; // cells output to current file
      unsigned long int nOutputFile; // # of current output file

      unsigned long int updateFreq; // how often to output updates
      unsigned long int maxCellsPerFile; // max cells in a single output file
      unsigned long int outFirstSeqNum;  // start generating with this seqnum
      unsigned long int outLastSeqNum;   //  generate through this one

      long double geoDens;          // max arc length in radians

      bool buildShapeFileAttributes; // create fields for shapefile output
      bool buildClipFileAttributes;  // use clipping shape files (vs. others)

      int shapefileDefaultInt;
      long double shapefileDefaultDouble;
      string shapefileDefaultString;

      vector<string> attributeFiles;
      bool outCellAttributes; // true if cell output file is a shapefile
      bool outPointAttributes; // true if point output file is a shapefile
      set<DgDBFfield> allFields; // union of all input file attribute fields
      set<DgDBFfield> curFields; // union of all intersected fields for the
                                 // current cell
};

////////////////////////////////////////////////////////////////////////////////
// bin point values parameter structure

class BinValsParam : public MainParam {

   public:

      BinValsParam (DgParamList& plist);

      ~BinValsParam ();

      void dump (void);   // output to cout

      // the parameters

      bool wholeEarth;
      string outFileName;
      string outFileNameBase;
      ofstream* outFile;
      vector<string> inputFiles;
      string outAddType;
      bool outSeqNum;
      char inputDelimiter;
      char outputDelimiter;
      string inFormatStr;
      bool outputAllCells;   // or only occupied ones?
};

////////////////////////////////////////////////////////////////////////////////
// bin point presence parameter structure

class BinPresenceParam : public MainParam {

   public:

      BinPresenceParam (DgParamList& plist);

      ~BinPresenceParam ();

      void dump (void);   // output to cout

      // the parameters

      bool wholeEarth;
      string outFileName;
      string outFileNameBase;
      ofstream* outFile;
      vector<string> inputFiles;
      string outAddType;
      bool outSeqNum;
      char inputDelimiter;
      char outputDelimiter;
      string inFormatStr;
      bool outputAllCells;   // or only occupied ones?
      bool outputCount;

};

////////////////////////////////////////////////////////////////////////////////
// transform addresses parameter structure

class TransformParam : public MainParam {

   public:

      TransformParam (DgParamList& plist);

      void dump (void);   // output to cout

      // the parameters

      string inFileName;
      string inAddType;

      bool inSeqNum;
      bool outSeqNum;
      char inputDelimiter;
      char outputDelimiter;

      string outFileName;
      string outFileNameBase;
      string outFileNameAIG;
      string outFileNameKML;
      string outAddType;

      int nDensify;
      DgGeoSphRF::DgLonWrapMode lonWrapMode; 
      bool unwrapPts; 
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgGridPList : public DgParamList {

   public:

      DgGridPList (void);

   private:

      void init2 (void);

};

////////////////////////////////////////////////////////////////////////////////

#endif
