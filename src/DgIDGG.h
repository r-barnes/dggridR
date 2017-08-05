////////////////////////////////////////////////////////////////////////////////
//
// DgIDGG.h: DgIDGG class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGG_H 
#define DGIDGG_H

#include <limits>
#include <iostream>
#include <cstdint>

#include "DgIVec2D.h"
#include "DgDVec2D.h"
#include "DgPolygon.h"
#include "DgRF.h"
#include "DgLocation.h"
#include "DgConverter.h"
#include "Dg2WayConverter.h"
#include "DgContCartRF.h"
#include "DgGeoSphRF.h"
#include "DgDiscRF2D.h"
#include "DgDiscRFS2D.h"
#include "DgHexC1Grid2D.h"
#include "DgProjTriRF.h"
#include "DgIcosaProj.h"
#include "DgIcosaMap.h"
#include "DgEllipsoidRF.h"
#include "DgUtil.h"

class DgPolygon;
class DgIDGG;
class DgBoundedIDGG;

using namespace std;

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

      operator string (void) const
         { return string("quadNum: " + dgg::util::to_string(quadNum()) + 
                      ", coord: " + string(coord())); }

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
inline ostream&
operator<< (ostream& stream, const DgQ2DICoord& coord)
{ return stream << string(coord); }

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

      operator string (void) const
          { return string("quadNum: " + dgg::util::to_string(quadNum()) + ", coord: " 
                          + string(coord())); }

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
inline ostream&
operator<< (ostream& stream, const DgQ2DDCoord& coord)
{ return stream << string(coord); }

////////////////////////////////////////////////////////////////////////////////
class DgQ2DDRF : public DgRF<DgQ2DDCoord, long double> {

   public:

      DgQ2DDRF (DgRFNetwork& networkIn, const string& nameIn)
         : DgRF<DgQ2DDCoord, long double>(networkIn, nameIn) { }

      virtual long double dist (const DgQ2DDCoord& add1, 
                           const DgQ2DDCoord& add2) const 
                       { return add1.coord().distance(add2.coord()); }

      virtual string add2str (const DgQ2DDCoord& add) const
                       { return string(add); }

      virtual string add2str (const DgQ2DDCoord& add, char delimiter) const
        { return dgg::util::to_string(add.quadNum()) + delimiter + 
                    dgg::util::to_string(add.coord().x(), formatStr()) + delimiter + 
                    dgg::util::to_string(add.coord().y(), formatStr()); }

      virtual const char* str2add (DgQ2DDCoord* add, const char* str, 
                                   char delimiter) const;

      virtual string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual std::uint64_t dist2int (const long double& dist) const
                       { return (std::uint64_t) dist; }

      virtual const DgQ2DDCoord& undefAddress (void) const
                       { static DgQ2DDCoord undef; return undef; }

};

////////////////////////////////////////////////////////////////////////////////
class DgQuadEdgeCells {

   public:

      DgQuadEdgeCells (int quadNumIn, bool isType0In, int loneVertIn,
                      int upQuadIn, int rightQuadIn)
         : isType0_ (isType0In), quadNum_ (quadNumIn), loneVert_ (loneVertIn),
           upQuad_ (upQuadIn), rightQuad_ (rightQuadIn) { }

      bool isType0   (void) const { return isType0_; }
      int  quadNum   (void) const { return quadNum_; }
      int  loneVert  (void) const { return loneVert_; }
      int  upQuad    (void) const { return upQuad_; }
      int  rightQuad (void) const { return rightQuad_; }

   private:

      bool isType0_;

      int quadNum_;
      int loneVert_;
      int upQuad_;
      int rightQuad_;

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

      operator string (void) const
          { string keeper; if (keep()) keeper = "true"; else keeper = "false";
            return string("keep: " + keeper + ", triNum: " + dgg::util::to_string(triNum())
                      + ", quadNum: " + dgg::util::to_string(quadNum()) + 
           ", subTri: " + dgg::util::to_string(subTri()) + ", rot60: " + dgg::util::to_string(rot60()) + 
           ", trans: " + string(trans())); }

