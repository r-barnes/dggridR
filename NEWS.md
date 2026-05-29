# dggridR 4.1.1-4.1.7

* Continuing fix for the Debian/Linux `dyn.load()` segfault (`address (nil), cause 'memory not mapped'`) seen only on the CRAN pretest box (R-devel built with `gcc-16`, libstdc++-16, `-D_FORTIFY_SOURCE=3`).  The R Extensions manual (§5.6) is explicit that using C++ I/O in shared libraries loaded by R is best avoided: "Examples have been seen where merely loading a DLL that contained calls to C++ I/O upset R's own C I/O."  Earlier releases eliminated `#include <Rcpp.h>` from `DgBase.h` (used by ~100 translation units), disabled `RCPP_USE_GLOBAL_ROSTREAM`, and routed `dgcout` / `dgcerr` through `Rprintf` / `REprintf` instead of `Rcpp::Rcout` / `Rcpp::Rcerr`.  That still left file-scope `static Rcpp::Rostream<...> Rcout/Rcerr` objects in every TU that included `<Rcpp.h>` (Rcpp declares them inside `Rcpp/iostream/Rstreambuf.h`).  Their `std::ostream(streambuf*)` constructors run inside the dynamic loader before R has finished initialising the .so, and on this specific toolchain they trip a NULL dereference during libstdc++ locale init.  4.1.7 removes them from the .so entirely:
    * New `src/dggridR_overrides/Rcpp/iostream/Rstreambuf.h` shadows the upstream Rcpp header via `PKG_CPPFLAGS = -IdggridR_overrides` in `src/Makevars`.  The override keeps the `Rcpp::Rstreambuf` / `Rcpp::Rostream` template declarations that `Rcpp/routines.h` needs to compile its inline `Rcpp_cout_get()` / `Rcpp_cerr_get()` callable lookups, but drops the two file-scope `static Rostream<...>` instances.  `nm` on the resulting `dggridR.so` shows zero `Rostream`-related vtable, RTTI, or static instance symbols (down from a full set in 4.1.6).
    * Every header that included `<iostream>` purely to obtain `std::ostream` / `std::endl` for `operator<<` declarations now includes `<ostream>` instead.  This removes libstdc++'s `std::__ioinit` static initialiser from those translation units, dropping yet another category of load-time iostream construction.
    * Retained mitigations from 4.1.6: `DgBase.cpp` is `<Rcpp.h>`-free (custom `Rprintf`-backed `std::streambuf` for `dgcout` / `dgcerr`); `RcppExports.cpp` is `<Rcpp.h>`-free; `copy_to_src/dglib.cpp` is `<Rcpp.h>`-free.  Only `Rwrapper.cpp` and `Rwrapper.h` still include `<Rcpp.h>` (because they legitimately need `Rcpp::List`, `Rcpp::NumericVector`, and `RCPP_MODULE`), and with the Rstreambuf override above they no longer drag any `Rcpp::Rostream` static instances into the .so.
* Persisted the new patches in `update_from_upstream.sh` so they survive future syncs from the DGGRID submodule.

  After these changes `nm dggridR.so` shows the minimum static-initialiser footprint compatible with Rcpp modules: a handful of `__GLOBAL__sub_I_*` functions from the DGGRID engine's existing `const std::string` / `const DgDVec2D` / `const DgIcosaTri[]` etc. file-scope initialisers (which have been in the upstream code for years and pose no load-time risk), and **zero** `Rcpp::Rostream` / `Rcpp::Rstreambuf` symbols anywhere in the .so.

* Fixed install warnings from signed/unsigned comparisons in `DgDiscRFSGrids.h` and `DgZ3StringRF.cpp`.
* Guarded MSVC-only `#pragma comment(lib, ...)` in `safileio.c` to avoid GCC unknown pragma warnings.
* Fixed integer truncation warnings by replacing C `abs()` with `std::abs()` for `long long` arguments in DGGRID headers.
* Added durable patch steps in `update_from_upstream.sh` so these fixes survive source regeneration.

