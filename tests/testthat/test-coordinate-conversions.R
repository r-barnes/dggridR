library(dggridR)

# ── GEO → SEQNUM ─────────────────────────────────────────────────────────────

test_that("dgGEO_to_SEQNUM returns a list with 'seqnum' element", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  result <- dgGEO_to_SEQNUM(dggs, c(0, 90, -90, 180), c(0, 45, -45, 0))
  expect_type(result, "list")
  expect_named(result, "seqnum")
})

test_that("dgGEO_to_SEQNUM returns a value for each input point", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  lons <- c(0, 45, -90, 135, -180)
  lats <- c(0, 30, -60, 45, 0)
  result <- dgGEO_to_SEQNUM(dggs, lons, lats)
  expect_length(result$seqnum, length(lons))
})

test_that("dgGEO_to_SEQNUM returns positive cell IDs", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  result <- dgGEO_to_SEQNUM(dggs, c(0, 45, -90), c(0, 30, -60))
  expect_true(all(result$seqnum > 0))
})

test_that("dgGEO_to_SEQNUM returns known values for ISEA3H res=2 (regression)", {
  # Stable regression anchors verified against v4.0.0
  dggs <- dgconstruct(res = 2, show_info = FALSE)
  expect_equal(dgGEO_to_SEQNUM(dggs, 0,   0)$seqnum,  24)
  expect_equal(dgGEO_to_SEQNUM(dggs, 45, 30)$seqnum,  34)
  expect_equal(dgGEO_to_SEQNUM(dggs, -90, -60)$seqnum, 63)
})

test_that("dgGEO_to_SEQNUM argument order is (lon, lat), not (lat, lon) — Issue #16", {
  # Before dgtransform was removed, the deprecated function took (lat, lon).
  # The current API takes (lon, lat). Swapping should give a different cell
  # for non-zero, non-symmetric coordinates.
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  cell_lon_lat <- dgGEO_to_SEQNUM(dggs, 10, 50)$seqnum  # correct order
  cell_swapped <- dgGEO_to_SEQNUM(dggs, 50, 10)$seqnum  # swapped
  expect_false(isTRUE(cell_lon_lat == cell_swapped))
})

test_that("identical points yield identical cell IDs", {
  dggs <- dgconstruct(res = 7, show_info = FALSE)
  lon <- 13.4050; lat <- 52.5200  # Berlin
  expect_equal(dgGEO_to_SEQNUM(dggs, lon, lat)$seqnum,
               dgGEO_to_SEQNUM(dggs, lon, lat)$seqnum)
})

test_that("all cell IDs are within [1, dgmaxcell]", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  maxcell <- dgmaxcell(dggs)
  lons <- seq(-180, 175, by = 5)
  lats <- seq(-85, 85, by = 5)
  grid <- expand.grid(lon = lons, lat = lats)
  cells <- dgGEO_to_SEQNUM(dggs, grid$lon, grid$lat)$seqnum
  expect_true(all(cells >= 1 & cells <= maxcell))
})

test_that("dgGEO_to_SEQNUM handles boundary coordinates (poles, anti-meridian)", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  cells <- dgGEO_to_SEQNUM(dggs, c(0, 0, 180, -180), c(90, -90, 0, 0))$seqnum
  expect_length(cells, 4)
  expect_true(all(!is.na(cells)))
  expect_true(all(cells > 0))
})

# ── SEQNUM → GEO ─────────────────────────────────────────────────────────────

test_that("dgSEQNUM_to_GEO returns lon_deg and lat_deg", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  result <- dgSEQNUM_to_GEO(dggs, c(1, 2, 100, 500))
  expect_type(result, "list")
  expect_named(result, c("lon_deg", "lat_deg"))
})

test_that("dgSEQNUM_to_GEO returns valid geographic coordinates", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  result <- dgSEQNUM_to_GEO(dggs, c(1, 2, 100, 500, 1000))
  expect_true(all(result$lon_deg >= -180 & result$lon_deg <= 180))
  expect_true(all(result$lat_deg >= -90  & result$lat_deg <= 90))
})

