////////////////////////////////////////////////////////////////////////////////
//
// DgProjTriRF.h: DgProjTriRF class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPROJ_TRI_RF_H
#define DGPROJ_TRI_RF_H

#include <climits>
#include <iostream>

#include "DgIVec2D.h"
#include "DgDVec2D.h"
#include "DgContCartRF.h"
#include "DgEllipsoidRF.h"
#include "DgConstants.h"
#include "DgUtil.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgSphIcosa {

   public:

      DgSphIcosa (const DgGeoCoord& vert0 =
                                      DgGeoCoord(11.25L, 58.28252559L, false),
                  long double azimuthDegs = M_ZERO);

      SphIcosa& sphIcosa (void) { return sphIcosa_; }

      /* give the start point and an azimuth, return 12 vertices 
         of the icosahedron */
      void ico12verts(void);

      int whichIcosaTri (const GeoCoord& pt);

   private:

      SphIcosa sphIcosa_;

      friend ostream& operator<< (ostream& stream, const DgSphIcosa& dgsi);
};

////////////////////////////////////////////////////////////////////////////////
ostream& operator<< (ostream& str, const DgSphIcosa& dgsi);

////////////////////////////////////////////////////////////////////////////////
/* transformation between two different pole, (lat,lon) system */
GeoCoord coordtrans(const GeoCoord& newNPold,
                    const GeoCoord& ptold, long double lon0);

////////////////////////////////////////////////////////////////////////////////
class DgProjTriCoord {

   public:

      DgProjTriCoord (int triNumIn = -1, const DgDVec2D& coordIn = DgDVec2D()) 
        : triNum_ (triNumIn), coord_ (coordIn) { }

      int triNum (void) const { return triNum_; }

      const DgDVec2D& coord (void) const { return coord_; }
      
      void setTriNum  (int triNumIn) { triNum_ = triNumIn; }
      void setCoord   (const DgDVec2D& coordIn) { coord_ = coordIn; }

      operator string (void) const
          { return string("{ triNum: " + dgg::util::to_string(triNum())
                      + ", coord: " + string(coord())) + " }"; }

      bool operator== (const DgProjTriCoord& tpc) const
          { return triNum() == tpc.triNum() && coord() == tpc.coord(); }

   private:

      int triNum_;
      DgDVec2D coord_;

};

////////////////////////////////////////////////////////////////////////////////
inline ostream& 
operator<< (ostream& str, const DgProjTriCoord& coord) 
{ return str << string(coord); }

////////////////////////////////////////////////////////////////////////////////
class DgProjTriRF : public DgRF<DgProjTriCoord, long double> {

   public:

      DgProjTriRF (DgRFNetwork& networkIn, const string& nameIn = "ProjTriRF",
                   DgSphIcosa* sphIcosaIn = 0) 
                      //DgSphIcosa(DgGeoCoord(11.25L, 58.28252559L, false), M_ZERO))
         : DgRF<DgProjTriCoord, long double> (networkIn, nameIn),
           sphIcosa_ (sphIcosaIn) { }

      //virtual DgLocVector& convert (DgLocVector& vec) const;

      virtual long double dist (const DgProjTriCoord& add1,
                           const DgProjTriCoord& add2) const
                { return M_ZERO; }

      virtual string add2str (const DgProjTriCoord& add) const
                       { return string(add); }

      virtual string add2str (const DgProjTriCoord& add, char delimiter) const
          { return dgg::util::to_string(add.triNum()) + delimiter + 
                   dgg::util::to_string(add.coord().x(), formatStr()) + delimiter +
                   dgg::util::to_string(add.coord().y(), formatStr()); }

      virtual const char* str2add (DgProjTriCoord* add, const char* str, 
                                   char delimiter) const;

      virtual const DgProjTriCoord& undefAddress (void) const
                { static DgProjTriCoord undef; return undef; }

      virtual string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual unsigned long long int dist2int (const long double& dist) const
                       { return (unsigned long long int) dist; }

      DgSphIcosa& sphIcosa (void) const { return *sphIcosa_; }

   private:

      DgSphIcosa* sphIcosa_;

};

#endif
