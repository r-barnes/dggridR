library(dggridR)
library(sf)

# ── dgquakes dataset ──────────────────────────────────────────────────────────

test_that("dgquakes dataset loads and has expected structure", {
  data(dgquakes, package = "dggridR")
  expect_s3_class(dgquakes, "data.frame")
  expect_true(all(c("lat", "lon", "mag") %in% colnames(dgquakes)))
  expect_gt(nrow(dgquakes), 0)
})

test_that("dgquakes coordinates are in valid geographic range", {
  data(dgquakes, package = "dggridR")
  expect_true(all(dgquakes$lon >= -180 & dgquakes$lon <= 180))
  expect_true(all(dgquakes$lat >= -90  & dgquakes$lat <= 90))
})

# ── README example: earthquake binning ────────────────────────────────────────

test_that("README earthquake binning example runs end-to-end", {
  data(dgquakes, package = "dggridR")
  dggs <- dgconstruct(spacing = 1000, metric = FALSE, resround = "down", show_info = FALSE)
  dgquakes$cell <- dgGEO_to_SEQNUM(dggs, dgquakes$lon, dgquakes$lat)$seqnum
  quakecounts <- collapse::fcount(dgquakes, cell)
  expect_s3_class(quakecounts, "data.frame")
  expect_true(all(c("cell", "N") %in% colnames(quakecounts)))
  expect_gt(nrow(quakecounts), 0)
  # More data points than bins (aggregation happened)
  expect_lte(nrow(quakecounts), nrow(dgquakes))
})

test_that("earthquake cell IDs are within valid range", {
  data(dgquakes, package = "dggridR")
  dggs    <- dgconstruct(spacing = 1000, metric = FALSE, resround = "down", show_info = FALSE)
  cells   <- dgGEO_to_SEQNUM(dggs, dgquakes$lon, dgquakes$lat)$seqnum
  maxcell <- dgmaxcell(dggs)
  expect_true(all(cells >= 1 & cells <= maxcell))
})

test_that("dgcellstogrid on earthquake cells returns one polygon per unique cell", {
  data(dgquakes, package = "dggridR")
  dggs <- dgconstruct(spacing = 1000, metric = FALSE, resround = "down", show_info = FALSE)
  dgquakes$cell <- dgGEO_to_SEQNUM(dggs, dgquakes$lon, dgquakes$lat)$seqnum
  unique_cells  <- unique(dgquakes$cell)
  grid <- dgcellstogrid(dggs, unique_cells)
  expect_equal(nrow(grid), length(unique_cells))
  expect_setequal(grid$seqnum, unique_cells)
})

test_that("cell count from fcount matches grid row count — Issue #32 consistency", {
  # Regression: cell assignment should be consistent with grid generation.
  data(dgquakes, package = "dggridR")
  dggs <- dgconstruct(res = 4, show_info = FALSE)
  cells <- dgGEO_to_SEQNUM(dggs, dgquakes$lon, dgquakes$lat)$seqnum
  n_unique <- length(unique(cells))
  grid     <- dgcellstogrid(dggs, unique(cells))
  expect_equal(nrow(grid), n_unique)
})

# ── Resolution workflow ────────────────────────────────────────────────────────

test_that("full resolution-selection workflow: spacing → dgsetres → dgearthgrid", {
  dggs <- dgconstruct(res = 20, show_info = FALSE)
  res  <- dg_closest_res_to_spacing(dggs, spacing = 1000, round = "down",
                                    metric = FALSE, show_info = FALSE)
  dggs <- dgsetres(dggs, res)
  grid <- dgearthgrid(dggs)
  expect_s3_class(grid, "sf")
  expect_gt(nrow(grid), 0)
})

# ── Equal-area property ───────────────────────────────────────────────────────

test_that("area_km values from dggetres are approximately equal at each resolution", {
  # All cells at a given resolution should have the same nominal area.
  # The getres table reports the mean; verify it matches README values for ISEA3H.
  dggs <- dgconstruct(res = 9, show_info = FALSE)
  ret  <- dggetres(dggs)
  # res=9 row: area_km ~ 2591.4 (from README table)
  row9 <- ret[ret$res == 9, ]
  expect_equal(round(row9$area_km, 0), 2591)
})

test_that("cell area monotonically decreases with finer resolution", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  ret  <- dggetres(dggs)
  expect_true(all(diff(ret$area_km) < 0))
})

test_that("cell count monotonically increases with finer resolution", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  ret  <- dggetres(dggs)
  expect_true(all(diff(ret$cells) > 0))
})

# ── Vignette workflow: South Africa grid ──────────────────────────────────────

test_that("vignette South Africa shapefile grid workflow completes", {
  dggs <- dgconstruct(spacing = 25, metric = FALSE, resround = "nearest",
                      show_info = FALSE)
  grid <- dgshptogrid(dggs, dg_shpfname_south_africa())
  expect_s3_class(grid, "sf")
  expect_gt(nrow(grid), 0)
  # South Africa grid at ~25 miles spacing should have many cells
  expect_gt(nrow(grid), 50)
})

# ── Custom grid orientation ───────────────────────────────────────────────────

test_that("azimuth rotation produces a different grid", {
  dggs_0   <- dgconstruct(res = 4, azimuth_deg = 0,   show_info = FALSE)
  dggs_90  <- dgconstruct(res = 4, azimuth_deg = 90,  show_info = FALSE)
  cell_0   <- dgGEO_to_SEQNUM(dggs_0,  13, 52)$seqnum
  cell_90  <- dgGEO_to_SEQNUM(dggs_90, 13, 52)$seqnum
  expect_false(isTRUE(cell_0 == cell_90))
})

test_that("non-default pole coordinates produce a different grid", {
  dggs_def <- dgconstruct(res = 5, show_info = FALSE)
  dggs_eq  <- dgconstruct(res = 5, pole_lat_deg = 0, pole_lon_deg = 90,
                           show_info = FALSE)
  cell_def <- dgGEO_to_SEQNUM(dggs_def, 10, 50)$seqnum
  cell_eq  <- dgGEO_to_SEQNUM(dggs_eq,  10, 50)$seqnum
  expect_false(isTRUE(cell_def == cell_eq))
})

# ── High-resolution point → cell workflow ─────────────────────────────────────

test_that("high-resolution binning assigns distinct cells for close-by points", {
  # At res=12 cells are ~96 km² — two cities ~100 km apart should land in
  # different cells.
  dggs <- dgconstruct(res = 12, show_info = FALSE)
  # Berlin and Hamburg (Germany), ~250 km apart
  berlin  <- dgGEO_to_SEQNUM(dggs, 13.405, 52.520)$seqnum
  hamburg <- dgGEO_to_SEQNUM(dggs, 10.000, 53.550)$seqnum
  expect_false(isTRUE(berlin == hamburg))
})

test_that("nearby points within a single low-res cell share the same cell ID", {
  # At res=2 cells are ~5.7 million km² — opposite corners of a small country
  # should share a cell.
  dggs <- dgconstruct(res = 2, show_info = FALSE)
  # Two points in central Europe
  cell1 <- dgGEO_to_SEQNUM(dggs, 13.0, 52.0)$seqnum
  cell2 <- dgGEO_to_SEQNUM(dggs, 14.0, 52.5)$seqnum
  expect_equal(cell1, cell2)
})
