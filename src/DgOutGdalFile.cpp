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
// DgOutGdalFile.cpp: DgOutGdalFile class implementation
//
// Version 7.0 - Elijah Anderson-Justis, 5/20/17
//
////////////////////////////////////////////////////////////////////////////////

// USE_GDAL is set in MakeIncludes
#ifdef USE_GDAL

#include <sstream>
#include <iostream>
#include <string>

#include "DgOutGdalFile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgGeoSphRF.h"
#include "DgIDGGSBase.h"
#include "DgBoundedIDGG.h"

////////////////////////////////////////////////////////////////////////////////
DgOutGdalFile::DgOutGdalFile (const DgGeoSphDegRF& rf,
                    const std::string& filename, const std::string& gdalDriver,
                    DgOutGdalFileMode mode, int /* precision */, bool isPointFile,
                    DgReportLevel failLevel)
    : DgOutLocFile (filename, rf, isPointFile, failLevel), _mode (mode),
         _gdalDriver(""), _driver(NULL), _dataset(NULL), _oLayer(NULL),
         fileNameOnly_("")
{
   // test for override of vecAddress
   DgAddressBase* dummy = rf.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy)
        ::report("DgOutGdalFile::DgOutGdalFile(): RF " + rf.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   delete dummy;

   _gdalDriver = gdalDriver;
}

////////////////////////////////////////////////////////////////////////////////
DgOutGdalFile::~DgOutGdalFile()
{
   close();
}

////////////////////////////////////////////////////////////////////////////////
void
DgOutGdalFile::init (bool outputPoint, bool outputRegion,
                     bool outputNeighbors, bool outputChildren,
                     bool outputNdxParent, bool outputNdxChildren,
                     const DgDataList* dataList)
{
   fileNameOnly_ = DgOutLocFile::fileName();

   GDALAllRegister();
   _driver = GetGDALDriverManager()->GetDriverByName(_gdalDriver.c_str());
   if (_driver == NULL)
        ::report( _gdalDriver + " driver not available.",  DgBase::Fatal);

   _dataset = _driver->Create(fileNameOnly_.c_str(), 0, 0, 0, GDT_Unknown, NULL );
   if (_dataset == NULL)
      ::report( "Creation of output file '" + fileNameOnly_ + "' failed.",
          DgBase::Fatal );

   delete _oLayer;
   _oLayer = NULL;

   // create the layer that we will be using
   OGRwkbGeometryType geomType = wkbUnknown;
   switch (_mode) {
      case Polygon:
         geomType = wkbPolygon;
         break;
      case Point:
         geomType = wkbPoint;
         break;
      case Collection:
         if (outputPoint) {
            if (outputRegion)
               geomType = wkbGeometryCollection;
            else // just points
               geomType = wkbPoint;
         } else // just regions
            geomType = wkbPolygon;
         break;
      default:
         ::report( "Invalid GDAL file mode.", DgBase::Fatal );
   }

   std::string baseName = dgg::util::baseName(fileNameOnly_);
   _oLayer = _dataset->CreateLayer(baseName.c_str(), NULL, geomType, NULL );
   if (_oLayer == NULL)
      ::report( "Layer creation failed.", DgBase::Fatal );

   // create the name field
   OGRFieldDefn *fldDfn = new OGRFieldDefn( "name", OFTString );
   fldDfn->SetWidth(32);
   if (_oLayer->CreateField(fldDfn) != OGRERR_NONE)
      ::report("Creating name field failed.", DgBase::Fatal );
   delete fldDfn;
   fldDfn = NULL;

   // create the data fields
   if (dataList) {
      dataList->createFields(_oLayer);
   }

   if (outputNeighbors) {
      fldDfn = new OGRFieldDefn( "neighbors", OFTStringList );
      fldDfn->SetWidth(32);
      if (_oLayer->CreateField(fldDfn) != OGRERR_NONE)
         ::report("Creating neighbors field failed.", DgBase::Fatal );
      delete fldDfn;
      fldDfn = NULL;
   }

   if (outputChildren) {
      fldDfn = new OGRFieldDefn( "children", OFTStringList );
      fldDfn->SetWidth(32);
      if (_oLayer->CreateField(fldDfn) != OGRERR_NONE)
         ::report("Creating children field failed.", DgBase::Fatal );
      delete fldDfn;
      fldDfn = NULL;
   }

   if (outputNdxChildren) {
      fldDfn = new OGRFieldDefn( "ndxChildren", OFTStringList );
      fldDfn->SetWidth(32);
      if (_oLayer->CreateField(fldDfn) != OGRERR_NONE)
         ::report("Creating indexing children field failed.", DgBase::Fatal );
      delete fldDfn;
      fldDfn = NULL;
   }

   if (outputNdxParent) {
      fldDfn = new OGRFieldDefn( "ndxParent", OFTString );
      fldDfn->SetWidth(32);
      if (_oLayer->CreateField(fldDfn) != OGRERR_NONE)
         ::report("Creating indexing parent field failed.", DgBase::Fatal );
      delete fldDfn;
      fldDfn = NULL;
   }
}

