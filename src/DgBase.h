#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

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
// DgBase.h: DgBase class definitions
//
// Version 7.0 - Kevin Sahr, 11/15/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBASE_H
#define DGBASE_H

#include <iostream>
#include <string>

using namespace std;

#define DGDEBUG         0
#define DGGRID_VERSION  "7.7" 

// adapted from stackoverflow user Pierre
#define WHERE fprintf(stderr,"[LOG]%s:%s#%d\n",__PRETTY_FUNCTION__,__FILE__,__LINE__);
// macro for intentional switch statement fallthrough
#if defined(__GNUC__) && __GNUC__ >= 7 || defined(__clang__) && __clang_major__ >= 12
#define FALLTHROUGH __attribute__ ((fallthrough));
#else
#if __has_cpp_attribute(fallthrough)
#define FALLTHROUGH [[fallthrough]];
#else
#define FALLTHROUGH
#endif
#endif

// allow for R output from dggridR
#ifdef DGGRIDR
#include <Rcpp.h>
#define dgcout Rcpp::Rcout
#define dgcerr Rcpp::Rcerr
#else
#define dgcout std::cout
#define dgcerr std::cerr
#endif

////////////////////////////////////////////////////////////////////////////////
class DgBase {

   public:

      enum DgReportLevel { Debug1, Debug0, Info, Warning, Fatal, Silent };

   private:

      static const string defaultName;
      static DgReportLevel minReportLevel_;

   public:

      virtual ~DgBase (void);

      static void setMinReportLevel (DgReportLevel newRLevel)
                   { minReportLevel_ = newRLevel; }

      static DgReportLevel minReportLevel (void) { return minReportLevel_; }

      static bool testArgEqual (int argc, int expected, 
                     const string& message = string("invalid argument count"),
                     DgReportLevel level = Fatal);

      static bool testArgEqual (int argc, char* argv[], int expected, 
                     const string& message = string("invalid argument count"));

      static bool testArgMin (int argc, int minExpected, 
                     const string& message = string("invalid argument count"),
                     DgReportLevel level = Fatal);

      static bool testArgMin (int argc, char* argv[], int minExpected, 
                     const string& message = string("invalid argument count"));

      DgBase (const string& instanceName = defaultName);

      DgBase (const string* instanceName = NULL);
      
      void setInstanceName (const string& instanceName)
              { instanceName_ = instanceName; }

      const string& instanceName (void) const { return instanceName_; }

   protected:

      void report (const string& message, DgReportLevel level = Info) const;
      void debug  (const string& message) const;

   private:

      // data members

      string instanceName_;
};

////////////////////////////////////////////////////////////////////////////////
extern "C" void report (const string& message, 
                        DgBase::DgReportLevel level = DgBase::Info);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline void
#if DGDEBUG
DgBase::debug (const string& message) const
#else
DgBase::debug (const string&) const // unused parameter
#endif
//
// Print-out a debugging message if the DGDEBUG flag is set. Otherwise this
// is a null operation.
//
////////////////////////////////////////////////////////////////////////////////
{

#if DGDEBUG
   dgcout << "DEBUG: [" << instanceName_ << "] " << message << endl;
#endif

} // void DgBase::debug

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgBase& b)
            { return stream << b.instanceName(); }

#endif
