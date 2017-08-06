
#' @name GEO_to_GEO
#' 
#' @title      Convert from GEO to GEO
#'
#' @description
#'             Uses a discrete global grid system to convert between GEO
#'             and GEO (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' GEO_to_GEO(dggs, in_lon_deg, in_lat_deg)
#'
#' @export 
dgGEO_to_GEO <- function(dggs, in_lon_deg, in_lat_deg){
  dgverify(dggs)

  N <- length(in_lon_deg)
  out_lon_deg <- rep(0,length(N))
  out_lat_deg <- rep(0,length(N))

  dggridR:::GEO_to_GEO(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_lon_deg, in_lat_deg, out_lon_deg, out_lat_deg)
}

#' @name GEO_to_PROJTRI
#' 
#' @title      Convert from GEO to PROJTRI
#'
#' @description
#'             Uses a discrete global grid system to convert between GEO
#'             and PROJTRI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' GEO_to_PROJTRI(dggs, in_lon_deg, in_lat_deg)
#'
#' @export 
dgGEO_to_PROJTRI <- function(dggs, in_lon_deg, in_lat_deg){
  dgverify(dggs)

  N <- length(in_lon_deg)
  out_tnum <- rep(0,length(N))
  out_tx <- rep(0,length(N))
  out_ty <- rep(0,length(N))

  dggridR:::GEO_to_PROJTRI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_lon_deg, in_lat_deg, out_tnum, out_tx, out_ty)
}

#' @name GEO_to_Q2DD
#' 
#' @title      Convert from GEO to Q2DD
#'
#' @description
#'             Uses a discrete global grid system to convert between GEO
#'             and Q2DD (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' GEO_to_Q2DD(dggs, in_lon_deg, in_lat_deg)
#'
#' @export 
dgGEO_to_Q2DD <- function(dggs, in_lon_deg, in_lat_deg){
  dgverify(dggs)

  N <- length(in_lon_deg)
  out_quad <- rep(0,length(N))
  out_qx <- rep(0,length(N))
  out_qy <- rep(0,length(N))

  dggridR:::GEO_to_Q2DD(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_lon_deg, in_lat_deg, out_quad, out_qx, out_qy)
}

#' @name GEO_to_Q2DI
#' 
#' @title      Convert from GEO to Q2DI
#'
#' @description
#'             Uses a discrete global grid system to convert between GEO
#'             and Q2DI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' GEO_to_Q2DI(dggs, in_lon_deg, in_lat_deg)
#'
#' @export 
dgGEO_to_Q2DI <- function(dggs, in_lon_deg, in_lat_deg){
  dgverify(dggs)

  N <- length(in_lon_deg)
  out_quad <- rep(0,length(N))
  out_i <- rep(0,length(N))
  out_j <- rep(0,length(N))

  dggridR:::GEO_to_Q2DI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_lon_deg, in_lat_deg, out_quad, out_i, out_j)
}

#' @name GEO_to_SEQNUM
#' 
#' @title      Convert from GEO to SEQNUM
#'
#' @description
#'             Uses a discrete global grid system to convert between GEO
#'             and SEQNUM (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' GEO_to_SEQNUM(dggs, in_lon_deg, in_lat_deg)
#'
#' @export 
dgGEO_to_SEQNUM <- function(dggs, in_lon_deg, in_lat_deg){
  dgverify(dggs)

  N <- length(in_lon_deg)
  out_seqnum <- rep(0,length(N))

  dggridR:::GEO_to_SEQNUM(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_lon_deg, in_lat_deg, out_seqnum)
}

#' @name GEO_to_PLANE
#' 
#' @title      Convert from GEO to PLANE
#'
#' @description
#'             Uses a discrete global grid system to convert between GEO
#'             and PLANE (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' GEO_to_PLANE(dggs, in_lon_deg, in_lat_deg)
#'
#' @export 
dgGEO_to_PLANE <- function(dggs, in_lon_deg, in_lat_deg){
  dgverify(dggs)

  N <- length(in_lon_deg)
  out_px <- rep(0,length(N))
  out_py <- rep(0,length(N))

  dggridR:::GEO_to_PLANE(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_lon_deg, in_lat_deg, out_px, out_py)
}

