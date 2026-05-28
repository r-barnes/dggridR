#!/bin/bash

# Path to SebKrantz/DGGRID clone.
# Override via: DGGRID_PATH=/path/to/DGGRID bash update_from_upstream.sh
DGGRID_PATH=${DGGRID_PATH:-/Users/sebastiankrantz/Documents/R/DGGRID}

if [ ! -d "$DGGRID_PATH" ]; then
  echo "Error: DGGRID not found at $DGGRID_PATH" >&2
  echo "Set DGGRID_PATH env var to point to SebKrantz/DGGRID clone." >&2
  exit 1
fi

rm -rf src/*

# Core discrete global grid library
cp -f "$DGGRID_PATH"/src/lib/dglib/include/dglib/* ./src/
cp -f "$DGGRID_PATH"/src/lib/dglib/lib/* ./src/

# Map projection library (libproj4)
cp -f "$DGGRID_PATH"/src/lib/proj4lib/include/* ./src/
cp -f "$DGGRID_PATH"/src/lib/proj4lib/lib/* ./src/

# Shapefile I/O library
cp -f "$DGGRID_PATH"/src/lib/shapelib/include/shapelib/* ./src/
cp -f "$DGGRID_PATH"/src/lib/shapelib/lib/* ./src/

# Flatten <dglib/XXX.h> angle-bracket includes (all DGGRID sources use this form)
find ./src/ -type f -exec perl -pi -e 's{#include <dglib/([^>]+)>}{#include "$1"}g' {} \;

# Flatten "../lib/DgXXX.hpp" relative includes (template implementation files)
find ./src/ -type f -exec perl -pi -e 's{#include "\.\./lib/}{#include "}g' {} \;

# Fix shapefil.h include paths
find ./src/ -type f -exec perl -pi -e \
  's{#include <shapefil\.h>}{#include "shapefil.h"}g;
   s{#include "shapelib/shapefil\.h"}{#include "shapefil.h"}g' {} \;

# Insert DGGRIDR compile-time marker at top of every source file
find ./src/ -type f -exec perl -pi -e \
  'print "#ifndef DGGRIDR\n#define DGGRIDR\n#endif\n" if $. == 1' {} \;

# CRAN rejects .hpp extensions — rename to _hpp.h
find ./src/ -type f -name "*.hpp" | while IFS= read -r f; do
  mv "$f" "${f%.hpp}_hpp.h"
done
# Update all in-file references to the renamed headers
find ./src/ -type f -exec perl -pi -e 's/\.hpp/_hpp\.h/g' {} \;

# Remove M_2PI constant definition (conflicts with macOS system headers)
find ./src/ -type f -exec perl -pi -e 's/constexpr long double M_2PI.*\n?//g' {} \;

# Remove non-R build artifacts
rm -f src/Makefile.noCMake

# C++20: template-id not allowed for constructors — strip template args from ctor names
find ./src/ -type f -name "DgBoundedRF.h" -exec perl -pi -e \
  's/\bDgBoundedRF<A, B, DB> \(/DgBoundedRF (/g' {} \;
find ./src/ -type f -name "DgHierNdxRF.h" -exec perl -pi -e \
  's/\bDgHierNdxCoord<T> \(/DgHierNdxCoord (/g;
   s/\bDgHierNdxRF<C> \(/DgHierNdxRF (/g' {} \;
find ./src/ -type f -name "DgHierNdxSystemRF.h" -exec perl -pi -e \
  's/\bDgHierNdxSystemRF<TINT, TSTR> \(/DgHierNdxSystemRF (/g' {} \;
find ./src/ -type f -name "DgPhysicalRF.h" -exec perl -pi -e \
  's/\bDgPhysicalRF<A, C> \(/DgPhysicalRF (/g' {} \;
find ./src/ -type f -name "DgSpatialDB.h" -exec perl -pi -e \
  's/\bDgSpatialDB<C> \(/DgSpatialDB (/g' {} \;

# Windows: fix DgHierNdxRF.h — sys_.dggs() returns const DgIDGGSBase& (a reference, not a
# pointer); remove the erroneous * dereference and widen the return type to DgIDGGSBase&
find ./src/ -type f -name "DgHierNdxRF.h" -exec perl -pi -e \
  's{const DgIDGGS\s*&\s*dggs\s*\(void\)\s*const\s*\{[^}]*\}}{const DgIDGGSBase\& dggs (void) const { return sys_.dggs(); }}' {} \;

# Windows/modern-GCC: replace obsolete <tr1/> block with standard C++11 headers in DgUtil.h
# (-0 slurps the whole file so the multiline /s match works)
find ./src/ -type f -name "DgUtil.h" -exec perl -0pi -e \
  's{// make sure we have the necessary C99 support.*?^#endif\n}{// C++11 provides all the math/integer headers we need in the standard locations\n#include <cfloat>\n#include <climits>\n#include <cmath>\n#include <cstdint>\n}ms' {} \;
find ./src/ -type f -name "DgUtil.h" -exec perl -0pi -e \
  's{#if DGGS_GCC_VERSION >= 40401.*?#endif\n}{ return std::lrintl(x);\n}s' {} \;

# CRAN compliance: replace std::cerr debug prints with nothing (developer leftovers)
find ./src/ -type f \( -name "*.cpp" -o -name "*.h" \) -exec \
  perl -pi -e 's/.*std::cerr.*\n//' {} \;

# CRAN compliance: replace puts() with dgprintf() in shapelib sources
find ./src/ -type f -name "*.c" -exec \
  perl -pi -e 's/\bputs\(\s*(".*?")\s*\)/dgprintf("%s\n", $1)/g' {} \;

# CRAN compliance: replace sprintf(buf,...) with snprintf(buf, sizeof(buf),...) in shapelib sources
find ./src/ -type f -name "*.c" -exec \
  perl -pi -e 's/\bsprintf\s*\(\s*stmp\s*,/snprintf(stmp, sizeof(stmp),/g' {} \;

# Fix abs() truncation warnings: replace abs() with std::abs() for long long arguments
# This prevents segfaults from integer truncation when abs() is called with long long values
find ./src/ -type f \( -name "*.cpp" -o -name "*.h" \) -exec \
  perl -pi -e 's/\babs\(/std::abs(/g' {} \;

# Silence -Wsign-compare in DGGRID resolution checks.
find ./src/ -type f -name "DgDiscRFSGrids.h" -exec perl -pi -e \
  's/static_cast<unsigned long>\(res\(\)\) >= discRFS\(\)\.nRes\(\) \+ 1/res() >= discRFS().nRes() + 1/g' {} \;

# Silence -Wsign-compare in Z3 parsing loop.
find ./src/ -type f -name "DgZ3StringRF.cpp" -exec perl -pi -e \
  's/for \(int i = 0; i < z3str\.length\(\); i \+= 2\)/for (size_t i = 0; i < z3str.length(); i += 2)/' {} \;

# Silence GCC warning for MSVC-only pragma in shapelib.
find ./src/ -type f -name "safileio.c" -exec perl -0pi -e \
  's/#       include <windows\.h>\n#       pragma comment\(lib, "kernel32\.lib"\)/#       include <windows.h>\n#       if defined(_MSC_VER)\n#       pragma comment(lib, "kernel32.lib")\n#       endif/s' {} \;

# CRAN/dyn.load: avoid static-init of &dgcout (Rcpp::Rcout) in DgConverterBase
find ./src/ -type f -name "DgConverterBase.cpp" -exec perl -pi -e \
  's/std::ostream\* DgConverterBase::traceStream_ = &dgcout;/\/\/ Do not initialize with \&dgcout: taking the address of Rcpp::Rcout at\n\/\/ static-init time segfaults during dyn.load() before R is ready.\nstd::ostream* DgConverterBase::traceStream_ = nullptr;/' {} \;
find ./src/ -type f -name "DgConverterBase.h" -exec perl -0pi -e \
  's/static void setTraceStream \(std::ostream& stream = dgcout\)\s*\{ traceStream_ = &stream; \}\s*static std::ostream& traceStream \(void\) \{ return \*traceStream_; \}/static void setTraceStream (std::ostream\& stream)\n                           { traceStream_ = \&stream; }\n      static std::ostream\& traceStream (void) { return *traceStream_; }/s' {} \;

# CRAN/dyn.load (Debian gcc-16 + libstdc++-16 + _FORTIFY_SOURCE=3):
# Replace the upstream DgBase.h dgcout/dgcerr definitions (which alias
# Rcpp::Rcout/Rcerr and pull <Rcpp.h> into every translation unit) with
# forward declarations of dggridR_cout()/dggridR_cerr(), implemented as
# Rprintf-backed std::ostream& accessors in DgBase.cpp.  This removes the
# file-scope static Rcpp::Rostream instances that <Rcpp.h> would otherwise
# instantiate in ~100 TUs and that crash this specific dyn.load path.
find ./src/ -type f -name "DgBase.h" -exec perl -0pi -e \
  's{#ifdef DGGRIDR\s*\n#include <Rcpp\.h>\s*\n#define dgcout Rcpp::Rcout\s*\n#define dgcerr Rcpp::Rcerr\s*\n#else}{#ifdef DGGRIDR\n// Do NOT include <Rcpp.h> here: it places file-scope static Rcpp::Rostream\n// objects into every TU that includes DgBase.h, whose constructors run\n// during dyn.load() before R is ready and segfault on the CRAN Debian\n// gcc-16 + libstdc++-16 + _FORTIFY_SOURCE=3 pretest box.  Forward-declare\n// the R-aware ostream accessors here; they are defined lazily in DgBase.cpp.\nstd::ostream\& dggridR_cout();\nstd::ostream\& dggridR_cerr();\n#define dgcout dggridR_cout()\n#define dgcerr dggridR_cerr()\n#else}s' {} \;

# Replace the upstream DgBase.cpp body to define dggridR_cout/dggridR_cerr
# via a minimal Rprintf-backed streambuf, with NO <Rcpp.h> include in this
# TU.  The std::ostream itself is wrapped in a function-local static so it
# constructs lazily on first use, not during dyn.load().
find ./src/ -type f -name "DgBase.cpp" -exec perl -0pi -e \
  's{(\n////////////////////////////////////////////////////////////////////////////////\n\nconst std::string DgBase::defaultName)}{\n////////////////////////////////////////////////////////////////////////////////\n\n#ifdef DGGRIDR\n// Lazily-initialised, R-aware ostreams that do NOT depend on <Rcpp.h>.\n// Routing through Rprintf/REprintf via a custom std::streambuf avoids the\n// file-scope static Rcpp::Rostream instances that <Rcpp.h> would otherwise\n// inject into this TU \(and that segfault during dyn.load\(\) on the CRAN\n// Debian gcc-16 + libstdc++-16 + _FORTIFY_SOURCE=3 box\).  It also avoids\n// pulling in std::cout/std::cerr, silencing the _ZSt4cout CRAN NOTE.\n#include <ostream>\n#include <streambuf>\n#include <R_ext/Print.h>\nnamespace {\nclass DgRPrintfBuf : public std::streambuf {\npublic:\n   explicit DgRPrintfBuf \(bool is_err\) : is_err_\(is_err\) {}\nprotected:\n   int overflow \(int c\) override {\n      if \(c != EOF\) {\n         char ch = static_cast<char>\(c\);\n         if \(is_err_\) REprintf\(\"%.1s\", \&ch\); else Rprintf\(\"%.1s\", \&ch\);\n      }\n      return c;\n   }\n   std::streamsize xsputn \(const char\* s, std::streamsize n\) override {\n      if \(is_err_\) REprintf\(\"%.\*s\", static_cast<int>\(n\), s\);\n      else         Rprintf \(\"%.\*s\", static_cast<int>\(n\), s\);\n      return n;\n   }\nprivate:\n   bool is_err_;\n};\n} // anonymous namespace\nstd::ostream\& dggridR_cout \(void\) {\n   static DgRPrintfBuf buf\(false\);\n   static std::ostream stream\(\&buf\);\n   return stream;\n}\nstd::ostream\& dggridR_cerr \(void\) {\n   static DgRPrintfBuf buf\(true\);\n   static std::ostream stream\(\&buf\);\n   return stream;\n}\n#endif\n$1}s' {} \;

# CRAN/dyn.load: Replace `#include <iostream>` with `#include <ostream>` in
# every DGGRID header/source.  The `<iostream>` header drags libstdc++'s
# `std::__ioinit` static initialiser into every including TU, which on the
# CRAN pretest Debian box can interact badly with the dynamic loader.
# Nothing in DGGRID's R-mode build actually uses std::cout/std::cerr
# (all such uses are inside /* */ blocks or `#if DGDEBUG` guards), so the
# narrower <ostream> include is sufficient and strictly reduces load-time
# iostream construction in the .so.
find ./src/ -type f \( -name "*.cpp" -o -name "*.h" \) -exec \
  perl -pi -e 's{^#include\s+<iostream>}{#include <ostream>}' {} \;

# Copy the Rcpp bridge layer (dggridR-specific, not from DGGRID upstream).
# This also drops in dggridR_overrides/ which shadows
# <Rcpp/iostream/Rstreambuf.h> with a stub that omits the file-scope
# static Rcpp::Rostream Rcout/Rcerr declarations -- the only remaining
# place where <Rcpp.h> would otherwise inject iostream-derived static
# constructors into the package's .so.  See src/Makevars and the override
# file's header comment for the rationale.
cp -r copy_to_src/* ./src/

# Exclude standalone test executable source from R shared library build.
rm -f ./src/test.cpp
