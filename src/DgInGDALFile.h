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
// DgInGdalFile.h: DgInGdalFile class definitions
//
// Version 7.0b - Kevin Sahr, 7/15/19
// Version 6.9 - Elijah Anderson-Justis, 5/20/17
//
///////////////////////////////////////////////////////////////////////////////

// USE_GDAL is set in MakeIncludes
#ifdef USE_GDAL

#ifndef DGINGDALFILE_H
#define DGINGDALFILE_H

#include <ogrsf_frmts.h>

#include "DgInLocStreamFile.h"

class DgPolygon;
class DgGeoSphRF;
class DgLocationData;
class DgDataList;

////////////////////////////////////////////////////////////////////////////////
class DgInGdalFile : public DgInLocStreamFile
{
public:

    DgInGdalFile (const DgRFBase& rfIn, const std::string* fileNameIn = NULL,
                   DgReportLevel failLevel = DgBase::Fatal);

   ~DgInGdalFile (void);

    bool forcePolyLine (void) const { return forcePolyLine_; }
    bool forceCells    (void) const { return forceCells_; }

    void setForcePolyLine (bool forcePolyLine = false)
                           { forcePolyLine_ = forcePolyLine; }

    void setForceCells (bool forceCells = false) { forceCells_ = forceCells; }

    virtual DgInLocFile& extract (DgPolygon&      poly);
    virtual DgInLocFile& extract (DgCell&         cell);
    virtual DgInLocFile& extract (DgLocation&     loc);
    virtual DgInLocFile& extract (DgLocationData& loc);

protected:

    void ogrLinearRingToDg (OGRLinearRing* oLinearRing, DgPolygon& poly);
    void ogrPolyToDg (OGRPolygon* oPolygon, DgPolygon& poly);
    void ogrPointToDg (const OGRPoint& oPolygon, DgLocation& point);

    DgInLocFile& extractPointGeometry(DgLocation& point);
    DgDataList*  extractDataFields (void);

private:

    bool forcePolyLine_;
    bool forceCells_;

    GDALDataset* gdalDataset_;
    OGRFeature* oFeature_;
    bool insideMultiPoly_;
    int multiPolyIndex_;
    int numMultiPolyGeometries_;
};

////////////////////////////////////////////////////////////////////////////////
inline DgInLocFile& operator>> (DgInGdalFile& input, DgPolygon& poly)
              { return input.extract(poly); }

inline DgInLocFile& operator>> (DgInGdalFile& input, DgLocation& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInGdalFile& input, DgLocationData& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInGdalFile& input, DgCell& cell)
              { return input.extract(cell); }

////////////////////////////////////////////////////////////////////////////////

#endif
#endif
