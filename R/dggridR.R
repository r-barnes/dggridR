#' @importFrom sf st_bbox st_as_sf write_sf st_read
#' @importFrom s2 s2_geog_point s2_convex_hull_agg
#' @importFrom collapse qDF fgroup_by fsummarise fmutate funique fmean fsum fnobs
#' @importFrom tools file_path_sans_ext
#' @useDynLib dggridR, .registration = TRUE, .fixes = "C_"
#'

utils::globalVariables(c("seqnum", "C_dg_process_polydata_native"))

# Convert sf::st_bbox to sp::bbox
st_bbox_to_sp <- function(x) {
  if(length(x) != 4L) stop("invalid bbox, needs to be a vector from st_bbox(), with exactly 4 elements")
  matrix(x, ncol = 2, dimnames = list(c("x", "y"), c("min", "max")))
}

# Copied from sp to get rid of it as a dependency
makegrid <- function(bb, n = 10000, nsig = 2, cellsize, offset = rep(0.5, nrow(bb)), pretty = TRUE) {
  # if (is(x, "Spatial"))
  #   bb = bbox(x)
  # else bb = x
  if (missing(cellsize)) {
    pw = 1/nrow(bb)
    cellsize = signif((prod(apply(bb, 1, diff))/n)^pw, nsig)
  }
  if (length(cellsize) == 1) cellsize = rep(cellsize, nrow(bb))
  min.coords = bb[, 1] + offset * cellsize
  if (pretty) min.coords = signif(min.coords, max(ceiling(log10(abs(bb[1, ])/cellsize))))
  sel = min.coords - offset * cellsize > bb[, 1]
  if (any(sel)) min.coords[sel] = min.coords[sel] - cellsize[sel]
  expand.grid.arglist = list()
  for (i in 1:nrow(bb)) {
    name = paste("x", i, sep = "")
    from = min.coords[i]
    by = cellsize[i]
    length.out = round(1 + (bb[i, 2] - from)/by)
    expand.grid.arglist[[name]] = seq(from, by = by, length.out = length.out)
  }
  xy = do.call(expand.grid, expand.grid.arglist)
  attr(xy, "cellsize") = cellsize
  return(xy)
}


#' @name dg_env
#'
#' @title Control global aspects of the dggridR package
#'
#' @description
#'        This environment is used to control global features of the dggridR
#'        package. At the moment the only option is 'dg_debug' which, when set
#'        to TRUE provides extensive outputs useful for tracking down bugs.
#'
#' @keywords internal
#'
dg_env <- new.env()
assign("dg_debug", FALSE, envir=dg_env)

#' @name dg_shpfname_south_africa
#'
#' @title National border of South Africa
#'
#' @description
#'        This variable points to a shapefile containing the national border
#'        of South Africa
#'
#' @return A filename of a shapefile containing the national border of South Africa
#'
#' @export
dg_shpfname_south_africa <- function(){
  system.file("extdata", "ZAF_adm0.shp", package = "dggridR")
}



