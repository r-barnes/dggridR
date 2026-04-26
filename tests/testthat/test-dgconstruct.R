library(dggridR)

# ── dgconstruct ───────────────────────────────────────────────────────────────

test_that("dgconstruct returns a named list with expected slots", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  expect_type(dggs, "list")
  expect_true(all(c("pole_lon_deg", "pole_lat_deg", "azimuth_deg", "aperture",
                    "aperture_type", "num_aperture_4_res", "isMixed43", "numAp4",
                    "res", "topology", "projection", "precision") %in% names(dggs)))
})

test_that("dgconstruct errors when no sizing param is given", {
  expect_error(dgconstruct(show_info = FALSE))
})

test_that("dgconstruct errors when multiple sizing params are given", {
  expect_error(dgconstruct(res = 5, area = 100, show_info = FALSE))
  expect_error(dgconstruct(res = 5, spacing = 100, show_info = FALSE))
  expect_error(dgconstruct(area = 100, cls = 50, show_info = FALSE))
})

test_that("dgconstruct respects the res parameter", {
  dggs <- dgconstruct(res = 7, show_info = FALSE)
  expect_equal(dggs[["res"]], 7)
})

test_that("dgconstruct with area param selects a resolution", {
  dggs <- dgconstruct(area = 1000, show_info = FALSE)
  expect_true(is.numeric(dggs[["res"]]))
  expect_gte(dggs[["res"]], 0)
  expect_lte(dggs[["res"]], 30)
})

test_that("dgconstruct with spacing param selects a resolution", {
  dggs <- dgconstruct(spacing = 1000, metric = FALSE, resround = "down", show_info = FALSE)
  expect_true(is.numeric(dggs[["res"]]))
  expect_gte(dggs[["res"]], 0)
})

test_that("dgconstruct with cls param selects a resolution", {
  dggs <- dgconstruct(cls = 100, show_info = FALSE)
  expect_true(is.numeric(dggs[["res"]]))
  expect_gte(dggs[["res"]], 0)
})

test_that("dgconstruct stores projection, topology, and aperture", {
  dggs <- dgconstruct(projection = "FULLER", aperture = 4, topology = "HEXAGON",
                      res = 3, show_info = FALSE)
  expect_equal(dggs[["projection"]], "FULLER")
  expect_equal(dggs[["aperture"]], 4)
  expect_equal(dggs[["topology"]], "HEXAGON")
})

test_that("dgconstruct stores custom pole parameters", {
  dggs <- dgconstruct(res = 3, pole_lat_deg = 0, pole_lon_deg = 0,
                      azimuth_deg = 45, show_info = FALSE)
  expect_equal(dggs[["pole_lat_deg"]], 0)
  expect_equal(dggs[["pole_lon_deg"]], 0)
  expect_equal(dggs[["azimuth_deg"]], 45)
})

# ── dgsetres ──────────────────────────────────────────────────────────────────

test_that("dgsetres changes only the resolution", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  dggs2 <- dgsetres(dggs, 8)
  expect_equal(dggs2[["res"]], 8)
  expect_equal(dggs2[["projection"]], dggs[["projection"]])
  expect_equal(dggs2[["topology"]],   dggs[["topology"]])
})

# ── dgverify ──────────────────────────────────────────────────────────────────

test_that("dgverify errors on resolution out of range", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  dggs[["res"]] <- 31
  expect_error(dgverify(dggs))
  dggs[["res"]] <- -1
  expect_error(dgverify(dggs))
})

test_that("dgverify errors on unrecognised projection", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  dggs[["projection"]] <- "BOGUS"
  expect_error(dgverify(dggs))
})

test_that("dgverify errors on unrecognised topology", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  dggs[["topology"]] <- "PENTAGON"
  expect_error(dgverify(dggs))
})

test_that("dgverify errors on unrecognised aperture", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  dggs[["aperture"]] <- 5
  expect_error(dgverify(dggs))
})

test_that("dgverify errors on out-of-range pole_lat_deg", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  dggs[["pole_lat_deg"]] <- 91
  expect_error(dgverify(dggs))
})

test_that("dgverify errors on out-of-range pole_lon_deg", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  dggs[["pole_lon_deg"]] <- 181
  expect_error(dgverify(dggs))
})

test_that("dgverify errors on non-integer resolution", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  dggs[["res"]] <- 5.5
  expect_error(dgverify(dggs))
})

# ── dggetres ──────────────────────────────────────────────────────────────────

test_that("dggetres returns a data frame with expected columns", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  ret <- dggetres(dggs)
  expect_s3_class(ret, "data.frame")
  expect_true(all(c("res", "cells", "area_km", "spacing_km", "cls_km") %in% colnames(ret)))
})

test_that("dggetres has 31 rows (resolutions 0..30)", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  ret <- dggetres(dggs)
  expect_equal(nrow(ret), 31)
})

test_that("dggetres area_km is strictly decreasing with resolution", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  ret <- dggetres(dggs)
  expect_true(all(diff(ret$area_km) < 0))
})

test_that("dggetres cells is strictly increasing with resolution", {
  dggs <- dgconstruct(res = 5, show_info = FALSE)
  ret <- dggetres(dggs)
  expect_true(all(diff(ret$cells) > 0))
})

# ── dgmaxcell ─────────────────────────────────────────────────────────────────

test_that("dgmaxcell returns known cell counts for ISEA3H", {
  # Regression values from the README table
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  expect_equal(dgmaxcell(dggs), 32)

  dggs <- dgconstruct(res = 2, show_info = FALSE)
  expect_equal(dgmaxcell(dggs), 92)

  dggs <- dgconstruct(res = 5, show_info = FALSE)
  expect_equal(dgmaxcell(dggs), 2432)
})

test_that("dgmaxcell accepts an explicit res override", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  expect_equal(dgmaxcell(dggs, res = 2), 92)
  expect_equal(dgmaxcell(dggs, res = 5), 2432)
})

# ── dg_closest_res_* ──────────────────────────────────────────────────────────

test_that("dg_closest_res_to_area returns a valid resolution", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  res <- dg_closest_res_to_area(dggs, area = 1000, show_info = FALSE)
  expect_gte(res, 0)
  expect_lte(res, 30)
})

test_that("dg_closest_res_to_spacing returns a valid resolution", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  res <- dg_closest_res_to_spacing(dggs, spacing = 500, show_info = FALSE)
  expect_gte(res, 0)
  expect_lte(res, 30)
})

test_that("dg_closest_res_to_cls returns a valid resolution", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  res <- dg_closest_res_to_cls(dggs, cls = 200, show_info = FALSE)
  expect_gte(res, 0)
  expect_lte(res, 30)
})

test_that("dg_closest_res resround='up' selects coarser resolution than 'down'", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  res_down <- dg_closest_res_to_area(dggs, area = 1000, round = "down",    show_info = FALSE)
  res_up   <- dg_closest_res_to_area(dggs, area = 1000, round = "up",      show_info = FALSE)
  # 'up' means cells larger than target → lower resolution number
  expect_lte(res_up, res_down)
})

test_that("dg_closest_res errors on unrecognised round direction", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  expect_error(dg_closest_res(dggs, "area_km", 1000, round = "sideways", show_info = FALSE))
})

test_that("dg_closest_res errors on unrecognised column", {
  dggs <- dgconstruct(res = 1, show_info = FALSE)
  expect_error(dg_closest_res(dggs, "bogus_col", 1000, show_info = FALSE))
})

test_that("dginfo prints output without error", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  expect_output(dginfo(dggs))
})
