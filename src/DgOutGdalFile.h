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
// DgOutGdalFile.h: DgOutGdalFile class definitions
//
// Version 7.0 - Elijah Anderson-Justis, 5/20/17
//
////////////////////////////////////////////////////////////////////////////////

// USE_GDAL is set in MakeIncludes
#ifdef USE_GDAL

#ifndef DGOUTGDALFILE_H
#define DGOUTGDALFILE_H

#include "DgOutLocTextFile.h"
#include "DgUtil.h"

#include <ogrsf_frmts.h>

class DgDVec2D;
class DgPolygon;
class DgGeoSphDegRF;
class DgIDGGBase;

class DgOutGdalFile : public DgOutLocFile
{
   using DgOutLocFile::insert;

   public:

      static OGRLinearRing* createLinearRing (const DgPolygon& poly);
      static OGRPolygon*    createPolygon (const DgPolygon& poly);

      DgOutGdalFile(const DgGeoSphDegRF& rf, const std::string& filename = "", 
               const std::string& gdalDriver = "GeoJSON", 
               DgOutGdalFileMode mode = InvalidMode, int precision = 7,
               bool isPointFile = false, DgReportLevel failLevel = DgBase::Fatal);

      ~DgOutGdalFile();

      // direct the DgOutLocFile abstract methods to the DgOutputStream ones
      virtual bool open (const string& /* fileName */,
                       DgReportLevel /* failLevel = DgBase::Fatal */) {
         return true;
      }

      virtual void close (void) {
         GDALClose( _dataset );
      }

      virtual DgOutLocFile& insert (DgLocation& loc, const string* label = NULL);
      virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL,
                                    const DgLocation* cent = NULL);
      virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL,
                                    const DgLocation* cent = NULL);

      // collection output
      virtual DgOutLocFile& insert (const DgIDGGBase& dgg, DgCell& cell,
                      bool outputPoint, bool outputRegion, 
                      const DgLocVector* neighbors, const DgLocVector* children);

      virtual void setFormatStr(void) { }

   protected:

      DgOutGdalFileMode _mode;

      virtual DgOutLocFile& insert(const DgDVec2D& pt);

      OGRFeature* createFeature (const string& label) const;
      OGRPoint* createPoint (const DgLocation& loc) const;
      OGRGeometryCollection* createCollection (const DgCell& cell) const;
      void createSeqnumsProperty (const DgIDGGBase& dgg, OGRFeature* feature,
           const char* fieldName, const DgLocVector& vec);

      void addFeature (OGRFeature *feature);

   private:

      // Gdal helpers:
      std::string   _gdalDriver;
      GDALDriver   *_driver;
      GDALDataset  *_dataset;
      OGRLayer     *_oLayer;

      std::string fileNameOnly_;

      void init (bool outputPoint, bool outputRegion = false,
              bool outputNeighbors = false, bool outputChildren = false);
};

#endif

#endif
