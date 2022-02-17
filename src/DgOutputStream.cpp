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
// DgOutputStream.C: DgOutputStream class implementation
//
//   This class provides wrappers around some basic output stream functionality
//   to increase ease of use. 
//
////////////////////////////////////////////////////////////////////////////////

#include "DgOutputStream.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutputStream::DgOutputStream (const string& fileName, const string& suffix,
                                DgReportLevel failLevel)
   : DgBase ("DgOutputStream:" + fileName), suffix_ (suffix)
{
   if (!open(fileName, DgBase::Silent))
   {
      report("DgOutputStream::DgOutputStream() unable to open file " +
             fileName, failLevel);
   }

} // DgOutputStream::DgOutputStream

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool 
DgOutputStream::open (string fileName, DgReportLevel failLevel)
//
// Open fileName as an output file. Report with a report level of failLevel
// if the open is unsuccessful.
//
// Returns true if successful and false if unsuccessful.
//
{
   // make sure we are not already open

   if ((rdbuf())->is_open()) close();
   
   // get all the possible name variations

   fileName_ 
      = (suffix() == string("")) ? fileName : 
                                     fileName + string(".") + suffix();

   ofstream::open(fileName_.c_str(), ios::out);
   if (!good()) 
   {
      report("DgOutputStream::open() unable to open file " + fileName_, 
             failLevel);
      return false;
   }
   else
   {
      debug("DgOutputStream::open() opened file " + fileName_);
      return true;
   }
   
} // DgOutputStream::open

////////////////////////////////////////////////////////////////////////////////
