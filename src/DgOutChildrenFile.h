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
// DgOutChildrenFile.h: DgOutChildrenFile class definitions
//
// Version 7.0 - Kevin Sahr, 12/14/14
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTCHILDREN_H
#define DGOUTCHILDREN_H

#include <fstream>
#include <string>

#include "DgOutLocFile.h"
#include "DgOutputStream.h"
#include "DgUtil.h"

class DgIDGGBase;

////////////////////////////////////////////////////////////////////////////////
class DgOutChildrenFile : public DgOutputStream {

   public:

      DgOutChildrenFile (const std::string& fileName,
                        const DgIDGGBase& dgg,
                        const DgIDGGBase& chdDgg,
                        const DgRFBase* outRF = NULL,
                        const DgRFBase* chdOutRF = NULL,
                        const std::string& suffix = std::string("chd"),
                        DgReportLevel failLevel = DgBase::Fatal);

      virtual DgOutChildrenFile& insert (const DgLocation& center, DgLocVector& vec);

   private:

      const DgIDGGBase& dgg_;    // primary res dgg
      const DgIDGGBase& chdDgg_; // child res dgg
      const DgRFBase* outRF_;    // primary res output RF (NULL indicates seqNum)
      const DgRFBase* chdOutRF_; // child res output RF

};

inline DgOutChildrenFile& operator<< (DgOutChildrenFile& file, const char* str)
              { std::ostream& o = file; o << str; return file; }

inline DgOutChildrenFile& operator<< (DgOutChildrenFile& file, const std::string& str)
              { std::ostream& o = file; o << str; return file; }

inline DgOutChildrenFile& operator<< (DgOutChildrenFile& file, long double val)
              { std::ostream& o = file; o << val; return file; }

inline DgOutChildrenFile& operator<< (DgOutChildrenFile& file, float val)
              { std::ostream& o = file; o << val; return file; }

inline DgOutChildrenFile& operator<< (DgOutChildrenFile& file, int val)
              { std::ostream& o = file; o << val; return file; }

inline DgOutChildrenFile& operator<< (DgOutChildrenFile& file, unsigned long long val)
              { std::ostream& o = file; o << val; return file; }

#endif
