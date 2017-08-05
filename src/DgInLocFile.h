////////////////////////////////////////////////////////////////////////////////
//
// DgInLocFile.h: DgInLocFile class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINLOCFILE_H
#define DGINLOCFILE_H

#include <string>

#include "DgBase.h"

using namespace std;

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgCell;
class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class DgInLocFile : public DgBase {

   public:

      DgInLocFile (const DgRFBase& rfIn, const string* fileNameIn = NULL,
           bool isPointFileIn = false, DgReportLevel failLevelIn = DgBase::Fatal)
         : DgBase (fileNameIn),
           rf_ (&rfIn), isPointFile_ (isPointFileIn), failLevel_ (failLevelIn)
      { if (fileNameIn) fileName_ = *fileNameIn; } 

      const DgRFBase& rf (void) const { return *rf_; }

      const string& fileName (void) const { return fileName_; }

      bool isPointFile (void) const { return isPointFile_; }

      void setIsPointFile (bool isPointFile = false)
                 { isPointFile_ = isPointFile; }

      DgReportLevel failLevel (void) { return failLevel_; }

      // pure virtual methods

      virtual bool open (const string* fileName = NULL,
                 DgReportLevel failLevel = DgBase::Fatal) = 0;

      virtual void close (void) = 0;

      virtual bool isEOF (void) = 0;

      virtual DgInLocFile& extract (DgLocList&   list) = 0;
      virtual DgInLocFile& extract (DgLocVector& vec)  = 0;
      virtual DgInLocFile& extract (DgPolygon&   poly) = 0;
      virtual DgInLocFile& extract (DgLocation&  loc)  = 0;
      virtual DgInLocFile& extract (DgCell&      cell) = 0;

   protected:

      string fileName_;

      const DgRFBase* rf_;

      bool isPointFile_;

      DgReportLevel failLevel_;
};

inline DgInLocFile& operator>> (DgInLocFile& input, DgLocList& list)
              { return input.extract(list); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgLocVector& vec)
              { return input.extract(vec); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgPolygon& poly)
              { return input.extract(poly); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgLocation& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInLocFile& input, DgCell& cell)
              { return input.extract(cell); }

////////////////////////////////////////////////////////////////////////////////

#endif
