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

# Copy the Rcpp bridge layer (dggridR-specific, not from DGGRID upstream)
cp copy_to_src/* ./src/