# ── Round-trip GEO → SEQNUM → GEO ────────────────────────────────────────────

test_that("round-trip GEO → SEQNUM → GEO recovers the same cell", {
  # The centroid of a cell must hash back to the same cell.
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  lons <- c(0, 45, -90, 135, -180)
  lats <- c(0, 30, -60, 45, 0)
  seqnums  <- dgGEO_to_SEQNUM(dggs, lons, lats)$seqnum
  centers  <- dgSEQNUM_to_GEO(dggs, seqnums)
  seqnums2 <- dgGEO_to_SEQNUM(dggs, centers$lon_deg, centers$lat_deg)$seqnum
  expect_equal(seqnums, seqnums2)
})

test_that("Apple Silicon regression: dgGEO_to_SEQNUM on dgquakes data — Issue #54", {
  # On M1 Macs this previously produced 'FATAL ERROR: coordinate out of range'.
  # Fixed in v3.1.0.
  data(dgquakes, package = "dggridR")
  dggs <- dgconstruct(spacing = 1000, metric = FALSE, resround = "down", show_info = FALSE)
  result <- dgGEO_to_SEQNUM(dggs, dgquakes$lon, dgquakes$lat)
  expect_length(result$seqnum, nrow(dgquakes))
  expect_true(all(!is.na(result$seqnum)))
  expect_true(all(result$seqnum > 0))
  expect_true(all(result$seqnum <= dgmaxcell(dggs)))
})

# ── Q2DI ─────────────────────────────────────────────────────────────────────

test_that("dgGEO_to_Q2DI returns quad, i, j components", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  result <- dgGEO_to_Q2DI(dggs, c(0, 45), c(0, 30))
  expect_type(result, "list")
  expect_true(all(c("quad", "i", "j") %in% names(result)))
  expect_length(result$quad, 2)
})

test_that("Q2DI round-trip: GEO → Q2DI → SEQNUM matches GEO → SEQNUM", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  lons <- c(10, 50, -100, -30)
  lats <- c(30, -20, 60, -45)
  seqnum_direct   <- dgGEO_to_SEQNUM(dggs, lons, lats)$seqnum
  q2di            <- dgGEO_to_Q2DI(dggs, lons, lats)
  seqnum_via_q2di <- dgQ2DI_to_SEQNUM(dggs, q2di$quad, q2di$i, q2di$j)$seqnum
  expect_equal(seqnum_direct, seqnum_via_q2di)
})

test_that("dgQ2DI_to_GEO round-trips back to the original cell", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  lons <- c(10, 50)
  lats <- c(30, -20)
  q2di <- dgGEO_to_Q2DI(dggs, lons, lats)
  geo  <- dgQ2DI_to_GEO(dggs, q2di$quad, q2di$i, q2di$j)
  seqnum_orig <- dgGEO_to_SEQNUM(dggs, lons, lats)$seqnum
  seqnum_rt   <- dgGEO_to_SEQNUM(dggs, geo$lon_deg, geo$lat_deg)$seqnum
  expect_equal(seqnum_orig, seqnum_rt)
})

# ── Q2DD ─────────────────────────────────────────────────────────────────────

test_that("Q2DD round-trip: GEO → Q2DD → SEQNUM matches GEO → SEQNUM", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  lons <- c(10, 50, -100)
  lats <- c(30, -20, 60)
  seqnum_direct   <- dgGEO_to_SEQNUM(dggs, lons, lats)$seqnum
  q2dd            <- dgGEO_to_Q2DD(dggs, lons, lats)
  seqnum_via_q2dd <- dgQ2DD_to_SEQNUM(dggs, q2dd$quad, q2dd$qx, q2dd$qy)$seqnum
  expect_equal(seqnum_direct, seqnum_via_q2dd)
})

# ── SEQNUM → Q2DI / Q2DD ─────────────────────────────────────────────────────

