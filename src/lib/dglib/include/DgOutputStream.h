////////////////////////////////////////////////////////////////////////////////
//
// DgOutputStream.h: DgOutputStream class definition
//
//   This class provides wrappers around some basic output stream functionality
//   to increase ease of use. 
//
// Version 6.2 - Kevin Sahr, 9/10/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTPUTSTREAM_H
#define DGOUTPUTSTREAM_H

#include <string>
#include <fstream>

using namespace std;

#include "DgBase.h"

////////////////////////////////////////////////////////////////////////////////
class DgOutputStream : public ofstream, public DgBase {

   public:

      DgOutputStream (void) : DgBase ("DgOutputStream") {}

      DgOutputStream (const string& fileName,
                      const string& suffix    = string(""), 
                      DgReportLevel failLevel = DgBase::Fatal);

     ~DgOutputStream (void) { close(); }

      bool open (string fileName, DgReportLevel failLevel = DgBase::Fatal);

      virtual void close (void) { ofstream::close(); }

      void setSuffix (const string& suffix) { suffix_ = suffix; }

      const string& fileName (void) { return fileName_; }
      const string& suffix   (void) { return suffix_; }

   protected:

      string fileName_;
      string suffix_;
};

////////////////////////////////////////////////////////////////////////////////

#endif
