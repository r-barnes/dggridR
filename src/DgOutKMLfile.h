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
// DgOutKMLfile.h: DgOutKMLfile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTKMLFILE_H
#define DGOUTKMLFILE_H

#include "DgOutLocTextFile.h"
#include "DgUtil.h"

#include <iostream>
#include <string>

class DgDVec2D;
class DgPolygon;
class DgGeoSphDegRF;

class DgOutKMLfile : public DgOutLocTextFile
{
   using DgOutLocFile::insert;

   public:

      DgOutKMLfile(const DgGeoSphDegRF& rf, const std::string& filename = "",
               int precision = 7, bool isPointFile = false,
               const string& colorIn = defaultKMLColor, int widthIn = defaultKMLWidth,
               const string& nameIn = defaultKMLName, const string& descIn = defaultKMLDescription,
               DgReportLevel failLevel = DgBase::Fatal);

      ~DgOutKMLfile();

      void setColor(const string& colorIn);
      void setWidth(int widthIn);
      void setName(const string& nameIn);
      void setDescription(const string& descIn);

      const string& color(void) const { return color_; }
      int           width(void) const { return width_; }
      const string& name (void) const { return name_; }
      const string& description (void) const { return description_; }

      virtual void close(void) { ofstream::close(); }

      virtual DgOutLocFile& insert (DgLocation& loc, const string* label = NULL);
      virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL,
                                const DgLocation* cent = NULL);
      virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL,
                                const DgLocation* cent = NULL);

   protected:

      virtual DgOutLocFile& insert(const DgDVec2D& pt);

      virtual void setFormatStr(void)
      {
          ostringstream os;
          os << "%#." << getPrecision() << "LF,"
             << "%#." << getPrecision() << "LF,0.0\n";

          formatStr_ = os.str();
      }

      string color_;
      int width_;
      string name_;
      string description_;

   private:

      // KML helpers:
      void preamble();
      void postamble();

      /* take a string that contains a colour in RRGGBB format, and convert this
         to the BBGGRR format used by KML */
      string RGBtoBGR(string colour)
      {
	     return colour.substr(4,2) + colour.substr(2,2) + colour.substr(0,2);
      }
};

#endif
