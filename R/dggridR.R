#' @name dg_exe_path
#' 
#' @title Returns a path to the dggrid executable. Used for running stuff.
#'
#' @return A string representing the path to the dggrid executable.
#'
#' @examples 
#' dg_exe_path()
dg_exe_path <- function(){
  file.path(system.file(package="dggridR"), "bin", "dggrid")
}







#' @name dgconstruct
#' 
#' @title Construct a discrete global grid system (dggs) object
#'
#' @details 
#' 
#' @param type Type of grid to use. Options are: ISEA3H, ISEA4H, ISEA43H, 
#'             ISEA4T, ISEA4D, FULLER3H, FULLER4H, FULLER43H, FULLER4T,
#'             and FULLER4D. Default: ISEA3H
#'
#' @param res  Resolution. Must be in the range [0,35]. Larger values represent
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
#' @param round     What direction to search in. Must be nearest, up, or down.
#'
#' @param show_info Print the area, spacing, and CLS of the chosen resolution.
#'
#' @param metric    Whether input and output should be in metric (TRUE) or
#'                  imperial (FALSE)
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
#' @export 
dgconstruct <- function(type='ISEA3H', res=NA, precision=7, area=NA, spacing=NA, cls=NA, resround='nearest', metric=TRUE, show_info=TRUE){
  if(sum(!is.na(c(res,area,spacing,cls)))!=1)
    stop('dgconstruct(): Only one of res, area, length, or cls can have a value!')

  #Use a dummy resolution, we'll fix it in a moment
  dggs <- list(dggs_type=type, dggs_res_spec=1, precision=7)

  if(!is.na(res))
    dggs[['dggs_res_spec']] = res
  else if(!is.na(area))
    dggs[['dggs_res_spec']] = dg_closest_res_to_area   (dggs,area=area,      round=resround,metric=metric,show_info=TRUE)
  else if(!is.na(spacing))
    dggs[['dggs_res_spec']] = dg_closest_res_to_spacing(dggs,spacing=spacing,round=resround,metric=metric,show_info=TRUE)
  else if(!is.na(cls))
    dggs[['dggs_res_spec']] = dg_closest_res_to_cls    (dggs,cls=cls,        round=resround,metric=metric,show_info=TRUE)
  else
    stop('dgconstruct(): Logic itself has failed us.')

  dgverify(dggs)

  dggs
}



#' @name dgsetres
#' 
#' @title Set the resolution of a dggs object
#'
#' @details 
#'
#' @param dggs A dggs object from dgconstruct().
#'
#' @param res  Resolution. Must be in the range [0,35]. Larger values represent
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
#'
#' @export 
dgsetres <- function(dggs,res){
  dggs[['dggs_res_spec']] = res
  dgverify(dggs)
  dggs
}





#' @name dgverify
#' 
#' @title Verifies that a dggs object has appropriate values
#'
#' @details 
#' 
#' @param dggs The dggs object to be verified
#'
#' @return     The function has no return value. A stop signal is raised if the
#'             object is misspecified
#' @examples
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' dgverify(dggs)
dgverify <- function(dggs){
  #See page 21 of documentation for further bounds
  if(!(dggs[['dggs_type']] %in% c('ISEA3H','ISEA4H','ISEA43H','ISEA4T','ISEA4D','FULLER3H','FULLER4H','FULLER43H','FULLER4T','FULLER4D')))
    stop('Unrecognised dggs type', call.=FALSE) #TODO: Where can they get valid types?
  if(dggs[['dggs_res_spec']]<0)
    stop('dggs resolution must be >=0', call.=FALSE)
  if(dggs[['dggs_res_spec']]>35)
    stop('dggs resolution must be <=35', call.=FALSE)
  if(dggs[['precision']]<0)
    stop('dggs precision must be >=0', call.=FALSE)
  if(dggs[['precision']]>35)
    stop('dggs precision must be <=30', call.=FALSE)
  if(!all.equal(dggs[['dggs_res_spec']], as.integer(dggs[['dggs_res_spec']])))
    stop('dggs resolution must be an integer', call.=FALSE)
}









