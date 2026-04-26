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

# Copy the Rcpp bridge layer (dggridR-specific, not from DGGRID upstream)
cp copy_to_src/* ./src/
