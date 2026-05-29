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
// DgInGdalFile.cpp: DgInGdalFile class implementation
//
// Version 7.0b - Kevin Sahr, 7/15/19
// Version 6.9 - Elijah Anderson-Justis, 5/20/17
//
////////////////////////////////////////////////////////////////////////////////

// USE_GDAL is set in MakeIncludes
#ifdef USE_GDAL

#include <cstdint>
#include <ogrsf_frmts.h>

#include "DgInGdalFile.h"
#include "DgLocation.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgContCartRF.h"
#include "DgEllipsoidRF.h"
#include "DgGeoSphRF.h"
#include "DgDataList.h"
#include "DgDataField.h"

#include <sstream>

////////////////////////////////////////////////////////////////////////////////
DgInGdalFile::DgInGdalFile (const DgRFBase& rfIn, const std::string* fileNameIn,
                              DgReportLevel failLevel)
    : DgInLocStreamFile (rfIn, fileNameIn, false, failLevel),
      forcePolyLine_ (false), forceCells_ (false),
      gdalDataset_ (nullptr), oFeature_ (nullptr),
      insideMultiPoly_ (false), multiPolyIndex_ (0), numMultiPolyGeometries_ (0)
{
   // the rf needs to be lat/lon
   degRF_ = dynamic_cast<const DgGeoSphDegRF*>(&rfIn);
   if (!degRF_) {
      report("DgInGdalFile::DgInGdalFile(): RF " + rfIn.name() +
             " is not longitude/latitude", DgBase::Fatal);
   }

/* not needed since must be lat/lon
   // test for override of vecAddress
   DgAddressBase* dummy = rfIn.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy) {
      report("DgInGdalFile::DgInGdalFile(): RF " + rfIn.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   }
   delete dummy;
*/

   GDALAllRegister();
   gdalDataset_ = (GDALDataset*) GDALOpenEx(fileName().c_str(),
                                        GDAL_OF_VECTOR, NULL, NULL, NULL);
   if (gdalDataset_ == NULL) {
      report("Invalid GDAL data model in file " + fileName(), DgBase::Fatal);
   }

} // DgInGdalFile::DgInGdalFile

