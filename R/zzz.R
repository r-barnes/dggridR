Rcpp:::loadModule("dgfuncs", TRUE)

#TODO: Used to display start-up messages
#.onAttach <- function(...) {
#    if (!interactive()) return()
    
    #intro_message <- paste("If you're new to the rEDM package, please check out the tutorial:",
    #                       "> vignette(\"rEDM_tutorial\")", sep = "\n")
    #packageStartupMessage(intro_message)
#}


#TODO
#.Call('rEDM_compute_stats', PACKAGE = 'rEDM', observed, predicted)