#' @name PROJTRI_to_GEO
#' 
#' @title      Convert from PROJTRI to GEO
#'
#' @description
#'             Uses a discrete global grid system to convert between PROJTRI
#'             and GEO (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' PROJTRI_to_GEO(dggs, in_tnum, in_tx, in_ty)
#'
#' @export 
dgPROJTRI_to_GEO <- function(dggs, in_tnum, in_tx, in_ty){
  dgverify(dggs)

  N <- length(in_tnum)
  out_lon_deg <- rep(0,length(N))
  out_lat_deg <- rep(0,length(N))

  dggridR:::PROJTRI_to_GEO(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_tnum, in_tx, in_ty, out_lon_deg, out_lat_deg)
}

#' @name PROJTRI_to_PROJTRI
#' 
#' @title      Convert from PROJTRI to PROJTRI
#'
#' @description
#'             Uses a discrete global grid system to convert between PROJTRI
#'             and PROJTRI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' PROJTRI_to_PROJTRI(dggs, in_tnum, in_tx, in_ty)
#'
#' @export 
dgPROJTRI_to_PROJTRI <- function(dggs, in_tnum, in_tx, in_ty){
  dgverify(dggs)

  N <- length(in_tnum)
  out_tnum <- rep(0,length(N))
  out_tx <- rep(0,length(N))
  out_ty <- rep(0,length(N))

  dggridR:::PROJTRI_to_PROJTRI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_tnum, in_tx, in_ty, out_tnum, out_tx, out_ty)
}

#' @name PROJTRI_to_Q2DD
#' 
#' @title      Convert from PROJTRI to Q2DD
#'
#' @description
#'             Uses a discrete global grid system to convert between PROJTRI
#'             and Q2DD (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' PROJTRI_to_Q2DD(dggs, in_tnum, in_tx, in_ty)
#'
#' @export 
dgPROJTRI_to_Q2DD <- function(dggs, in_tnum, in_tx, in_ty){
  dgverify(dggs)

  N <- length(in_tnum)
  out_quad <- rep(0,length(N))
  out_qx <- rep(0,length(N))
  out_qy <- rep(0,length(N))

  dggridR:::PROJTRI_to_Q2DD(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_tnum, in_tx, in_ty, out_quad, out_qx, out_qy)
}

#' @name PROJTRI_to_Q2DI
#' 
#' @title      Convert from PROJTRI to Q2DI
#'
#' @description
#'             Uses a discrete global grid system to convert between PROJTRI
#'             and Q2DI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' PROJTRI_to_Q2DI(dggs, in_tnum, in_tx, in_ty)
#'
#' @export 
dgPROJTRI_to_Q2DI <- function(dggs, in_tnum, in_tx, in_ty){
  dgverify(dggs)

  N <- length(in_tnum)
  out_quad <- rep(0,length(N))
  out_i <- rep(0,length(N))
  out_j <- rep(0,length(N))

  dggridR:::PROJTRI_to_Q2DI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_tnum, in_tx, in_ty, out_quad, out_i, out_j)
}

#' @name PROJTRI_to_SEQNUM
#' 
#' @title      Convert from PROJTRI to SEQNUM
#'
#' @description
#'             Uses a discrete global grid system to convert between PROJTRI
#'             and SEQNUM (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' PROJTRI_to_SEQNUM(dggs, in_tnum, in_tx, in_ty)
#'
#' @export 
dgPROJTRI_to_SEQNUM <- function(dggs, in_tnum, in_tx, in_ty){
  dgverify(dggs)

  N <- length(in_tnum)
  out_seqnum <- rep(0,length(N))

  dggridR:::PROJTRI_to_SEQNUM(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_tnum, in_tx, in_ty, out_seqnum)
}

