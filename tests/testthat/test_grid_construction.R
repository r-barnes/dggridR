
grid_types <- c("ISEA3H", "ISEA4H", "ISEA43H", "ISEA4T", "ISEA4D", "FULLER3H", "FULLER4H", "FULLER43H", "FULLER4T", "FULLER4D")

test_that("All grids construct properly", {
  for(gt in grid_types){
     expect_error( dgconstruct(type=gt, spacing=1000, metric=FALSE, resround='down'), NA)
  }
})