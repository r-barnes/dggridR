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
// DgOutShapefile.cpp: DgOutShapefile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <sstream>
#include <fstream>
#include <sys/stat.h>

#include "DgOutShapefile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgGeoSphRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutShapefile::DgOutShapefile (const DgGeoSphDegRF& rfIn,
            const string& fileNameIn, int precisionIn, bool isPointFileIn,
            int shapefileIdLen, DgReportLevel failLevelIn)
   : DgOutLocFile (fileNameIn, rfIn, isPointFileIn, failLevelIn),
     geoRF_ (rfIn.geoRF()), dbFile_ (NULL), shpFile_ (NULL), recNum_ (0),
     numDigits_ (precisionIn), numFields_ (0), idLen_ (shapefileIdLen)
{
   // test for override of vecAddress
   DgAddressBase* dummy = rfIn.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy)
      report("DgOutShapefile::DgOutShapefile(): RF " + rfIn.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   delete dummy;

   if (!open(fileNameIn, failLevelIn))
      report("DgOutShapefile::DgOutShapefile() unable to open file " +
             fileNameIn, failLevelIn);

} // DgOutShapefile::DgOutShapefile

////////////////////////////////////////////////////////////////////////////////
bool
DgOutShapefile::open (const string& fileName, DgReportLevel failLevel)
{
   // create the database file

   dbFileName_ = fileName + ".dbf";
   dbFile_ = DBFCreate(dbFileName_.c_str());
   if (dbFile_ == NULL)
   {
      report("DgOutShapefile::open() unable to create database file " +
             dbFileName_, failLevel);
   }

/* as integer
   if (DBFAddField( dbFile_, fieldname, FTInteger, 11, 0 ) == -1) {
      fprintf(stderr, "DBFAddField(dbFile_,%s,FTInteger,11,0) failed.\n", fieldname);
      exit(ERR_DBFADDFIELD);
   }
*/
   char fieldname[] = "global_id";
   if (DBFAddField(dbFile_, fieldname, FTString, idLen_, 0) == -1)
      report("DgOutShapefile::open() unable to create global_id field in file " +
             dbFileName_, failLevel);

   DBFClose(dbFile_);

   dbFile_ = DBFOpen(dbFileName_.c_str(), "rb+");
   if (!dbFile_)
      report("DgOutShapefile::open() unable to open database file " +
             dbFileName_, failLevel);

   numFields_ = 1;

   // create the shape file

   shpFileName_ = fileName + ".shp";
   if (isPointFile())
      shpFile_ = SHPCreate(shpFileName_.c_str(), SHPT_POINT);
   else
      shpFile_ = SHPCreate(shpFileName_.c_str(), SHPT_POLYGON);

   if (!shpFile_)
      report("DgOutShapefile::open() unable to create shapefile " +
             shpFileName_, failLevel);

   recNum_ = 0;

   // create the projection file
   string prjFileName = fileName + ".prj";
   ofstream prjFile;
   prjFile.open(prjFileName.c_str(), ios::out);
   if (!prjFile.good())
      report("DgOutShapefile::open() unable to open file " + prjFileName,
             failLevel);
   else
      debug("DgOutShapefile::open() opened file " + prjFileName);

   int precision = 0;
   string datumName;
   long double earthRadiusM = geoRF_.earthRadiusKM() * 1000.0;
   if (geoRF_.name() == "WGS84_AUTHALIC_SPHERE")
   {
      datumName = "AuthalicSphereWGS84radius";
      precision = 12;
   }
   else if (geoRF_.name() == "WGS84_MEAN_SPHERE")
   {
      datumName = "SphereWGS84meanRadius";
      precision = 7;
   }
   else if (geoRF_.name() == "CUSTOM_SPHERE")
   {
      datumName = "CustomSphere";
      precision = 8;
   }
   else
      report("DgOutShapefile::open() invalid datum " + geoRF_.name(),
             failLevel);

   prjFile << "GEOGCS[\"" << datumName;
   prjFile << "\",DATUM[\"D_unknown\",SPHEROID[\"" << datumName << "\",";
   prjFile << std::fixed << setprecision(precision) << earthRadiusM;
   prjFile << ",0]],PRIMEM[\"Greenwich\",0],";
   prjFile << "UNIT[\"Degree\",0.017453292519943295]]\n";
   prjFile.close();

   return (dbFile_ && shpFile_);

} // DgOutShapefile::open

