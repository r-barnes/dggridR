#!/bin/bash

rm -rf src/*

cp -f submodules/DGGRID/src/apps/dggrid/{dggrid,util}.h ./src/
cp -f submodules/DGGRID/src/apps/dggrid/util.cpp ./src/

cp -f submodules/DGGRID/src/lib/dglib/include/dglib/* ./src/
cp -f submodules/DGGRID/src/lib/dglib/lib/* ./src/
cp -f submodules/DGGRID/src/lib/proj4lib/include/* ./src/
cp -f submodules/DGGRID/src/lib/proj4lib/lib/* ./src/

cp -f submodules/DGGRID/src/lib/shapelib/include/shapelib/* ./src/
cp -f submodules/DGGRID/src/lib/shapelib/lib/* ./src/

# Fix a header include
# TODO(r-barnes): Remove when this is fixed upstream https://github.com/sahrk/DGGRID/pull/52
find ./src/ -type f -exec sed -i -r 's/#include "dglib\/DgBase.h"/#include <dglib\/DgBase.h>/' {} \;

find ./src/ -type f -exec sed -i '1 i\#ifndef DGGRIDR\n#define DGGRIDR\n#endif' {} \;
find ./src/ -type f -exec sed -i -r 's/#include <dglib\/([^>]+)>/#include "\1"/' {} \;
find ./src/ -type f -exec sed -i -r 's/#include "..\/lib\//#include "/' {} \;
find ./src/ -type f -exec sed -i -r 's/#include <shapefil.h>/#include "shapefil.h"/' {} \;
find ./src/ -type f -exec sed -i -r 's/#include "shapelib\/shapefil.h"/#include "shapefil.h"/' {} \;

rm -rf src/Makefile.noCMake

cp copy_to_src/* ./src/