#' @name dgconstruct
#'
#' @title      Construct a discrete global grid system (dggs) object
#'
#' @description
#'             Construct a discrete global grid system (dggs) object
#'
#' @param projection Type of grid to use. Options are: ISEA and FULLER.
#'                   Default: ISEA3H
#'
#' @param topology   Shape of cell. Options are: HEXAGON, DIAMOND, TRIANGLE.
#'                   Default: HEXAGON
#'
#' @param aperture   How finely subsequent resolution levels divide the grid.
#'                   Options are: 3, 4, 7 (HEXAGON only). Not all options work
#'                   with all projections and topologies. Default: 3
#'
#' @param aperture_type  Aperture sequence type. Options are: \code{"PURE"} (single
#'                       aperture value), \code{"MIXED43"} (alternating aperture-4
#'                       and aperture-3, HEXAGON only). Default: \code{"PURE"}.
#'
#' @param num_aperture_4_res  For \code{aperture_type = "MIXED43"}: number of
#'                            aperture-4 resolutions before switching to aperture-3.
#'                            Must be in [0, res]. Default: 0.
#'
#' @param res  Resolution. Must be in the range [0,30]. Larger values represent
#'             finer resolutions. Appropriate resolutions can be found with
#'             dg_closest_res_to_area(), dg_closest_res_to_spacing(), and
#'             dg_closest_res_to_cls(). Default is 9, which corresponds to a
#'             cell area of ~2600 sq km and a cell spacing of ~50 km.
#'             Only one of res, area, length, or cls should be used.
#'
#' @param precision Round output to this number of decimal places. Must be in
#'                  the range [0,30]. Default: 7.
#'
#' @param area      The desired area of the grid's cells.
#'                  Only one of res, area, length, or cls should be used.
#'
#' @param spacing   The desired spacing between the center of adjacent cells.
#'                  Only one of res, area, length, or cls should be used.
#'
#' @param cls       The desired CLS of the cells.
#'                  Only one of res, area, length, or cls should be used.
#'
#' @param resround  What direction to search in. Must be nearest, up, or down.
#'
#' @param show_info Print the area, spacing, and CLS of the chosen resolution.
#'
#' @param metric    Whether input and output should be in metric (TRUE) or
#'                  imperial (FALSE)
#'
#' @param orient    Grid orientation. Options are: \code{"SPECIFIED"} (use explicit
#'                  pole and azimuth values) or \code{"RANDOM"} (random uniform
#'                  orientation; use \code{set.seed()} before for reproducibility).
#'                  Default: \code{"SPECIFIED"}.
#'
#' @param azimuth_deg   Rotation in degrees of grid about its pole, value in [0,360].
#'                      Default=0.
#'
#' @param pole_lat_deg  Latitude in degrees of the pole, value in [-90,90].
#'                      Default=58.28252559.
#'
#' @param pole_lon_deg  Longitude in degrees of the pole, value in [-180,180].
#'                      Default=11.25.
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' dggs <- dgconstruct(area=5,metric=FALSE)
#'
#' # Aperture-7 hexagonal grid (ISEA7H)
#' dggs7 <- dgconstruct(aperture=7, res=3)
#'
#' # Mixed aperture grid (ISEA43H)
#' dggsM <- dgconstruct(aperture_type='MIXED43', num_aperture_4_res=2, res=5)
#'
#' # Random orientation (reproducible with set.seed)
#' set.seed(42)
#' dggs_r <- dgconstruct(res=4, orient='RANDOM')
#' @export
dgconstruct <- function(
  projection        = 'ISEA',
  aperture          = 3,
  topology          = 'HEXAGON',
  aperture_type     = 'PURE',
  num_aperture_4_res = 0L,
  res               = NA,
  precision         = 7,
  area              = NA,
  spacing           = NA,
  cls               = NA,
  resround          = 'nearest',
  metric            = TRUE,
  show_info         = TRUE,
  orient            = 'SPECIFIED',
  azimuth_deg       = 0,
  pole_lat_deg      = 58.28252559,
  pole_lon_deg      = 11.25
){
  if(sum(!is.na(c(res,area,spacing,cls)))!=1)
    stop('dgconstruct(): Only one of res, area, length, or cls can have a value!')

  if(!(orient %in% c('SPECIFIED', 'RANDOM')))
    stop("dgconstruct(): orient must be 'SPECIFIED' or 'RANDOM'", call.=FALSE)

  if(orient == 'RANDOM') {
    pole_lat_deg <- asin(stats::runif(1, -1, 1)) * 180 / pi
    pole_lon_deg <- stats::runif(1, -180, 180)
    azimuth_deg  <- stats::runif(1, 0, 360)
  }

  #Use a dummy resolution, we'll fix it in a moment
  dggs <- list(
    pole_lon_deg       = pole_lon_deg,
    pole_lat_deg       = pole_lat_deg,
    azimuth_deg        = azimuth_deg,
    aperture           = aperture,
    aperture_type      = aperture_type,
    num_aperture_4_res = as.integer(num_aperture_4_res),
    isMixed43          = (aperture_type == 'MIXED43'),
    numAp4             = as.integer(num_aperture_4_res),
    res                = 1L,
    topology           = topology,
    projection         = projection,
    precision          = precision
  )

  if(!is.na(res))
    dggs[['res']] = res
  else if(!is.na(area))
    dggs[['res']] = dg_closest_res_to_area   (dggs,area=area,      round=resround,metric=metric,show_info=TRUE)
  else if(!is.na(spacing))
    dggs[['res']] = dg_closest_res_to_spacing(dggs,spacing=spacing,round=resround,metric=metric,show_info=TRUE)
  else if(!is.na(cls))
    dggs[['res']] = dg_closest_res_to_cls    (dggs,cls=cls,        round=resround,metric=metric,show_info=TRUE)
  else
    stop('dgconstruct(): Logic itself has failed us.')

  dgverify(dggs)

  dggs
}



#' @name dgsetres
#'
#' @title Set the resolution of a dggs object
#'
#' @description
#'             Set the resolution of a dggs object
#'
#' @param dggs A dggs object from dgconstruct().
#'
#' @param res  Resolution. Must be in the range [0,30]. Larger values represent
#'             finer resolutions. Appropriate resolutions can be found with
#'             dg_closest_res_to_area(), dg_closest_res_to_spacing(), and
#'             dg_closest_res_to_cls(). Default is 9, which corresponds to a
#'             cell area of ~2600 sq km and a cell spacing of ~50 km.
#'             Default: 9.
#'
#' @return     Returns a dggs object which can be passed to other dggridR
#'             functions
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' dggs <- dgsetres(dggs,10)
#' @export
dgsetres <- function(dggs,res){
  dggs[['res']] = res
  dgverify(dggs)
  dggs
}



