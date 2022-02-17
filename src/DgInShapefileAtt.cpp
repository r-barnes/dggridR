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
// DgInShapefileAtt.cpp: DgInShapefileAtt class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include "DgInShapefileAtt.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgContCartRF.h"

////////////////////////////////////////////////////////////////////////////////
DgInShapefileAtt::DgInShapefileAtt (const DgGeoSphRF& geoRFIn,
            const string* fileNameIn, DgReportLevel failLevelIn)
   : DgInShapefile (geoRFIn, fileNameIn, failLevelIn), 
      dbfFile_ (NULL), numFields_ (0)
{
   if (fileNameIn)
      if (!open(NULL, DgBase::Silent))
         report("DgInShapefileAtt::DgInShapefileAtt() unable to open file " +
                fileName_, failLevel());

} // DgInShapefileAtt::DgInShapefileAtt

////////////////////////////////////////////////////////////////////////////////
bool 
DgInShapefileAtt::open (const string* fileNameIn, DgReportLevel failLevelIn)
{
   if (!DgInShapefile::open(fileNameIn, failLevelIn))
      return false;

   dbfFile_ = DBFOpen(fileName_.c_str(), "rb");
   if (dbfFile_ == NULL)
   {
      report("DgInShapefileAtt::open() unable to open dbf file " +
             fileName_, failLevelIn);
      return false;
   }

   DBFFieldType type;
   char fName[12];
   int w, p;
   numFields_ = DBFGetFieldCount(dbfFile_);
   for (int i = 0; i < numFields_; i++)
   {
      type = DBFGetFieldInfo(dbfFile_, i, fName, &w, &p);
      if (type == FTInvalid)
      {
         report("DgInShapefileAtt::open() invalid field #" + 
                dgg::util::to_string(i) + " in dbf file " + 
                fileName_, failLevelIn);
         return false;
      }

      DgDBFfield f(fName, type, i, w, p);
      fields_.insert(f);
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////
void 
DgInShapefileAtt::close (void) 
{ 
   if (dbfFile_)
   {
      DBFClose(dbfFile_);
      dbfFile_ = NULL;
      numFields_ = 0;
      fields_.clear();
   }

   DgInShapefile::close();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgInShapefileAtt::getNextEntity (void)
{
   DgInShapefile::getNextEntity();
   
   curObjFields_.clear();

   for (set<DgDBFfield>::iterator it = fields_.begin(); it != fields_.end(); it++)
   {
      if (!DBFIsAttributeNULL(dbfFile_, curRecNum_, it->fieldNum()))
         curObjFields_.insert(*it);
   }
}

////////////////////////////////////////////////////////////////////////////////
