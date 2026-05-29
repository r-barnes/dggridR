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
// DgIDGGutil.h: common classes used by IDGG classes
//
// Version 7.0 - Kevin Sahr, 11/16/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGUTIL_H
#define DGIDGGUTIL_H

#include "Dg2WayConverter.h"
#include "DgContCartRF.h"
#include "DgConverter.h"
#include "DgDiscRF2D.h"
#include "DgDiscRFS2D.h"
#include "DgDVec2D.h"
#include "DgEllipsoidRF.h"
#include "DgGeoSphRF.h"
#include "DgHexC1Grid2D.h"
#include "DgIcosaMap.h"
#include "DgIcosaProj.h"
#include "DgIVec2D.h"
#include "DgLocation.h"
#include "DgPolygon.h"
#include "DgProjTriRF.h"
#include "DgRF.h"
#include "DgUtil.h"

#include <climits>
#include <ostream>

class DgPolygon;
class DgBoundedIDGG;
class DgIDGGBase;

////////////////////////////////////////////////////////////////////////////////
//
//   Coordinate consisting of vertex number and (i, j) coordinates.
//
class DgQ2DICoord  {

   public:

      static const DgQ2DICoord undefDgQ2DICoord;

      DgQ2DICoord (int quadNumIn = -1, const DgIVec2D& coordIn = DgIVec2D())
        : quadNum_ (quadNumIn), coord_ (coordIn) { }

      int quadNum (void) const { return quadNum_; }

      const DgIVec2D& coord (void) const { return coord_; }

      void setVertNum (int quadNumIn)           { quadNum_ = quadNumIn; }
      void setCoord   (const DgIVec2D& coordIn) { coord_ = coordIn; }

      operator std::string (void) const
/*
         { return std::string("quadNum: " + dgg::util::to_string(quadNum()) +
                      ", coord: " + std::string(coord())); }
*/
         { return std::string("q" + dgg::util::to_string(quadNum(),2) +
                      ":" + std::string(coord())); }

      bool operator== (const DgQ2DICoord& qc) const
          { return quadNum() == qc.quadNum() && coord() == qc.coord(); }

      bool operator!= (const DgQ2DICoord& qc) const
          { return !(*this == qc); }

      DgQ2DICoord& operator= (const DgQ2DICoord& add)
          {
             if (add != *this)
             {
                setVertNum(add.quadNum());
                setCoord(add.coord());
             }

             return *this;
          }

   private:

      int quadNum_;
      DgIVec2D coord_;

};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, const DgQ2DICoord& coord)
{ return stream << std::string(coord); }

////////////////////////////////////////////////////////////////////////////////
//
//   Coordinate consisting of vertex number and (x, y) coordinates.
//
class DgQ2DDCoord  {

   public:

      static const DgQ2DDCoord undefDgQ2DDCoord;

      DgQ2DDCoord (int quadNumIn = -1,
                   const DgDVec2D& coordIn = DgDVec2D())
        : quadNum_ (quadNumIn), coord_ (coordIn) { }

      int quadNum (void) const { return quadNum_; }

      const DgDVec2D& coord (void) const { return coord_; }

      void setVertNum (int quadNumIn)           { quadNum_ = quadNumIn; }
      void setCoord   (const DgDVec2D& coordIn) { coord_ = coordIn; }

      operator std::string (void) const
          { return std::string("quadNum: " + dgg::util::to_string(quadNum()) + ", coord: "
                          + std::string(coord())); }

      bool operator== (const DgQ2DDCoord& qc) const
          { return quadNum() == qc.quadNum() && coord() == qc.coord(); }

      bool operator!= (const DgQ2DDCoord& qc) const
          { return !(*this == qc); }

      DgQ2DDCoord& operator= (const DgQ2DDCoord& add)
          {
             if (add != *this)
             {
                setVertNum(add.quadNum());
                setCoord(add.coord());
             }

             return *this;
          }

   private:

      int quadNum_;
      DgDVec2D coord_;

};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, const DgQ2DDCoord& coord)
{ return stream << std::string(coord); }

////////////////////////////////////////////////////////////////////////////////
class DgQ2DDRF : public DgRF<DgQ2DDCoord, long double> {

