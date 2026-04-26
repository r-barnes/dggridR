library(dggridR)

# ── Aperture 7 ────────────────────────────────────────────────────────────────

test_that("aperture 7 HEXAGON grid constructs and materializes", {
  dggs <- dgconstruct(projection = "ISEA", topology = "HEXAGON", aperture = 7,
                      res = 3, show_info = FALSE)
  expect_equal(dggs[["aperture"]], 7)
  g <- dgearthgrid(dggs)
  expect_s3_class(g, "sf")
  expect_gt(nrow(g), 0)
})

test_that("aperture 7 requires HEXAGON topology", {
  expect_error(dgconstruct(aperture = 7, topology = "DIAMOND",  res = 3, show_info = FALSE))
  expect_error(dgconstruct(aperture = 7, topology = "TRIANGLE", res = 3, show_info = FALSE))
})

test_that("dgverify rejects unsupported aperture values", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  dggs[["aperture"]] <- 5
  expect_error(dgverify(dggs))
})

# ── MIXED43 ───────────────────────────────────────────────────────────────────

test_that("MIXED43 grid constructs with isMixed43=TRUE in dggs", {
  dggs <- dgconstruct(aperture_type = "MIXED43", num_aperture_4_res = 2,
                      res = 5, show_info = FALSE)
  expect_true(isTRUE(dggs[["isMixed43"]]))
  expect_equal(dggs[["numAp4"]], 2L)
})

test_that("MIXED43 grid materializes as sf", {
  dggs <- dgconstruct(aperture_type = "MIXED43", num_aperture_4_res = 0,
                      res = 3, show_info = FALSE)
  g <- dgearthgrid(dggs)
  expect_s3_class(g, "sf")
  expect_gt(nrow(g), 0)
})

test_that("MIXED43 requires HEXAGON topology", {
  expect_error(
    dgconstruct(aperture_type = "MIXED43", topology = "DIAMOND",
                res = 3, show_info = FALSE)
  )
})

test_that("dgverify rejects MIXED43 with non-HEXAGON topology", {
  dggs <- dgconstruct(res = 3, show_info = FALSE)
  dggs[["aperture_type"]] <- "MIXED43"
  dggs[["topology"]]      <- "TRIANGLE"
  expect_error(dgverify(dggs))
})
