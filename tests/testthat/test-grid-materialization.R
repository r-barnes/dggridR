library(dggridR)
library(sf)

# ── dgcellstogrid ─────────────────────────────────────────────────────────────

test_that("dgcellstogrid returns an sf object", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  cells <- dgGEO_to_SEQNUM(dggs, c(0, 45, 90), c(0, 30, 60))$seqnum
  grid <- dgcellstogrid(dggs, unique(cells))
  expect_s3_class(grid, "sf")
})

test_that("dgcellstogrid output has a seqnum column", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  grid <- dgcellstogrid(dggs, 1:5)
  expect_true("seqnum" %in% colnames(grid))
})

test_that("dgcellstogrid returns one row per unique cell", {
  dggs  <- dgconstruct(res = 3, show_info = FALSE)
  cells <- c(1, 2, 3, 2, 1)  # duplicates
  grid  <- dgcellstogrid(dggs, cells)
  expect_equal(nrow(grid), 3)
})

test_that("dgcellstogrid seqnum column matches requested cells", {
  dggs  <- dgconstruct(res = 3, show_info = FALSE)
  cells <- c(5, 10, 20)
  grid  <- dgcellstogrid(dggs, cells)
  expect_setequal(grid$seqnum, cells)
})

test_that("dgcellstogrid errors when a cell id exceeds dgmaxcell", {
  dggs    <- dgconstruct(res = 3, show_info = FALSE)
  maxcell <- dgmaxcell(dggs)
  expect_error(dgcellstogrid(dggs, maxcell + 1))
})

test_that("dgcellstogrid with return_sf=FALSE returns a data frame", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  result <- dgcellstogrid(dggs, 1:5, return_sf = FALSE)
  expect_s3_class(result, "data.frame")
  expect_true("seqnum" %in% colnames(result))
})

test_that("dgcellstogrid return_sf=FALSE has x, y, seqnum columns", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  result <- dgcellstogrid(dggs, 1:3, return_sf = FALSE)
  expect_true(all(c("x", "y", "seqnum") %in% colnames(result)))
})

test_that("dgcellstogrid sf output has valid geometry", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  grid <- dgcellstogrid(dggs, 1:20)
  expect_true(all(sf::st_is_valid(grid)))
})

test_that("dgcellstogrid savegrid writes a shapefile and returns its path", {
  dggs    <- dgconstruct(res = 2, show_info = FALSE)
  tmpfile <- tempfile(fileext = ".shp")
  result  <- dgcellstogrid(dggs, 1:5, savegrid = tmpfile)
  on.exit({
    pat <- tools::file_path_sans_ext(tmpfile)
    to_rm <- list.files(dirname(tmpfile),
                        pattern = paste0("^", basename(pat), "\\."),
                        full.names = TRUE)
    file.remove(to_rm)
  })
  expect_equal(result, tmpfile)
  expect_true(file.exists(tmpfile))
})

# ── dgearthgrid ───────────────────────────────────────────────────────────────

test_that("dgearthgrid returns an sf object", {
  dggs <- dgconstruct(res = 2, show_info = FALSE)
  grid <- dgearthgrid(dggs)
  expect_s3_class(grid, "sf")
})

test_that("dgearthgrid cell count matches dgmaxcell at the same resolution", {
  dggs <- dgconstruct(res = 2, show_info = FALSE)
  grid <- dgearthgrid(dggs)
  expect_equal(nrow(grid), dgmaxcell(dggs))
  # ISEA3H res=2 → 92 cells
  expect_equal(nrow(grid), 92)
})

test_that("dgearthgrid output has seqnum column", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  grid <- dgearthgrid(dggs)
  expect_true("seqnum" %in% colnames(grid))
})

test_that("dgearthgrid with return_sf=FALSE returns a data frame", {
  dggs   <- dgconstruct(res = 2, show_info = FALSE)
  result <- dgearthgrid(dggs, return_sf = FALSE)
  expect_s3_class(result, "data.frame")
})

test_that("dgearthgrid savegrid writes a file and returns its path", {
  dggs    <- dgconstruct(res = 1, show_info = FALSE)
  tmpfile <- tempfile(fileext = ".shp")
  result  <- dgearthgrid(dggs, savegrid = tmpfile)
  on.exit({
    pat   <- tools::file_path_sans_ext(tmpfile)
    to_rm <- list.files(dirname(tmpfile),
                        pattern = paste0("^", basename(pat), "\\."),
                        full.names = TRUE)
    file.remove(to_rm)
  })
  expect_equal(result, tmpfile)
  expect_true(file.exists(tmpfile))
})