#' @name dgtransform
#' 
#' @title Converts lat-long pairs into discrete global grid cell numbers
#'
#' @details A discrete global grid maps lat-long points to particular cells.
#'          These cells are uniquely numbered, for a given resolution, from
#'          1 to some maximum number. Cell numbers may be reused from one
#'          resolution to the next.
#' 
#' @param dggs A dggs object from dgconstruct().
#'
#' @param lat  A vector of latitudes. Same length at the longtiudes
#'
#' @param long A vector of longitudes. Same length as the latitudes.
#'
#' @return     A vector of the same length as latitudes and longitudes 
#'             containing the cell id numbers of the points' cells
#'             in the discrete grid.
#'
#' @examples 
#' library(dggridR)
#' data(dgquakes)
#'
#' #Construct a grid with cells about ~1000 miles wide
#' dggs          <- dgconstruct(spacing=1000,metric=FALSE) 
#' dgquakes$cell <- dgtransform(dggs,dgquakes$lat,dgquakes$lon)
#'
#' @export 
dgtransform <- function(dggs, lat, lon){ #TODO: Make sure we're not modifying the original dggs
  dgverify(dggs)

  glon      <- lon>180
  lon[glon] <- lon[glon]-360
  llon      <- lon< (-180)
  lon[llon] <- lon[llon]+360

  inputfile  <- tempfile(pattern = "dggridR-", fileext=".indat" )
  outputfile <- tempfile(pattern = "dggridR-", fileext=".outdat")

  message(inputfile)
  message(outputfile)

  df <- data.frame(long=lon,lat=lat)
  write.table(df, inputfile, sep=",", col.names=FALSE, row.names=FALSE) #TODO: Verify output precision

  dggs[['dggrid_operation']]   = 'TRANSFORM_POINTS'

  dggs[['input_file_name']]    = inputfile
  dggs[['input_address_type']] = 'GEO'
  dggs[['input_delimiter']]    = '","'
  
  dggs[['output_file_name']]    = outputfile
  dggs[['output_address_type']] = 'SEQNUM'
  dggs[['output_delimiter']]    = '","'

  dgrun(dggs)

  #TODO: Consider reading cell ids as strings instead :-(
  ret <- read.csv(dggs[['output_file_name']], header=FALSE)$V1

  if(any(ret>=2^53)) #R stores large numbers as an IEEE754 double, so we get 53 bits of exact integer goodness.
    message('dgtransform(): Length of cell ids overflowed R\'s numeric storage capacity. Use a lower resolution')

  ret
}











#' @name dgrun
#' 
#' @title A generic function for running dggrid and returning values from it
#'
#' @details A discrete global grid maps lat-long points to particular cells.
#'          These cells are uniquely numbered, for a given resolution, from
#'          1 to some maximum number. Cell numbers may be reused from one
#'          resolution to the next.
#'
#' @param dggs  A dggs object from dgconstruct()
#'
#' @param clean If TRUE, delete the temporary files used/produced by dggrid. TODO
#'
#' @param check If TRUE, raise a stop signal if dggrid doesn't run successfully.
#'
#' @param has_output_file If TRUE, look for output file. If FALSE, capture and
#'                        return stdout and stderr.
#'
#' @return  If \code{has_output_file} is TRUE, a data frame is returned.
#'          The calling function is responsible for understanding this frame.
#'          If \code{has_output_file} is FALSE, the stdout and stderr of dggrid
#'          are captured and returned as a character vector.
dgrun <- function(dggs, clean=TRUE, check=TRUE, has_output_file=TRUE){
  metafile <- tempfile(pattern = "dggridR-", fileext=".meta")
  write.table(as.data.frame(do.call(rbind, dggs)), metafile, col.names=FALSE, quote=FALSE)
  com <- paste(dg_exe_path(),metafile)
  ret <- system(com, intern = TRUE, ignore.stdout = FALSE, ignore.stderr = FALSE)
  if(check && length(grep("complete \\*\\*",ret))!=1){
      cat(ret)
      stop('dggridR: Error in processing!', call.=FALSE)
  }
  ret
}












#' @name dginfo
#' 
#' @title Print a buncha info about a dggs object to the screen
#'
#' @details dggs objects have many settings. This returns all of them, along
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
#'
#' @export 
dginfo <- function(dggs){
  dgverify(dggs)

  dggs[['dggrid_operation']] = 'OUTPUT_STATS'
  dggs[['dggs_res_spec']]    = 35

  cat(dgrun(dggs, check=FALSE, has_output_file=FALSE), sep="\r\n")

  NULL
}













