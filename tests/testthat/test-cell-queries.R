library(dggridR)

# ── dgneighbors ───────────────────────────────────────────────────────────────

test_that("dgneighbors returns a data frame with seqnum and neighbor columns", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  nbrs <- dgneighbors(dggs, c(1, 2, 3))
  expect_s3_class(nbrs, "data.frame")
  expect_true(all(c("seqnum", "neighbor") %in% names(nbrs)))
})

test_that("dgneighbors returns 6 neighbors per hexagon cell", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  nbrs <- dgneighbors(dggs, 100)
  # hexagonal cells have exactly 6 neighbors
  expect_equal(nrow(nbrs), 6)
  expect_equal(unique(nbrs$seqnum), 100)
})

test_that("dgneighbors neighbors are distinct cell IDs", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  nbrs <- dgneighbors(dggs, 100)
  expect_equal(length(unique(nbrs$neighbor)), nrow(nbrs))
})

test_that("dgneighbors errors for TRIANGLE grids", {
  dggs <- dgconstruct(topology = "TRIANGLE", aperture = 4, res = 3, show_info = FALSE)
  expect_error(dgneighbors(dggs, 1))
})

# ── dgchildren ────────────────────────────────────────────────────────────────

test_that("dgchildren returns a data frame with seqnum and child columns", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  chld <- dgchildren(dggs, c(1, 2))
  expect_s3_class(chld, "data.frame")
  expect_true(all(c("seqnum", "child") %in% names(chld)))
})

test_that("dgchildren returns child cells at res+1", {
  dggs_parent <- dgconstruct(res = 3, show_info = FALSE)
  dggs_child  <- dgconstruct(res = 4, show_info = FALSE)
  chld <- dgchildren(dggs_parent, 100)
  # all child seqnums should be valid at the child resolution
  max_child_cell <- dgmaxcell(dggs_child)
  expect_true(all(chld$child >= 1 & chld$child <= max_child_cell))
})

test_that("dgchildren errors for non-HEXAGON grids", {
  dggs <- dgconstruct(topology = "DIAMOND", aperture = 4, res = 3, show_info = FALSE)
  expect_error(dgchildren(dggs, 1))
})

# ── dgparent ──────────────────────────────────────────────────────────────────

test_that("dgparent returns a data frame with seqnum and parent columns", {
  dggs <- dgconstruct(res = 4, show_info = FALSE)
  prnt <- dgparent(dggs, c(1, 2))
  expect_s3_class(prnt, "data.frame")
  expect_true(all(c("seqnum", "parent") %in% names(prnt)))
})

test_that("dgparent result has one row per input cell", {
  dggs <- dgconstruct(res = 4, show_info = FALSE)
  prnt <- dgparent(dggs, c(10, 20, 30))
  expect_equal(nrow(prnt), 3)
  expect_equal(prnt$seqnum, c(10, 20, 30))
})

test_that("dgparent parent IDs are valid at res-1", {
  dggs_child  <- dgconstruct(res = 4, show_info = FALSE)
  dggs_parent <- dgconstruct(res = 3, show_info = FALSE)
  prnt <- dgparent(dggs_child, c(10, 20, 30))
  max_parent_cell <- dgmaxcell(dggs_parent)
  expect_true(all(prnt$parent >= 1 & prnt$parent <= max_parent_cell))
})

test_that("dgchildren and dgparent are consistent (interior child maps back)", {
  dggs_parent <- dgconstruct(res = 3, show_info = FALSE)
  dggs_child  <- dgconstruct(res = 4, show_info = FALSE)
  parent_cell <- 100
  # get children of parent_cell at res+1
  chld <- dgchildren(dggs_parent, parent_cell)
  # get parent of each child — interior child should map back to parent_cell;
  # boundary children may belong to a neighbor (shared boundary)
  prnt <- dgparent(dggs_child, chld$child)
  expect_true(any(prnt$parent == parent_cell))
  # parents should be valid cell IDs
  max_parent_cell <- dgmaxcell(dggs_parent)
  expect_true(all(prnt$parent >= 1 & prnt$parent <= max_parent_cell))
})

test_that("dgparent errors for non-HEXAGON grids", {
  dggs <- dgconstruct(topology = "DIAMOND", aperture = 4, res = 3, show_info = FALSE)
  expect_error(dgparent(dggs, 1))
})

test_that("dgparent errors at res=0 (no parent exists)", {
  dggs <- dgconstruct(res = 0, show_info = FALSE)
  expect_error(dgparent(dggs, 1))
})

# ── densification ─────────────────────────────────────────────────────────────

test_that("densify parameter produces more vertices per cell", {
  dggs  <- dgconstruct(res = 2, show_info = FALSE)
  g0    <- dgearthgrid(dggs, densify = 0)
  g4    <- dgearthgrid(dggs, densify = 4)
  # More vertices means more rows in the long-format coordinate frame
  g0_df <- dgearthgrid(dggs, densify = 0, return_sf = FALSE)
  g4_df <- dgearthgrid(dggs, densify = 4, return_sf = FALSE)
  expect_gt(nrow(g4_df), nrow(g0_df))
})

# ── RANDOM orientation ────────────────────────────────────────────────────────

test_that("RANDOM orientation produces different pole each time", {
  set.seed(1)
  d1 <- dgconstruct(res = 3, orient = "RANDOM", show_info = FALSE)
  set.seed(2)
  d2 <- dgconstruct(res = 3, orient = "RANDOM", show_info = FALSE)
  expect_false(d1[["pole_lat_deg"]] == d2[["pole_lat_deg"]] &&
               d1[["pole_lon_deg"]] == d2[["pole_lon_deg"]])
})

test_that("RANDOM orientation is reproducible with set.seed", {
  set.seed(42); d1 <- dgconstruct(res = 3, orient = "RANDOM", show_info = FALSE)
  set.seed(42); d2 <- dgconstruct(res = 3, orient = "RANDOM", show_info = FALSE)
  expect_equal(d1[["pole_lat_deg"]], d2[["pole_lat_deg"]])
  expect_equal(d1[["pole_lon_deg"]], d2[["pole_lon_deg"]])
})

test_that("RANDOM orientation grid materializes without error", {
  set.seed(7)
  dggs <- dgconstruct(res = 3, orient = "RANDOM", show_info = FALSE)
  g    <- dgearthgrid(dggs)
  expect_s3_class(g, "sf")
  expect_gt(nrow(g), 0)
})