   private:

      int quadNum_;
      int triNum_;
      int subTri_;
      bool keep_;
      DgDVec2D trans_;
      int rot60_; // number of deosil 60 deg rotations after translation

};

////////////////////////////////////////////////////////////////////////////////
inline ostream& 
operator<< (ostream& str, const DgVertTriVals& coord) 
{ return str << string(coord); }

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

      operator string (void) const
          { string keeper; if (keep()) keeper = "true"; else keeper = "false";
            return string("keep: " + keeper + ", triNum: " + 
                       dgg::util::to_string(triNum()) + ", vertNum: " + 
                       dgg::util::to_string(vertNum()) + ", coord: " 
                       + string(coord())); }

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
inline ostream& 
operator<< (ostream& str, const DgVertex2DDCoord& coord) 
{ return str << string(coord); }

////////////////////////////////////////////////////////////////////////////////
class DgVertex2DDRF : public DgRF<DgVertex2DDCoord, long double> {

   public:

      DgVertex2DDRF (DgRFNetwork& networkIn, const string& nameIn)
         : DgRF<DgVertex2DDCoord, long double> (networkIn, nameIn) { }

      //virtual DgLocVector& convert (DgLocVector& vec) const;

      virtual long double dist (const DgVertex2DDCoord& add1,
                           const DgVertex2DDCoord& add2) const
                { return M_ZERO; }

      virtual string add2str (const DgVertex2DDCoord& add) const
                       { return string(add); }

      virtual string add2str (const DgVertex2DDCoord& add, char delimiter) const
          { return dgg::util::to_string(add.vertNum()) + delimiter +
                   dgg::util::to_string(add.triNum()) + delimiter + 
                   ((add.keep()) ? "keep" : "nokeep") + delimiter +
                   dgg::util::to_string(add.coord().x(), formatStr()) + delimiter +
                   dgg::util::to_string(add.coord().y(), formatStr()); }

      virtual const char* str2add (DgVertex2DDCoord* add, const char* str, 
                                   char delimiter) const;

      virtual const DgVertex2DDCoord& undefAddress (void) const
                { static DgVertex2DDCoord undef(false, -1, -1); return undef; }

      virtual string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual std::uint64_t dist2int (const long double& dist) const
                       { return (std::uint64_t) dist; }

      static const DgVertTriVals& vertTable (int vertNum, int subTri) 
                       { return vertTable_[vertNum][subTri]; }

      static const DgVertTriVals& triTable (int triNum)
                       { return triTable_[triNum]; }

   private:

      static DgVertTriVals vertTable_[12][6];
      static const DgVertTriVals triTable_[20];

};

////////////////////////////////////////////////////////////////////////////////
class DgQ2DItoDConverter : 
        public DgConverter<DgQ2DICoord, std::int64_t, DgQ2DDCoord, long double>
{
   public:

      DgQ2DItoDConverter (const DgRF<DgQ2DICoord, std::int64_t>& from,
                          const DgRF<DgQ2DDCoord, long double>& to);

      const DgIDGG& IDGG (void) const { return *pIDGG_; }

      virtual DgQ2DDCoord convertTypedAddress 
                                (const DgQ2DICoord& addIn) const;

   protected:

      const DgIDGG* pIDGG_;

};

