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
// DgOutGdalFile.cpp: DgOutGdalFile class implementation
//
// Version 7.0 - Elijah Anderson-Justis, 5/20/17
//
////////////////////////////////////////////////////////////////////////////////

// USE_GDAL is set in MakeIncludes
#ifdef USE_GDAL

#include <sstream>
#include <iostream>

#include "DgOutGdalFile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgGeoSphRF.h"

////////////////////////////////////////////////////////////////////////////////
DgOutGdalFile::DgOutGdalFile(const DgGeoSphDegRF& rf,
                    const std::string& filename, const std::string& gdalDriver, 
                    int precision, bool isPointFile, DgReportLevel failLevel)
    : DgOutLocFile (filename, rf, isPointFile, failLevel), _gdalDriver(""),
	_driver(NULL), _dataset(NULL), _oLayer(NULL), _oField(NULL), fileNameOnly_("")
{
   // test for override of vecAddress
   DgAddressBase* dummy = rf.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy)
        ::report("DgOutGdalFile::DgOutGdalFile(): RF " + rf.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   delete dummy;

   _gdalDriver = gdalDriver;
	
   //Initialize the gdal requirements
   init(filename);
}

DgOutGdalFile::~DgOutGdalFile()
{
   delete _oField;
   _oField = NULL;
   //delete _driver;
   //_driver = NULL;
   delete _dataset;
   _dataset = NULL;
   //delete _oLayer;
   //_oLayer = NULL;
   delete _oField;
   _oField = NULL;
   close();
}

void DgOutGdalFile::init(const std::string& filename)
{

/*
   // get just the file name
   string s = DgOutLocFile::fileName();

   char sep = '/';

   #ifdef _WIN32
      sep = '\\';
   #endif

   bool found = false;
   size_t i = s.rfind(sep, s.length());
   if (i != string::npos) 
      s = s.substr(i+1, s.length() - i);

   fileNameOnly_ = s;
*/
   fileNameOnly_ = DgOutLocFile::fileName();

   GDALAllRegister();

    delete _driver;
    _driver = GetGDALDriverManager()->GetDriverByName(_gdalDriver.c_str());
    if( _driver == NULL )
    {
        ::report( _gdalDriver + " driver not available.",  DgBase::Fatal);
        exit(1);
    }

    delete _dataset;
    _dataset = _driver->Create( fileNameOnly_.c_str(), 0, 0, 0, GDT_Unknown, NULL );
    if( _dataset == NULL )
    {
        ::report( "Creation of output file failed.", DgBase::Fatal );
        exit( 1 );
    }
	
    delete _oLayer;
    _oLayer = NULL;
    delete _oField;
    _oField = NULL;
}

DgOutLocFile&
DgOutGdalFile::insert(const DgDVec2D& pt)
{
	//Probably isn't needed but keep in for safety
    return *this;
}

DgOutLocFile&
DgOutGdalFile::insert (DgLocation& loc, const string* label)
{
	//Make sure _oLayer is instantiated
	if( _oLayer == NULL ) {
		
		//Create the point layer that we will be using
		_oLayer = _dataset->CreateLayer( fileNameOnly_.c_str(), NULL, wkbPoint, NULL );
		if( _oLayer == NULL ) {
		    ::report( "Layer creation failed.", DgBase::Fatal );
		    exit( 1 );
		}
		
		//Create the field that will hold the point information
		_oField = new OGRFieldDefn( "Name", OFTString );
		_oField->SetWidth(32);
		if( _oLayer->CreateField( _oField ) != OGRERR_NONE )
		{
		    ::report( "Creating Name field failed.", DgBase::Fatal );
		    exit( 1 );
		}
	}
	
	//This will hold the point information
	OGRFeature *feature;
	
	//Create a feature bound to the layer
	feature = OGRFeature::CreateFeature( _oLayer->GetLayerDefn() );
	feature->SetField("Name", label->c_str());
	
	DgDVec2D pt = rf().getVecLocation(loc);
	OGRPoint oPt;

	oPt.setX(pt.x());
	oPt.setY(pt.y());

	feature->SetGeometry(&oPt);

	//Make sure no errors occur with binding the feature to the layer
	if( _oLayer->CreateFeature( feature ) != OGRERR_NONE )
    {
        ::report( "Failed to create feature in file", DgBase::Fatal );
        exit( 1 );
    }
	
	//Clean up the feature and ready for the next one    
	OGRFeature::DestroyFeature( feature );
	
    return *this;
}