#' @name PROJTRI_to_PLANE
#' 
#' @title      Convert from PROJTRI to PLANE
#'
#' @description
#'             Uses a discrete global grid system to convert between PROJTRI
#'             and PLANE (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' PROJTRI_to_PLANE(dggs, in_tnum, in_tx, in_ty)
#'
#' @export 
dgPROJTRI_to_PLANE <- function(dggs, in_tnum, in_tx, in_ty){
  dgverify(dggs)

  N <- length(in_tnum)
  out_px <- rep(0,length(N))
  out_py <- rep(0,length(N))

  dggridR:::PROJTRI_to_PLANE(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_tnum, in_tx, in_ty, out_px, out_py)
}

#' @name Q2DD_to_GEO
#' 
#' @title      Convert from Q2DD to GEO
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DD
#'             and GEO (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DD_to_GEO(dggs, in_quad, in_qx, in_qy)
#'
#' @export 
dgQ2DD_to_GEO <- function(dggs, in_quad, in_qx, in_qy){
  dgverify(dggs)

  N <- length(in_quad)
  out_lon_deg <- rep(0,length(N))
  out_lat_deg <- rep(0,length(N))

  dggridR:::Q2DD_to_GEO(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_qx, in_qy, out_lon_deg, out_lat_deg)
}

#' @name Q2DD_to_PROJTRI
#' 
#' @title      Convert from Q2DD to PROJTRI
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DD
#'             and PROJTRI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DD_to_PROJTRI(dggs, in_quad, in_qx, in_qy)
#'
#' @export 
dgQ2DD_to_PROJTRI <- function(dggs, in_quad, in_qx, in_qy){
  dgverify(dggs)

  N <- length(in_quad)
  out_tnum <- rep(0,length(N))
  out_tx <- rep(0,length(N))
  out_ty <- rep(0,length(N))

  dggridR:::Q2DD_to_PROJTRI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_qx, in_qy, out_tnum, out_tx, out_ty)
}

#' @name Q2DD_to_Q2DD
#' 
#' @title      Convert from Q2DD to Q2DD
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DD
#'             and Q2DD (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DD_to_Q2DD(dggs, in_quad, in_qx, in_qy)
#'
#' @export 
dgQ2DD_to_Q2DD <- function(dggs, in_quad, in_qx, in_qy){
  dgverify(dggs)

  N <- length(in_quad)
  out_quad <- rep(0,length(N))
  out_qx <- rep(0,length(N))
  out_qy <- rep(0,length(N))

  dggridR:::Q2DD_to_Q2DD(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_qx, in_qy, out_quad, out_qx, out_qy)
}

#' @name Q2DD_to_Q2DI
#' 
#' @title      Convert from Q2DD to Q2DI
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DD
#'             and Q2DI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DD_to_Q2DI(dggs, in_quad, in_qx, in_qy)
#'
#' @export 
dgQ2DD_to_Q2DI <- function(dggs, in_quad, in_qx, in_qy){
  dgverify(dggs)

  N <- length(in_quad)
  out_quad <- rep(0,length(N))
  out_i <- rep(0,length(N))
  out_j <- rep(0,length(N))

  dggridR:::Q2DD_to_Q2DI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_qx, in_qy, out_quad, out_i, out_j)
}

#' @name Q2DD_to_SEQNUM
#' 
#' @title      Convert from Q2DD to SEQNUM
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DD
#'             and SEQNUM (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DD_to_SEQNUM(dggs, in_quad, in_qx, in_qy)
#'
#' @export 
dgQ2DD_to_SEQNUM <- function(dggs, in_quad, in_qx, in_qy){
  dgverify(dggs)

  N <- length(in_quad)
  out_seqnum <- rep(0,length(N))

  dggridR:::Q2DD_to_SEQNUM(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_qx, in_qy, out_seqnum)
}

#' @name Q2DD_to_PLANE
#' 
#' @title      Convert from Q2DD to PLANE
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DD
#'             and PLANE (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DD_to_PLANE(dggs, in_quad, in_qx, in_qy)
#'
#' @export 
dgQ2DD_to_PLANE <- function(dggs, in_quad, in_qx, in_qy){
  dgverify(dggs)

  N <- length(in_quad)
  out_px <- rep(0,length(N))
  out_py <- rep(0,length(N))

  dggridR:::Q2DD_to_PLANE(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_qx, in_qy, out_px, out_py)
}

