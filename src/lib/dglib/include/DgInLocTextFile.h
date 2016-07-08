////////////////////////////////////////////////////////////////////////////////
//
// DgInLocTextFile.h: DgInLocTextFile class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINLOCTEXTFILE_H
#define DGINLOCTEXTFILE_H

#include <string>
#include <fstream>

#include "DgInLocFile.h"

using namespace std;

class DgLocList;
class DgLocVector;
class DgPolygon;
class DgLocation;
class DgCell;
class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class DgInLocTextFile : public ifstream, public DgInLocFile {

   public:

      DgInLocTextFile (const DgRFBase& rfIn, 
                       const string* fileNameIn = NULL, 
                       bool isPointFileIn = false,
                       DgReportLevel failLevel = DgBase::Fatal);

      void rewind (void) { seekg(streampos(0)); clear(); }

      virtual bool open (const string* fileName = NULL,
                 DgReportLevel failLevel = DgBase::Fatal);

      virtual void close (void) { ifstream::close(); }

      virtual bool isEOF (void) { return eof(); }

      // pure virtual methods
      virtual DgInLocFile& extract (DgLocList&   list) = 0;
      virtual DgInLocFile& extract (DgLocVector& vec)  = 0;
      virtual DgInLocFile& extract (DgPolygon&   poly) = 0;
      virtual DgInLocFile& extract (DgLocation&  loc)  = 0;
      virtual DgInLocFile& extract (DgCell&      cell) = 0;

};

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgLocList& list)
              { return input.extract(list); }

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgLocVector& vec)
              { return input.extract(vec); }

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgPolygon& poly)
              { return input.extract(poly); }

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgLocation& loc)
              { return input.extract(loc); }

inline DgInLocFile& operator>> (DgInLocTextFile& input, DgCell& cell)
              { return input.extract(cell); }

////////////////////////////////////////////////////////////////////////////////

#endif
