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
// DgOutKMLfile.h: DgOutKMLfile class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTKMLFILE_H
#define DGOUTKMLFILE_H

#include "DgOutLocTextFile.h"
#include "DgUtil.h"

#include <ostream>
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
               const std::string& colorIn = defaultKMLColor, int widthIn = defaultKMLWidth,
               const std::string& nameIn = defaultKMLName, const std::string& descIn = defaultKMLDescription,
               DgReportLevel failLevel = DgBase::Fatal);

      ~DgOutKMLfile();

      void setColor(const std::string& colorIn);
      void setWidth(int widthIn);
      void setName(const std::string& nameIn);
      void setDescription(const std::string& descIn);

      const std::string& color (void) const { return color_; }
      int                width (void) const { return width_; }
      const std::string& name  (void)  const { return name_; }

      const std::string& description (void) const { return description_; }

      virtual void close(void) { std::ofstream::close(); }

      virtual DgOutLocFile& insert (DgLocation& loc, const std::string* label = nullptr,
                                const DgDataList* dataList = nullptr);

      virtual DgOutLocFile& insert (DgLocVector& vec, const std::string* label = nullptr,
                                const DgLocation* cent = nullptr,
                                const DgDataList* dataList = nullptr);

      virtual DgOutLocFile& insert (DgPolygon& poly, const std::string* label = nullptr,
                                const DgLocation* cent = nullptr,
                                const DgDataList* dataList = nullptr);


   protected:

      virtual DgOutLocFile& insert(const DgDVec2D& pt);

      virtual void setFormatStr(void)
      {
          std::ostringstream os;
          os << "%#." << getPrecision() << "LF,"
             << "%#." << getPrecision() << "LF\n";
             //<< "%#." << getPrecision() << "LF,0.0\n"; // with altitude

          formatStr_ = os.str();
      }

      std::string color_;
      int width_;
      std::string name_;
      std::string description_;

   private:

      // KML helpers:
      void preamble();
      void postamble();

      /* take a string that contains a colour in RRGGBB format, and convert this
         to the BBGGRR format used by KML */
      std::string RGBtoBGR(std::string colour)
      {
	     return colour.substr(4,2) + colour.substr(2,2) + colour.substr(0,2);
      }
};

#endif