# dggridR 4.1.0

* `dgconstruct()` gains `aperture = 7` for ISEA7H / FULLER7H hexagonal grids.
* `dgconstruct()` gains `aperture_type = "MIXED43"` with `num_aperture_4_res` for ISEA43H / FULLER43H mixed-aperture grids.
* `dgconstruct()` gains `orient = "RANDOM"` for a uniformly random icosahedral orientation (reproducible via `set.seed()`).
* New `dgneighbors(dggs, cells)` returns the adjacent cell IDs for each input cell (hexagonal grids only).
* New `dgchildren(dggs, cells)` returns the child cell IDs at the next finer resolution.
* New `dgparent(dggs, cells)` returns the parent cell ID at the next coarser resolution.
* New `dgpoints_to_cells(dggs, lon, lat)` maps lon/lat points to grid cells and returns an sf grid with optional per-cell counts.
* New `dgbin_points(dggs, lon, lat, values)` aggregates point data into grid cells, returning per-cell count, mean, and/or total as a data frame.
* `dgearthgrid()` and `dgcellstogrid()` gain a `densify` parameter to insert extra vertices along cell edges for smoother boundaries.
* `dgearthgrid()` and `dgcellstogrid()` now materialize sf polygon grids through a native C implementation using R's C API, preserving DGGRID ring order and avoiding the previous s2/collapse polygon construction path. Illustrative conversion timings for full-earth grids improved from 3.854s to 0.150s at res 9 (~26x). This is achieved also via the *wk* package.
* pkgdown site added at <https://sebkrantz.github.io/dggridR/>.

# dggridR 4.0.0

* Updated bundled DGGRID C++ engine to v9.0b (upstream [SebKrantz/DGGRID](https://github.com/SebKrantz/DGGRID)).
* Re-architected `DgIDGGSBase::makeRF()` call for new v9.0b parameter order (`isApSeq`, `DgApSeq`, `hierNdxSysType`).
* `update_from_upstream.sh` rewritten for macOS compatibility (Perl-based text transforms) and updated to track SebKrantz/DGGRID.
* Fixed numeric precision issue on Apple Silicon introduced by DGGRID v9.0b.
* New DeepWiki AI documentation at <https://deepwiki.com/r-barnes/dggridR>.

# dggridR 3.1.1

* Bumped R dependency to 4.1.0 to fix CRAN issues.

# dggridR 3.1.0

* Fixed a numeric precision issue on Apple Silicon (#63).
* Removed dplyr, rlang, and sp as dependencies; added collapse and s2 (zero additional transitive dependencies), yielding significant performance gains especially in grid materialization.
* `dgshptogrid()` now accepts an sf object in memory in addition to a shapefile path.
* `dgcellstogrid()`, `dgearthgrid()`, `dgrectgrid()`, and `dgshptogrid()` gain `return_sf = TRUE`; setting `return_sf = FALSE` returns a memory-efficient long data frame of cell-vertex coordinates.

# dggridR 3.0.0

* Fixes memory leaks in DGGRID.
* Switches from rgdal to the sf package.
* `dgtransform()` removed (previously deprecated); use `dgGEO_to_SEQNUM()` instead.

# dggridR 2.0.3

* Fixed bugs in `dgshptogrid()` and `dgrectgrid()` where `cellsize` was not respected.
* More informative warning messages.

# dggridR 2.0.0

* Complete restructuring of the internal C++ engine for improved speed and robustness across operating systems.
* Restructured engine directory layout to reduce compilation complexity.
* `dg_shpfname_south_africa()` converted from an object to a function.

# dggridR 1.0.0

* `dgconstruct()` gains `azimuth`, `pole_lat`, and `pole_lon` parameters.
* Added pentagon location map to the vignette.

# dggridR 0.1.0 and earlier

* Initial development releases: grid generation, shapefile support (`dgshptogrid()`), debugging utilities, FLOSS licensing via Clipper, and various compiler-compatibility and CRAN-compliance fixes.
