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
// DgBase.cpp: DgBase class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgBase.h"

#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////

#ifdef DGGRIDR
// Lazily-initialised R-aware output streams.
//
// We deliberately do NOT include <Rcpp.h> here, even though Rcpp provides
// Rcpp::Rostream/Rcpp::Rcout/Rcpp::Rcerr.  Including <Rcpp.h> pulls in
// Rcpp/iostream/Rstreambuf.h which (when RCPP_USE_GLOBAL_ROSTREAM is not
// defined) places  `static Rostream<true> Rcout;`  /  `static Rostream<false>
// Rcerr;`  at file scope.  Those constructors run during dyn.load() inside
// the dynamic loader, BEFORE the package's R_init_<pkg> entry point and
// before any of R's own setup completes for this .so.  On certain Linux
// configurations (notably the CRAN pretest box running R-devel built with
// gcc-16 + libstdc++-16 and `-D_FORTIFY_SOURCE=3`) this triggers a NULL
// dereference inside std::basic_ios / locale initialisation and aborts
// the package load with `address (nil), cause 'memory not mapped'`.
//
// Routing dgcout/dgcerr through a tiny Rprintf-backed std::streambuf keeps
// the API (`std::ostream&`) identical, eliminates the static Rcpp::Rostream
// instances that <Rcpp.h> would otherwise instantiate in this TU, and
// avoids referencing std::cout/std::cerr (which CRAN flags via the
// `_ZSt4cout` / `_ZSt4cerr` NOTE on Debian).  The std::ostream itself is
// constructed via a function-local static, so it is created on first use
// (lazily, after R is fully initialised), not at dyn.load() time.

#include <ostream>
#include <streambuf>
#include <R_ext/Print.h>

namespace {

class DgRPrintfBuf : public std::streambuf {
public:
   explicit DgRPrintfBuf (bool is_err) : is_err_(is_err) {}
protected:
   int overflow (int c) override {
      if (c != EOF) {
         char ch = static_cast<char>(c);
         if (is_err_) REprintf("%.1s", &ch);
         else         Rprintf ("%.1s", &ch);
      }
      return c;
   }
   std::streamsize xsputn (const char* s, std::streamsize n) override {
      if (is_err_) REprintf("%.*s", static_cast<int>(n), s);
      else         Rprintf ("%.*s", static_cast<int>(n), s);
      return n;
   }
private:
   bool is_err_;
};

} // anonymous namespace

std::ostream& dggridR_cout (void) {
   static DgRPrintfBuf buf(false);
   static std::ostream stream(&buf);
   return stream;
}
std::ostream& dggridR_cerr (void) {
   static DgRPrintfBuf buf(true);
   static std::ostream stream(&buf);
   return stream;
}
#endif

////////////////////////////////////////////////////////////////////////////////

const std::string DgBase::defaultName = "UNDEFNAME";
DgBase::DgReportLevel DgBase::minReportLevel_ = DgBase::Info;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void report (const std::string& message, DgBase::DgReportLevel level)
//
// Global report function accessible to all objects, even if they are not
// derived from DgBase. If level is Info, the message is printed to stdout
// followed by a newline and flush. If level is Warning, "WARNING" is
// printed to stderr followed by the message, newline, and flush. If level
// is Fatal, "FATAL ERROR" is printed to stderr, followed by the message,
// newline, and flush, and then the program is exited.
//
////////////////////////////////////////////////////////////////////////////////
{
   if (level < DgBase::minReportLevel()) return;

   switch (level)
   {
      case DgBase::Debug0:
	 dgcout << "DEBUG0: " << message << std::endl;
	 break;

      case DgBase::Debug1:
	 dgcout << "DEBUG1: " << message << std::endl;
	 break;

      case DgBase::Info:

         dgcout << message << std::endl;
         break;

      case DgBase::Warning:

         dgcout.flush();  // in case stdout and stderr go to the same place
         dgcerr << "WARNING: " << message << std::endl;
         break;

      case DgBase::Fatal:

         dgcout.flush();  // in case stdout and stderr go to the same place
         dgcerr << "FATAL ERROR: " << message << std::endl;
#ifndef DGGRIDR
         exit(1);
#endif

      case DgBase::Silent:

         break;
   }

} // void report

////////////////////////////////////////////////////////////////////////////////
DgBase::~DgBase (void)
{ }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
DgBase::testArgEqual (int argc, int expected, const std::string& message,
                      DgReportLevel level)
{
   if ((argc - 1) != expected)
   {
      ::report(message, level);
      return false;
   }
   else
   {
      return true;
   }

} // bool DgBase::testArgEqual

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
DgBase::testArgEqual (int argc, char* argv[], int expected,
                      const std::string& message)
{
   if ((argc - 1) != expected)
   {
      std::string mess(std::string("usage: ") + std::string(argv[0]));

      if ((message.c_str())[0] != ' ') mess += " ";

      ::report(mess + message, DgBase::Fatal);

      return false;
   }
   else
   {
      return true;
   }

} // bool DgBase::testArgEqual

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
DgBase::testArgMin (int argc, int minExpected, const std::string& message,
                      DgReportLevel level)
{
   if ((argc - 1) < minExpected)
   {
      ::report(message, level);
      return false;
   }
   else
   {
      return true;
   }

} // bool DgBase::testArgMin

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool
DgBase::testArgMin (int argc, char* argv[], int minExpected,
                     const std::string& message)
{
   if ((argc - 1) < minExpected)
   {
      std::string mess(std::string("usage: ") + std::string(argv[0]));

      if ((message.c_str())[0] != ' ') mess += " ";

      ::report(mess + message, DgBase::Fatal);

      return false;
   }
   else
   {
      return true;
   }

} // bool DgBase::testArgMin

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBase::DgBase (const std::string& instanceName)

   : instanceName_ (instanceName)

{
   //debug("DgBase::constructor()");

} // DgBase::DgBase

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBase::DgBase (const std::string* instanceName)
{
   //debug("DgBase::constructor()");

   if (instanceName)
      instanceName_ = *instanceName;
   else
      instanceName_ = defaultName;

} // DgBase::DgBase

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgBase::report (const std::string& message, DgReportLevel level) const
//
// Call the global report function preceded with my instanceName.
//
////////////////////////////////////////////////////////////////////////////////
{
   ::report(std::string("[") + instanceName_ + std::string("] ") + message, level);

} // void DgBase::report

////////////////////////////////////////////////////////////////////////////////
