The code herein is based off of DGGRIDv6.2b by Kevin Sahr, available at

    http://www.discreteglobalgrids.org

However, Richard Barnes (http://rbarnes.org) has made some significant
alterations. These include:

* Replacement of gpclib with clipper, thus making DGGRID into FLOSS software
  available for both commerical and non-commerical use without restriction
* Restructuring and simplifying all of the makefiles to enable compilation in R
* Direct inclusion of the shapelib library
* Addition of the SEQTOPOLY option under the GENERATE_GRID faculty
* Alterations that enable the code to be compiled under GCC6
* Alterations that make the code ISO C conformant
* Alterations to resolve -pedantic and -Wall warnings