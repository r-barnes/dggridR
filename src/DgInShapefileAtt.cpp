////////////////////////////////////////////////////////////////////////////////
//
// DgInShapefileAtt.cpp: DgInShapefileAtt class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
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