////////////////////////////////////////////////////////////////////////////////
void
DgOutShapefile::addFields (const set<DgDBFfield>& fields)
{
   if (DBFGetRecordCount(dbFile_) > 0)
      report("cannot add fields to Shapefile after records have been added.",
             DgBase::Fatal);

//cout << "Adding fields: " << endl;

   for (set<DgDBFfield>::iterator it = fields.begin();
           it != fields.end(); it++)
   {
      if (DBFAddField(dbFile_, it->fieldName().c_str(), it->type(),
             it->width(), it->precision()) == -1)
         ::report("DgOutShapefile::addFields() unable to add " + it->fieldName()
                + " in file " + dbFileName_, DgBase::Fatal);

      //int fNum = DBFGetFieldIndex(dbFile_, it->fieldName().c_str());
      //cout << fNum << " " << it->fieldName().c_str() << endl;
   }

   DBFClose(dbFile_);

   dbFile_ = DBFOpen(dbFileName_.c_str(), "rb+");
   if (!dbFile_)
      report("DgOutShapefile::addFields() unable to reopen database file " +
             dbFileName_, DgBase::Fatal);

   numFields_ = DBFGetFieldCount(dbFile_);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutShapefile::insert (const DgDVec2D&)
//
// Put the point pt.
//
////////////////////////////////////////////////////////////////////////////////
{
   report("DgOutShapefile::insert() simple pt insertion not defined for "
          "shapefiles.", failLevel());

   return *this;

} // DgOutLocFile& DgOutShapefile::insert

////////////////////////////////////////////////////////////////////////////////
void
DgOutShapefile::writeDbf (const string& id)
{
   // write the label
   if (id.length() > (unsigned int) idLen_)
      ::report("DgOutShapefile::writeDbf() index string length of " +
               dgg::util::to_string(id.length()) + " exceeds value "
               "of parameter shapefile_id_field_length.", failLevel());

   if (!DBFWriteStringAttribute(dbFile_, recNum_, 0, id.c_str()))
      report("DgOutShapefile::writeDbf() unable to write to " +
             dbFileName_, failLevel());

//cout << "** recnum: " << recNum_ << endl;
   if (numFields_ > 1)
   {
      // first null-out all the fields for this record
      for (int i = 1; i < numFields_; i++)
         if (!DBFWriteNULLAttribute(dbFile_, recNum_, i))
            ::report("DgOutShapefile::writeDbf() unable to write to " +
                dbFileName_, failLevel());

      // now write default values to the fields that are present
      for (set<DgDBFfield>::iterator it = curFields_.begin();
           it != curFields_.end(); it++)
      {
         int fNum = DBFGetFieldIndex(dbFile_, it->fieldName().c_str());
         bool res = false;
//cout << "writing " << fNum << " " << it->fieldName() << " ";
         switch (it->type()) {
            case FTString:
//cout << defStrAttribute() << endl;
               res = DBFWriteStringAttribute(dbFile_, recNum_, fNum, defStrAttribute().c_str());
               break;
            case FTInteger:
//cout << defIntAttribute() << endl;
               res = DBFWriteIntegerAttribute(dbFile_, recNum_, fNum, defIntAttribute());
               break;
            case FTDouble:
//cout << defDblAttribute() << endl;
               res = DBFWriteDoubleAttribute(dbFile_, recNum_, fNum, defDblAttribute());
               break;
            case FTLogical:
               ::report("DgOutShapefile::writeDbf() logical shapefile "
                   "attributes not supported", failLevel());
               break;
            case FTInvalid:
               ::report("DgOutShapefile::writeDbf() invalid field",
                   failLevel());
               break;
            default:
               ::report("DgOutShapefile::writeDbf() unexpected field type",
                   failLevel());
               break;
         }

         if (!res)
            ::report("DgOutShapefile::writeDbf() unable to write to " +
                dbFileName_, failLevel());
      }
   }

} // void DgOutShapefile::writeDbf

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutShapefile::insert (DgLocation& loc, const string* label)
//
// Put the point loc.
//
////////////////////////////////////////////////////////////////////////////////
{
   rf().convert(&loc);

   string id;
   if (label)
     id = *label;
   else
     id = "0";

   DgDVec2D vec = rf().getVecLocation(loc);
   double x = vec.x();
   double y = vec.y();

   // now write to the files
   writeDbf(id.c_str());

   SHPObject *ptShape = SHPCreateObject(SHPT_POINT, recNum_, 0, NULL, NULL, 1,
                             &x, &y, NULL, NULL);
   SHPWriteObject(shpFile_, -1, ptShape);
   SHPDestroyObject(ptShape);

   ++recNum_;

   return *this;

} // DgOutLocFile& DgOutShapefile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutShapefile::insert (DgLocVector&, const string*, const DgLocation*)
//
// Put the polyline vec.
//
////////////////////////////////////////////////////////////////////////////////
{
   report("DgOutShapefile::insert() location vector insertion not defined for "
          "shapefiles.", failLevel());

   return *this;

} // DgOutLocFile& DgOutShapefile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutLocFile&
DgOutShapefile::insert (DgPolygon& poly, const string* label,
                     const DgLocation* /* cent */)
//
// Put the polygon poly.
//
////////////////////////////////////////////////////////////////////////////////
{
   rf().convert(poly);

   string id;
   if (label)
     id = *label;
   else
     id = "0";

   // output the vertices
   const vector<DgAddressBase*>& v = poly.addressVec();
   int numVerts = (int) v.size() + 1;
   double *x = new double[numVerts];
   double *y = new double[numVerts];

   // need to reverse order to get clockwise winding
   int oldNdx = (int) v.size() - 1;
   for (int newNdx = 0; newNdx < numVerts - 1; newNdx++)
   {
      DgDVec2D vec = rf().getVecAddress(*v[oldNdx]);
      x[newNdx] = vec.x();
      y[newNdx] = vec.y();

      oldNdx--;
   }

   // complete the ring by repeating the first vertex
   x[numVerts - 1] = x[0];
   y[numVerts - 1] = y[0];

   // now write to the files
   writeDbf(id.c_str());

   SHPObject *pShape = SHPCreateObject(SHPT_POLYGON, recNum_, 0, NULL, NULL,
      numVerts, x, y, NULL, NULL );
   SHPWriteObject(shpFile_, -1, pShape);
   SHPDestroyObject(pShape);
   delete[] x;
   delete[] y;

   ++recNum_;

   return *this;

} // DgOutLocFile& DgOutShapefile::insert

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