#' @name dgverify
#'
#' @title Verify that a dggs object has appropriate values
#'
#' @description
#'             Verify that a dggs object has appropriate values
#'
#' @param dggs The dggs object to be verified
#'
#' @return     The function has no return value. A stop signal is raised if the
#'             object is misspecified
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' dgverify(dggs)
#' @export
dgverify <- function(dggs){
  if(!(dggs[['projection']] %in% c('ISEA','FULLER')))
    stop('Unrecognised dggs projection', call.=FALSE)
  if(!(dggs[['topology']] %in% c('HEXAGON','DIAMOND','TRIANGLE')))
    stop('Unrecognised dggs topology', call.=FALSE)

  aperture_type <- dggs[['aperture_type']]
  if(is.null(aperture_type)) aperture_type <- 'PURE'

  if(!(aperture_type %in% c('PURE', 'MIXED43')))
    stop("Unrecognised dggs aperture_type: must be 'PURE' or 'MIXED43'", call.=FALSE)

  if(aperture_type == 'MIXED43') {
    if(dggs[['topology']] != 'HEXAGON')
      stop("aperture_type 'MIXED43' requires topology 'HEXAGON'", call.=FALSE)
    n4 <- dggs[['num_aperture_4_res']]
    if(is.null(n4)) n4 <- 0L
    if(!is.numeric(n4) || n4 < 0 || n4 > dggs[['res']])
      stop('num_aperture_4_res must be an integer in [0, res]', call.=FALSE)
  } else {
    if(!(dggs[['aperture']] %in% c(3,4,7)))
      stop('Unrecognised dggs aperture: must be 3, 4, or 7', call.=FALSE)
    if(dggs[['aperture']] == 7 && dggs[['topology']] != 'HEXAGON')
      stop("aperture 7 requires topology 'HEXAGON'", call.=FALSE)
  }

  if(dggs[['res']]<0)
    stop('dggs resolution must be >=0', call.=FALSE)
  if(dggs[['res']]>30)
    stop('dggs resolution must be <=30', call.=FALSE)
  if(dggs[['azimuth_deg']]<0 || dggs[['azimuth_deg']]>360)
    stop('dggs azimuth_deg must be in the range [0,360]')
  if(dggs[['pole_lat_deg']]<(-90) || dggs[['pole_lat_deg']]>90)
    stop('dggs pole_lat_deg must be in the range [-90,90]')
  if(dggs[['pole_lon_deg']]<(-180) || dggs[['pole_lon_deg']]>180)
    stop('dggs pole_lon_deg must be in the range [-180,180]')
  if(!all.equal(dggs[['res']], as.integer(dggs[['res']])))
    stop('dggs resolution must be an integer', call.=FALSE)
}



#' @name dginfo
#'
#' @title Print info about a dggs object to the screen
#'
#' @description
#'          dggs objects have many settings. This returns all of them, along
#'          with info about the grid being specified.
#'
#' @param dggs A dggs object from dgconstruct()
#'
#' @return No return. All info is printed to the screen.
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' dginfo(dggs)
#' @export
dginfo <- function(dggs){
  dgverify(dggs)

  res <- dggetres(dggs)

  oops <- options()
  on.exit(options(oops))

  options(scipen=999)

  print(res, sep="\r\n")

  NULL
}



#' @name dggetres
#'
#' @title      Get table of grid resolution information
#'
#' @description
#'             Gets a grid's resolution and cell property info as a data frame.
#'
#' @param dggs A dggs object from dgconstruct()
#'
#' @return     A data frame containing the resolution levels, number of cells,
#'             area of those cells, intercell spacing, and characteristic length
#'             scale of the cells. All values are in kilometres.
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' dggetres(dggs)
#' @export
dggetres <- function(dggs){
  dgverify(dggs)

  ress <- 0:30

  data.frame(
    res        = ress,
    cells      = sapply(ress, function(r) GridStat_nCells(dggs[['projection']], dggs[['topology']], dggs[['aperture']], r)),
    area_km    = sapply(ress, function(r) GridStat_cellAreaKM(dggs[['projection']], dggs[['topology']], dggs[['aperture']], r)),
    spacing_km = sapply(ress, function(r) GridStat_cellDistKM(dggs[['projection']], dggs[['topology']], dggs[['aperture']], r)),
    cls_km     = sapply(ress, function(r) GridStat_cls(dggs[['projection']], dggs[['topology']], dggs[['aperture']], r))
  )
}



