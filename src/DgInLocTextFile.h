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
// DgInLocTextFile.h: DgInLocTextFile class definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DGINLOCTEXTFILE_H
#define DGINLOCTEXTFILE_H

#include "DgInLocStreamFile.h"

class DgPolygon;
class DgGeoSphRF;
class DgLocationData;

////////////////////////////////////////////////////////////////////////////////
class DgInLocTextFile : public DgInLocStreamFile
{
public:

    DgInLocTextFile (const DgRFBase& rfIn,
                     const std::string* fileNameIn = nullptr,
                     DgReportLevel failLevel = DgBase::Fatal);

   ~DgInLocTextFile (void);

    bool forcePolyLine (void) const { return forcePolyLine_; }
    bool forceCells    (void) const { return forceCells_; }

    void setForcePolyLine (bool forcePolyLine = false)
                           { forcePolyLine_ = forcePolyLine; }

    void setForceCells (bool forceCells = false) { forceCells_ = forceCells; }

    //virtual DgInLocFile& extract (DgPolygon&      poly);
    virtual DgInLocFile& extract (DgCell&         cell);
    virtual DgInLocFile& extract (DgLocation&     loc);
    virtual DgInLocFile& extract (DgLocationData& loc);

protected:

    DgInLocFile& extractPointGeometry(DgLocation& point);

private:

    bool forcePolyLine_;
    bool forceCells_;
};

////////////////////////////////////////////////////////////////////////////////

#endif