////////////////////////////////////////////////////////////////////////////////
class DgQ2DDtoIConverter : 
        public DgConverter<DgQ2DDCoord, long double, DgQ2DICoord, std::int64_t>
{
   public:

      DgQ2DDtoIConverter (const DgRF<DgQ2DDCoord, long double>& from,
                          const DgRF<DgQ2DICoord, std::int64_t>& to);

      const DgIDGG& IDGG (void) const { return *pIDGG_; }

      virtual DgQ2DICoord convertTypedAddress 
                                (const DgQ2DDCoord& addIn) const;

   protected:

      const DgIDGG* pIDGG_;

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

      DgPlaneTriRF (DgRFNetwork& networkIn, const string& nameIn = "PlaneTri",
                    const DgIcosaMap& icosaMapIn = DgIcosaMap::defIcosaMap)
         : DgContCartRF(networkIn, nameIn), icosaMap_ (icosaMapIn) { }

      const DgIcosaMap& icosaMap (void) const { return icosaMap_; }

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
//
//   Coordinate consisting of a string containing quad number, (aperture 3 hex
//        level indicator), and radix string
//
class DgInterleaveCoord  {

   public:

      static const DgInterleaveCoord undefDgInterleaveCoord;

      DgInterleaveCoord (void) { }

      DgInterleaveCoord (const DgInterleaveCoord& coord) 
              { valString_ = coord.valString(); }

      void setValString (const string strIn) { valString_ = strIn; }

      const string& valString (void) const { return valString_; }

      operator string (void) const { return valString(); }

      bool operator== (const DgInterleaveCoord& c) const
          { return valString() == c.valString(); }

      bool operator!= (const DgInterleaveCoord& c) const
          { return !(*this == c); }

      DgInterleaveCoord& operator= (const DgInterleaveCoord& add)
          {
             if (add != *this) setValString(add.valString());

             return *this;
          }

   private:

      string valString_;

};

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgInterleaveCoord& coord)
{ return stream << string(coord); }

////////////////////////////////////////////////////////////////////////////////
class DgInterleaveRF : public DgRF<DgInterleaveCoord, std::int64_t> {

   public:

      DgInterleaveRF (DgRFNetwork& networkIn, const string& nameIn)
         : DgRF<DgInterleaveCoord, std::int64_t>(networkIn, nameIn) { }

      virtual std::int64_t dist (const DgInterleaveCoord& add1, 
                        const DgInterleaveCoord& add2) const 
                       { return 0; }

      virtual string add2str (const DgInterleaveCoord& add) const
                       { return string(add); }

      virtual string add2str (const DgInterleaveCoord& add, char delimiter) 
                                                                         const
                       { return string(add); }

      virtual const char* str2add (DgInterleaveCoord* add, const char* str, 
                                   char delimiter) const;

      virtual string dist2str (const std::int64_t& dist) const
                       { return dgg::util::to_string(dist); }

      virtual long double dist2dbl (const std::int64_t& dist) const
                       { return dist; }

      virtual std::uint64_t dist2int (const std::int64_t& dist) const
                       { return dist; }

      virtual const DgInterleaveCoord& undefAddress (void) const
                       { static DgInterleaveCoord undef; return undef; }

};

