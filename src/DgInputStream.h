////////////////////////////////////////////////////////////////////////////////
//
// DgInputStream.h: DgInputStream class definition
//
//   This class provides wrappers around some basic input stream functionality
//   to increase ease of use. 
//
// Version 6.2 - Kevin Sahr, 9/10/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINPUTSTREAM_H
#define DGINPUTSTREAM_H

#include <fstream>
#include <string>

using namespace std;

#include "DgBase.h"

////////////////////////////////////////////////////////////////////////////////
class DgInputStream : public ifstream, public DgBase {

   public:

      DgInputStream (void) : DgBase ("DgInputStream") {}

      DgInputStream (const string& fileNameIn,
                     const string& suffixIn  = string(""), 
                     DgReportLevel failLevel = DgBase::Fatal);

      bool open (string fileName, DgReportLevel failLevel = DgBase::Fatal);

      static void setDefaultDir (const string& defaultDirIn) 
                     { defaultDirectory_ = defaultDirIn; }

      void setSuffix (const string& suffixIn) { suffix_ = suffixIn; }

      const string& defaultDir (void) const { return defaultDirectory_; }
      const string& fileName   (void) const { return fileName_; }
      const string& suffix     (void) const { return suffix_; }

      void rewind (void) { seekg(streampos(0)); clear(); }

   private:

      static string defaultDirectory_;

      string fileName_;
      string suffix_;
};

////////////////////////////////////////////////////////////////////////////////

#endif
