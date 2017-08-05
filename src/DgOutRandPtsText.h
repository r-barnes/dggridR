////////////////////////////////////////////////////////////////////////////////
//
// DgOutRandPtsText.h: DgOutRandPtsText class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTRANDPTSTEXT_H
#define DGOUTRANDPTSTEXT_H

#include <cstdio>

#include "DgOutLocTextFile.h"

class DgDVec2D;
class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgOutRandPtsText : public DgOutLocTextFile {

   using DgOutLocFile::insert;

   public:

      DgOutRandPtsText (const DgRFBase& rfIn, const string& fileNameIn = "",
                   int precision = 7, DgReportLevel failLevel = DgBase::Fatal);

      virtual ~DgOutRandPtsText (void) { if (good()) close(); }

      virtual void close (void) { *this << "END" << endl; ofstream::close(); }

      virtual DgOutLocFile& insert (DgLocation& loc, const string* label = NULL);
      virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL, 
                                const DgLocation* cent = NULL);
      virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL, 
                                const DgLocation* cent = NULL);

   private:

      virtual DgOutLocFile& insert (const DgDVec2D& pt);

      virtual void setFormatStr(void)
      {
          ostringstream os;
          os << "%#." << getPrecision() << "LF, "
             << "%#." << getPrecision() << "LF\n";

          formatStr_ = os.str();
      }
};

////////////////////////////////////////////////////////////////////////////////

#endif
