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
///////////////////////////////////////////////////////////////////////////////
//
// DgIDGGBase.h: DgIDGGBase class definitions
//
// Version 7.0 - Kevin Sahr, 11/16/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGIDGGBASE_H 
#define DGIDGGBASE_H

#include "DgIDGGutil.h"

class DgIDGGSBase;

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//  Icosahedral DGG class.
//
class DgIDGGBase : public DgDiscRF<DgQ2DICoord, DgGeoCoord, long double> {

   using DgDiscRF<DgQ2DICoord, DgGeoCoord, long double>::setVertices;

   public:

      static const DgIDGGBase* makeRF (const DgIDGGSBase* dggs, const DgGeoSphRF& geoRFIn, 
                  unsigned int apertureIn, int resIn, const string& nameIn = "IDGG", 
                  DgGridTopology gridTopo = Hexagon, DgGridMetric gridMetric = D6,
                  unsigned int precisionIn = DEFAULT_PRECISION)
         { return new DgIDGGBase (dggs, geoRFIn, apertureIn, resIn, nameIn, 
                  gridTopo, gridMetric, precisionIn); }

      virtual ~DgIDGGBase();

      const DgIDGGSBase* dggs (void) const { return dggs_; }

      const DgGeoSphRF&  geoRF      (void) const;
      const DgGeoCoord&  vert0      (void) const;
      long double        azDegs     (void) const;
      const string&      projType   (void) const;
      DgGridTopology     gridTopo   (void) const;
      DgGridMetric       gridMetric (void) const;

      unsigned int  aperture    (void) const { return aperture_; }
      int           res         (void) const { return res_; }
      int           allocRes    (void) const { return allocRes_; }
      long long int maxI        (void) const { return maxI_; }
      long long int maxJ        (void) const { return maxJ_; }
      long long int mag         (void) const { return mag_; }
      long long int maxD        (void) const { return maxD_; }

      bool               isAligned   (void) const { return isAligned_; }
      bool               isCongruent (void) const { return isCongruent_; }
      bool               isClassI    (void) const { return isClassI_; }
      bool               isClassIII  (void) const { return isClassIII_; }

      int                radix       (void) const { return radix_; }
      const DgQ2DICoord& firstAdd    (void) const { return firstAdd_; }
      const DgQ2DICoord& lastAdd     (void) const { return lastAdd_; }

      virtual int outputRes (void) const { return res_; }

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

      virtual long long int dist (const DgQ2DICoord&, const DgQ2DICoord&) const 
           {
              report(string("DgIDGGBase::dist() this method has not been defined "
                     "for DgIDGGBase ") + this->name(), DgBase::Fatal);

	      return M_ZERO;
           }

      virtual void setAddNeighbors (const DgQ2DICoord& add,
                                    DgLocVector& vec) const; 

      virtual void setAddNeighborsBdry2 (const DgQ2DICoord& add,
                                    DgLocVector& vec) const; 

      // give dummy definitions; these will not be used but will be
      // overridden by series converters

      virtual DgQ2DICoord quantify (const DgGeoCoord&) const
                { DgQ2DICoord v; return v; }

      virtual DgGeoCoord invQuantify (const DgQ2DICoord&) const
                { DgGeoCoord g; return g; }

      static const DgQuadEdgeCells& edgeTable (int quadNum)
                       { return edgeTable_[quadNum]; }

   protected:

      DgIDGGBase (const DgIDGGSBase* dggs, const DgGeoSphRF& geoRFIn, 
                  unsigned int apertureIn, int resIn, const string& nameIn = "IDGG", 
                  DgGridTopology gridTopo = Hexagon, DgGridMetric gridMetric = D6,
                  unsigned int precisionIn = DEFAULT_PRECISION);

      DgIDGGBase (const DgIDGGBase& grd);

      virtual void createConverters (void);

      const DgDiscRFS2D& grid2DS (void) const { return *grid2DS_; }

      const DgIDGGSBase* dggs_;

      DgSphIcosa* sphIcosa_;

      unsigned int aperture_;
      int res_;
      int allocRes_;
      bool isClassI_;
      bool isClassIII_;
      bool isAligned_;
      bool isCongruent_;

      long long int mag_;
      long long int maxD_;
      int radix_;
      long long int maxI_;
      long long int maxJ_;

      DgQ2DICoord firstAdd_;
      DgQ2DICoord lastAdd_;

      DgGridStats gridStats_;
      unsigned int precision_;

      static const DgQuadEdgeCells edgeTable_[12];

      // local internal network

      DgRFNetwork locNet_;

      const DgDiscRF2D* grid2D_;
      const DgDiscRFS2D* grid2DS_;
      const DgContCartRF* ccFrame_;

      // intermediate RFs

      const DgProjTriRF* projTriRF_;
      const DgVertex2DDRF* vertexRF_;
      const DgQ2DDRF* q2ddRF_;
      const DgBoundedIDGG* bndRF_;
      const DgInterleaveRF* intRF_;
      const DgPlaneTriRF* planeRF_;

   friend class DgQ2DItoDConverter;
   friend class DgQ2DDtoIConverter;
   friend class DgBoundedIDGG;

   friend ostream& operator<< (ostream& stream, const DgIDGGBase& dgg);
};

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgIDGGBase& dgg)
{
   stream << "geoRF: " << dgg.geoRF();
   stream << "\nvert0: " << dgg.vert0();
   stream << "\nazDegs: " << dgg.azDegs();

   stream << "\ngridTopo: " << dgg.gridTopo();
   stream << "\ngridMetric: " << dgg.gridMetric();
   stream << "\nprojType: " << dgg.projType();

   stream << "\naperture: " << dgg.aperture_;

   stream << "\nres: " << dgg.res_;
   stream << "\nallocRes: " << dgg.allocRes_;
   stream << "\nisClassI: " << dgg.isClassI_;
   stream << "\nisClassIII: " << dgg.isClassIII_;
   stream << "\nisAligned: " << dgg.isAligned_;
   stream << "\nisCongruent: " << dgg.isCongruent_;
//   stream << "\nisMixed43: " << dgg.isMixed43_;
//   stream << "\nnumAp4: " << dgg.numAp4_;
//   stream << "\nisSuperfund: " << dgg.isSuperfund_;
//   stream << "\nsfRes: " << dgg.sfRes_;

   stream << "\nmag: " << dgg.mag_;
   stream << "\nmaxD: " << dgg.maxD_;
   stream << "\nradix: " << dgg.radix_;
   stream << "\nmaxI: " << dgg.maxI_;
   stream << "\nmaxJ: " << dgg.maxJ_;

   stream << "\nfirstAdd: " << dgg.firstAdd_;
   stream << "\nlastAdd: " << dgg.lastAdd_;

   stream << "\nprecision: " << dgg.precision_ << endl;

   return stream;
}

////////////////////////////////////////////////////////////////////////////////
#endif
