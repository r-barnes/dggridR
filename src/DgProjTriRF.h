#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

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
// DgProjTriRF.h: DgProjTriRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPROJ_TRI_RF_H
#define DGPROJ_TRI_RF_H

#include "DgConstants.h"
#include "DgContCartRF.h"
#include "DgDVec2D.h"
#include "DgEllipsoidRF.h"
#include "DgIVec2D.h"
#include "DgUtil.h"

#include <climits>
#include <iostream>

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

      friend std::ostream& operator<< (std::ostream& stream, const DgSphIcosa& dgsi);
};

////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& str, const DgSphIcosa& dgsi);

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

      operator std::string (void) const
          { return std::string("{ triNum: " + dgg::util::to_string(triNum())
                      + ", coord: " + std::string(coord())) + " }"; }

      bool operator== (const DgProjTriCoord& tpc) const
          { return triNum() == tpc.triNum() && coord() == tpc.coord(); }

   private:

      int triNum_;
      DgDVec2D coord_;

};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& str, const DgProjTriCoord& coord)
{ return str << std::string(coord); }

////////////////////////////////////////////////////////////////////////////////
class DgProjTriRF : public DgRF<DgProjTriCoord, long double> {

   public:

      static const DgProjTriRF* makeRF (DgRFNetwork& networkIn, const std::string& nameIn = "ProjTriRF",
                   DgSphIcosa* sphIcosaIn = 0)
                      //DgSphIcosa(DgGeoCoord(11.25L, 58.28252559L, false), M_ZERO))
         { return new DgProjTriRF(networkIn, nameIn, sphIcosaIn); }

      //virtual DgLocVector& convert (DgLocVector& vec) const;

      virtual long double dist (const DgProjTriCoord&, const DgProjTriCoord&) const
                { return M_ZERO; }

      virtual std::string add2str (const DgProjTriCoord& add) const
                       { return std::string(add); }

      virtual std::string add2str (const DgProjTriCoord& add, char delimiter) const
          { return dgg::util::to_string(add.triNum()) + delimiter +
                   dgg::util::to_string(add.coord().x(), formatStr()) + delimiter +
                   dgg::util::to_string(add.coord().y(), formatStr()); }

      virtual const char* str2add (DgProjTriCoord* add, const char* str,
                                   char delimiter) const;

      virtual const DgProjTriCoord& undefAddress (void) const
                { static DgProjTriCoord undef; return undef; }

      virtual std::string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual unsigned long long int dist2int (const long double& dist) const
                       { return (unsigned long long int) dist; }

      DgSphIcosa& sphIcosa (void) const { return *sphIcosa_; }

   protected:

      DgProjTriRF (DgRFNetwork& networkIn, const std::string& nameIn = "ProjTriRF",
                   DgSphIcosa* sphIcosaIn = 0)
                      //DgSphIcosa(DgGeoCoord(11.25L, 58.28252559L, false), M_ZERO))
         : DgRF<DgProjTriCoord, long double> (networkIn, nameIn),
           sphIcosa_ (sphIcosaIn) { }

   private:

      DgSphIcosa* sphIcosa_;

};

#endif