////////////////////////////////////////////////////////////////////////////////
OGRFeature*
DgOutGdalFile::createFeature (const std::string& label) const
{
   OGRFeature *feature = OGRFeature::CreateFeature(_oLayer->GetLayerDefn());
   if (!feature)
      ::report("GDAL feature creation failed.", DgBase::Fatal );

   feature->SetField("name", label.c_str());

   return feature;
}

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutGdalFile::insert(const DgDVec2D&)
{
   //Probably isn't needed but keep in for safety
   return *this;
}

////////////////////////////////////////////////////////////////////////////////
void
DgOutGdalFile::createAddressProperty (const DgIDGGBase& dgg, OGRFeature* feature,
           const char* fieldName, const DgLocation& loc, const DgRFBase* outRF)
{
    DgLocation tmpLoc(loc);
    dgg.convert(&tmpLoc);
    std::string str;
    if (!outRF) { // assumed seqnum
        str = std::to_string(dgg.bndRF().seqNum(tmpLoc));
    } else {
        outRF->convert(&tmpLoc);
        str = tmpLoc.asString(' ');
    }

    char* cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());

   // add to the feature
   feature->SetField(fieldName, cstr);

   // cleanup
   delete [] cstr;
}

////////////////////////////////////////////////////////////////////////////////
void
DgOutGdalFile::createAddressesProperty (const DgIDGGBase& dgg, OGRFeature* feature,
           const char* fieldName, const DgLocVector& vec, const DgRFBase* outRF)
{
   // create the string list
   int n = vec.size();
   char** strArr = new char*[n + 1];
   strArr[n] = NULL; // null-terminate the array for OGRStringList
   for (int i = 0; i < n; i++) {
      DgLocation tmpLoc(vec[i]);
      dgg.convert(&tmpLoc);
      std::string str;
      if (!outRF) { // assumed seqnum
         str = std::to_string(dgg.bndRF().seqNum(tmpLoc));
      } else {
         outRF->convert(&tmpLoc);
         str = tmpLoc.asString(' ');
      }

      strArr[i] = new char[str.length() + 1];
      strcpy(strArr[i], str.c_str());
   }

   // add to the feature
   feature->SetField(fieldName, strArr);

   // cleanup
   for (int i = 0; i < n; i++)
      delete strArr[i];
   delete [] strArr;
}

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutGdalFile::insert (const DgIDGGBase& dgg, DgCell& cell,
     bool outputPoint, bool outputRegion, const DgIDGGBase& chdDgg,
     const DgIDGGBase* prtDgg, const DgRFBase* outRF,
     const DgRFBase* chdOutRF, const DgRFBase* prtOutRF,
     const DgLocVector* neighbors, const DgLocVector* children,
     const DgLocation* ndxParent, const DgLocVector* ndxChildren)
{
   if (_mode != Collection)
      ::report("invalid GDAL output file mode encountered.", DgBase::Fatal);

   if (!_oLayer)
      init(outputPoint, outputRegion, neighbors, children, ndxParent, ndxChildren, cell.dataList());

   // first build the geometry

   rf().convert(&cell);

   // create the named feature
   OGRFeature *feature = createFeature(cell.label());

   // set the data fields
   if (cell.dataList()) {
      cell.dataList()->setFields(feature);
   }

   // determine the geometry

   // first check for multi
   if (outputPoint && outputRegion) {

      OGRGeometryCollection* collect = createCollection(cell);
      feature->SetGeometry(collect);

   } else if (outputPoint) {

      OGRPoint* pt = createPoint(cell.node());
      feature->SetGeometry(pt);

   } else if (outputRegion) {

      OGRPolygon* poly = createPolygon(cell.region());
      feature->SetGeometry(poly);
   } else
      ::report( "No geometry specified for GDAL collection feature.", DgBase::Fatal );

   if (children) {
      createAddressesProperty (chdDgg, feature, "children", *children, chdOutRF);
   }

   if (neighbors)
      createAddressesProperty(dgg, feature, "neighbors", *neighbors, outRF);

   if (ndxParent && prtDgg) {
      createAddressProperty (*prtDgg, feature, "ndxParent", *ndxParent, prtOutRF);
   }

   if (ndxChildren) {
      createAddressesProperty (chdDgg, feature, "ndxChildren", *ndxChildren, chdOutRF);
   }

   addFeature(feature);

   return *this;
}