test_that("dgearthgrid and dgcellstogrid produce the same cell set at res=2", {
  dggs       <- dgconstruct(res = 2, show_info = FALSE)
  all_cells  <- seq_len(dgmaxcell(dggs))
  grid_earth <- dgearthgrid(dggs)
  grid_cells <- dgcellstogrid(dggs, all_cells)
  expect_equal(nrow(grid_earth), nrow(grid_cells))
  expect_setequal(grid_earth$seqnum, grid_cells$seqnum)
})

# ── dgrectgrid ────────────────────────────────────────────────────────────────

test_that("dgrectgrid returns an sf object for a simple bounding box", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  grid <- dgrectgrid(dggs, minlat = 24, minlon = -125, maxlat = 50, maxlon = -66)
  expect_s3_class(grid, "sf")
  expect_gt(nrow(grid), 0)
})

test_that("dgrectgrid cells are within [1, dgmaxcell]", {
  dggs    <- dgconstruct(res = 3, show_info = FALSE)
  maxcell <- dgmaxcell(dggs)
  grid    <- dgrectgrid(dggs, minlat = 0, minlon = 0, maxlat = 30, maxlon = 30)
  expect_true(all(grid$seqnum >= 1 & grid$seqnum <= maxcell))
})

test_that("dgrectgrid with return_sf=FALSE returns a data frame", {
  dggs   <- dgconstruct(res = 3, show_info = FALSE)
  result <- dgrectgrid(dggs, minlat = 0, minlon = 0, maxlat = 10, maxlon = 10,
                       return_sf = FALSE)
  expect_s3_class(result, "data.frame")
})

# ── dgshptogrid ───────────────────────────────────────────────────────────────

test_that("dgshptogrid works with the bundled South Africa shapefile", {
  dggs  <- dgconstruct(spacing = 25, metric = FALSE, resround = "nearest",
                       show_info = FALSE)
  shpf  <- dg_shpfname_south_africa()
  grid  <- dgshptogrid(dggs, shpf)
  expect_s3_class(grid, "sf")
  expect_gt(nrow(grid), 0)
})

test_that("dgshptogrid works when passed an sf object directly", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  poly <- sf::st_sf(
    geometry = sf::st_sfc(
      sf::st_polygon(list(cbind(c(10, 20, 20, 10, 10),
                                c(45, 45, 55, 55, 45)))),
      crs = 4326
    )
  )
  grid <- dgshptogrid(dggs, poly)
  expect_s3_class(grid, "sf")
  expect_gt(nrow(grid), 0)
})

test_that("dgshptogrid errors when file is missing", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  expect_error(dgshptogrid(dggs, "/nonexistent/path.shp"))
})

test_that("dgshptogrid errors when filename doesn't end in .shp", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  expect_error(dgshptogrid(dggs, "/tmp/myfile.geojson"))
})

test_that("dg_shpfname_south_africa returns a file that exists", {
  shpf <- dg_shpfname_south_africa()
  expect_true(file.exists(shpf))
})

# ── FULLER / TRIANGLE / DIAMOND grid materialization ─────────────────────────

test_that("FULLER projection: dgearthgrid produces valid sf at res=2", {
  dggs <- dgconstruct(projection = "FULLER", res = 2, show_info = FALSE)
  grid <- dgearthgrid(dggs)
  expect_s3_class(grid, "sf")
  expect_gt(nrow(grid), 0)
})

test_that("ISEA4H: dgcellstogrid produces valid sf", {
  dggs  <- dgconstruct(projection = "ISEA", aperture = 4, topology = "HEXAGON",
                       res = 3, show_info = FALSE)
  cells <- dgGEO_to_SEQNUM(dggs, c(0, 45), c(0, 30))$seqnum
  grid  <- dgcellstogrid(dggs, unique(cells))
  expect_s3_class(grid, "sf")
})

test_that("ISEA4T (TRIANGLE): dgearthgrid produces valid sf at res=2", {
  dggs <- dgconstruct(aperture = 4, topology = "TRIANGLE", res = 2, show_info = FALSE)
  grid <- dgearthgrid(dggs)
  expect_s3_class(grid, "sf")
  expect_gt(nrow(grid), 0)
})

test_that("ISEA4D (DIAMOND): dgearthgrid produces valid sf at res=2", {
  dggs <- dgconstruct(aperture = 4, topology = "DIAMOND", res = 2, show_info = FALSE)
  grid <- dgearthgrid(dggs)
  expect_s3_class(grid, "sf")
  expect_gt(nrow(grid), 0)
})