   public:

      static const DgQ2DDRF* makeRF (DgRFNetwork& networkIn, const std::string& nameIn = "Q2DDRF")
         { return new DgQ2DDRF(networkIn, nameIn); }

      virtual long double dist (const DgQ2DDCoord& add1,
                           const DgQ2DDCoord& add2) const
                       { return add1.coord().distance(add2.coord()); }

      virtual std::string add2str (const DgQ2DDCoord& add) const
                       { return std::string(add); }

      virtual std::string add2str (const DgQ2DDCoord& add, char delimiter) const
        { return dgg::util::to_string(add.quadNum()) + delimiter +
                    dgg::util::to_string(add.coord().x(), formatStr()) + delimiter +
                    dgg::util::to_string(add.coord().y(), formatStr()); }

      virtual const char* str2add (DgQ2DDCoord* add, const char* str,
                                   char delimiter) const;

      virtual std::string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual unsigned long long int dist2int (const long double& dist) const
                       { return (unsigned long long int) dist; }

      virtual const DgQ2DDCoord& undefAddress (void) const
                       { static DgQ2DDCoord undef; return undef; }

   protected:

      DgQ2DDRF (DgRFNetwork& networkIn, const std::string& nameIn)
         : DgRF<DgQ2DDCoord, long double>(networkIn, nameIn) { }

};

////////////////////////////////////////////////////////////////////////////////
class DgQuadEdgeCells {

   public:

      DgQuadEdgeCells (int quadNumIn, bool isType0In, int loneVertIn,
                      int upQuadIn, int downQuadIn, int rightQuadIn, int leftQuadIn)
         : isType0_ (isType0In), quadNum_ (quadNumIn), loneVert_ (loneVertIn),
           upQuad_ (upQuadIn), downQuad_ (downQuadIn), rightQuad_ (rightQuadIn),
           leftQuad_ (leftQuadIn) { }

      bool isType0   (void) const { return isType0_; }
      int  quadNum   (void) const { return quadNum_; }
      int  loneVert  (void) const { return loneVert_; }
      int  upQuad    (void) const { return upQuad_; }
      int  downQuad  (void) const { return downQuad_; }
      int  rightQuad (void) const { return rightQuad_; }
      int  leftQuad  (void) const { return leftQuad_; }

   private:

      bool isType0_;

      int quadNum_;
      int loneVert_;
      int upQuad_;
      int downQuad_;
      int rightQuad_;
      int leftQuad_;
};

////////////////////////////////////////////////////////////////////////////////
class DgVertTriVals {

   public:

      DgVertTriVals (int quadNum, int triNum, int subTri, bool keep,
                 const DgDVec2D& trans, int rot60)
         : quadNum_ (quadNum), triNum_ (triNum), subTri_ (subTri), keep_ (keep),
           trans_ (trans), rot60_ (rot60) { }

      int      quadNum (void) const { return quadNum_; }
      int      triNum  (void) const { return triNum_; }
      int      subTri  (void) const { return subTri_; }
      bool     keep    (void) const { return keep_; }
      int      rot60   (void) const { return rot60_; }

      const DgDVec2D& trans (void) const { return trans_; }

      void setQuadNum (int quadNumIn) { quadNum_ = quadNumIn; }
      void setTriNum  (int triNumIn) { triNum_ = triNumIn; }
      void setSubTri  (int subTriIn) { subTri_ = subTriIn; }
      void setKeep    (bool keepIn) { keep_ = keepIn; }
      void setTrans   (const DgDVec2D& transIn) { trans_ = transIn; }
      void setRot60   (int rot60In) { rot60_ = rot60In; }

      operator std::string (void) const
          { std::string keeper; if (keep()) keeper = "true"; else keeper = "false";
            return std::string("keep: " + keeper + ", triNum: " + dgg::util::to_string(triNum())
                      + ", quadNum: " + dgg::util::to_string(quadNum()) +
           ", subTri: " + dgg::util::to_string(subTri()) + ", rot60: " + dgg::util::to_string(rot60()) +
           ", trans: " + std::string(trans())); }

   private:

