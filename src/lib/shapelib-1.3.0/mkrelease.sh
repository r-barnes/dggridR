#!/bin/sh
#
# mkrelease.sh - prepares Shapelib source distribution package
#
if [ $# -lt 1 ] ; then
  echo "Usage: mkrelease.sh <version>"
  echo " <version> - version number used in name of generated archive."
  echo
  echo "Example: mkrelease 1.3.0beta1"
  exit
fi

#
# Processing script input arguments
#
VERSION=$1

#
# Checkout Shapelib sources from the repository
#
echo "* Downloading Shapelib sources from CVS..."
rm -rf dist_wrk  
mkdir dist_wrk
cd dist_wrk

cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot export -D now shapelib 

if [ \! -d shapelib ] ; then
	echo "checkout reported an error ..."
        echo "perhaps you need to do:"
        echo "cvs -d :pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot login"
	cd ..
	rm -rf dist_wrk
	exit
fi

#
# Make distribution packages
#
echo "* Making distribution packages..."

mv shapelib shapelib-${VERSION}

rm -f ../shapelib-${VERSION}.tar.gz ../shapelib-${VERSION}.zip

tar cf ../shapelib-${VERSION}.tar shapelib-${VERSION}
gzip -9 ../shapelib-${VERSION}.tar
zip -r ../shapelib-${VERSION}.zip shapelib-${VERSION}

echo "* Cleaning..."
cd ..
rm -rf dist_wrk

echo "*** The End ***"

