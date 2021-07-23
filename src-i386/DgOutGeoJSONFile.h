////////////////////////////////////////////////////////////////////////////////
//
// DgOutGeoJSONFile.h: DgOutGeoJSONFile class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
// - Matt Gregory, Added GeoJSON support, 10/4/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTGEOJSONFILE_H
#define DGOUTGEOJSONFILE_H

#include <string>
#include <iostream>

#include "DgUtil.h"
#include "DgOutLocTextFile.h"

class DgDVec2D;
class DgPolygon;
class DgGeoSphDegRF;

class DgOutGeoJSONFile : public DgOutLocTextFile
{
   using DgOutLocFile::insert;

   public:

      DgOutGeoJSONFile(const DgGeoSphDegRF& rf,
         const std::string& filename = "", int precision = 7,
         bool isPointFile = false, DgReportLevel failLevel = DgBase::Fatal);

      ~DgOutGeoJSONFile();

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
          os << "[%#." << getPrecision() << "LF,"
             << "%#." << getPrecision() << "LF,0.0]";

          formatStr_ = os.str();
      }

   private:

      // GeoJSON helpers:
      void preamble();
      void postamble();
};

#endif