#' @name dggetres
#' 
#' @title Gets a grid's resolution and cell property info as a data frame.
#'
#' @details 
#' 
#' @param dggs A dggs object from dgconstruct()
#'
#' @return A data frame containing the resolution levels, number of cells,
#'         area of those cells, intercell spacing, and characteristic length
#'         scale of the cells. All values are in kilometres.
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' dggetres(dggs)
#'
#' @export 
dggetres <- function(dggs){
  dgverify(dggs)

  dggs[['dggrid_operation']] = 'OUTPUT_STATS'
  dggs[['dggs_res_spec']]    = 35 #Used so that we get all resolution levels
  dggs[['precision']]        = 30 #Used so that very fine meshes still give us numbers >0

  ret <- dgrun(dggs, check=FALSE, has_output_file=FALSE)
  ret <- dgrun(dggs, check=FALSE, has_output_file=FALSE)
  ret <- tail(ret,-26)
  ret <- gsub(',','',ret)

  #Redefine table header so that it converts to a nice data frame
  ret[[1]]<-"Res Cells AreaKm SpacingKm CLSKm"

  read.table(textConnection(ret), header=TRUE)
}


#' @name dg_closest_res
#' 
#' @title Determine an appropriate grid resolution based on input data.
#'
#' @details This is a generic function that is used to determine an appropriate
#'          resolution given an area, cell spacing, or correlated length scale.
#'          It does so by extracting the appropriate length/area column and
#'          searching it for a value close to the input.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param col       Column in which to search for a close value. Should be: 
#'                  AreaKm, SpacingKm, or CLSKm.
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
#' res  <- dg_closest_res(dggs,'AreaKm',1)
#' dggs <- dgsetres(dggs,res)
dg_closest_res <- function(dggs,col,val,round='nearest',show_info=TRUE,metric=TRUE){
  KM_TO_M <- 0.621371

  dgverify(dggs)

  ret <- dggetres(dggs)

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
    cat(paste("Resolution: ",ret$Res[idx],", Area (km^2): ",ret$AreaKm[idx],", Spacing (km): ", ret$SpacingKm[idx],", CLS (km): ", ret$CLSKm[idx], "\n", sep=""))
  else if(show_info && !metric)
    cat(paste("Resolution: ",ret$Res[idx],", Area (mi^2): ",ret$AreaKm[idx]*KM_TO_M,", Spacing (mi): ", ret$SpacingKm[idx]*KM_TO_M,", CLS (mi): ", ret$CLSKm[idx]*KM_TO_M, "\n", sep=""))

  ret$Res[idx]
}


#' @name dg_closest_res_to_area
#' 
#' @title Determine an appropriate grid resolution based on a desired cell area.
#'
#' @details 
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
#'
#' @export 
dg_closest_res_to_area <- function(dggs,area,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'AreaKm',area,round,show_info,metric)
}


#' @name dg_closest_res_to_spacing
#' 
#' @title Determine an appropriate grid resolution based on a desired spacing
#'        between the center of adjacent cells.
#'
#' @details 
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
#'
#' @export 
dg_closest_res_to_spacing <- function(dggs,spacing,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'SpacingKm',spacing,round,show_info,metric)
}



#' @name dg_closest_res_to_cls
#' 
#' @title Determine an appropriate grid resolution based on a desired 
#'        characteristic length scale of the cells.
#'
#' @details The characteristic length scale (CLS) is the diameter of a spherical
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
#'
#' @export
dg_closest_res_to_cls <- function(dggs,cls,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'CLSKm',cls,round,show_info,metric)
}










#' @name dg_process_kml
#' 
#' @title   Clean up and/or convert a KML grid cell boundary file
#'
#' @details         
#'
#' @param kmlfile   A KML file generated by dgrectgrid() or dgearthgrid()
#'
#' @param frame     If TRUE, return a data frame suitable for ggplot plotting.
#'                  If FALSE, return an OGR poly object
#'
#' @return Returns a data frame or OGR poly object, as specified by \code{frame}
#'
dg_process_kml <- function(kmlfile,frame){
  require(rgdal)   #For reading KML output of dggrid

  map <- readOGR(kmlfile,kmlfile)

  map@data$timestamp    <- NULL
  map@data$begin        <- NULL
  map@data$end          <- NULL
  map@data$altitudeMode <- NULL
  map@data$extrude      <- NULL
  map@data$visibility   <- NULL
  map@data$drawOrder    <- NULL
  map@data$icon         <- NULL
  map@data$description  <- NULL
  map@data$tessellate   <- NULL
  map@data$id           <- rownames(map@data)
  if(frame){
    require(ggplot2) #For fortify
    map.points <- fortify(map, region="id")
    map.df     <- merge(map.points, map@data, by="id")
  } else {
    map
  }
}