DgOutLocFile&
DgOutGdalFile::insert (DgLocVector& vec, const string* label,
                          const DgLocation* cent)
{

	if( _oLayer == NULL ) {
		//Create the point layer that we will be using
		_oLayer = _dataset->CreateLayer( fileNameOnly_.c_str(), NULL, wkbPolygon, NULL );
		if( _oLayer == NULL )
		{
		    ::report( "Layer creation failed.", DgBase::Fatal );
		    exit( 1 );
		}

		_oField = new OGRFieldDefn( "Name", OFTString );
		_oField->SetWidth(32);
		if( _oLayer->CreateField( _oField ) != OGRERR_NONE )
		{
		    ::report( "Creating Name field failed.", DgBase::Fatal );
		    exit( 1 );
		}
	}
	
	//First create a linearRing
	OGRLinearRing *linearRing;
	linearRing = (OGRLinearRing*)OGRGeometryFactory::createGeometry(wkbLinearRing);

	//Fill linearRing with points
	vector<DgAddressBase *>& v = vec.addressVec();
	for(vector<DgAddressBase *>::iterator i = v.begin(); v.end() != i; ++i) {
		DgDVec2D pt = rf().getVecAddress(*(*i));
       		linearRing->addPoint(pt.x(), pt.y());
	}

	//Create an OGRPolygon and attach ring to it
	OGRPolygon polygon;
	polygon.addRingDirectly(linearRing);

	//This will hold the geometry information
	OGRFeature *feature;
	
	//Create a feature bound to the layer
	feature = OGRFeature::CreateFeature( _oLayer->GetLayerDefn() );
	feature->SetField("Name", label->c_str());

	feature->SetGeometry(&polygon);
	//Make sure no errors occure with binding the feature to the layer
	if( _oLayer->CreateFeature( feature ) != OGRERR_NONE )
    {
        ::report( "Failed to create feature in file", DgBase::Fatal );
        exit( 1 );
    }
	
	//Clean up the feature and ready for the next one    
	OGRFeature::DestroyFeature( feature );

    return *this;
}

DgOutLocFile&
DgOutGdalFile::insert (DgPolygon& poly, const string* label,
                          const DgLocation* cent)
{
	if( _oLayer == NULL ) {
		//Create the point layer that we will be using
		_oLayer = _dataset->CreateLayer( fileNameOnly_.c_str(), NULL, wkbPolygon, NULL );
		if( _oLayer == NULL )
		{
		    ::report( "Layer creation failed.", DgBase::Fatal );
		    exit( 1 );
		}

		_oField = new OGRFieldDefn( "Name", OFTString );
		_oField->SetWidth(32);
		if( _oLayer->CreateField( _oField ) != OGRERR_NONE )
		{
		    ::report( "Creating Name field failed.", DgBase::Fatal );
		    exit( 1 );
		}
	}

	//First create a linearRing
	OGRLinearRing *linearRing;
	linearRing = (OGRLinearRing*) OGRGeometryFactory::createGeometry(wkbLinearRing);

	//Fill linearRing with points
	vector<DgAddressBase *>& v = poly.addressVec();
    for(vector<DgAddressBase *>::iterator i = v.begin(); v.end() != i; ++i) {
		DgDVec2D pt = rf().getVecAddress(*(*i));
        linearRing->addPoint(pt.x(), pt.y());
    }

        // add the first point to the end
	DgDVec2D pt = rf().getVecAddress(*v[0]);
        linearRing->addPoint(pt.x(), pt.y());
	
	//Create an OGRPolygon and attach ring to it
	OGRPolygon polygon;
	polygon.addRingDirectly(linearRing);

	//This will hold the geometry information
	OGRFeature *feature;
	
	//Create a feature bound to the layer
	if(_oLayer != NULL) {
		feature = OGRFeature::CreateFeature( _oLayer->GetLayerDefn() );
		feature->SetField("Name", label->c_str());
	}
	else {
		::report( "Could not get LayerDefn", DgBase::Fatal );
		exit(1);
	}

	feature->SetGeometry(&polygon);
	//Make sure no errors occure with binding the feature to the layer
	if( _oLayer->CreateFeature( feature ) != OGRERR_NONE )
    {
        ::report( "Failed to create feature in file", DgBase::Fatal );
        exit( 1 );
    }
	
	//Clean up the feature and ready for the next one    
	OGRFeature::DestroyFeature( feature );
    return *this;
}

#endif

