#!/bin/sh

if [ $# -lt 1 ] ; then
  echo "Usage: mkdist.sh <version>"
  echo " <version> - version number used in name of generated archive."
  echo "Example: mkdist.sh 1.1.4"
  exit
fi

VERSION=$1

rm -rf dist_wrk
mkdir dist_wrk
cd dist_wrk

REP=:pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot
if cvs -d $REP co shapelib ; then
  echo checkout succeeds.
else
  cvs -d $REP login
  cvs -d $REP co shapelib 
fi 

if [ ! -d shapelib ] ; then
  exit 1
fi

mv shapelib shapelib-$VERSION

find . -name CVS -type d -exec echo rm -rf {} \;

tar czvf ../shapelib-$VERSION.tar.gz shapelib-$VERSION
zip -r ../shapelib-$VERSION.zip shapelib-$VERSION
cd ..
rm -rf dist_wrk


