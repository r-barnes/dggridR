////////////////////////////////////////////////////////////////////////////////
//
// DgOutAIGenFile.h: DgOutAIGenFile class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTAIGENFILE_H
#define DGOUTAIGENFILE_H

#include <cstdio>

#include "DgOutLocTextFile.h"

class DgDVec2D;
class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgOutAIGenFile : public DgOutLocTextFile {

   using DgOutLocFile::insert;

   public:

      DgOutAIGenFile (const DgRFBase& rfIn, const string& fileNameIn = "",
                   int precision = 7, bool isPointFile = false,
                   DgReportLevel failLevel = DgBase::Fatal);

      virtual ~DgOutAIGenFile (void) { if (good()) close(); }
      
      virtual void close (void) { *this << "END" << endl; ofstream::close(); }

      virtual DgOutLocFile& insert (DgLocation& loc, const string* label = NULL);
      virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL, 
                                const DgLocation* cent = NULL);
      virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL, 
                                const DgLocation* cent = NULL);

   protected:

      virtual void setFormatStr(void)
      {
          ostringstream os;
          os << "%#." << getPrecision() << "LF"
             << " %#." << getPrecision() << "LF"
             << '\n';

          formatStr_ = os.str();
      }

   private:

      virtual DgOutLocFile& insert (const DgDVec2D& pt);
};

////////////////////////////////////////////////////////////////////////////////

#endif
