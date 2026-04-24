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
// DgInLocStreamFile.cpp: DgInLocStreamFile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgInLocStreamFile.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInLocStreamFile::DgInLocStreamFile (const DgRFBase& rfIn,
              const std::string* fileNameIn, bool isPointFileIn,
              DgReportLevel failLevel)
         : DgInLocFile (rfIn, fileNameIn, isPointFileIn, failLevel),
           degRF_ (nullptr)
{
   if (fileNameIn)
      if (!open(NULL, DgBase::Silent))
         report("DgInLocStreamFile::DgInLocStreamFile() unable to open file " +
                fileName_, failLevel);

} // DgInLocStreamFile::DgInLocStreamFile

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
DgInLocStreamFile::open (const std::string *fileNameIn, DgReportLevel failLevel)
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

   std::ifstream::open(fileName_.c_str(), std::ios::in);
   if (good()) {
      debug("DgInLocStreamFile::open() opened file " + fileName_);
      return true;
   } else {
      report("DgInLocStreamFile::open() unable to open file " + fileName_,
             failLevel);
      return false;
   }

} // DgInLocStreamFile::open

////////////////////////////////////////////////////////////////////////////////
