#' @importFrom Rcpp loadModule

.onLoad <- function(libname, pkgname) {
  # Delayed module loading to avoid segfaults during dyn.load()
  loadModule("dgfuncs",   TRUE)
  loadModule("gridgens",  TRUE)
  loadModule("gridstats", TRUE)
}

#TODO: Used to display start-up messages
#.onAttach <- function(...) {
#    if (!interactive()) return()

    #intro_message <- paste("If you're new to the rEDM package, please check out the tutorial:",
    #                       "> vignette(\"rEDM_tutorial\")", sep = "\n")
    #packageStartupMessage(intro_message)
#}
