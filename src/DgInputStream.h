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
// DgInputStream.h: DgInputStream class definition
//
//   This class provides wrappers around some basic input stream functionality
//   to increase ease of use.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINPUTSTREAM_H
#define DGINPUTSTREAM_H

#include "DgBase.h"

#include <fstream>
#include <string>

using namespace std;


////////////////////////////////////////////////////////////////////////////////
class DgInputStream : public ifstream, public DgBase {

   public:

      DgInputStream (void) : DgBase ("DgInputStream") {}

      DgInputStream (const string& fileNameIn,
                     const string& suffixIn  = string(""),
                     DgReportLevel failLevel = DgBase::Fatal);

      bool open (string fileName, DgReportLevel failLevel = DgBase::Fatal);

      static void setDefaultDir (const string& defaultDirIn)
                     { defaultDirectory_ = defaultDirIn; }

      void setSuffix (const string& suffixIn) { suffix_ = suffixIn; }

      const string& defaultDir (void) const { return defaultDirectory_; }
      const string& fileName   (void) const { return fileName_; }
      const string& suffix     (void) const { return suffix_; }

      void rewind (void) { seekg(std::streampos(0)); clear(); }

   private:

      static string defaultDirectory_;

      string fileName_;
      string suffix_;
};

////////////////////////////////////////////////////////////////////////////////

#endif