#' @name dgrectgrid
#' 
#' @title   Return the coordinates constituting the boundary of cells within a
#'          specified region
#'
#' @details         Note: This may generate odd results for very large
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
#' @param frame     If TRUE, return a data frame suitable for ggplot plotting.
#'                  If FALSE, return an OGR poly object
#'
#' @return Returns a data frame or OGR poly object, as specified by \code{frame}
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(spacing=1000,metric=FALSE,round='down')
#'
#' #Get grid cells for the conterminous United States
#' grid <- dgrectgrid(dggs,minlat=24.7433195, minlon=-124.7844079, maxlat=49.3457868, maxlon=-66.9513812, frame=TRUE)
#'
#' @export
dgrectgrid <- function(dggs,minlat=-1,minlon=-1,maxlat=-1,maxlon=-1,frame=TRUE){ #TODO: Densify?
  dgverify(dggs) 

  library(rgdal)   #For reading KML output of dggrid

  inputfile <- tempfile(pattern = "dggridR-", fileext=".indat"   )
  cellfile  <- tempfile(pattern = "dggridR-", fileext=".cell_dat")

  message(inputfile)
  message(cellfile)
  
  dggs[['dggrid_operation']] = 'GENERATE_GRID'
  dggs[['update_frequency']] = 10000000

  #Write output in ARC/INFO Generate polygon format
  #minlon,maxlat maxlon,maxlat
  #minlon,minlat maxlon,minlat
  fout <- file(inputfile, "w")  # open an output file connection
  cat("1", sep="\n", file=fout)
  cat(paste(minlon,maxlat), sep="\n", file=fout)
  cat(paste(maxlon,maxlat), sep="\n", file=fout)
  cat(paste(maxlon,minlat), sep="\n", file=fout)
  cat(paste(minlon,minlat), sep="\n", file=fout)
  cat(paste(minlon,maxlat), sep="\n", file=fout)
  cat("END", sep="\n", file=fout) #End list of polygon points
  cat("END", sep="\n", file=fout) #End file
  close(fout)
  dggs[['clip_region_files']] = inputfile
  dggs[['clip_subset_type']]  = 'AIGEN'

  dggs[['cell_output_file_name']] = cellfile
  dggs[['cell_output_type']]      = 'KML' #SHAPEFILE or KML or GEOJSON or AIGEN

  ret <- dgrun(dggs,check=FALSE,has_output_file=FALSE)

  cellfile <- paste(cellfile,".kml",sep="")
  dg_process_kml(cellfile,frame)
}















#' @name dgearthgrid
#' 
#' @title   Return the coordinates constituting the boundary of cells for the
#'          entire Earth
#'
#' @details         Note: If you have a high-resolution grid this may take a
#'                  loooooong time to execute.
#'
#' @param dggs      A dggs object from dgconstruct()
#'
#' @param frame     If TRUE, return a data frame suitable for ggplot plotting.
#'                  If FALSE, return an OGR poly object
#'
#' @return Returns a data frame or OGR poly object, as specified by \code{frame}
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#' res  <- dg_closest_res_to_spacing(dggs,spacing=1000,round='down',metric=FALSE)
#' dggs <- dgsetres(dggs,res)
#' grid <- dgearthgrid(dggs,frame=TRUE)
#'
#' @export
dgearthgrid <- function(dggs,frame=TRUE){ #TODO: Densify?
  dgverify(dggs) 

  inputfile <- tempfile(pattern = "dggridR-", fileext=".indat"   )
  cellfile  <- tempfile(pattern = "dggridR-", fileext=".cell_dat")

  message(inputfile)
  message(cellfile)
  
  dggs[['dggrid_operation']] = 'GENERATE_GRID'
  dggs[['update_frequency']] = 10000000

  dggs[['clip_subset_type']] = 'WHOLE_EARTH'

  dggs[['cell_output_file_name']] = cellfile
  dggs[['cell_output_type']]      = 'KML' #SHAPEFILE or KML or GEOJSON or AIGEN

  ret <- dgrun(dggs,check=FALSE,has_output_file=FALSE)

  cellfile <- paste(cellfile,".kml",sep="")

  dg_process_kml(cellfile,frame)
}