#' @name dgmaxcell
#'
#' @title      Get largest cell id for a dggs
#'
#' @description
#'             Cells are labeled 1-N. This function returns N. This is useful if
#'             you want to choose cells from the dggs randomly.
#'
#' @param dggs A dggs object from dgconstruct()
#'
#' @param res  If NA, use the resolution specified by the dggs. Otherwise,
#'             override the resolution.
#'
#' @return     The maximum cell id.
#'
#' @examples
#' #Choose a set of cells randomly distributed over the Earth
#' library(dggridR)
#' dggs    <- dgconstruct(spacing=1000, metric=FALSE, resround='down')
#' N       <- 100                                 #Number of cells
#' maxcell <- dgmaxcell(dggs)                     #Get maximum cell id
#' cells   <- sample(1:maxcell, N, replace=FALSE) #Choose random cells
#' grid    <- dgcellstogrid(dggs,cells) #Get grid
#' @export
dgmaxcell <- function(dggs,res=NA){
  dgverify(dggs)

  reses <- dggetres(dggs)

  restoget <- dggs[['res']]
  if(!is.na(res))
    restoget <- res

  #Add one because R uses 1-based indexing and there is a row 0
  reses$cells[restoget+1]
}



#' @name dg_closest_res
#'
#' @title Determine an appropriate grid resolution based on input data.
#'
#' @description
#'          This is a generic function that is used to determine an appropriate
#'          resolution given an area, cell spacing, or correlated length scale.
#'          It does so by extracting the appropriate length/area column and
#'          searching it for a value close to the input.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param col       Column in which to search for a close value. Should be:
#'                  area_km, spacing_km, or cls_km.
#'
#' @param val       The value to search for
#'
#' @param round     What direction to search in. Must be nearest, up, or down.
#'
#' @param show_info Print the area, spacing, and CLS of the chosen resolution.
#'
#' @param metric    Whether input and output should be in metric (TRUE) or
#'                  imperial (FALSE)
#'
#' @return A number representing the grid resolution
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' res  <- dg_closest_res(dggs,'area_km',1)
#' dggs <- dgsetres(dggs,res)
#' @export
dg_closest_res <- function(dggs,col,val,round='nearest',show_info=TRUE,metric=TRUE){
  KM_TO_M <- 0.621371

  dgverify(dggs)

  ret <- dggetres(dggs)

  if(!(col %in% c('area_km', 'spacing_km', 'cls_km')))
    stop("'col' must be one of area_km, spacing_km, or cls_km")

  searchvec = ret[col]

  if(!metric)
    searchvec <- searchvec*KM_TO_M #Convert kilometers to miles

  searchvec <- unlist(searchvec, use.names=FALSE)

  if(round=='up')
    idx <- max(which(searchvec>val))
  else if(round=='down')
    idx <- min(which(searchvec<val))
  else if(round=='nearest')
    idx <- which.min(abs(searchvec-val))
  else
    stop('Unrecognised rounding direction. Must be up, down, or nearest.', call.=FALSE)

  if(show_info && metric)
    cat(paste("Resolution: ",ret$res[idx],", Area (km^2): ",ret$area_km[idx],", Spacing (km): ", ret$spacing_km[idx],", CLS (km): ", ret$cls_km[idx], "\n", sep=""))
  else if(show_info && !metric)
    cat(paste("Resolution: ",ret$res[idx],", Area (mi^2): ",ret$area_km[idx]*KM_TO_M,", Spacing (mi): ", ret$spacing_km[idx]*KM_TO_M,", CLS (mi): ", ret$cls_km[idx]*KM_TO_M, "\n", sep=""))

  ret$res[idx]
}



#' @name dg_closest_res_to_area
#'
#' @title           Determine resolution based on desired area
#'
#' @description
#'                  Determine an appropriate grid resolution based on a desired
#'                  cell area.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param area      The desired area of the grid's cells
#'
#' @param round     What direction to search in. Must be nearest, up, or down.
#'
#' @param show_info Print the area, spacing, and CLS of the chosen resolution.
#'
#' @param metric    Whether input and output should be in metric (TRUE) or
#'                  imperial (FALSE)
#'
#' @return A number representing the grid resolution
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' res  <- dg_closest_res_to_area(dggs,1)
#' dggs <- dgsetres(dggs,res)
#' @export
dg_closest_res_to_area <- function(dggs,area,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'area_km',area,round,show_info,metric)
}



#' @name dg_closest_res_to_spacing
#'
#' @title           Determine grid resolution from desired spacing.
#'
#' @description     Determine an appropriate grid resolution based on a desired
#'                  spacing between the center of adjacent cells.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param spacing   The desired spacing between the center of adjacent cells
#'
#' @param round     What direction to search in. Must be nearest, up, or down.
#'
#' @param show_info Print the area, spacing, and CLS of the chosen resolution.
#'
#' @param metric    Whether input and output should be in metric (TRUE) or
#'                  imperial (FALSE)
#'
#' @return A number representing the grid resolution
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' res  <- dg_closest_res_to_spacing(dggs,1)
#' dggs <- dgsetres(dggs,res)
#' @export
dg_closest_res_to_spacing <- function(dggs,spacing,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'spacing_km',spacing,round,show_info,metric)
}



