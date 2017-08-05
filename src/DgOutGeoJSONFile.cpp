////////////////////////////////////////////////////////////////////////////////
//
// DgOutGeoJSONFile.cpp: DgOutGeoJSONFile class implementation
//
// Version 6.2 - Matt Gregory, added GeoJSON support, 11/4/13
//
////////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iostream>

#include "DgOutGeoJSONFile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgGeoSphRF.h"

DgOutGeoJSONFile::DgOutGeoJSONFile(const DgGeoSphDegRF& rf,
    const std::string& filename, int precision, bool isPointFile,
    DgReportLevel failLevel)
   : DgOutLocTextFile (filename, rf, isPointFile, "geojson", precision,
        failLevel)
{
   if (0 == rf.vecAddress(DgDVec2D()))
   {
      DgOutputStream::report("DgOutGeoJSONFile::DgOutGeoJSONFile(): RF " + rf.name() +
        " must override the vecAddress() method", DgBase::Fatal);
   }

   setFormatStr();
   preamble();
}

DgOutGeoJSONFile::~DgOutGeoJSONFile()
{
   postamble();
   close();
}

void DgOutGeoJSONFile::preamble()
{
   DgOutGeoJSONFile& o(*this);   // make life a bit easier
   o << "{";
   o << "\"type\":\"FeatureCollection\",";
   o << "\"features\":[";
   o.flush();
}

void DgOutGeoJSONFile::postamble()
{
   DgOutGeoJSONFile& o(*this);   

   // Delete the comma from the last feature 
   long pos = o.tellp();
   o.seekp(pos - 1);
   o << "]}";
   o.flush();
}

DgOutLocFile& 
DgOutGeoJSONFile::insert(const DgDVec2D& pt)
{
   DgOutGeoJSONFile& o(*this);

   const int maxBuffSize = 200;
   char buff[maxBuffSize];

   sprintf(buff, formatStr(), pt.x(), pt.y());

   o << buff;

   o.flush();

   return o;
}

DgOutLocFile& 
DgOutGeoJSONFile::insert (DgLocation& loc, const string* label)
{
   DgOutGeoJSONFile& o(*this);

   rf().convert(&loc);

   o << "{\"type\":\"Feature\",";
   o << "\"properties\":{";
   if (label)
      o << "\"name\":" << *label;
   o << "},";
   o << "\"geometry\":{";
   o << "\"type\":\"Point\",";
   o << "\"coordinates\":";

   o.insert(rf().getVecLocation(loc));

   o << "}},";
   o.flush();

   return *this;
}

DgOutLocFile& 
DgOutGeoJSONFile::insert (DgLocVector& vec, const string* label,
   const DgLocation* cent)
{
   DgOutGeoJSONFile& o(*this);   

   rf().convert(vec);

   o << "{\"type\":\"Feature\",";
   o << "\"properties\":{";
   if (label)
      o << "\"name\":" << *label;
   o << "},";
   o << "\"geometry\":{";
   o << "\"type\":\"Polygon\",";
   o << "\"coordinates\":[[";

   vector<DgAddressBase *>& v = vec.addressVec();
   for(vector<DgAddressBase *>::iterator i = v.begin(); v.end() != i; ++i)
   {
         o.insert(rf().getVecAddress(*(*i)));
         o << ",";
   }

   // rewrite first vertex:
   o.insert(rf().getVecAddress(*v[0]));

   o << "]]}},";
   o.flush();

   return *this;
}

DgOutLocFile& 
DgOutGeoJSONFile::insert (DgPolygon& poly, const string* label,
   const DgLocation* cent)
{
   DgOutGeoJSONFile& o(*this);   

   rf().convert(poly);

   o << "{\"type\":\"Feature\",";
   o << "\"properties\":{";
   if (label)
      o << "\"name\":" << *label;
   o << "},";
   o << "\"geometry\":{";
   o << "\"type\":\"Polygon\",";
   o << "\"coordinates\":[[";

   vector<DgAddressBase *>& v = poly.addressVec();
   for(vector<DgAddressBase *>::iterator i = v.begin(); v.end() != i; ++i)
   {
         o.insert(rf().getVecAddress(*(*i)));
         o << ",";
   }

   // rewrite first vertex:
   o.insert(rf().getVecAddress(*v[0]));

   o << "]]}},";
   o.flush();

   return *this;
}