#' @name Q2DI_to_GEO
#' 
#' @title      Convert from Q2DI to GEO
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DI
#'             and GEO (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DI_to_GEO(dggs, in_quad, in_i, in_j)
#'
#' @export 
dgQ2DI_to_GEO <- function(dggs, in_quad, in_i, in_j){
  dgverify(dggs)

  N <- length(in_quad)
  out_lon_deg <- rep(0,length(N))
  out_lat_deg <- rep(0,length(N))

  dggridR:::Q2DI_to_GEO(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_i, in_j, out_lon_deg, out_lat_deg)
}

#' @name Q2DI_to_PROJTRI
#' 
#' @title      Convert from Q2DI to PROJTRI
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DI
#'             and PROJTRI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DI_to_PROJTRI(dggs, in_quad, in_i, in_j)
#'
#' @export 
dgQ2DI_to_PROJTRI <- function(dggs, in_quad, in_i, in_j){
  dgverify(dggs)

  N <- length(in_quad)
  out_tnum <- rep(0,length(N))
  out_tx <- rep(0,length(N))
  out_ty <- rep(0,length(N))

  dggridR:::Q2DI_to_PROJTRI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_i, in_j, out_tnum, out_tx, out_ty)
}

#' @name Q2DI_to_Q2DD
#' 
#' @title      Convert from Q2DI to Q2DD
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DI
#'             and Q2DD (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DI_to_Q2DD(dggs, in_quad, in_i, in_j)
#'
#' @export 
dgQ2DI_to_Q2DD <- function(dggs, in_quad, in_i, in_j){
  dgverify(dggs)

  N <- length(in_quad)
  out_quad <- rep(0,length(N))
  out_qx <- rep(0,length(N))
  out_qy <- rep(0,length(N))

  dggridR:::Q2DI_to_Q2DD(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_i, in_j, out_quad, out_qx, out_qy)
}

#' @name Q2DI_to_Q2DI
#' 
#' @title      Convert from Q2DI to Q2DI
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DI
#'             and Q2DI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DI_to_Q2DI(dggs, in_quad, in_i, in_j)
#'
#' @export 
dgQ2DI_to_Q2DI <- function(dggs, in_quad, in_i, in_j){
  dgverify(dggs)

  N <- length(in_quad)
  out_quad <- rep(0,length(N))
  out_i <- rep(0,length(N))
  out_j <- rep(0,length(N))

  dggridR:::Q2DI_to_Q2DI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_i, in_j, out_quad, out_i, out_j)
}

#' @name Q2DI_to_SEQNUM
#' 
#' @title      Convert from Q2DI to SEQNUM
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DI
#'             and SEQNUM (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DI_to_SEQNUM(dggs, in_quad, in_i, in_j)
#'
#' @export 
dgQ2DI_to_SEQNUM <- function(dggs, in_quad, in_i, in_j){
  dgverify(dggs)

  N <- length(in_quad)
  out_seqnum <- rep(0,length(N))

  dggridR:::Q2DI_to_SEQNUM(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_i, in_j, out_seqnum)
}

#' @name Q2DI_to_PLANE
#' 
#' @title      Convert from Q2DI to PLANE
#'
#' @description
#'             Uses a discrete global grid system to convert between Q2DI
#'             and PLANE (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' Q2DI_to_PLANE(dggs, in_quad, in_i, in_j)
#'
#' @export 
dgQ2DI_to_PLANE <- function(dggs, in_quad, in_i, in_j){
  dgverify(dggs)

  N <- length(in_quad)
  out_px <- rep(0,length(N))
  out_py <- rep(0,length(N))

  dggridR:::Q2DI_to_PLANE(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_quad, in_i, in_j, out_px, out_py)
}

#' @name SEQNUM_to_GEO
#' 
#' @title      Convert from SEQNUM to GEO
#'
#' @description
#'             Uses a discrete global grid system to convert between SEQNUM
#'             and GEO (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' SEQNUM_to_GEO(dggs, in_seqnum)
#'
#' @export 
dgSEQNUM_to_GEO <- function(dggs, in_seqnum){
  dgverify(dggs)

  N <- length(in_seqnum)
  out_lon_deg <- rep(0,length(N))
  out_lat_deg <- rep(0,length(N))

  dggridR:::SEQNUM_to_GEO(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_seqnum, out_lon_deg, out_lat_deg)
}

