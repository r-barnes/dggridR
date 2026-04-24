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
// DgInLocFile.h: DgInLocFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINLOCFILE_H
#define DGINLOCFILE_H

#include "DgBase.h"

#include <string>

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgLocationData;
class DgCell;
class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class DgInLocFile : public DgBase {

   public:

      DgInLocFile (const DgRFBase& rfIn, const std::string* fileNameIn = nullptr,
           bool isPointFileIn = false, DgReportLevel failLevelIn = DgBase::Fatal)
         : DgBase (fileNameIn),
           rf_ (&rfIn), isPointFile_ (isPointFileIn), failLevel_ (failLevelIn)
      { if (fileNameIn) fileName_ = *fileNameIn; }

      const DgRFBase& rf (void) const { return *rf_; }

      const std::string& fileName (void) const { return fileName_; }

      bool isPointFile (void) const { return isPointFile_; }

      void setIsPointFile (bool isPointFile = false)
                 { isPointFile_ = isPointFile; }

      DgReportLevel failLevel (void) { return failLevel_; }

      // pure virtual methods

      virtual bool open (const std::string* fileName = NULL,
                 DgReportLevel failLevel = DgBase::Fatal) = 0;

      virtual void close (void) = 0;

      virtual bool isEOF (void) = 0;

      virtual DgInLocFile& extract (DgLocList& list)
            { report("DgInLocFile::extract(DgLocList) not implemented.",
              DgBase::Fatal);
              return *this; }

      virtual DgInLocFile& extract (DgLocVector& vec)
            { report("DgInLocFile::extract(DgLocVector) not implemented.",
              DgBase::Fatal);
              return *this; }

      virtual DgInLocFile& extract (DgPolygon& poly)
            { report("DgInLocFile::extract(DgPolygon) not implemented.",
              DgBase::Fatal);
              return *this; }

      virtual DgInLocFile& extract (DgLocation& loc)
            { report("DgInLocFile::extract(DgLocation) not implemented.",
              DgBase::Fatal);
              return *this; }

      virtual DgInLocFile& extract (DgLocationData& locData)
            { report("DgInLocFile::extract(DgLocationData) not implemented.",
              DgBase::Fatal);
              return *this; }

      virtual DgInLocFile& extract (DgCell& cell)
            { report("DgInLocFile::extract(DgCell) not implemented.",
              DgBase::Fatal);
              return *this; }

   protected:

      std::string fileName_;

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

inline DgInLocFile& operator>> (DgInLocFile& input, DgLocationData& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgCell& cell)
              { return input.extract(cell); }

////////////////////////////////////////////////////////////////////////////////

#endif