#' @name dg_closest_res_to_cls
#'
#' @title Determine an appropriate grid resolution based on a desired
#'        characteristic length scale of the cells.
#'
#' @description
#'          The characteristic length scale (CLS) is the diameter of a spherical
#'          cap of the same area as a cell of the specified resolution.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param cls       The desired CLS of the cells.
#'
#' @param round     What direction to search in. Must be nearest, up, or down.
#'
#' @param show_info Print the area, spacing, and CLS of the chosen resolution.
#'
#' @param metric    Whether input and output should be in metric (TRUE) or
#'                  imperial (FALSE)
#'
#' @return A number representing the grid resolution
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' res  <- dg_closest_res_to_cls(dggs,1)
#' dggs <- dgsetres(dggs,res)
#' @export
dg_closest_res_to_cls <- function(dggs,cls,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'cls_km',cls,round,show_info,metric)
}



#' @name dg_process_polydata
#'
#' @title   Load a KML file
#'
#' @description     Convert data from internal dggrid functions into an sf object.
#'
#' @param polydata  Polygons generated by dggrid. These will be converted.
#'
#' @return Returns an sf object.
#'
#' @keywords internal
#'
dg_process_polydata <- function(polydata) {
  native <- .Call(C_dg_process_polydata_native, polydata, collapse::fndistinct(polydata$seqnum))
  geometry <- sf::st_as_sfc(native$wkb, crs = 4326)
  sf::st_sf(seqnum = native$seqnum, geometry = geometry)

  # Previous implementation:
  # x <- y <- seqnum <- geometry <- NULL # For R CMD Check: no visible binding for global variables
  # qDF(polydata) |>
  #   fmutate(geometry = s2_geog_point(x, y)) |>
  #   fgroup_by(seqnum, sort = TRUE) |>
  #   fsummarise(geometry = s2_convex_hull_agg(geometry)) |>
  #   st_as_sf(crs = 4326)
}



#' @name dgrectgrid
#'
#' @title   Return the coordinates constituting the boundary of cells within a
#'          specified region
#'
#' @description     Note: This may generate odd results for very large
#'                  rectangles, because putting rectangles on spheres is
#'                  weird... as you should know, if you're using this package.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param minlat    Minimum latitude of region of interest
#'
#' @param minlon    Minimum longitude of region of interest
#'
#' @param maxlat    Maximum latitude of region of interest
#'
#' @param maxlon    Maximum longitude of region of interest
#'
#' @param cellsize  Distance, in degrees, between the sample points used to
#'                  generate the grid. Small values yield long generation times
#'                  while large values may omit cells.
#'
#' @param \dots Further arguments passed to \code{\link{dgcellstogrid}}.
#'
#' @return Returns an sf object.
#'         If \code{!is.na(savegrid)}, returns a filename.
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(spacing=1000,metric=FALSE,resround='down')
#'
#' #Get grid cells for the conterminous United States
#' grid <- dgrectgrid(dggs,
#'                minlat=24.7433195, minlon=-124.7844079,
#'                maxlat=49.3457868, maxlon=-66.9513812)
#' @export
dgrectgrid <- function(dggs,minlat=-1,minlon=-1,maxlat=-1,maxlon=-1,cellsize=0.1, ...){ #TODO: Densify?
  dgverify(dggs)

  bbox_coords <- matrix(c(minlon, minlat, maxlon, maxlat), ncol = 2, dimnames = list(c("x", "y"), c("min", "max")))

  #Generate a dense grid of points
  samp_points <- makegrid(bbox_coords, cellsize = cellsize)

  #Convert the points to SEQNUM ids for dggs
  samp_points <- dgGEO_to_SEQNUM(dggs, samp_points$x1, samp_points$x2)$seqnum

  dgcellstogrid(dggs, samp_points, ...)
}



#' @name dgearthgrid
#'
#' @title   Return the coordinates constituting the boundary of cells for the
#'          entire Earth
#'
#' @description     Note: If you have a high-resolution grid this may take a
#'                  very long time to execute.
#'
#'
#' @param dggs      A dggs object from dgconstruct().
#' @param densify   Integer. Number of extra vertices to add along each cell
#'                  edge (0 = none). Larger values produce smoother boundaries
#'                  for coarse-resolution cells. Default: 0.
#' @inheritParams dgcellstogrid
#'
#'
#' @return Returns an sf object.
#'         If \code{!is.na(savegrid)}, returns a filename.
#'
#' @examples
#' \donttest{
#' library(dggridR)
#' dggs         <- dgconstruct(res=20)
#' res          <- dg_closest_res_to_spacing(dggs,spacing=1000,round='down',metric=FALSE)
#' dggs         <- dgsetres(dggs,res)
#' gridfilename <- dgearthgrid(dggs,savegrid=tempfile(fileext=".shp")) #Save directly to a file
#' }
#' @export
dgearthgrid <- function(dggs, savegrid = NA, return_sf = TRUE, densify = 0L) {
  dgverify(dggs)
  isMixed43 <- isTRUE(dggs[["isMixed43"]])
  numAp4    <- if(is.null(dggs[["numAp4"]])) 0L else dggs[["numAp4"]]

  grid <- GlobalGrid(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], isMixed43, numAp4, as.integer(densify))
  if(is.na(savegrid)) {
    if(!return_sf) return(qDF(grid))
    dg_process_polydata(grid)
  } else {
    grid <- dg_process_polydata(grid)
    dgsavegrid(grid,savegrid)
  }
}