////////////////////////////////////////////////////////////////////////////////
class DgQ2DItoInterleaveConverter : 
        public DgConverter<DgQ2DICoord, std::int64_t, DgInterleaveCoord, std::int64_t>
{
   public:

      DgQ2DItoInterleaveConverter (const DgRF<DgQ2DICoord, std::int64_t>& from,
                                   const DgRF<DgInterleaveCoord, std::int64_t>& to);

      const DgIDGG& IDGG (void) const { return *pIDGG_; }

      virtual DgInterleaveCoord convertTypedAddress 
                                (const DgQ2DICoord& addIn) const;

   protected:

      const DgIDGG* pIDGG_;

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
////////////////////////////////////////////////////////////////////////////////
//
// Class to hold statistics on a particular DGG.
//
class DgGridStats {

   public:

      DgGridStats (void)
         : nCells_ (0), cls_ (M_ZERO), cellDistKM_ (M_ZERO), 
           cellAreaKM_ (M_ZERO), precision_ (DEFAULT_PRECISION) { }

      std::uint64_t nCells     (void) const { return nCells_; }
      long double            cls        (void) const { return cls_; }
      long double            cellDistKM (void) const { return cellDistKM_; }
      long double            cellAreaKM (void) const { return cellAreaKM_; }
      unsigned int           precision  (void) const { return precision_; }
   
      void setNCells (std::int64_t nCellsIn) { nCells_ = nCellsIn; }
      void setCLS    (long double clsIn)             { cls_ = clsIn; }
      void setCellDistKM (long double cellDistKMin)  { cellDistKM_ = cellDistKMin; }
      void setCellAreaKM (long double cellAreaIn)    { cellAreaKM_ = cellAreaIn; }
      void setPrecision (unsigned int precisionIn)    { precision_ = precisionIn; }
      
   private:

      std::uint64_t nCells_;
      long double cls_;
      long double cellDistKM_;
      long double cellAreaKM_;
      unsigned int precision_;

};

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgGridStats& gs)
{ 
  unsigned int prec = gs.precision();

  stream << "DGG Statistics (calculated in projection space):\n";
  stream << " total #cells: " << dgg::util::addCommas(gs.nCells()) << "\n";
  stream << " intercell distance: " 
         << dgg::util::addCommas(gs.cellDistKM(), prec) << " km\n";
  stream << " cell area: " 
         << dgg::util::addCommas(gs.cellAreaKM(), prec) << " km^2\n";
  stream << " characteristic length scale: " 
         << dgg::util::addCommas(gs.cls(), prec) << " km" << endl;

  return stream;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//  Icosahedral DGG class.
//
class DgIDGG : public DgDiscRF<DgQ2DICoord, DgGeoCoord, long double> {

   using DgDiscRF<DgQ2DICoord, DgGeoCoord, long double>::setVertices;

   public:

      DgIDGG (const DgGeoSphRF& geoRFIn, const DgGeoCoord& vert0,
              long double azDegs, unsigned int apertureIn, int resIn, 
              const string& nameIn = "IDGG", 
              const string& gridTopoIn = string("HEXAGON"),
              const string& projType = string("ISEA"),
              bool isMixedApIn = false, int numAp4 = 0, 
              bool isSuperfund = false, int sfRes = 17, 
              unsigned int precisionIn = DEFAULT_PRECISION);

      DgIDGG (const DgIDGG& grd);

      virtual ~DgIDGG();

      const DgGeoSphRF&  geoRF       (void) const { return geoRF_; }
      const DgGeoCoord&  vert0       (void) const { return vert0_; }
      long double        azDegs      (void) const { return azDegs_; }
      unsigned int       aperture    (void) const { return aperture_; }
      bool               isMixed43   (void) const { return isMixed43_; }
      bool               isSuperfund (void) const { return isSuperfund_; }
      int                sfRes       (void) const { return sfRes_; }
      int                numAp4      (void) const { return numAp4_; }
      int                res         (void) const { return res_; }
      int                adjRes      (void) const { return adjRes_; }
      std::int64_t maxI        (void) const { return maxI_; }
      std::int64_t maxJ        (void) const { return maxJ_; }
      std::int64_t mag         (void) const { return mag_; }
      std::int64_t maxD        (void) const { return maxD_; }
      int                radix       (void) const { return radix_; }
      const string&      gridTopo    (void) const { return gridTopo_; }
      const string&      projType    (void) const { return projType_; }
      bool               isClassI    (void) const { return isClassI_; }
      bool               isAligned   (void) const { return isAligned_; }
      bool               isCongruent (void) const { return isCongruent_; }
      const DgQ2DICoord& firstAdd    (void) const { return firstAdd_; }
      const DgQ2DICoord& lastAdd     (void) const { return lastAdd_; }

      int                outputRes   (void) const 
                               { return (isSuperfund_) ? sfRes_ : res_; }

      const DgProjTriRF&    projTriRF (void) const { return *projTriRF_; }
      const DgVertex2DDRF&  vertexRF  (void) const { return *vertexRF_; }
      const DgQ2DDRF&       q2ddRF    (void) const { return *q2ddRF_; }
      const DgBoundedIDGG&  bndRF     (void) const { return *bndRF_; }
      const DgInterleaveRF& intRF     (void) const { return *intRF_; }
      const DgPlaneTriRF&   planeRF   (void) const { return *planeRF_; }

      const DgContCartRF&   ccFrame (void) const { return *ccFrame_; }
      const DgDiscRF2D&     grid2D  (void) const { return *grid2D_; }

      unsigned int          precision (void) const { return precision_; } 

      const DgGridStats& gridStats (void) const { return gridStats_; }

      void setPrecision (unsigned int precisionIn)
        { precision_ = precisionIn;
          gridStats_.setPrecision(precision()); }

      virtual string add2str (const DgQ2DICoord& add) const 
                 { return string(add); }

      virtual string add2str (const DgQ2DICoord& add, char delimiter) const 
        { return dgg::util::to_string(add.quadNum()) + delimiter + 
                 dgg::util::to_string(add.coord().i()) + delimiter + 
                 dgg::util::to_string(add.coord().j()); }

      virtual const char* str2add (DgQ2DICoord* add, const char* str, 
                                   char delimiter) const;

      virtual const DgQ2DICoord& undefAddress (void) const 
                            { return DgQ2DICoord::undefDgQ2DICoord; }

      virtual void setVertices (const DgLocation& loc, DgPolygon& vec,
                                int densify) const;

      virtual void setAddVertices (const DgQ2DICoord& add,
                                   DgPolygon& vec) const
                  { setAddVertices(add, vec, 0); }

      virtual void setAddVertices (const DgQ2DICoord& add,
                                   DgPolygon& vec, int densify) const;

      // we'll give dummy definitions for these since some of our IDGG's may
      // not have defined these yet

      virtual std::int64_t dist (const DgQ2DICoord& add1, 
                                       const DgQ2DICoord& add2) const
           {
              report(string("DgIDGG::dist() this method has not been defined "
                     "for DgIDGG ") + this->name(), DgBase::Fatal);

	      return M_ZERO;
           }

      virtual void setAddNeighbors (const DgQ2DICoord& add,
                                    DgLocVector& vec) const 
           {
              report(string("DgIDGG::setAddNeighbors() this method has not "
                "been defined for DgIDGG ") + this->name(), DgBase::Fatal);
           }

      // give dummy definitions; these will not be used but will be
      // overridden by series converters

      virtual DgQ2DICoord quantify (const DgGeoCoord& add) const
                { DgQ2DICoord v; return v; }

      virtual DgGeoCoord invQuantify (const DgQ2DICoord& add) const
                { DgGeoCoord g; return g; }

      static const DgQuadEdgeCells& edgeTable (int quadNum)
                       { return edgeTable_[quadNum]; }

   protected:

      virtual void initialize (void);

      const DgDiscRFS2D& grid2DS (void) const { return *grid2DS_; }

   private:

      DgSphIcosa* sphIcosa_;

      const DgGeoSphRF& geoRF_;

      DgGeoCoord vert0_;
      long double azDegs_;

      string gridTopo_;
      string projType_;

      unsigned int aperture_;

      int res_;
      int adjRes_;
      bool isClassI_;
      bool isAligned_;
      bool isCongruent_;
      bool isMixed43_;
      int  numAp4_;
      bool isSuperfund_;
      int  sfRes_;

      std::int64_t mag_;
      std::int64_t maxD_;
      int radix_;
      std::int64_t maxI_;
      std::int64_t maxJ_;

      DgQ2DICoord firstAdd_;
      DgQ2DICoord lastAdd_;

      DgGridStats gridStats_;
      unsigned int precision_;

      static const DgQuadEdgeCells edgeTable_[12];

      // local internal network

      DgRFNetwork locNet_;

      DgDiscRF2D* grid2D_;
      DgDiscRFS2D* grid2DS_;
      DgContCartRF* ccFrame_;

      // intermediate RFs

      DgProjTriRF* projTriRF_;
      DgVertex2DDRF* vertexRF_;
      DgQ2DDRF* q2ddRF_;
      DgBoundedIDGG* bndRF_;
      DgInterleaveRF* intRF_;
      DgPlaneTriRF* planeRF_;

   friend class DgQ2DItoDConverter;
   friend class DgQ2DDtoIConverter;
   friend class DgBoundedIDGG;
};

////////////////////////////////////////////////////////////////////////////////
#endif
