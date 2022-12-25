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
// DgInShapefile.cpp: DgInShapefile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include "DgInShapefile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgContCartRF.h"
#include "DgGeoSphRF.h"

////////////////////////////////////////////////////////////////////////////////
DgInShapefile::DgInShapefile (const DgGeoSphRF& geoRFIn, 
                   const string* fileNameIn, DgReportLevel failLevelIn)
   : DgInLocFile (geoRFIn, fileNameIn, false, failLevelIn), 
     geoRF_ (geoRFIn), shpFile_ (NULL), numEntities_ (0), nextRecNum_ (0), 
     curShpObj_ (NULL), curRecNum_ (0), curPart_ (0), nextPart_ (0), 
     isEOF_ (false)
{
   if (fileNameIn)
      if (!open(NULL, DgBase::Silent))
         report("DgInShapefile::DgInShapefile() unable to open file " +
                fileName_, failLevel());

} // DgInShapefile::DgInShapefile

////////////////////////////////////////////////////////////////////////////////
bool 
DgInShapefile::open (const string* fileNameIn, DgReportLevel failLevelIn)
{
   if (fileNameIn)
      fileName_ = *fileNameIn;

   numEntities_ = 0;
   nextRecNum_ = 0;
   curShpObj_ = NULL;
   curRecNum_ = 0;
   nextPart_ = 0;
   isEOF_ = false;

   shpFile_ = SHPOpen(fileName_.c_str(), "rb");
   if (shpFile_ == NULL)
      report("DgInShapefile::open() unable to open shapefile " +
             fileName_, failLevelIn);

   int shpType;
   SHPGetInfo(shpFile_, &numEntities_, &shpType, NULL, NULL);

   if (shpType == SHPT_POINT)
      isPointFile_ = true;
   else if (shpType == SHPT_POLYGON)
      isPointFile_ = false;
   else
   {
      report("DgInShapefile::open() unsupported shapefile entity type; "
             "only SHPT_POINT and SHPT_POLYGON are supported.", failLevelIn);
      close();
      return false;
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////
void 
DgInShapefile::close (void) 
{ 
   if (shpFile_) 
   {
      SHPClose(shpFile_);
      shpFile_ = NULL;
      numEntities_ = 0;
      nextRecNum_ = 0;
      curRecNum_ = 0;
      curShpObj_ = NULL;
      nextPart_ = 0;
      isEOF_ = false;
   }
}

////////////////////////////////////////////////////////////////////////////////
void 
DgInShapefile::rewind (void) 
{ 
   close();
   open();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgInShapefile::getNextEntity (void)
{
   if (curShpObj_ && nextPart_ < curShpObj_->nParts)
   {
      curPart_ = nextPart_;
      ++nextPart_;
      return;
   }

   SHPDestroyObject(curShpObj_);
   curShpObj_ = NULL;
   curPart_ = nextPart_ = 0;

   if (isEOF() || nextRecNum_ >= numEntities_)
   {
      isEOF_ = true;
      return;
   }

   curRecNum_ = nextRecNum_;
   ++nextRecNum_;
   curShpObj_ = SHPReadObject(shpFile_, curRecNum_);
   if (!curShpObj_) getNextEntity();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInShapefile::extract (DgLocVector& vec)
//
// Get the next set of points from me and put it in vec.
//
////////////////////////////////////////////////////////////////////////////////
{
   if (!isPointFile())
   {
      report("DgInShapefile::extract() not implemented for polylines.", 
          failLevel());

      return *this;
   }

   vec.clearAddress();
   rf().convert(vec);

   getNextEntity();
   if (curShpObj_)
   {
      for (int i = 0; i < curShpObj_->nVertices; i++)
      {
         long double x = curShpObj_->padfX[i];
         long double y = curShpObj_->padfY[i];
         DgAddressBase* add = rf().vecAddress(DgDVec2D(x, y));
         vec.addressVec().push_back(add); // vector should delete when done
      }
   }

   return *this;

} // DgInShapefile& DgInShapefile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInShapefile::extract (DgPolygon& poly)
//
// Get the next polygon from me and put it in poly.
//
////////////////////////////////////////////////////////////////////////////////
{
   if (isPointFile())
   {
      report("DgInShapefile::extract() attempting to extract a polygon "
             "from a point file.", failLevel());
      return *this;
   }

   poly.clearAddress();
   rf().convert(poly);

   getNextEntity();
   if (curShpObj_)
   {
      int begin = curShpObj_->panPartStart[curPart_];
      int end = begin;
      if (curPart_ < curShpObj_->nParts - 1)
         end = curShpObj_->panPartStart[curPart_+1];
      else
         end = curShpObj_->nVertices;

      for (int i = begin; i < end; i++)
      {
         long double x = curShpObj_->padfX[i];
         long double y = curShpObj_->padfY[i];
         DgAddressBase* add = rf().vecAddress(DgDVec2D(x, y));
         poly.addressVec().push_back(add); // polygon should delete when done
      }
   }

   return *this;

} // DgInShapefile& DgInShapefile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInShapefile::extract (DgCell&)
//
// Get the next cell from me and put it in cell.
//
////////////////////////////////////////////////////////////////////////////////
{
   report("DgInShapefile::extract() not implemented for cells.", failLevel());

   return *this;

} // DgInShapefile& DgInShapefile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInShapefile::extract (DgLocList& list)
//
// Determine whether the file is a point or polygon/polyline file. If it's
// a point file, read-in the points. 
//
// If not, get the sets which constitute 
// me. If the last point in a set is the same as the first, assume it's a 
// polygon. Otherwise, make it a polyline.
//
////////////////////////////////////////////////////////////////////////////////
{
   list.destroy();
   geoRF_.convert(&list);
   list.setIsOwner(true);

   if (isEOF())
      return *this;

   if (isPointFile())
   {
/*
      getNextEntity();
      if (curShpObj_)
      {
         int begin = curShpObj_->panPartStart[curPart_];
         int end = begin;
         if (curPart_ < curShpObj_->nParts - 1)
            end = curShpObj_->panPartStart[curPart_+1];
         else
            end = curShpObj_->nVertices;

         for (int i = begin; i < end; i++)
         {
            long double x = curShpObj_->padfX[i];
            long double y = curShpObj_->padfY[i];
            DgLocation* pt = geoRF_.makeLocation(DgDVec2D(x, y));
            list.push_back(pt); // polygon should delete when done
         }
      }
*/
   }
   else
   {
      while (true)
      {
         DgPolygon* poly = new DgPolygon();
         extract(*poly);
         if (isEOF()) 
         {
            delete poly;
            break;
         }

         list.push_back(poly);
      }
   }

   return *this;

} // DgInLocFile& DgInShapefile::extract

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocFile&
DgInShapefile::extract (DgLocation& loc)
//
// Get the next DgLocation. 
//
////////////////////////////////////////////////////////////////////////////////
{
   if (!isPointFile())
   {
      report("DgInShapefile::extract() attempting to extract a point "
             "from a polygon file.", failLevel());
      return *this;
   }

   rf().convert(&loc);
   DgLocation* tmpLoc = rf().vecLocation(DgDVec2D::undefDgDVec2D);
   loc = *tmpLoc;

   getNextEntity();
   if (curShpObj_)
   {
      if (curShpObj_->nParts != 0)
      {
         report("DgInShapefile::extract() multi-part entities are not supported.",
             failLevel());
         return *this;
      }

      if (curShpObj_->nVertices != 0)
      {
         report("DgInShapefile::extract() multi-vertex point entities are not "
                "supported.", failLevel());
         return *this;
      }

      long double x = curShpObj_->padfX[0];
      long double y = curShpObj_->padfY[0];
      tmpLoc = rf().vecLocation(DgDVec2D(x, y));
      loc = *tmpLoc;
   }

   delete tmpLoc;

   return *this;

} // DgInLocFile& DgInShapefile::extract

////////////////////////////////////////////////////////////////////////////////