#' @name dgcellstogrid
#'
#' @title           Return boundary coordinates for specified cells
#'
#' @description     Returns the coordinates constituting the boundary of a
#'                  specified set of cells. Duplicates are eliminated to reduce
#'                  processing and storage requirements.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param cells     The cells to get the boundaries of
#'
#' @param savegrid  If savegrid is set to a file path, then a shapefile
#'                  containing the grid is written to that path and the filename
#'                  is returned. No other manipulations are done.
#'                  Default: NA (do not save grid, return it)
#'
#' @param return_sf logical. If \code{FALSE}, a long-format data frame giving the coordinates of the vertices of each cell is returned. This is is considerably faster and more memory efficient than creating an sf data frame.
#'
#' @param densify   Integer. Number of extra vertices to add along each cell
#'                  edge (0 = none). Larger values produce smoother boundaries
#'                  for coarse-resolution cells. Default: 0.
#'
#' @return Returns an sf object.
#'         If \code{!is.na(savegrid)}, returns a filename.
#'
#' @examples
#' library(dggridR)
#' data(dgquakes)
#'
#' #Construct a grid with cells about ~1000 miles wide
#' dggs          <- dgconstruct(spacing=1000,metric=FALSE)
#' dgquakes$cell <- dgGEO_to_SEQNUM(dggs,dgquakes$lat,dgquakes$lon)$seqnum
#'
#' #Get grid cells for the earthquakes identified
#' grid          <- dgcellstogrid(dggs, dgquakes$cell)
#' @export
dgcellstogrid <- function(dggs, cells, savegrid=NA, return_sf = TRUE, densify = 0L) {
  dgverify(dggs)
  isMixed43 <- isTRUE(dggs[["isMixed43"]])
  numAp4    <- if(is.null(dggs[["numAp4"]])) 0L else dggs[["numAp4"]]

  #dggrid also eliminates duplicate cells, but doing so here saves disk space
  #and likely wall time, given the costs of IO, not that it matters unless the
  #data set is huge
  cells <- funique(cells)

  if(max(cells) > dgmaxcell(dggs))
    stop("'cells' contained cell ids which were larger than the maximum id!")

  grid <- SeqNumGrid(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], cells, isMixed43, numAp4, as.integer(densify))
  if(is.na(savegrid)){
    if(!return_sf) return(qDF(grid))
    dg_process_polydata(grid)
  } else {
    grid <- dg_process_polydata(grid)
    dgsavegrid(grid,savegrid)
  }
}



#' @name dgsavegrid
#'
#' @title           Saves a generated grid to a shapefile
#'
#' @description     Saves a generated grid to a shapefile
#'
#' @param grid      Grid to be saved
#' @param shpfname  File to save the grid to
#'
#' @return          The filename the grid was saved to
#'
#' @keywords internal
dgsavegrid <- function(grid,shpfname) {
  write_sf(grid, shpfname, driver='ESRI Shapefile', layer='dggrid')
  shpfname
}



#' @name dgshptogrid
#'
#' @title           Return boundary coordinates for cells intersecting a
#'                  shapefile
#'
#' @description     Returns the coordinates constituting the boundary of a
#'                  set of cells which intersect or are contained by a polygon
#'                  (or polygons) specified in a shapefile. Note that grid cells
#'                  are also generated for holes in the shapefile's polygon(s).
#'
#'                  Note that coordinates in the shapefile must be rounded to
#'                  check polygon intersections. Currently this round preserves
#'                  eight decimal digits of precision.
#'
#'                  The eighth decimal place is worth up to 1.1 mm of precision:
#'                  this is good for charting the motions of tectonic plates and
#'                  the movements of volcanoes. Permanent, corrected,
#'                  constantly-running GPS base stations might be able to
#'                  achieve this level of accuracy.
#'
#'                  In other words: you should be just fine with this level of
#'                  precision.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param shpfname  Either a sf data frame or the file name of the shapefile. Filename should end with '.shp'.
#'
#' @param cellsize  Distance, in degrees, between the sample points used to
#'                  generate the grid. Small values yield long generation times
#'                  while large values may omit cells.
#'
#' @param \dots Further arguments passed to \code{\link{dgcellstogrid}}.
#'
#' @return Returns an sf object.
#'         If \code{!is.na(savegrid)}, returns a filename.
#'
#' @examples
#' library(dggridR)
#'
#' dggs <- dgconstruct(spacing=25, metric=FALSE, resround='nearest')
#' south_africa_grid <- dgshptogrid(dggs,dg_shpfname_south_africa())
#' @export
dgshptogrid <- function(dggs, shpfname, cellsize = 0.1, ...) { #TODO: Densify?
  dgverify(dggs)

  if(!(is.data.frame(shpfname) && inherits(shpfname, "sf"))) {

    shpfname <- trimws(shpfname)

    if(!grepl('\\.shp$',shpfname))
      stop("Shapefile name does to end with '.shp'!")
    if(!file.exists(shpfname))
      stop('Shapefile does not exist!')

    dsn   <- dirname(shpfname)
    layer <- file_path_sans_ext(basename(shpfname))
    bb  <- st_bbox_to_sp(st_bbox(st_read(dsn, layer=layer)))

  } else {
    bb <- st_bbox_to_sp(st_bbox(shpfname))
  }

  #Generate a dense grid of points
  samp_points <- makegrid(bb, cellsize = cellsize)

  #Convert the points to SEQNUM ids for dggs
  samp_points <- dgGEO_to_SEQNUM(dggs, samp_points$x1, samp_points$x2)$seqnum

  dgcellstogrid(dggs, samp_points, ...)
}



