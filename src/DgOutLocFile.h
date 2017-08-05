////////////////////////////////////////////////////////////////////////////////
//
// DgOutLocFile.h: DgOutLocFile class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTLOCFILE_H
#define DGOUTLOCFILE_H

#include <string>

#include "DgBase.h"

using namespace std;

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgCell;
class DgRFBase;
class DgDVec2D;

////////////////////////////////////////////////////////////////////////////////
class DgOutLocFile : public DgBase {

   public:

      const static string defaultKMLColor;
      const static int    defaultKMLWidth;
      const static string defaultKMLName;
      const static string defaultKMLDescription;

      virtual ~DgOutLocFile (void);

      // factory method
      static DgOutLocFile* makeOutLocFile (const string& type,
               const string& fileName, const DgRFBase& rf, 
               bool isPointFile = false, int precision = 7,
               int shapefileIdLen = 11, const string& kmlColor = defaultKMLColor,
               int kmlWidth = defaultKMLWidth, 
               const string& kmlName = defaultKMLName, const string& kmlDesc = defaultKMLDescription,
               DgReportLevel failLevelIn = DgBase::Fatal);

      const string& fileName (void) { return fileName_; }

      DgReportLevel failLevel (void) { return failLevel_; }

      virtual bool open (const string& fileName, 
                DgReportLevel failLevel = DgBase::Fatal) = 0;

      virtual void close (void) = 0;

      const DgRFBase& rf (void) const { return *rf_; }

      bool isPointFile() const { return isPointFile_; }

      void setIsPointFile(bool isPointFile = false) 
              { isPointFile_ = isPointFile; }

      virtual DgOutLocFile& insert (DgCell& cell);
      virtual DgOutLocFile& insert (DgLocList& list);

      // abstract virtual methods
      virtual DgOutLocFile& insert (DgLocation& loc, 
                                const string* label = NULL) = 0;

      virtual DgOutLocFile& insert (DgLocVector& vec, const string* label = NULL, 
                                const DgLocation* cent = NULL) = 0;

      virtual DgOutLocFile& insert (DgPolygon& poly, const string* label = NULL, 
                                const DgLocation* cent = NULL) = 0;

   protected:

      DgOutLocFile (const string& fileName, 
               const DgRFBase& rf, bool isPointFile = false,
               DgReportLevel failLevelIn = DgBase::Fatal)
         : DgBase ("DgOutLocFile:" + fileName), 
           rf_ (&rf), fileName_ (fileName), isPointFile_ (isPointFile),
           failLevel_ (failLevelIn)
            { /* subclass must perform open */ }

      const DgRFBase* rf_;

      string fileName_;
      bool   isPointFile_;

      DgReportLevel failLevel_;
};

////////////////////////////////////////////////////////////////////////////////

//inline ostream& operator<< (ostream& output, const DgOutLocFile& f)
              //{ return output << f.name(); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgLocList& list)
              { return output.insert(list); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgLocVector& vec)
              { return output.insert(vec); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgPolygon& poly)
              { return output.insert(poly); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgLocation& loc)
              { return output.insert(loc); }

inline DgOutLocFile& operator<< (DgOutLocFile& output, DgCell& cell)
              { return output.insert(cell); }


////////////////////////////////////////////////////////////////////////////////

#endif
