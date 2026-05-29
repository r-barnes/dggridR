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
// DgInLocStreamFile.h: DgInLocStreamFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINLOCSTREAMFILE_H
#define DGINLOCSTREAMFILE_H

#include "DgInLocFile.h"

#include <fstream>
#include <string>

class DgGeoSphDegRF;
class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgLocationData;
class DgCell;
class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class DgInLocStreamFile : public std::ifstream, public DgInLocFile {

   public:

      DgInLocStreamFile (const DgRFBase& rfIn,
                       const std::string* fileNameIn = NULL,
                       bool isPointFileIn = false,
                       DgReportLevel failLevel = DgBase::Fatal);

      void rewind (void) { seekg(std::streampos(0)); clear(); }

      virtual bool open (const std::string* fileName = NULL,
                 DgReportLevel failLevel = DgBase::Fatal);

      virtual void close (void) { std::ifstream::close(); }

      virtual bool isEOF (void) { return eof(); }

   protected:

      const DgGeoSphDegRF* degRF_;
};

inline DgInLocFile& operator>> (DgInLocStreamFile& input, DgLocList& list)
              { return input.extract(list); }

inline DgInLocFile& operator>> (DgInLocStreamFile& input, DgLocVector& vec)
              { return input.extract(vec); }

inline DgInLocFile& operator>> (DgInLocStreamFile& input, DgPolygon& poly)
              { return input.extract(poly); }

inline DgInLocFile& operator>> (DgInLocStreamFile& input, DgLocation& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInLocStreamFile& input, DgLocationData& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInLocStreamFile& input, DgCell& cell)
              { return input.extract(cell); }

////////////////////////////////////////////////////////////////////////////////

#endif