      int quadNum_;
      int triNum_;
      int subTri_;
      bool keep_;
      DgDVec2D trans_;
      int rot60_; // number of deosil 60 deg rotations after translation

};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& str, const DgVertTriVals& coord)
{ return str << std::string(coord); }

////////////////////////////////////////////////////////////////////////////////
class DgVertex2DDCoord {

   public:

      DgVertex2DDCoord (bool keepIn = true, int vertNumIn = -1,
           int triNumIn = -1, const DgDVec2D& coordIn = DgDVec2D())
         : vertNum_ (vertNumIn), triNum_ (triNumIn), keep_ (keepIn),
           coord_ (coordIn) { }

      int  vertNum (void) const { return vertNum_; }
      int  triNum  (void) const { return triNum_; }
      bool keep    (void) const { return keep_; }

      const DgDVec2D& coord (void) const { return coord_; }

      void setVertNum (int vertNumIn) { vertNum_ = vertNumIn; }
      void setTriNum  (int triNumIn) { triNum_ = triNumIn; }
      void setKeep    (bool keepIn) { keep_ = keepIn; }
      void setCoord   (const DgDVec2D& coordIn) { coord_ = coordIn; }

      operator std::string (void) const
          { std::string keeper; if (keep()) keeper = "true"; else keeper = "false";
            return std::string("keep: " + keeper + ", triNum: " +
                       dgg::util::to_string(triNum()) + ", vertNum: " +
                       dgg::util::to_string(vertNum()) + ", coord: "
                       + std::string(coord())); }

      bool operator== (const DgVertex2DDCoord& c) const
          { return keep() == c.keep() && triNum() == c.triNum() &&
                   vertNum() == c.vertNum() && coord() == c.coord(); }

   private:

      int vertNum_;
      int triNum_;
      bool keep_;
      DgDVec2D coord_;

};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& str, const DgVertex2DDCoord& coord)
{ return str << std::string(coord); }

////////////////////////////////////////////////////////////////////////////////
class DgVertex2DDRF : public DgRF<DgVertex2DDCoord, long double> {

   public:

      static const DgVertex2DDRF* makeRF (DgRFNetwork& networkIn,
                const std::string& nameIn = "DgVertex2DDRF")
         { return new DgVertex2DDRF(networkIn, nameIn); }

      //virtual DgLocVector& convert (DgLocVector& vec) const;

      virtual long double dist (const DgVertex2DDCoord&, const DgVertex2DDCoord&) const
                { return M_ZERO; }

      virtual std::string add2str (const DgVertex2DDCoord& add) const
                       { return std::string(add); }

      virtual std::string add2str (const DgVertex2DDCoord& add, char delimiter) const
          { return dgg::util::to_string(add.vertNum()) + delimiter +
                   dgg::util::to_string(add.triNum()) + delimiter +
                   ((add.keep()) ? "keep" : "nokeep") + delimiter +
                   dgg::util::to_string(add.coord().x(), formatStr()) + delimiter +
                   dgg::util::to_string(add.coord().y(), formatStr()); }

      virtual const char* str2add (DgVertex2DDCoord* add, const char* str,
                                   char delimiter) const;

      virtual const DgVertex2DDCoord& undefAddress (void) const
                { static DgVertex2DDCoord undef(false, -1, -1); return undef; }

      virtual std::string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual unsigned long long int dist2int (const long double& dist) const
                       { return (unsigned long long int) dist; }

      static const DgVertTriVals& vertTable (int vertNum, int subTri)
                       { return vertTable_[vertNum][subTri]; }

      static const DgVertTriVals& triTable (int triNum)
                       { return triTable_[triNum]; }

   protected:

      DgVertex2DDRF (DgRFNetwork& networkIn, const std::string& nameIn)
         : DgRF<DgVertex2DDCoord, long double> (networkIn, nameIn) { }

   private:

      static DgVertTriVals vertTable_[12][6];
      static const DgVertTriVals triTable_[20];

};

