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
// DgOutNdxParentFile.h: DgOutNdxParentFile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTNDXPARENT_H
#define DGOUTNDXPARENT_H

#include <fstream>
#include <string>

#include "DgOutLocFile.h"
#include "DgOutputStream.h"
#include "DgUtil.h"

class DgIDGGBase;

////////////////////////////////////////////////////////////////////////////////
class DgOutNdxParentFile : public DgOutputStream {

   public:

      DgOutNdxParentFile (const std::string& fileName,
                        const DgIDGGBase& dgg,
                        const DgIDGGBase& ndxPrtDgg,
                        const DgRFBase* outRF = NULL,
                        const DgRFBase* ndxPrtOutRF = NULL,
                        const std::string& suffix = std::string("ndxPrt"),
                        DgReportLevel failLevel = DgBase::Fatal);

      virtual DgOutNdxParentFile& insert
                      (const DgLocation& center, DgLocation& parent);

   private:

      const DgIDGGBase& dgg_;    // primary res dgg
      const DgIDGGBase& ndxPrtDgg_; // indexing parent res dgg
      const DgRFBase* outRF_;    // primary res output RF (NULL indicates seqNum)
      const DgRFBase* ndxPrtOutRF_; // indexing parent res output RF
};

inline DgOutNdxParentFile& operator<< (DgOutNdxParentFile& file, const char* str)
              { std::ostream& o = file; o << str; return file; }

inline DgOutNdxParentFile& operator<< (DgOutNdxParentFile& file, const std::string& str)
              { std::ostream& o = file; o << str; return file; }

inline DgOutNdxParentFile& operator<< (DgOutNdxParentFile& file, long double val)
              { std::ostream& o = file; o << val; return file; }

inline DgOutNdxParentFile& operator<< (DgOutNdxParentFile& file, float val)
              { std::ostream& o = file; o << val; return file; }

inline DgOutNdxParentFile& operator<< (DgOutNdxParentFile& file, int val)
              { std::ostream& o = file; o << val; return file; }

inline DgOutNdxParentFile& operator<< (DgOutNdxParentFile& file, unsigned long long val)
              { std::ostream& o = file; o << val; return file; }

#endif
