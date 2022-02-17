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
// DgOutNeighborsFile.h: DgOutNeighborsFile class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTNEIGHBORS_H
#define DGOUTNEIGHBORS_H

#include "DgOutLocFile.h"
#include "DgOutputStream.h"
#include "DgUtil.h"

#include <fstream>
#include <string>

using namespace std;

class DgIDGGBase;

////////////////////////////////////////////////////////////////////////////////
class DgOutNeighborsFile : public DgOutputStream {

   public:

      DgOutNeighborsFile (const string& fileName,
                        const string& suffix = string("nbr"),
                        DgReportLevel failLevel = DgBase::Fatal);

      virtual DgOutNeighborsFile& insert (const DgIDGGBase& dgg,
                   const DgLocation& center, DgLocVector& vec);

      virtual bool open (const string& fileName, DgReportLevel failLevel = DgBase::Fatal)
              { return DgOutputStream::open(fileName, failLevel); }

      virtual void close (void) { DgOutputStream::close(); }
};

inline DgOutNeighborsFile& operator<< (DgOutNeighborsFile& file, const char* str)
              { ostream& o = file; o << str; return file; }

inline DgOutNeighborsFile& operator<< (DgOutNeighborsFile& file, const string& str)
              { ostream& o = file; o << str; return file; }

inline DgOutNeighborsFile& operator<< (DgOutNeighborsFile& file, long double val)
              { ostream& o = file; o << val; return file; }

inline DgOutNeighborsFile& operator<< (DgOutNeighborsFile& file, float val)
              { ostream& o = file; o << val; return file; }

inline DgOutNeighborsFile& operator<< (DgOutNeighborsFile& file, int val)
              { ostream& o = file; o << val; return file; }

inline DgOutNeighborsFile& operator<< (DgOutNeighborsFile& file, unsigned long long val)
              { ostream& o = file; o << val; return file; }

#endif
