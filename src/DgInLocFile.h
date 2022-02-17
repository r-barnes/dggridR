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
// DgInLocFile.h: DgInLocFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINLOCFILE_H
#define DGINLOCFILE_H

#include "DgBase.h"

#include <string>

using namespace std;

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgCell;
class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class DgInLocFile : public DgBase {

   public:

      DgInLocFile (const DgRFBase& rfIn, const string* fileNameIn = NULL,
           bool isPointFileIn = false, DgReportLevel failLevelIn = DgBase::Fatal)
         : DgBase (fileNameIn),
           rf_ (&rfIn), isPointFile_ (isPointFileIn), failLevel_ (failLevelIn)
      { if (fileNameIn) fileName_ = *fileNameIn; }

      const DgRFBase& rf (void) const { return *rf_; }

      const string& fileName (void) const { return fileName_; }

      bool isPointFile (void) const { return isPointFile_; }

      void setIsPointFile (bool isPointFile = false)
                 { isPointFile_ = isPointFile; }

      DgReportLevel failLevel (void) { return failLevel_; }

      // pure virtual methods

      virtual bool open (const string* fileName = NULL,
                 DgReportLevel failLevel = DgBase::Fatal) = 0;

      virtual void close (void) = 0;

      virtual bool isEOF (void) = 0;

      virtual DgInLocFile& extract (DgLocList&   list) = 0;
      virtual DgInLocFile& extract (DgLocVector& vec)  = 0;
      virtual DgInLocFile& extract (DgPolygon&   poly) = 0;
      virtual DgInLocFile& extract (DgLocation&  loc)  = 0;
      virtual DgInLocFile& extract (DgCell&      cell) = 0;

   protected:

      string fileName_;

      const DgRFBase* rf_;

      bool isPointFile_;

      DgReportLevel failLevel_;
};

inline DgInLocFile& operator>> (DgInLocFile& input, DgLocList& list)
              { return input.extract(list); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgLocVector& vec)
              { return input.extract(vec); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgPolygon& poly)
              { return input.extract(poly); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgLocation& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgCell& cell)
              { return input.extract(cell); }

////////////////////////////////////////////////////////////////////////////////

#endif
