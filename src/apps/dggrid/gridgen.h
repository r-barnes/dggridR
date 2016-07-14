////////////////////////////////////////////////////////////////////////////////
//
// gridgen.h: gridgen class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GRIDGEN_H
#define GRIDGEN_H

#include <set>
#include <map>
#include "clipper.hpp"
#include "DgIVec2D.h"
#include "DgProjGnomonicRF.h"
#include "DgInShapefileAtt.h"

#define DBL_TO_INT 1000000000000

class GridGenParam;
class DgIDGG;
class DgContCartRF;
class DgDiscRF2D;
class DgContCartRF;

////////////////////////////////////////////////////////////////////////////////
class DgQuadClipRegion {

   public:

      DgQuadClipRegion (void) 
        : isQuadUsed_ (false), gnomProj_ (0), overI_ (false), overJ_ (false) {}

     ~DgQuadClipRegion (void) {}

      //LINESET& region (void) { return region_; }

      vector<ClipperLib::Paths>& clpPolys (void) { return clpPolys_; }
      vector < set<DgDBFfield> >& polyFields (void) { return polyFields_; }

      set<DgIVec2D>& points (void) { return points_; }
      map<DgIVec2D, set<DgDBFfield> >& ptFields (void) { return ptFields_; }

      bool isQuadUsed (void) const { return isQuadUsed_; }
      void setIsQuadUsed (bool isQuadUsedIn) { isQuadUsed_ = isQuadUsedIn; }

      const DgIVec2D& offset (void) const { return offset_; }
      void setOffset (const DgIVec2D& offsetIn) { offset_ = offsetIn; }

      const DgIVec2D& upperRight (void) const { return upperRight_; }
      void setUpperRight (const DgIVec2D& upperRightIn) 
             { upperRight_ = upperRightIn; }

      const DgProjGnomonicRF& gnomProj (void) const { return *gnomProj_; }
      void setGnomProj (DgProjGnomonicRF* gnomProjIn) 
                                  { gnomProj_ = gnomProjIn; }

      ClipperLib::Paths& gnomBndry (void) { return gnomBndry_; }

      bool overI (void) const { return overI_; }
      bool overJ (void) const { return overJ_; }

      void setOverI (const bool overIIn) { overI_ = overIIn; }
      void setOverJ (const bool overJIn) { overJ_ = overJIn; }

   private:

      bool isQuadUsed_; // indicates which quads intersect the region
      
      vector<ClipperLib::Paths> clpPolys_; // clipper region intersection with quads in
                                   // quad Snyder space

      vector < set<DgDBFfield> > polyFields_; // shapefile attribute fields

      set<DgIVec2D> points_; // points that fall on this quad

      map<DgIVec2D, set<DgDBFfield> > ptFields_; // shapefile attribute fields

      //LINESET region_; // region intersection with quads in quad Snyder 
                       // space (Evan's pt-in-poly stuff)

      DgProjGnomonicRF* gnomProj_; // gnomonic proj centered on this quad

      ClipperLib::Paths gnomBndry_; // quad boundary in gnomonic

      DgIVec2D offset_; // offset of min (i, j)
      DgIVec2D upperRight_; // (maxi, maxj) relative to offset

      bool overI_; // is our maxi > maxI (quad overage)
      bool overJ_; // is our maxj > maxJ (quad overage)
};

////////////////////////////////////////////////////////////////////////////////
// DgEvalData class for Superfund
class DgEvalData {

   public:

      GridGenParam& dp;
      const DgIDGG& dgg;
      const DgContCartRF& cc1;
      const DgDiscRF2D& grid; 
      DgQuadClipRegion& clipRegion;
      set<DgIVec2D>& overageSet; 
      map<DgIVec2D, set<DgDBFfield> > overageFields;
      const DgContCartRF& deg;
      const DgIVec2D& lLeft;
      const DgIVec2D& uRight;

      DgEvalData (GridGenParam& dpIn, const DgIDGG& dggIn, 
               const DgContCartRF& cc1In, const DgDiscRF2D& gridIn, 
               DgQuadClipRegion& clipRegionIn, set<DgIVec2D>& overageSetIn,
               map<DgIVec2D, set<DgDBFfield> >& overageFieldsIn,
               const DgContCartRF& degIn, const DgIVec2D& lLeftIn, 
               const DgIVec2D& uRightIn)
      : dp (dpIn), dgg (dggIn), cc1 (cc1In), grid (gridIn), 
               clipRegion (clipRegionIn), overageSet (overageSetIn),
               overageFields (overageFieldsIn),
               deg (degIn), lLeft (lLeftIn), uRight (uRightIn) 
        { }
};

// function prototypes

void outputCell (GridGenParam& dp, const DgIDGG& dgg,
                   const DgLocation& add2D, const DgPolygon& verts,
                   const DgContCartRF& deg, const string& label);

bool evalCell (DgEvalData* data, DgIVec2D& add2D);

bool evalCell (GridGenParam& dp,  const DgIDGG& dgg, const DgContCartRF& cc1,
               const DgDiscRF2D& grid, DgQuadClipRegion& clipRegion,
               const DgIVec2D& add2D);

void createClipRegions (GridGenParam& dp, const DgIDGG& dgg,
                DgQuadClipRegion clipRegions[], set<DgIVec2D> overageSet[],
                map<DgIVec2D, set<DgDBFfield> > overageFields[]);

#endif