////////////////////////////////////////////////////////////////////////////////
class DgQ2DItoDConverter :
        public DgConverter<DgQ2DICoord, long long int, DgQ2DDCoord, long double>
{
   public:

      DgQ2DItoDConverter (const DgRF<DgQ2DICoord, long long int>& from,
                          const DgRF<DgQ2DDCoord, long double>& to);

      const DgIDGGBase& IDGG (void) const { return *pIDGG_; }

      virtual DgQ2DDCoord convertTypedAddress
                                (const DgQ2DICoord& addIn) const;

   protected:

      const DgIDGGBase* pIDGG_;

};

////////////////////////////////////////////////////////////////////////////////
class DgQ2DDtoIConverter :
        public DgConverter<DgQ2DDCoord, long double, DgQ2DICoord, long long int>
{
   public:

      DgQ2DDtoIConverter (const DgRF<DgQ2DDCoord, long double>& from,
                          const DgRF<DgQ2DICoord, long long int>& to);

      const DgIDGGBase& IDGG (void) const { return *pIDGG_; }

      virtual DgQ2DICoord convertTypedAddress
                                (const DgQ2DDCoord& addIn) const;

   protected:

      const DgIDGGBase* pIDGG_;

};

////////////////////////////////////////////////////////////////////////////////
class DgVertex2DDToQ2DDConverter :
        public DgConverter<DgVertex2DDCoord, long double, DgQ2DDCoord, long double>
{
   public:

      DgVertex2DDToQ2DDConverter (const DgRF<DgVertex2DDCoord, long double>& from,
                                  const DgRF<DgQ2DDCoord, long double>& to);

      virtual DgQ2DDCoord convertTypedAddress
                                (const DgVertex2DDCoord& addIn) const;
};

////////////////////////////////////////////////////////////////////////////////
class DgQ2DDtoVertex2DDConverter :
        public DgConverter<DgQ2DDCoord, long double, DgVertex2DDCoord, long double>
{
   public:

      DgQ2DDtoVertex2DDConverter (const DgRF<DgQ2DDCoord, long double>& from,
                                  const DgRF<DgVertex2DDCoord, long double>& to);

      virtual DgVertex2DDCoord convertTypedAddress
                                (const DgQ2DDCoord& addIn) const;

    private:
        inline int compute_subtriangle(const long double& x, const long double&  y) const;
};

////////////////////////////////////////////////////////////////////////////////
class DgProjTriToVertex2DD :
        public DgConverter<DgProjTriCoord, long double, DgVertex2DDCoord, long double> {

   public:

      DgProjTriToVertex2DD (const DgRF<DgProjTriCoord, long double>& from,
                            const DgRF<DgVertex2DDCoord, long double>& to)
         : DgConverter<DgProjTriCoord, long double, DgVertex2DDCoord, long double>
             (from, to) { }

      virtual DgVertex2DDCoord convertTypedAddress
                                          (const DgProjTriCoord& addIn) const;

};

////////////////////////////////////////////////////////////////////////////////
class DgVertex2DDtoProjTri :
        public DgConverter<DgVertex2DDCoord, long double, DgProjTriCoord, long double> {

   public:

      DgVertex2DDtoProjTri (const DgRF<DgVertex2DDCoord, long double>& from,
                            const DgRF<DgProjTriCoord, long double>& to)
         : DgConverter<DgVertex2DDCoord, long double, DgProjTriCoord, long double>
             (from, to) { }

      virtual DgProjTriCoord convertTypedAddress
                                          (const DgVertex2DDCoord& addIn) const;
};

////////////////////////////////////////////////////////////////////////////////
class DgPlaneTriRF : public DgContCartRF {

   public:

      static DgPlaneTriRF* makeRF (DgRFNetwork& networkIn, const std::string& nameIn = "PlaneTri",
                    const DgIcosaMap& icosaMapIn = DgIcosaMap::defIcosaMap)
      { return new DgPlaneTriRF(networkIn, nameIn, icosaMapIn); }

      const DgIcosaMap& icosaMap (void) const { return icosaMap_; }

   protected:

      DgPlaneTriRF (DgRFNetwork& networkIn, const std::string& nameIn = "PlaneTri",
                    const DgIcosaMap& icosaMapIn = DgIcosaMap::defIcosaMap)
         : DgContCartRF(networkIn, nameIn), icosaMap_ (icosaMapIn) { }

