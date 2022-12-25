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
// DgOutLocFile.h: DgOutLocFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTLOCFILE_H
#define DGOUTLOCFILE_H

#include "DgBase.h"

#include <string>

using namespace std;

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgCell;
class DgRFBase;
class DgDVec2D;
class DgIDGGBase;

////////////////////////////////////////////////////////////////////////////////
class DgOutLocFile : public DgBase {

   public:

      enum DgOutGdalFileMode { Polygon, Point, Collection, InvalidMode };

      const static string defaultKMLColor;
      const static int    defaultKMLWidth;
      const static string defaultKMLName;
      const static string defaultKMLDescription;

      virtual ~DgOutLocFile (void);

      // factory method
      static DgOutLocFile* makeOutLocFile (const string& type,
               const string& fileName, const string& gdalDriver, const DgRFBase& rf,
               bool isPointFile = false, int precision = 7, DgOutGdalFileMode mode = Polygon,
               int shapefileIdLen = 11, const string& kmlColor = defaultKMLColor,
               int kmlWidth = defaultKMLWidth,
               const string& kmlName = defaultKMLName, const string& kmlDesc = defaultKMLDescription,
               DgReportLevel failLevelIn = DgBase::Fatal);

      const string& fileName (void) { return fileName_; }

      DgReportLevel failLevel (void) { return failLevel_; }

      virtual bool open (const string& fileName,
                DgReportLevel failLevel = DgBase::Fatal) = 0;

      virtual void close (void) = 0;

      const DgRFBase& rf (void) const { return *rf_; }

      bool isPointFile() const { return isPointFile_; }

      void setIsPointFile(bool isPointFile = false)
              { isPointFile_ = isPointFile; }

      virtual DgOutLocFile& insert (DgCell& cell);
      virtual DgOutLocFile& insert (DgLocList& list);

      // collection output; does nothing by default
/* with parameter names
      virtual DgOutLocFile& insert (const DgIDGGBase& dgg, DgCell& cell,
                      bool outputPoint, bool outputRegion,
                      const DgLocVector* neighbors, const DgLocVector* children)
*/
      virtual DgOutLocFile& insert (const DgIDGGBase&, DgCell&, bool, bool,
                      const DgLocVector*, const DgLocVector*)
        { return *this; }

      // abstract virtual methods
      virtual DgOutLocFile& insert (DgLocation& loc,
                                const string* label = NULL) = 0;

      virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL,
                                const DgLocation* cent = NULL) = 0;

      virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL,
                                const DgLocation* cent = NULL) = 0;

   protected:

      DgOutLocFile (const string& fileName,
               const DgRFBase& rf, bool isPointFile = false,
               DgReportLevel failLevelIn = DgBase::Fatal)
         : DgBase ("DgOutLocFile:" + fileName),
           rf_ (&rf), fileName_ (fileName), isPointFile_ (isPointFile),
           failLevel_ (failLevelIn)
            { /* subclass must perform open */ }

      const DgRFBase* rf_;

      string fileName_;
      bool   isPointFile_;

      DgReportLevel failLevel_;
};

////////////////////////////////////////////////////////////////////////////////

//inline ostream& operator<< (ostream& output, const DgOutLocFile& f)
              //{ return output << f.name(); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgLocList& list)
              { return output.insert(list); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgLocVector& vec)
              { return output.insert(vec); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgPolygon& poly)
              { return output.insert(poly); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgLocation& loc)
              { return output.insert(loc); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgCell& cell)
              { return output.insert(cell); }


////////////////////////////////////////////////////////////////////////////////

#endif
