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
// DgInLocTextFile.cpp: DgInLocTextFile class implementation
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