#' @name SEQNUM_to_PROJTRI
#' 
#' @title      Convert from SEQNUM to PROJTRI
#'
#' @description
#'             Uses a discrete global grid system to convert between SEQNUM
#'             and PROJTRI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' SEQNUM_to_PROJTRI(dggs, in_seqnum)
#'
#' @export 
dgSEQNUM_to_PROJTRI <- function(dggs, in_seqnum){
  dgverify(dggs)

  N <- length(in_seqnum)
  out_tnum <- rep(0,length(N))
  out_tx <- rep(0,length(N))
  out_ty <- rep(0,length(N))

  dggridR:::SEQNUM_to_PROJTRI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_seqnum, out_tnum, out_tx, out_ty)
}

#' @name SEQNUM_to_Q2DD
#' 
#' @title      Convert from SEQNUM to Q2DD
#'
#' @description
#'             Uses a discrete global grid system to convert between SEQNUM
#'             and Q2DD (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' SEQNUM_to_Q2DD(dggs, in_seqnum)
#'
#' @export 
dgSEQNUM_to_Q2DD <- function(dggs, in_seqnum){
  dgverify(dggs)

  N <- length(in_seqnum)
  out_quad <- rep(0,length(N))
  out_qx <- rep(0,length(N))
  out_qy <- rep(0,length(N))

  dggridR:::SEQNUM_to_Q2DD(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_seqnum, out_quad, out_qx, out_qy)
}

#' @name SEQNUM_to_Q2DI
#' 
#' @title      Convert from SEQNUM to Q2DI
#'
#' @description
#'             Uses a discrete global grid system to convert between SEQNUM
#'             and Q2DI (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' SEQNUM_to_Q2DI(dggs, in_seqnum)
#'
#' @export 
dgSEQNUM_to_Q2DI <- function(dggs, in_seqnum){
  dgverify(dggs)

  N <- length(in_seqnum)
  out_quad <- rep(0,length(N))
  out_i <- rep(0,length(N))
  out_j <- rep(0,length(N))

  dggridR:::SEQNUM_to_Q2DI(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_seqnum, out_quad, out_i, out_j)
}

#' @name SEQNUM_to_SEQNUM
#' 
#' @title      Convert from SEQNUM to SEQNUM
#'
#' @description
#'             Uses a discrete global grid system to convert between SEQNUM
#'             and SEQNUM (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' SEQNUM_to_SEQNUM(dggs, in_seqnum)
#'
#' @export 
dgSEQNUM_to_SEQNUM <- function(dggs, in_seqnum){
  dgverify(dggs)

  N <- length(in_seqnum)
  out_seqnum <- rep(0,length(N))

  dggridR:::SEQNUM_to_SEQNUM(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_seqnum, out_seqnum)
}

#' @name SEQNUM_to_PLANE
#' 
#' @title      Convert from SEQNUM to PLANE
#'
#' @description
#'             Uses a discrete global grid system to convert between SEQNUM
#'             and PLANE (see below for details)
#' 
#'
#'
#' @return          Returns a dggs object which can be passed to other dggridR
#'                  functions
#'
#' @examples 
#' library(dggridR)
#' dggs <- dgconstruct(res=20)
#'
#' SEQNUM_to_PLANE(dggs, in_seqnum)
#'
#' @export 
dgSEQNUM_to_PLANE <- function(dggs, in_seqnum){
  dgverify(dggs)

  N <- length(in_seqnum)
  out_px <- rep(0,length(N))
  out_py <- rep(0,length(N))

  dggridR:::SEQNUM_to_PLANE(dggs[["pole_lon_deg"]], dggs[["pole_lat_deg"]], dggs[["azimuth_deg"]], dggs[["aperture"]], dggs[["res"]], dggs[["topology"]], dggs[["projection"]], N, in_seqnum, out_px, out_py)
}