#' @name dgpoints_to_cells
#'
#' @title Return grid cells containing input points
#'
#' @description
#'   Finds the grid cells that contain each of the supplied lon/lat points
#'   and returns their boundaries as an sf data frame (equivalent to DGGRID's
#'   \code{GENERATE_GRID_FROM_POINTS} operation). Duplicate points in the same
#'   cell are deduplicated; only cells with at least one point are returned.
#'
#' @param dggs        A dggs object from \code{\link{dgconstruct}}.
#' @param lon         Numeric vector of longitudes (decimal degrees).
#' @param lat         Numeric vector of latitudes (decimal degrees).
#' @param return_count Logical. If \code{TRUE}, add a \code{count} column with
#'                    the number of input points in each cell. Default: \code{FALSE}.
#' @param \dots       Further arguments passed to \code{\link{dgcellstogrid}}.
#'
#' @return An sf data frame of cell boundaries. If \code{return_count=TRUE},
#'         includes a \code{count} integer column.
#'
#' @examples
#' library(dggridR)
#' data(dgquakes)
#' dggs <- dgconstruct(spacing=1000, metric=FALSE, resround='down')
#' grid <- dgpoints_to_cells(dggs, dgquakes$lon, dgquakes$lat, return_count=TRUE)
#' @export
dgpoints_to_cells <- function(dggs, lon, lat, return_count = FALSE, ...) {
  dgverify(dggs)
  seqnums <- dgGEO_to_SEQNUM(dggs, lon, lat)$seqnum
  grid    <- dgcellstogrid(dggs, funique(seqnums), ...)
  if(return_count) {
    grid$count <- as.integer(tabulate(match(seqnums, grid$seqnum)))
  }
  grid
}



#' @name dgbin_points
#'
#' @title Aggregate point data into grid cells
#'
#' @description
#'   Bins a set of lon/lat points — optionally with associated numeric values
#'   — into the cells of a discrete global grid. Returns a data frame (no
#'   geometry) with per-cell statistics (equivalent to DGGRID's
#'   \code{BIN_POINT_VALS} / \code{BIN_POINT_PRESENCE} operations).
#'
#' @param dggs         A dggs object from \code{\link{dgconstruct}}.
#' @param lon          Numeric vector of longitudes (decimal degrees).
#' @param lat          Numeric vector of latitudes (decimal degrees).
#' @param values       Optional numeric vector of values to aggregate (same
#'                     length as \code{lon}/\code{lat}). If \code{NULL}, only
#'                     point counts are computed.
#' @param output_count Logical. Return count of points per cell.
#'                     Default: \code{TRUE} when \code{values} is \code{NULL},
#'                     \code{FALSE} otherwise.
#' @param output_mean  Logical. Return mean of values per cell. Only meaningful
#'                     when \code{values} is supplied. Default: \code{TRUE} when
#'                     \code{values} is not \code{NULL}.
#' @param output_total Logical. Return total (sum) of values per cell. Only
#'                     meaningful when \code{values} is supplied.
#'                     Default: \code{FALSE}.
#'
#' @return A data frame with a \code{seqnum} column (cell ID) plus whichever
#'         of \code{count}, \code{mean}, \code{total} were requested.
#'
#' @examples
#' library(dggridR)
#' data(dgquakes)
#' dggs <- dgconstruct(spacing=1000, metric=FALSE, resround='down')
#'
#' # Count earthquakes per cell
#' dgbin_points(dggs, dgquakes$lon, dgquakes$lat)
#'
#' # Aggregate magnitude per cell
#' dgbin_points(dggs, dgquakes$lon, dgquakes$lat, values=dgquakes$mag,
#'              output_count=TRUE, output_total=TRUE)
#' @export
dgbin_points <- function(dggs, lon, lat, values = NULL,
                         output_count = is.null(values),
                         output_mean  = !is.null(values),
                         output_total = FALSE) {
  dgverify(dggs)
  if(!is.null(values) && length(values) != length(lon))
    stop('dgbin_points(): values must have the same length as lon/lat', call.=FALSE)

  seqnums <- dgGEO_to_SEQNUM(dggs, lon, lat)$seqnum
  val     <- if(is.null(values)) rep(NA_real_, length(seqnums)) else as.numeric(values)
  df      <- qDF(list(seqnum = seqnums, value = val))
  grp     <- fgroup_by(df, seqnum)
  tmp     <- fsummarise(grp, count = fnobs(seqnum), mean = fmean(value), total = fsum(value))

  result  <- tmp["seqnum"]
  if(output_count) result$count <- tmp$count
  if(!is.null(values) && output_mean)  result$mean  <- tmp$mean
  if(!is.null(values) && output_total) result$total <- tmp$total

  if(ncol(result) == 1L)
    stop('dgbin_points(): at least one of output_count, output_mean, output_total must be TRUE', call.=FALSE)

  result
}