////////////////////////////////////////////////////////////////////////////////
OGRPoint*
DgOutGdalFile::createPoint (const DgLocation& loc) const
{
   DgDVec2D pt = rf().getVecLocation(loc);
   OGRPoint* oPt = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);

   oPt->setX(pt.x());
   oPt->setY(pt.y());

   return oPt;
}

////////////////////////////////////////////////////////////////////////////////
OGRLinearRing*
DgOutGdalFile::createLinearRing (const DgPolygon& poly)
{
   // first create a linearRing
   OGRLinearRing *linearRing;
   linearRing = (OGRLinearRing*) OGRGeometryFactory::createGeometry(wkbLinearRing);

   // fill linearRing with points
   const std::vector<DgAddressBase *>& v = poly.addressVec();
   const DgRFBase& rf = poly.rf();
   for (std::vector<DgAddressBase *>::const_iterator i = v.begin(); v.end() != i; ++i) {
     DgDVec2D pt = rf.getVecAddress(*(*i));
     linearRing->addPoint(pt.x(), pt.y());
   }

   // add the first point to the end
   DgDVec2D pt = rf.getVecAddress(*v[0]);
   linearRing->addPoint(pt.x(), pt.y());

   return linearRing;
}

////////////////////////////////////////////////////////////////////////////////
OGRPolygon*
DgOutGdalFile::createPolygon (const DgPolygon& poly)
{
   // first create a linearRing
   OGRLinearRing *linearRing = createLinearRing(poly);

   // create an OGRPolygon and attach ring to it
   OGRPolygon* polygon = (OGRPolygon*) OGRGeometryFactory::createGeometry(wkbPolygon);
   polygon->addRingDirectly(linearRing);

   // add any holes
   for (unsigned long int i = 0; i < poly.holes().size(); i++) {
      OGRLinearRing* hole = createLinearRing(*poly.holes()[i]);
      polygon->addRingDirectly(hole);
   }

   return polygon;
}

////////////////////////////////////////////////////////////////////////////////
OGRGeometryCollection*
DgOutGdalFile::createCollection (const DgCell& cell) const
{
   OGRGeometryCollection* collection =
      (OGRGeometryCollection*) OGRGeometryFactory::createGeometry(wkbGeometryCollection);

   OGRPoint* oPt = createPoint(cell.node());
   collection->addGeometry(oPt);

   OGRPolygon* poly = createPolygon(cell.region());
   collection->addGeometry(poly);

   return collection;
}

////////////////////////////////////////////////////////////////////////////////
void
DgOutGdalFile::addFeature (OGRFeature *feature) {

   // make sure no errors occure with binding the feature to the layer
   if (_oLayer->CreateFeature( feature ) != OGRERR_NONE)
        ::report( "Failed to create feature in file", DgBase::Fatal );

   // clean up the feature and ready for the next one
   //OGRFeature::DestroyFeature( feature );
}

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutGdalFile::insert (DgLocation& loc, const std::string* label,
                  const DgDataList* dataList)
{
   if (_mode != Point)
      ::report( "invalid GDAL output file mode encountered.", DgBase::Fatal );

   if (!_oLayer)
      init(true, false, false, false, false, false, dataList);

   // create the feature
   OGRFeature *feature = createFeature(*label);

   // set the data fields
   if (dataList) {
      dataList->setFields(feature);
   }

   OGRPoint* oPt = createPoint(loc);
   feature->SetGeometry(oPt);

   addFeature(feature);

   return *this;
}

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutGdalFile::insert (DgLocVector&, const std::string*, const DgLocation*,
                  const DgDataList*)
{
   ::report( "polyline output not supported for GDAL file output", DgBase::Fatal );
   return *this;
}

////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutGdalFile::insert (DgPolygon& poly, const std::string* label,
                  const DgLocation* /* cent */,
                  const DgDataList* dataList)
{
   if (_mode != Polygon)
      ::report( "invalid GDAL output file mode encountered.", DgBase::Fatal );

   if (!_oLayer)
      init(false, true, false, false, false, false, dataList);

   OGRPolygon* polygon = createPolygon(poly);

   OGRFeature *feature = createFeature(*label);

   // set the data fields
   if (dataList) {
      dataList->setFields(feature);
   }

   feature->SetGeometry(polygon);

   addFeature(feature);

   return *this;
}

#endif

