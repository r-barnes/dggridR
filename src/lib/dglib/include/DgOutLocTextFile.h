////////////////////////////////////////////////////////////////////////////////
//
// DgOutLocTextFile.h: DgOutLocTextFile class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGOUTLOCTEXTFILE_H
#define DGOUTLOCTEXTFILE_H

#include <string>
#include <fstream>

#include "DgUtil.h"
#include "DgOutputStream.h"
#include "DgOutLocFile.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgOutLocTextFile : public DgOutputStream, public DgOutLocFile {

   public:

      void setPrecision (int prec) { precision_ = prec; setFormatStr(); }
      int getPrecision (void) { return precision_; }

      const char *formatStr() const { return formatStr_.c_str(); }

      // direct the DgOutLocFile abstract methods to the DgOutputStream ones
      virtual bool open (const string& fileName,
                DgReportLevel failLevel = DgBase::Fatal) 
              { return DgOutputStream::open(fileName, failLevel); }

      virtual void close (void) { DgOutputStream::close(); }


   protected:

      DgOutLocTextFile (const string& fileName, const DgRFBase& rf, 
                        bool isPointFile = false,
                        const string& suffix = string(""), 
			int precision = 7, 
                        DgReportLevel failLevel = DgBase::Fatal);

      virtual void setFormatStr(void) = 0;

      std::string formatStr_;

   private:
      
      int             precision_;
};

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, const char* str)
              { ostream& o = file; o << str; return file; }

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, const string& str)
              { ostream& o = file; o << str; return file; }

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, long double val)
              { ostream& o = file; o << val; return file; }

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, float val)
              { ostream& o = file; o << val; return file; }

inline DgOutLocTextFile& operator<< (DgOutLocTextFile& file, int val)
              { ostream& o = file; o << val; return file; }

#endif
