////////////////////////////////////////////////////////////////////////////////
//
// DgInLocTextFile.cpp: DgInLocTextFile class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgInLocTextFile.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocTextFile::DgInLocTextFile (const DgRFBase& rfIn, 
              const string* fileNameIn, bool isPointFileIn,
              DgReportLevel failLevel)
         : DgInLocFile (rfIn, fileNameIn, isPointFileIn, failLevel)
{
   if (fileNameIn)
      if (!open(NULL, DgBase::Silent))
         report("DgInLocTextFile::DgInLocTextFile() unable to open file " +
                fileName_, failLevel);

} // DgInLocTextFile::DgInLocTextFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool 
DgInLocTextFile::open (const string *fileNameIn, DgReportLevel failLevel)
//
// Open fileName as an input file. Report with a report level of failLevel
// if the open is unsuccessful.
//
// Returns true if successful and false if unsuccessful.
//
{
   // make sure we are not already open

   if ((rdbuf())->is_open()) close();
   
   if (fileNameIn)
      fileName_ = *fileNameIn;

   ifstream::open(fileName_.c_str(), ios::in);
   if (good())
   {
      debug("DgInLocTextFile::open() opened file " + fileName_);
      return true;
   }
   else
   {
      report("DgInLocTextFile::open() unable to open file " + fileName_, 
             failLevel);
      return false;
   }
   
} // DgInLocTextFile::open

////////////////////////////////////////////////////////////////////////////////
