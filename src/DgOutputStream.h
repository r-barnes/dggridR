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
// DgOutputStream.h: DgOutputStream class definition
//
//   This class provides wrappers around some basic output stream functionality
//   to increase ease of use.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTPUTSTREAM_H
#define DGOUTPUTSTREAM_H

#ifdef std
#  error "Bug: `std` is a macro before including <fstream>"
#endif
#include <fstream>
#include <string>

#include "DgBase.h"

////////////////////////////////////////////////////////////////////////////////
class DgOutputStream : public std::ofstream, public DgBase {

   public:

      DgOutputStream (void) : DgBase ("DgOutputStream") {}

      DgOutputStream (const std::string& fileName,
                      const std::string& suffix    = std::string(""),
                      DgReportLevel failLevel = DgBase::Fatal);

     ~DgOutputStream (void) { close(); }

    bool open (std::string fileName, DgReportLevel failLevel = DgBase::Fatal);

    virtual void close (void) { std::ofstream::close(); }

      void setSuffix (const std::string& suffix) { suffix_ = suffix; }

      const std::string& fileName (void) { return fileName_; }
      const std::string& suffix   (void) { return suffix_; }

   protected:

      std::string fileName_;
      std::string suffix_;
};

////////////////////////////////////////////////////////////////////////////////

#endif
