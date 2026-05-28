#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

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
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBASE_H
#define DGBASE_H

#include <iostream>
#include <string>

#define DGDEBUG             0
#define DGGRID_VERSION      "9.0b"
#define DGGRID_RELEASE_DATE "April 2, 2026"

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
// Do NOT include <Rcpp.h> here: it creates static Rcpp::Rostream objects in
// every translation unit that includes DgBase.h.  With GCC 16 / libstdc++
// those constructors can segfault during dyn.load() before R is ready.
// Instead, use lazily-initialised function-local statics (defined in DgBase.cpp).
std::ostream& dggridR_cout();
std::ostream& dggridR_cerr();
#define dgcout dggridR_cout()
#define dgcerr dggridR_cerr()
#else
#define dgcout std::cout
#define dgcerr std::cerr
#endif

////////////////////////////////////////////////////////////////////////////////
class DgBase {

   public:

      enum DgReportLevel { Debug1, Debug0, Info, Warning, Fatal, Silent };

   private:

      static const std::string defaultName;
      static DgReportLevel minReportLevel_;

   public:

      virtual ~DgBase (void);

      static void setMinReportLevel (DgReportLevel newRLevel)
                   { minReportLevel_ = newRLevel; }

      static DgReportLevel minReportLevel (void) { return minReportLevel_; }

      static bool testArgEqual (int argc, int expected,
                                const std::string& message = std::string("invalid argument count"),
                     DgReportLevel level = Fatal);

      static bool testArgEqual (int argc, char* argv[], int expected,
                                const std::string& message = std::string("invalid argument count"));

      static bool testArgMin (int argc, int minExpected,
                     const std::string& message = std::string("invalid argument count"),
                     DgReportLevel level = Fatal);

      static bool testArgMin (int argc, char* argv[], int minExpected,
                              const std::string& message = std::string("invalid argument count"));

      DgBase (const std::string& instanceName = defaultName);

      DgBase (const std::string* instanceName = NULL);

      void setInstanceName (const std::string& instanceName)
              { instanceName_ = instanceName; }

      const std::string& instanceName (void) const { return instanceName_; }

   protected:

      void report (const std::string& message, DgReportLevel level = Info) const;
      void debug  (const std::string& message) const;

   private:

      // data members

     std::string instanceName_;
};

////////////////////////////////////////////////////////////////////////////////
extern "C" void report (const std::string& message,
                        DgBase::DgReportLevel level = DgBase::Info);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline void
#if DGDEBUG
DgBase::debug (const std::string& message) const
#else
DgBase::debug (const std::string&) const // unused parameter
#endif
//
// Print-out a debugging message if the DGDEBUG flag is set. Otherwise this
// is a null operation.
//
////////////////////////////////////////////////////////////////////////////////
{

#if DGDEBUG
   dgcout << "DEBUG: [" << instanceName_ << "] " << message << std::endl;
#endif

} // void DgBase::debug

////////////////////////////////////////////////////////////////////////////////
inline std::ostream& operator<< (std::ostream& stream, const DgBase& b)
            { return stream << b.instanceName(); }

#endif