#' @name dgneighbors
#'
#' @title           Return neighboring cell IDs for each input cell
#'
#' @description     For each cell ID in \code{cells}, returns the IDs of all
#'                  adjacent cells. Triangle grids are not supported.
#'
#' @param dggs      A dggs object from \code{\link{dgconstruct}()}
#' @param cells     Integer vector of cell sequence numbers (SEQNUM)
#'
#' @return A data frame with columns \code{seqnum} (the input cell) and
#'         \code{neighbor} (each adjacent cell ID).
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=3)
#' nbrs <- dgneighbors(dggs, c(1, 2, 3))
#'
#' @export
dgneighbors <- function(dggs, cells) {
  dgverify(dggs)
  if(dggs[['topology']] == 'TRIANGLE')
    stop('dgneighbors() is not supported for TRIANGLE grids', call.=FALSE)
  isMixed43 <- isTRUE(dggs[["isMixed43"]])
  numAp4    <- if(is.null(dggs[["numAp4"]])) 0L else dggs[["numAp4"]]
  result <- GetNeighbors(
    dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]],
    dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]],
    cells, isMixed43, numAp4
  )
  qDF(result)
}



#' @name dgchildren
#'
#' @title           Return immediate child cell IDs for each input cell
#'
#' @description     For each cell ID in \code{cells} at the current grid
#'                  resolution, returns the IDs of its child cells at resolution
#'                  \code{dggs\$res + 1}. Only hexagonal grids are supported.
#'
#' @param dggs      A dggs object from \code{\link{dgconstruct}()}.
#'                  The child cells will be at \code{dggs\$res + 1}.
#' @param cells     Integer vector of cell sequence numbers (SEQNUM)
#'
#' @return A data frame with columns \code{seqnum} (the input cell) and
#'         \code{child} (each child cell ID at resolution + 1).
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=3)
#' chld <- dgchildren(dggs, c(1, 2))
#'
#' @export
dgchildren <- function(dggs, cells) {
  dgverify(dggs)
  if(dggs[['topology']] != 'HEXAGON')
    stop('dgchildren() is only supported for HEXAGON grids', call.=FALSE)
  if(dggs[['res']] < 1L)
    stop('dgchildren() requires res >= 1', call.=FALSE)
  isMixed43 <- isTRUE(dggs[["isMixed43"]])
  numAp4    <- if(is.null(dggs[["numAp4"]])) 0L else dggs[["numAp4"]]
  result <- GetChildren(
    dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]],
    dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]],
    cells, isMixed43, numAp4
  )
  qDF(result)
}



#' @name dgparent
#'
#' @title           Return parent cell ID for each input cell
#'
#' @description     For each cell ID in \code{cells} at the current grid
#'                  resolution, returns the ID of its parent cell at resolution
#'                  \code{dggs\$res - 1}. Only hexagonal grids are supported.
#'
#' @param dggs      A dggs object from \code{\link{dgconstruct}()}.
#'                  The parent cells will be at \code{dggs\$res - 1}.
#' @param cells     Integer vector of cell sequence numbers (SEQNUM)
#'
#' @return A data frame with columns \code{seqnum} (the input cell) and
#'         \code{parent} (the parent cell ID at resolution - 1).
#'
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=4)
#' prnt <- dgparent(dggs, c(1, 2))
#'
#' @export
dgparent <- function(dggs, cells) {
  dgverify(dggs)
  if(dggs[['topology']] != 'HEXAGON')
    stop('dgparent() is only supported for HEXAGON grids', call.=FALSE)
  if(dggs[['res']] < 1L)
    stop('dgparent() requires res >= 1 (no parent exists at res 0)', call.=FALSE)
  isMixed43 <- isTRUE(dggs[["isMixed43"]])
  numAp4    <- if(is.null(dggs[["numAp4"]])) 0L else dggs[["numAp4"]]
  result <- GetParent(
    dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]],
    dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]],
    cells, isMixed43, numAp4
  )
  qDF(result)
}
