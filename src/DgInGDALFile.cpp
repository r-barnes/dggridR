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
// DgInGDALFile.cpp: DgInGDALFile class implementation
//
// Version 7.0b - Kevin Sahr, 7/15/19
// Version 6.9 - Elijah Anderson-Justis, 5/20/17
//
////////////////////////////////////////////////////////////////////////////////

// USE_GDAL is set in MakeIncludes
#ifdef USE_GDAL


#include "DgInGDALFile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgContCartRF.h"

#include <ogrsf_frmts.h>

#include <sstream>

////////////////////////////////////////////////////////////////////////////////
DgInGDALFile::DgInGDALFile (const DgRFBase& rfIn, const string* fileNameIn,
                              DgReportLevel failLevel)
    : DgInLocTextFile (rfIn, fileNameIn, false, failLevel),
      forcePolyLine_ (false), forceCells_ (false),
      gdalDataset_ (NULL), oFeature_ (NULL),
      insideMultiPoly_ (false), multiPolyIndex_ (0), numMultiPolyGeometries_ (0)
{
   // test for override of vecAddress
   DgAddressBase* dummy = rfIn.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy)
      report("DgInGDALFile::DgInGDALFile(): RF " + rfIn.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   delete dummy;

   GDALAllRegister();

    gdalDataset_ = (GDALDataset*) GDALOpenEx(fileName().c_str(),
                                        GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (gdalDataset_ == NULL) {
        report("Invalid GDAL data model in file " + fileName(), DgBase::Fatal);
    }

} // DgInGDALFile::DgInGDALFile

////////////////////////////////////////////////////////////////////////////////
DgInGDALFile::~DgInGDALFile (void)
{
   if (oFeature_) OGRFeature::DestroyFeature(oFeature_);

} // DgInGDALFile::~DgInGDALFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGDALFile::extract (DgLocVector&)
//
// Get the next polyline from me and put it in vec.
//
////////////////////////////////////////////////////////////////////////////////
{
    report("DgInGDALFile::extract(DgLocVector) not yet implemented.", DgBase::Fatal);
    return *this;
} // DgInGDALFile& DgInGDALFile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgInGDALFile::ogrLinearRingToDg (OGRLinearRing* oLinearRing, DgPolygon& poly)
{
   int numPoints = oLinearRing->getNumPoints();
   long double x, y;
   OGRPoint oPoint;
   for (int i = 0; i < numPoints; i++) {
      oLinearRing->getPoint(i, &oPoint);
      x = oPoint.getX();
      y = oPoint.getY();
      DgAddressBase* add = rf().vecAddress(DgDVec2D(x, y));
      poly.addressVec().push_back(add);
   }

   // remove the duplicate first/last vertex
   DgAddressBase* lastPt = *(poly.addressVec().end() - 1);
   poly.addressVec().erase(poly.addressVec().end() - 1);
   delete lastPt;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgInGDALFile::ogrPolyToDg (OGRPolygon* oPolygon, DgPolygon& poly)
{
   OGRLinearRing* oLinearRing = oPolygon->getExteriorRing();
   ogrLinearRingToDg(oLinearRing, poly);
   for (int i = 0; i < oPolygon->getNumInteriorRings(); i++) {
      oLinearRing = oPolygon->getInteriorRing(i);

      DgPolygon* hole = new DgPolygon(rf());
      ogrLinearRingToDg(oLinearRing, *hole);
      // poly takes ownership of the hole memory
      poly.addHole(hole);
   }

/*
//OGRPolygon polygon;
OGRPolygon* polygon = (OGRPolygon*) OGRGeometryFactory::createGeometry(wkbPolygon);
polygon->addRingDirectly(oLinearRing);

cout << "HOLES?" << endl;
    // first one is the outer ring and, all the next ones are the 
    // inner rings/holes
    //oPolygon = (OGRPolygon*) oGeometry;
    for (int i = 0; i < oPolygon->getNumInteriorRings(); i++) {

   OGRLinearRing* oLinearRing = oPolygon->getInteriorRing(i);
OGRPolygon* hole = (OGRPolygon*) OGRGeometryFactory::createGeometry(wkbPolygon);
hole->addRingDirectly(oLinearRing);

      cout << "hole i: " << i << endl;
cout << " contained? " << polygon->Contains(hole) << " " <<
     hole->Contains(polygon) << endl;
    }
*/

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGDALFile::extract (DgPolygon& poly)
//
// Get the next polygon from me and put it in poly.
//
////////////////////////////////////////////////////////////////////////////////
{
    poly.clearAddress();
    rf().convert(poly);

    OGRPolygon* oPolygon = NULL;
    OGRGeometry* oGeometry = NULL;
    if (!insideMultiPoly_) {

       if (gdalDataset_->GetLayerCount() != 1) {
          report("Multiple layers in input file.", DgBase::Fatal);
       }

       OGRLayer* oLayer = gdalDataset_->GetLayer(0);
       if (oFeature_) OGRFeature::DestroyFeature(oFeature_);

       if ((oFeature_ = oLayer->GetNextFeature()) == NULL) {
           setstate(ios_base::eofbit);
           return *this;
       }

       // Get the polygon stored in Geometry, with special handling for MultiPolygon
       //OGRGeometry* oGeometry = oFeature_->GetGeometryRef();
       oGeometry = oFeature_->GetGeometryRef();
       OGRwkbGeometryType geomType = wkbFlatten((oGeometry->getGeometryType()));
       if (oGeometry != NULL && geomType == wkbPolygon) {
           oPolygon = (OGRPolygon*) oGeometry;
       } else if (geomType == wkbMultiPolygon || geomType == wkbGeometryCollection) {
           insideMultiPoly_ = true;
           multiPolyIndex_ = 0;
           OGRMultiPolygon* oMultiPolygon = (OGRMultiPolygon*) oGeometry;
           numMultiPolyGeometries_ = oMultiPolygon->getNumGeometries();
       } else {
           dgcout << "WKBGeometryType: " << geomType << endl;
           report("Geometry is not of type Polygon or MultiPolygon", DgBase::Fatal);
       }
    }

    // now we either have a polygon or we are inside a multi-polygon
    if (insideMultiPoly_) {
       //OGRGeometry* oGeometry = oFeature_->GetGeometryRef();
       oGeometry = oFeature_->GetGeometryRef();
       OGRMultiPolygon* oMultiPolygon = (OGRMultiPolygon*) oGeometry;
       oPolygon = (OGRPolygon*) oMultiPolygon->getGeometryRef(multiPolyIndex_);
       multiPolyIndex_++;
       // check if we're at the end of the multipolygon
       if (multiPolyIndex_ >= numMultiPolyGeometries_) {
          insideMultiPoly_ = false;
          multiPolyIndex_ = 0;
          numMultiPolyGeometries_ = 0;
       }
    }

    // convert the exterior ring to a DgPolygon
    ogrPolyToDg(oPolygon, poly);
//cout << "=====\n" << poly << endl;

    return *this;

} // DgInGDALFile& DgInGDALFile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGDALFile::extract (DgCell&)
//
// Get the next cell from me and put it in cell.
//
////////////////////////////////////////////////////////////////////////////////
{
    report("DgInGDALFile::extract(DgCell) not yet implemented.", DgBase::Fatal);
    return *this;
} // DgInGDALFile& DgInGDALFile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGDALFile::extract (DgLocList&)
//
// Determine whether the file is a point or polygon/polyline file. If it's
// a point file, read-in the points. If not, get the sets which constitute
// me. If the last point in a set is the same as the first, assume it's a
// polygon. Otherwise, make it a polyline.
//
////////////////////////////////////////////////////////////////////////////////
{
    report("DgInLocFile::extract(DgLocList) not yet implemented.", DgBase::Fatal);
    return *this;
} // DgInLocFile& DgInGDALFile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGDALFile::extract (DgLocation&)
//
// Get the next DgLocation. For speed will mistake file corruption for the "END"
// at the end of the gen file.
//
////////////////////////////////////////////////////////////////////////////////
{
    report("DgInGDALFile::extract(DgLocation) not yet implemented.", DgBase::Fatal);
    return *this;
} // DgInLocFile& DgInGDALFile::extract

////////////////////////////////////////////////////////////////////////////////

#endif
