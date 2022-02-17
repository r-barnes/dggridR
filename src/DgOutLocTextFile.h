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
// DgOutLocTextFile.h: DgOutLocTextFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTLOCTEXTFILE_H
#define DGOUTLOCTEXTFILE_H

#include "DgOutLocFile.h"
#include "DgOutputStream.h"
#include "DgUtil.h"

#include <fstream>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgOutLocTextFile : public DgOutputStream, public DgOutLocFile {

   public:

      void setPrecision (int prec) { precision_ = prec; setFormatStr(); }
      int getPrecision (void) { return precision_; }

      const char *formatStr() const { return formatStr_.c_str(); }

      // direct the DgOutLocFile abstract methods to the DgOutputStream ones
      virtual bool open (const string& fileName,
                DgReportLevel failLevel = DgBase::Fatal)
              { return DgOutputStream::open(fileName, failLevel); }

      virtual void close (void) { DgOutputStream::close(); }


   protected:

      DgOutLocTextFile (const string& fileName, const DgRFBase& rf,
                        bool isPointFile = false,
                        const string& suffix = string(""),
			int precision = 7,
                        DgReportLevel failLevel = DgBase::Fatal);

      virtual void setFormatStr(void) = 0;

      std::string formatStr_;

   private:

      int             precision_;
};

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, const char* str)
              { ostream& o = file; o << str; return file; }

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, const string& str)
              { ostream& o = file; o << str; return file; }

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, long double val)
              { ostream& o = file; o << val; return file; }

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, float val)
              { ostream& o = file; o << val; return file; }

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, int val)
              { ostream& o = file; o << val; return file; }

#endif
