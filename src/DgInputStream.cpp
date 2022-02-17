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
// DgInputStream.C: DgInputStream class implementation
//
//   This class provides wrappers around some basic input stream functionality
//   to increase ease of use. 
//
////////////////////////////////////////////////////////////////////////////////

#include "DgInputStream.h"

string DgInputStream::defaultDirectory_;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInputStream::DgInputStream (const string& fileNameIn, const string& suffixIn,
                              DgReportLevel failLevel)
   : DgBase ("DgInputStream:" + fileNameIn), suffix_ (suffixIn)
{
   if (!open(fileNameIn, DgBase::Silent))
   {
      report("DgInputStream::DgInputStream() unable to open file " +
             fileNameIn, failLevel);
   }

} // DgInputStream::DgInputStream

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool 
DgInputStream::open (string fileNameIn, DgReportLevel failLevel)
//
// Open fileName as an input file. Report with a report level of failLevel
// if the open is unsuccessful.
//
// Returns true if successful and false if unsuccessful.
//
{
   // make sure we are not already open

   if ((rdbuf())->is_open()) close();
   
   // get all the possible name variations

   string names[4];

   names[0] = fileNameIn;
   names[1] = fileNameIn + "." + suffix_;
   names[2] = defaultDirectory_ + "/" + names[0];
   names[3] = names[2] + "." + suffix_;

   int i;
   for (i = 0; i < 4; i++)
   {
      ifstream::open(names[i].c_str(), ios::in);
      if (good())
      {
         fileName_ = names[i];
         break;
      }
      else
      {
         close();
         clear();
      }
   } 

   if (i == 4) 
   {
      report("DgInputStream::open() unable to open file " + fileNameIn, 
             failLevel);
      return false;
   }
   else
   {
      debug("DgInputStream::open() opened file " + fileName_);
      return true;
   }
   
} // DgInputStream::open

////////////////////////////////////////////////////////////////////////////////