   private:

      const DgIcosaMap& icosaMap_;

};

////////////////////////////////////////////////////////////////////////////////
class DgPlaneTriProj : public DgConverter<DgProjTriCoord, long double,
                                          DgDVec2D, long double> {

   public:

      DgPlaneTriProj (const DgRF<DgProjTriCoord, long double>& from,
                      const DgRF<DgDVec2D, long double>& to);

      const DgPlaneTriRF& planeTriRF (void) const { return *pPlaneTriRF_; }

      virtual DgDVec2D convertTypedAddress (const DgProjTriCoord& addIn) const;

   private:

      const DgPlaneTriRF* pPlaneTriRF_;

};

////////////////////////////////////////////////////////////////////////////////
class DgDegConverter :
               public DgConverter<DgGeoCoord, long double, DgDVec2D, long double> {

   public:

      DgDegConverter (const DgRF<DgGeoCoord, long double>& from,
                       const DgRF<DgDVec2D, long double>& to)
         : DgConverter<DgGeoCoord, long double, DgDVec2D, long double>(from, to) { }

      virtual DgDVec2D convertTypedAddress (const DgGeoCoord& addIn) const
      {
         return DgDVec2D(addIn.lonDegs(), addIn.latDegs());
      }
};

////////////////////////////////////////////////////////////////////////////////
class DgRadConverter :
               public DgConverter<DgDVec2D, long double, DgGeoCoord, long double> {

   public:

      DgRadConverter (const DgRF<DgDVec2D, long double>& from,
                      const DgRF<DgGeoCoord, long double>& to)
         : DgConverter<DgDVec2D, long double, DgGeoCoord, long double>(from, to) { }

      virtual DgGeoCoord convertTypedAddress (const DgDVec2D& addIn) const
      {
         return DgGeoCoord(addIn.x(), addIn.y(), false);
      }
};

////////////////////////////////////////////////////////////////////////////////
class DgDegRadConverter : public Dg2WayConverter {

   public:

      DgDegRadConverter (const DgRF<DgGeoCoord, long double>& from,
                         const DgRF<DgDVec2D, long double>& to)
         : Dg2WayConverter (*(new DgDegConverter(from, to)),
                            *(new DgRadConverter(to, from))) { }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Class to hold statistics on a particular DGG.
//
class DgGridStats {

   public:

      DgGridStats (void)
         : nCells_ (0), cls_ (M_ZERO), cellDistKM_ (M_ZERO),
           cellAreaKM_ (M_ZERO), precision_ (DEFAULT_PRECISION) { }

      unsigned long long int nCells     (void) const { return nCells_; }
      long double            cls        (void) const { return cls_; }
      long double            cellDistKM (void) const { return cellDistKM_; }
      long double            cellAreaKM (void) const { return cellAreaKM_; }
      unsigned int           precision  (void) const { return precision_; }

      void setNCells (long long int nCellsIn) { nCells_ = nCellsIn; }
      void setCLS    (long double clsIn)             { cls_ = clsIn; }
      void setCellDistKM (long double cellDistKMin)  { cellDistKM_ = cellDistKMin; }
      void setCellAreaKM (long double cellAreaIn)    { cellAreaKM_ = cellAreaIn; }
      void setPrecision (unsigned int precisionIn)    { precision_ = precisionIn; }

   private:

      unsigned long long int nCells_;
      long double cls_;
      long double cellDistKM_;
      long double cellAreaKM_;
      unsigned int precision_;

};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, const DgGridStats& gs)
{
  unsigned int prec = gs.precision();

  stream << "DGG Statistics (calculated in projection space):\n";
  stream << " total #cells: " << dgg::util::addCommas(gs.nCells()) << "\n";
  stream << " approximate intercell distance: "
         << dgg::util::addCommas(gs.cellDistKM(), prec) << " km\n";
  stream << " average hex cell area: "
         << dgg::util::addCommas(gs.cellAreaKM(), prec) << " km^2\n";
  stream << " characteristic length scale: "
         << dgg::util::addCommas(gs.cls(), prec) << " km" << std::endl;

  return stream;
}

////////////////////////////////////////////////////////////////////////////////
#endif
