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
// DgOutGdalFile.h: DgOutGdalFile class definitions
//
// Version 7.0 - Elijah Anderson-Justis, 5/20/17
//
////////////////////////////////////////////////////////////////////////////////

// USE_GDAL is set in MakeIncludes
#ifdef USE_GDAL

#ifndef DGOUTGDALFILE_H
#define DGOUTGDALFILE_H

#include "DgOutLocTextFile.h"
#include "DgUtil.h"

#include <ogrsf_frmts.h>

class DgDVec2D;
class DgPolygon;
class DgGeoSphDegRF;

class DgOutGdalFile : public DgOutLocFile
{
    using DgOutLocFile::insert;

public:

    DgOutGdalFile(const DgGeoSphDegRF& rf,
                     const std::string& filename = "", const std::string& gdalDriver = "GeoJSON", int precision = 7,
                     bool isPointFile = false, DgReportLevel failLevel = DgBase::Fatal);

    ~DgOutGdalFile();

	// direct the DgOutLocFile abstract methods to the DgOutputStream ones
    virtual bool open (const string& fileName,
                       DgReportLevel failLevel = DgBase::Fatal)
    {
        return true;
    }

    virtual void close (void)
    {
       GDALClose( _dataset );
    }


    virtual DgOutLocFile& insert (DgLocation& loc, const string* label = NULL);
    virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL,
                                  const DgLocation* cent = NULL);
    virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL,
                                  const DgLocation* cent = NULL);

    virtual void setFormatStr(void)
    {

    }

protected:

    virtual DgOutLocFile& insert(const DgDVec2D& pt);

private:

    // Gdal helpers:
	std::string   _gdalDriver;
	GDALDriver   *_driver;
	GDALDataset  *_dataset;
	OGRLayer     *_oLayer;
	OGRFieldDefn *_oField;
        std::string fileNameOnly_;

    void init(const std::string& filename);
};

#endif

#endif
