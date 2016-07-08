dggrid_exe_path <- function(){
    file.path(system.file(package="dggridR"), "bin", "dggrid")
}

dgtransform <- function(lat, long, dggs){ #TODO: Make sure we're not modifying the original dggs
    verify_dggs(dggs)

    inputfile  <- tempfile(pattern = "dggridR-", fileext=".indat" )
    outputfile <- tempfile(pattern = "dggridR-", fileext=".outdat")

    message(inputfile)
    message(outputfile)

    df <- data.frame(long=long,lat=lat)
    write.table(df, inputfile, sep=",", col.names=FALSE, row.names=FALSE) #TODO: Verify output precision

    dggs[['dggrid_operation']]   = 'TRANSFORM_POINTS'

    dggs[['input_file_name']]    = inputfile
    dggs[['input_address_type']] = 'GEO'
    dggs[['input_delimiter']]    = '","'
    
    dggs[['output_file_name']]    = outputfile
    dggs[['output_address_type']] = 'SEQNUM'
    dggs[['output_delimiter']]    = '","'

    run_dggs(dggs)$V1
}

dgstats <- function(dggs){
  verify_dggs(dggs)

  dggs[['dggrid_operation']] = 'OUTPUT_STATS'
  dggs[['dggs_res_spec']]    = 35

  cat(run_dggs(dggs, check=FALSE, has_output_file=FALSE), sep="\r\n")
}

getres <- function(dggs){
  verify_dggs(dggs)

  dggs[['dggrid_operation']] = 'OUTPUT_STATS'
  dggs[['dggs_res_spec']]    = 35

  ret <- run_dggs(dggs, check=FALSE, has_output_file=FALSE)
  ret <- run_dggs(dggs, check=FALSE, has_output_file=FALSE)
  ret <- tail(ret,-26)
  ret <- gsub(',','',ret)

  ret[[1]]<-"Res Cells AreaKm SpacingKm CLSKm"

  read.table(textConnection(ret), header=TRUE)
}

get_closest <- function(vec,val,round){

}

dg_closest_res <- function(dggs,col,val,round='nearest',show_info=TRUE,metric=TRUE){
  KM_TO_M <- 0.621371

  verify_dggs(dggs)

  ret <- getres(dggs)

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
    stop('Unrecognised rounding direction. Must be up, down, or nearest.')

  if(show_info && metric)
    cat(paste("Resolution: ",ret$Res[idx],", Area (km^2): ",ret$AreaKm[idx],", Spacing (km): ", ret$SpacingKm[idx],", CLS (km): ", ret$CLSKm[idx], "\n", sep=""))
  else if(show_info && !metric)
    cat(paste("Resolution: ",ret$Res[idx],", Area (mi^2): ",ret$AreaKm[idx]*KM_TO_M,", Spacing (mi): ", ret$SpacingKm[idx]*KM_TO_M,", CLS (mi): ", ret$CLSKm[idx]*KM_TO_M, "\n", sep=""))

  idx
}

dg_closest_res_to_area <- function(dggs,area,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'AreaKm',area,round,show_info,metric)
}

dg_closest_res_to_spacing <- function(dggs,spacing,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'SpacingKm',spacing,round,show_info,metric)
}

#Characteristic Length Scale (CLS): the diameter of a spherical cap of the same area as a cell of the specified resolution. This metric was suggested by Ralph Kahn. 
dg_closest_res_to_cls <- function(dggs,cls,round='nearest',show_info=TRUE,metric=TRUE){
  dg_closest_res(dggs,'CLSKm',cls,round,show_info,metric)
}


construct_dggs <- function(type='ISEA3H', res=9, precision=7){
    list(dggs_type=type, dggs_res_spec=res, precision=7)
}

#See page 21 of documentation
verify_dggs <- function(dggs){
    if(!(dggs[['dggs_type']] %in% c('ISEA3H','ISEA4H','ISEA43H','ISEA4T','ISEA4D','FULLER3H','FULLER4H','FULLER43H','FULLER4T','FULLER4D')))
        stop('Unrecognised dggs type') #TODO: Where can they get valid types?
    if(dggs[['dggs_res_spec']]<0)
        stop('dggs resolution must be >=0')
    if(dggs[['dggs_res_spec']]>35)
        stop('dggs resolution must be <=35')
    if(dggs[['precision']]<0)
        stop('dggs precision must be >=0')
    if(dggs[['precision']]>35)
        stop('dggs precision must be <=30')
    if(!all.equal(dggs[['dggs_res_spec']], as.integer(dggs[['dggs_res_spec']])))
        stop('dggs resolution must be an integer')
}

run_dggs <- function(dggs, clean=TRUE, check=TRUE, has_output_file=TRUE){
    metafile <- tempfile(pattern = "dggridR-", fileext=".meta")
    write.table(as.data.frame(do.call(rbind, dggs)), metafile, col.names=FALSE, quote=FALSE)
    com <- paste(dggrid_exe_path(),metafile)
    ret <- system(com, intern = TRUE, ignore.stdout = FALSE, ignore.stderr = FALSE)
    if(check && grep("complete",tail(ret, n=1))!=1){
        cat(ret)
        stop('dggridR: Error in processing!')
    }
    if(has_output_file)
      read.csv(dggs[['output_file_name']], header=FALSE)
    else
      ret
}

dggs_types <- function(){
    message('todo')
}