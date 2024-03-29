Generating an environment
=========================

Install anaconda or miniconda. Run:
```
conda create --name dggridR
conda activate dggridR
conda install -c conda-forge r-base r-essentials
```
Check that R's libraries are getting installed where you want:
```
.libPaths()
```

Check that dggridR is good to submit
====================================

Clean:
```
find . -iname "*.so" | xargs rm -f
find . -iname "*.o" | xargs rm -f
find . -iname "*.a" | xargs rm -f
rm -f src/apps/appex/appex
rm -f src/apps/dggrid/dggrid
```

Set stack size, maybe:
```
ulimit -s 32768
```

Local testing:
```
devtools::check()
```

Submit to Rhub for testing
```
devtools::check_rhub()
```

Upload to CRAN:
```
devtools::release()
```



R CMD check --as-cran src

echo put my-local-file.txt | ftp win-builder.r-project.org


Releasing
====================

Build documentation:

    devtools::document()

Update NEWS

Bump version in `DESCRIPTION`

Build package with vignettes:

    devtools::build()




TODO
====================
dggridR.rmd citation info
Next time too: Is there some reference about the method you can add in the Description field in the form Authors (year) <doi:.....>?
Update Vignette to use new GEO_to_SEQNUM commands


Quick compile:

    export CCACHE_NOHASHDIR=true

Applications
============
* to calculate routes on the globe
  * http://www.bioinfo.mpg.de/flyways/
  * http://onlinelibrary.wiley.com/doi/10.1111/ele.12528/full


https://stackoverflow.com/questions/44140660/draw-spatialpolygons-with-multiple-subpolygons-and-holes-using-ggplot2
library("sf")
library("rgeos")
sf_poly <- as(xy.sp, "sf")
sf::st_crs(sf_poly) <- 4326
sf_poly$id <- c(1,2)
ggplot(sf_poly) +
  geom_sf(aes(fill = as.factor(id)))



https://web.archive.org/web/20160522033627/http://oceancolor.gsfc.nasa.gov/cms/L3Bins.html


Climate interpolation for dggridR

Featured in: https://cran.r-project.org/web/views/Spatial.html

https://gis.stackexchange.com/questions/8650/how-to-measure-the-accuracy-of-latitude-and-longitude



https://cran.r-project.org/doc/manuals/r-release/R-exts.html#Package-structure

http://r-pkgs.had.co.nz/vignettes.html

library(devtools)
document()

build_vignettes()


dgishex  <- function(grid,i){
  length(grid@lines[[54]]@Lines[[1]]@coords[,1])==7
}

dgispent <- function(grid,i){
  length(grid@lines[[54]]@Lines[[1]]@coords[,1])==6
}

dgpentsingrid <- function(grid){
  which(unlist(lapply(1:length(grid), function(i) dgispent(grid,i))))
}

dghexesingrid <- function(grid){
  which(unlist(lapply(1:length(grid), function(i) dgishex(grid,i))))
}


length(grid@lines[[54]]@Lines[[1]]@coords[,1])


https://gis.stackexchange.com/questions/151446/hex-grid-on-4326-globe-for-use-in-leaflet

https://gis.stackexchange.com/questions/88830/overlay-a-spatial-polygon-with-a-grid-and-check-in-which-grid-element-specific-c




Hex:
Clarence
Jeanneane
Brent
Facebook
StackOverflow
Ron Moen
Lee Frelich
eeb-r list
ergstudents list








Do you remember when spatial analysis was great?

That was probably before you learned how difficult it is to put a grid of equally-sized cells on a sphere. Before you felt the terror of projections and the horror of statistical corrections.

Fortunately, I've made spatial analysis great again with an R package that does divide spheres into equally-sized hexagonal cells (some technical caveats to this are discussed in the package).

I expect this to come out on CRAN soon, but you can find it at the link below in the meantime.

https://github.com/r-barnes/dggridR/






















Coordinate comparison
=====================

library(dggridR)
library(ggplot2)

coasts <- readOGR(dsn="/z/GSHHS_shp/l", layer="GSHHS_l_L1")
coasts <- coasts %>% fortify()

dggs <- dgconstruct(res=3)



xy      <- dgGEO_to_PROJTRI(dggs, coasts$long, coasts$lat)
xy      <- as.data.frame(xy)
xy$tnum <- as.factor(xy$tnum)
xy$group <- coasts$id
ggplot(data=xy, aes(x=tx,y=ty,group=group)) + geom_polygon(fill=NA, colour="black", size=1) + facet_wrap(~tnum)+ ggtitle('dggridR v2.0.1 PROJTRI')




xy      <- dgGEO_to_Q2DD(dggs, coasts$long, coasts$lat)
xy      <- as.data.frame(xy)
xy$quad <- as.factor(xy$quad)
xy$group <- coasts$id
ggplot(data=xy, aes(x=qx,y=qy,group=group)) + geom_polygon(fill=NA, colour="black", size=1) + facet_wrap(~quad)+ ggtitle('dggridR v2.0.1 Q2DD')



xy      <- dgGEO_to_Q2DI(dggs, coasts$long, coasts$lat)
xy      <- as.data.frame(xy)
xy$quad <- as.factor(xy$quad)
xy$group <- coasts$id
ggplot(data=xy, aes(x=i,y=j,group=quad)) + geom_polygon(fill=NA, colour="black", size=1) + facet_wrap(~quad) + ggtitle('dggridR v2.0.1 Q2DI')


xy      <- dgGEO_to_PLANE(dggs, coasts$long, coasts$lat)
xy      <- as.data.frame(xy)
ggplot(data=xy, aes(x=px,y=py)) + geom_point(fill=NA, colour="black", size=0.05) + ggtitle('dggridR v2.0.1 Plane')+ coord_fixed()