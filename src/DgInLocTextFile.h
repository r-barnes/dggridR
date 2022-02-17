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
// DgInLocTextFile.h: DgInLocTextFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINLOCTEXTFILE_H
#define DGINLOCTEXTFILE_H

#include "DgInLocFile.h"

#include <fstream>
#include <string>

using namespace std;

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgCell;
class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class DgInLocTextFile : public ifstream, public DgInLocFile {

   public:

      DgInLocTextFile (const DgRFBase& rfIn,
                       const string* fileNameIn = NULL,
                       bool isPointFileIn = false,
                       DgReportLevel failLevel = DgBase::Fatal);

      void rewind (void) { seekg(std::streampos(0)); clear(); }

      virtual bool open (const string* fileName = NULL,
                 DgReportLevel failLevel = DgBase::Fatal);

      virtual void close (void) { ifstream::close(); }

      virtual bool isEOF (void) { return eof(); }

      // pure virtual methods
      virtual DgInLocFile& extract (DgLocList&   list) = 0;
      virtual DgInLocFile& extract (DgLocVector& vec)  = 0;
      virtual DgInLocFile& extract (DgPolygon&   poly) = 0;
      virtual DgInLocFile& extract (DgLocation&  loc)  = 0;
      virtual DgInLocFile& extract (DgCell&      cell) = 0;

};

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgLocList& list)
              { return input.extract(list); }

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgLocVector& vec)
              { return input.extract(vec); }

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgPolygon& poly)
              { return input.extract(poly); }

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgLocation& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgCell& cell)
              { return input.extract(cell); }

////////////////////////////////////////////////////////////////////////////////

#endif