////////////////////////////////////////////////////////////////////////////////
DgInGdalFile::~DgInGdalFile (void)
{
   if (oFeature_) OGRFeature::DestroyFeature(oFeature_);

} // DgInGdalFile::~DgInGdalFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgInGdalFile::ogrLinearRingToDg (OGRLinearRing* oLinearRing, DgPolygon& poly)
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
void
DgInGdalFile::ogrPointToDg (const OGRPoint& oPoint, DgLocation& point)
{
   long double lon, lat;
   lon = oPoint.getX();
   lat = oPoint.getY();

   //DgAddressBase* add = rf().vecAddress(DgGeoCoord(lon, lat));
   //DgLocation* tmpLoc = rf().makeLocation(*add);
   //delete add;

   DgLocation* tmpLoc = degRF_->makeLocation(DgGeoCoord(lon, lat));

/* should be done by the operator=
   point.clearAddress();
   rf().convert(&point);
*/
   point = *tmpLoc;
   delete tmpLoc;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgInGdalFile::ogrPolyToDg (OGRPolygon* oPolygon, DgPolygon& poly)
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
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGdalFile::extract (DgPolygon& poly)
//
// Get the next polygon from me and put it in poly.
//
////////////////////////////////////////////////////////////////////////////////
{
/*
   if (isPointFile())
       report("Attempt to read a polygon from a point GDAL file", DgBase::Fatal);
*/

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
          dgcout << "WKBGeometryType: " << geomType << std::endl;
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
//cout << "=====\n" << poly << std::endl;

   return *this;

} // DgInGdalFile& DgInGdalFile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgDataList*
DgInGdalFile::extractDataFields (void)
//
// Extract all data fields from this feature into a DataList.
// Returns the new DataList or nullptr if there are no fields.
//
////////////////////////////////////////////////////////////////////////////////
{
   DgDataList* data = nullptr;
   if (oFeature_->GetFieldCount() > 0) {

      // need to grab the data fields
      data = new DgDataList();

      OGRLayer* oLayer = gdalDataset_->GetLayer(0);
      OGRFeatureDefn* oFDefn = oLayer->GetLayerDefn();
      for (int iField = 0; iField < oFDefn->GetFieldCount(); iField++) {

          if( !oFeature_->IsFieldSet(iField) ) {
              // ignore
              //printf("(unset),");
              continue;
          }

          if( oFeature_->IsFieldNull(iField) ) {
              // ignore
              //printf("(null),");
              continue;
          }

          DgDataFieldBase* fld = nullptr;
          OGRFieldDefn *oFieldDefn = oFDefn->GetFieldDefn( iField );
          std::string fldName = std::string(oFieldDefn->GetNameRef());
          switch( oFieldDefn->GetType() ) {
              case OFTInteger:
                 fld = new DgDataFieldInt(fldName,
                                      (*oFeature_)[iField].GetInteger());
                 break;
              case OFTInteger64:

//int64_t ggg = (*oFeature_)[iField].GetInteger64();
    //              int64_t abc = 0;

                 fld = new DgDataFieldInt64(fldName,
                                      (*oFeature_)[iField].GetInteger64());
                 break;
              case OFTReal:
                 fld = new DgDataFieldDouble(fldName,
                                      (*oFeature_)[iField].GetDouble());
                 break;

              case OFTString:
              default:
              {
                 const char* fldVal = (*oFeature_)[iField].GetAsString();
                 char* cellVal = new char[strlen(fldVal) + 1];
                 strcpy(cellVal, fldVal);
                 fld = new DgDataFieldString(fldName, cellVal);
/*
                 if (oFieldDefn->GetType() != OFTString)
                    dgcerr << "unsupported input field type: " <<
                                oFeature_->GetFieldAsString(iField) << std::endl;
*/
                 break;
              }
/*
              default: // convert to a string any field types not yet handled

                 const char* fldVal = (*oFeature_)[iField].GetFieldAsString();
                 char* cellVal = new char[strlen(fldVal) + 1];
                 strcpy(cellVal, fldVal);
                 fld = new DgDataFieldString(fldName, cellVal);
                 if (oFieldDefn->GetType() != OFTString)
                    dgcerr << "unsupported input field type: " <<
                                oFeature_->GetFieldAsString(iField) << std::endl;
                 break;
                 report("field ignored", DgBase::Warning);
*/
                 if (oFieldDefn->GetType() != OFTString)
                    dgcerr << "unsupported input field type: " <<
                                oFeature_->GetFieldAsString(iField) << std::endl;
                 continue;
          }

          if (fld)
             data->addField(fld);
      }
   }

   return data;

} // DgDataList* DgInGdalFile::extractDataFields

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGdalFile::extract (DgCell& cell)
//
// Get the next cell from me and put it in cell.
//
////////////////////////////////////////////////////////////////////////////////
{
   // cell label and region will be empty
   // need to check for name field below and use for label
   cell.setLabel("");
   cell.setRegion(nullptr);

   // get the cell node
   DgLocation point;
   extractPointGeometry(point); // should set oFeature_
   cell.setNode(point);

   // get any other data fields
   cell.setDataList(extractDataFields());

   return *this;

} // DgInGdalFile& DgInGdalFile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGdalFile::extractPointGeometry (DgLocation& point) {

    OGRPoint* oPoint = nullptr;
    OGRGeometry* oGeometry = nullptr;

    if (gdalDataset_->GetLayerCount() != 1) {
       report("Multiple layers in input file.", DgBase::Fatal);
    }

    OGRLayer* oLayer = gdalDataset_->GetLayer(0);
    if (oFeature_) OGRFeature::DestroyFeature(oFeature_);

    if ((oFeature_ = oLayer->GetNextFeature()) == nullptr) {
       setstate(ios_base::eofbit);
       return *this;
    }

    // Get the point stored in Geometry
    oGeometry = oFeature_->GetGeometryRef();
    OGRwkbGeometryType geomType = wkbFlatten((oGeometry->getGeometryType()));
    if (oGeometry != nullptr && geomType == wkbPoint) {
       oPoint = (OGRPoint*) oGeometry;
    } else {
       dgcout << "WKBGeometryType: " << geomType << std::endl;
       report("Geometry is not of type Point", DgBase::Fatal);
    }

    ogrPointToDg(*oPoint, point);

    return *this;

} // DgInLocFile& DgInGdalFile::extractPointGeometry

////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGdalFile::extract (DgLocation& point)
//
// Get the next point DgLocation. The input location must be a lat/lon coordinate.
//
////////////////////////////////////////////////////////////////////////////////
{
/*
   if (!isPointFile())
       report("Attempt to read a point from a non-point GDAL file", DgBase::Fatal);
*/

   point.clearAddress();
   rf().convert(&point);

   // get the point geometry
   extractPointGeometry(point);

   return *this;

} // DgInLocFile& DgInGdalFile::extract

////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInGdalFile::extract (DgLocationData& point)
//
// Get the next point DgLocation with data fields. The input location must be
// a lat/lon coordinate.
//
////////////////////////////////////////////////////////////////////////////////
{
   // first get the DgLocation stuff
   extract((DgLocation&) point);
   if (eof()) return *this;

   // get any other data fields
   point.setDataList(extractDataFields());

   return *this;

} // DgInLocFile& DgInGdalFile::extract

////////////////////////////////////////////////////////////////////////////////

#endif
