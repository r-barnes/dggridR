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
// DgInShapefile.h: DgInShapefile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINSHAPEFILE_H
#define DGINSHAPEFILE_H

#include "DgInLocFile.h"

#include "shapefil.h"

#include <fstream>
#include <string>

using namespace std;

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgCell;
class DgRFBase;
class DgGeoSphRF;

////////////////////////////////////////////////////////////////////////////////
class DgInShapefile : public DgInLocFile {

   public:

      DgInShapefile (const DgGeoSphRF& geoRFIn,
                     const string* fileNameIn = NULL,
                     DgReportLevel failLevel = DgBase::Fatal);

      virtual bool open (const string* fileName = NULL,
                 DgReportLevel failLevel = DgBase::Fatal);

      virtual void close (void);

      virtual void rewind (void);

      int numEntities (void) { return numEntities_; }

      bool isEOF (void) { return isEOF_; }

      // pure virtual methods
      virtual DgInLocFile& extract (DgLocList&   list);
      virtual DgInLocFile& extract (DgLocVector& vec);
      virtual DgInLocFile& extract (DgPolygon&   poly);
      virtual DgInLocFile& extract (DgLocation&  loc);
      virtual DgInLocFile& extract (DgCell&      cell);

      const DgGeoSphRF& geoRF (void) const { return geoRF_; }

   protected:

      const DgGeoSphRF& geoRF_;

      virtual void getNextEntity (void);

      SHPHandle shpFile_;
      int numEntities_;
      int nextRecNum_;
      SHPObject* curShpObj_;
      int curRecNum_;
      int curPart_;
      int nextPart_;
      bool isEOF_;
};

inline DgInLocFile& operator>> (DgInShapefile& input, DgLocList& list)
              { return input.extract(list); }

inline DgInLocFile& operator>> (DgInShapefile& input, DgLocVector& vec)
              { return input.extract(vec); }

inline DgInLocFile& operator>> (DgInShapefile& input, DgPolygon& poly)
              { return input.extract(poly); }

inline DgInLocFile& operator>> (DgInShapefile& input, DgLocation& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInShapefile& input, DgCell& cell)
              { return input.extract(cell); }

////////////////////////////////////////////////////////////////////////////////

#endif
