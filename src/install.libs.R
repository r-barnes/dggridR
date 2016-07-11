execs <-c("apps/dggrid/dggrid")
if(WINDOWS)
  execs <- c("apps/dggrid/dggrid.exe")

if ( any(file.exists(execs)) ) {
  dest <- file.path(R_PACKAGE_DIR, 'bin') #paste0('bin', R_ARCH)
  dir.create(dest, recursive = TRUE, showWarnings = FALSE)
  file.copy(execs, dest, overwrite = TRUE)
}