test_that("dgSEQNUM_to_Q2DI round-trips back to the same SEQNUM", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  seqnums <- c(1, 50, 200, 1000)
  q2di    <- dgSEQNUM_to_Q2DI(dggs, seqnums)
  back    <- dgQ2DI_to_SEQNUM(dggs, q2di$quad, q2di$i, q2di$j)$seqnum
  expect_equal(seqnums, back)
})

# ── Different projections, topologies, apertures ──────────────────────────────

test_that("FULLER projection coordinate conversion returns valid cells", {
  dggs <- dgconstruct(projection = "FULLER", res = 4, show_info = FALSE)
  cells <- dgGEO_to_SEQNUM(dggs, c(0, 45, -90), c(0, 30, -60))$seqnum
  expect_length(cells, 3)
  expect_true(all(cells > 0))
})

test_that("ISEA4H (aperture=4, HEXAGON) coordinate conversion works — Issue #3", {
  # Older versions failed to construct ISEA4H grids at all.
  dggs <- dgconstruct(projection = "ISEA", aperture = 4, topology = "HEXAGON",
                      res = 3, show_info = FALSE)
  cells <- dgGEO_to_SEQNUM(dggs, c(0, 90, 180), c(0, 45, -45))$seqnum
  expect_length(cells, 3)
  expect_true(all(cells > 0))
})

test_that("ISEA4T (aperture=4, TRIANGLE) coordinate conversion works — Issue #3", {
  dggs <- dgconstruct(aperture = 4, topology = "TRIANGLE", res = 3, show_info = FALSE)
  cells <- dgGEO_to_SEQNUM(dggs, c(0, 90), c(0, 45))$seqnum
  expect_true(all(cells > 0))
})

test_that("ISEA4D (aperture=4, DIAMOND) coordinate conversion works — Issue #3", {
  dggs <- dgconstruct(aperture = 4, topology = "DIAMOND", res = 3, show_info = FALSE)
  cells <- dgGEO_to_SEQNUM(dggs, c(0, 90), c(0, 45))$seqnum
  expect_true(all(cells > 0))
})

test_that("higher resolution produces more distinct cells for the same set of points", {
  lons <- seq(-170, 170, by = 20)
  lats <- seq(-80, 80, by = 20)
  pts  <- expand.grid(lon = lons, lat = lats)
  dggs5 <- dgconstruct(res = 5, show_info = FALSE)
  dggs9 <- dgconstruct(res = 9, show_info = FALSE)
  n_cells5 <- length(unique(dgGEO_to_SEQNUM(dggs5, pts$lon, pts$lat)$seqnum))
  n_cells9 <- length(unique(dgGEO_to_SEQNUM(dggs9, pts$lon, pts$lat)$seqnum))
  expect_gt(n_cells9, n_cells5)
})

test_that("different pole orientations produce different cell assignments", {
  dggs_def    <- dgconstruct(res = 5, show_info = FALSE)
  dggs_custom <- dgconstruct(res = 5, pole_lat_deg = 0, pole_lon_deg = 0, show_info = FALSE)
  cell_def    <- dgGEO_to_SEQNUM(dggs_def,    10, 50)$seqnum
  cell_custom <- dgGEO_to_SEQNUM(dggs_custom, 10, 50)$seqnum
  expect_false(isTRUE(cell_def == cell_custom))
})

# ── GEO → GEO (identity) ─────────────────────────────────────────────────────

test_that("dgGEO_to_GEO returns approximately the same coordinates", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  lons <- c(0, 45, -90)
  lats <- c(0, 30, -60)
  result <- dgGEO_to_GEO(dggs, lons, lats)
  expect_true(all(abs(result$lon_deg - lons) < 1e-5))
  expect_true(all(abs(result$lat_deg - lats) < 1e-5))
})

# ── GEO → PLANE ──────────────────────────────────────────────────────────────

test_that("dgGEO_to_PLANE returns px and py components", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  result <- dgGEO_to_PLANE(dggs, c(0, 45), c(0, 30))
  expect_type(result, "list")
  expect_true(all(c("px", "py") %in% names(result)))
  expect_length(result$px, 2)
})
