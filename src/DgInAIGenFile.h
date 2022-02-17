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
// DgInAIGenFile.h: DgInAIGenFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINAIGENFILE_H
#define DGINAIGENFILE_H

#include "DgInLocTextFile.h"

class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgInAIGenFile : public DgInLocTextFile {

   public:

      DgInAIGenFile (const DgRFBase& rfIn, const string* fileNameIn = NULL,
                        DgReportLevel failLevel = DgBase::Fatal);

      bool forcePolyLine (void) const { return forcePolyLine_; }
      bool forceCells    (void) const { return forceCells_; }

      void setForcePolyLine (bool forcePolyLine = false)
                 { forcePolyLine_ = forcePolyLine; }

      void setForceCells (bool forceCells = false)
                 { forceCells_ = forceCells; }

      virtual DgInLocFile& extract (DgLocList& list);
      virtual DgInLocFile& extract (DgLocVector& vec);
      virtual DgInLocFile& extract (DgPolygon& poly);
      virtual DgInLocFile& extract (DgLocation& loc);
      virtual DgInLocFile& extract (DgCell& cell);

   private:
      
      bool forcePolyLine_;
      bool forceCells_;

};

////////////////////////////////////////////////////////////////////////////////

#endif
