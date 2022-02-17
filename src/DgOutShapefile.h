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
// DgOutShapefile.h: DgOutShapefile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTSHAPEFILE_H
#define DGOUTSHAPEFILE_H

#include "DgGeoSphRF.h"
#include "DgInShapefileAtt.h"
#include "DgOutLocFile.h"

#include <cstdio>
#include <set>
#include "shapefil.h"

class DgDVec2D;
class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgOutShapefile : public DgOutLocFile {

   using DgOutLocFile::insert;

   public:

      DgOutShapefile (const DgGeoSphDegRF& rfIn, const string& fileNameIn = "",
            int precision = 6, bool isPointFile = false,
            int shapefileIdLen = 11, DgReportLevel failLevel = DgBase::Fatal);

      virtual ~DgOutShapefile (void) { if (good()) close(); }

      int numDigits (void) { return numDigits_; }

      int idLen (void) { return idLen_; }

      bool open (const string& fileName,
                 DgReportLevel failLevel = DgBase::Fatal);

      virtual void close (void)
      {
         DBFClose(dbFile_);
         SHPClose(shpFile_);
      }

      bool good (void) { return dbFile_ && shpFile_; }

      virtual DgOutLocFile& insert (DgLocation& loc, const string* label = NULL);
      virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL,
                                const DgLocation* cent = NULL);
      virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL,
                                const DgLocation* cent = NULL);

      void addFields (const set<DgDBFfield>& fields);

      void setCurFields (const set<DgDBFfield>& fields) { curFields_ = fields; }

      const DgGeoSphRF& geoRF (void) const { return geoRF_; }

      int    defIntAttribute (void) const { return defIntAttribute_; }
      double defDblAttribute (void) const { return defDblAttribute_; }
      string defStrAttribute (void) const { return defStrAttribute_; }

      void setDefIntAttribute (int val)    { defIntAttribute_ = val; }
      void setDefDblAttribute (double val) { defDblAttribute_ = val; }
      void setDefStrAttribute (string val) { defStrAttribute_ = val; }

   protected:

      const DgGeoSphRF& geoRF_;

      DBFHandle dbFile_;
      SHPHandle shpFile_;

      string dbFileName_;
      string shpFileName_;

      int recNum_;
      int numDigits_;
      int numFields_;
      int idLen_;

      // default attribute values
      int defIntAttribute_;
      long double defDblAttribute_;
      string defStrAttribute_;

      set<DgDBFfield> curFields_; // non-null fields for current cell being written

      virtual DgOutLocFile& insert (const DgDVec2D& pt);

      void writeDbf (const string& id);
};

////////////////////////////////////////////////////////////////////////////////

#endif
