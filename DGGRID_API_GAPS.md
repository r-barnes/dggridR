# DGGRID v9.0b — Features Not Exposed in the R Package

This document identifies DGGRID engine capabilities (from the v9.0b manual and source) that are not
currently accessible through the dggridR R API. The R package wraps DGGRID via an Rcpp bridge
(`copy_to_src/dglib.cpp`) that exposes three operations: whole-earth grid generation, sequence-number
grid generation, and coordinate transformation. Everything else in the DGGRID engine is unreachable
from R.

---

## 1. Operation Modes

DGGRID has six `dggrid_operation` modes. The R package partially covers two of them.

| DGGRID Mode | R Coverage |
|---|---|
| `GENERATE_GRID` | Partial — whole-earth (`dgearthgrid`) and by-SEQNUM (`dgcellstogrid`). Region clipping uses an approximate point-sampling workaround, not native polygon intersection. |
| `GENERATE_GRID_FROM_POINTS` | **None.** Given a set of lon/lat points, generate the cells that contain them and optionally return per-cell point counts. The R user must call `dgGEO_to_SEQNUM()` + `dgcellstogrid()` separately; this loses the native `output_count` field and GDAL input support. |
| `BIN_POINT_VALS` | **None.** Bins floating-point values associated with lon/lat points into DGG cells, outputting per-cell mean (`output_mean`), count (`output_count`), and/or total (`output_total`). Fundamental for gridded aggregation workflows. |
| `BIN_POINT_PRESENCE` | **None.** Presence/absence binning across multiple input classes. Each cell receives a binary vector indicating which classes appear in it, plus optional count and class-count fields. |
| `TRANSFORM_POINTS` | Full — all 30 pairwise conversions between GEO, SEQNUM, Q2DI, Q2DD, PROJTRI, PLANE are exposed via `cwrapper.R`. HIERNDX input/output address type is not exposed (see §4). |
| `OUTPUT_STATS` | Partial — `dggetres()` returns the same cell-count/area/spacing/CLS table, but uses the C++ `GridStat_*` functions directly, not the DGGRID metafile pipeline. No way to query stats for SEQUENCE/MIXED43 grids (see §2). |

---

## 2. DGG Types and Apertures

`dgconstruct()` accepts `projection ∈ {ISEA, FULLER}`, `aperture ∈ {3, 4}`, and
`topology ∈ {HEXAGON, DIAMOND, TRIANGLE}`. `dgverify()` hard-rejects any other combination.
The following DGGRID capabilities are therefore blocked.

### 2.1 Aperture 7 (pure)

`dggs_aperture = 7` with `dggs_aperture_type = PURE`. Preset types: **ISEA7H**, **FULLER7H**.
Aperture-7 hexagonal grids have a different area ratio between resolution levels (~7×) and are the
basis for the IGEO7 and Z7 hierarchical indexing systems (§4). Not constructible in R.

### 2.2 Mixed Aperture (MIXED43)

`dggs_aperture_type = MIXED43` alternates aperture-4 and aperture-3 refinements, controlled by
`dggs_num_aperture_4_res`. Preset types: **ISEA43H**, **FULLER43H**, **SUPERFUND**.
The SUPERFUND preset is the DGG used by the US EPA Superfund program and has a distinct
`output_cell_label_type = SUPERFUND` numbering. Not constructible in R.

### 2.3 Sequence Aperture

`dggs_aperture_type = SEQUENCE` with an arbitrary string of 3s, 4s, and 7s in
`dggs_aperture_sequence`. Preset type: **PLANETRISK** (sequence `"433347777777777777777777"`).
Allows user-defined multi-scale refinement patterns. Not constructible in R.

### 2.4 Summary of Inaccessible Preset Types

IGEO7, ISEA7H, ISEA43H, FULLER3H, FULLER4H, FULLER7H, FULLER43H, FULLER4T, FULLER4D,
SUPERFUND, PLANETRISK. Of the 16 `dggs_type` values in DGGRID, only ISEA3H, ISEA4H, ISEA4T,
ISEA4D, FULLER (custom) variants with aperture 3/4 are reachable.

---

## 3. Grid Orientation

`dgconstruct()` always uses `dggs_orient_specify_type = SPECIFIED` and exposes `pole_lat_deg`,
`pole_lon_deg`, `azimuth_deg`. Two alternative orientation modes are not exposed:

- **`RANDOM`** — randomize the grid orientation from `dggs_orient_rand_seed`. Essential for
  Monte Carlo analyses assessing sensitivity to grid placement.
- **`REGION_CENTER`** — automatically orient the grid so that vertex 0 of the icosahedron is
  aligned with a specified study-region centre (`region_center_lat`, `region_center_lon`).
- **`dggs_num_placements > 1`** — generate multiple distinct random placements of the same grid
  specification in a single call, writing separate output files per placement.

---

## 4. Hierarchical Indexing (HIERNDX Address Type)

DGGRID v9.0b introduces hierarchical cell indices — compact integers that encode a cell's position
in the refinement tree:

| System | Parameter value | Used by |
|---|---|---|
| Z7 | `hier_indexing_system_type = Z7` | IGEO7 preset |
| ZOrder | `ZORDER` | general aperture-4 grids |
| Z3 | `Z3` | aperture-3 grids |

These enable:
- Looking up the **parent cell** at a coarser resolution (`indexing_parent_output_type`).
- Listing all **children cells** at a finer resolution (`indexing_children_output_type`).
- Using `HIERNDX` as both input and output address type in `TRANSFORM_POINTS`.

None of these are exposed in R. The `cwrapper.R` coordinate conversion functions only cover the
five non-hierarchical address types (GEO, SEQNUM, Q2DI, Q2DD, PROJTRI) plus PLANE output.

---

## 5. Cell Relationship Queries

The following per-cell spatial relationships can be output by DGGRID but have no R equivalent:

- **Neighbors** — `neighbor_output_type ∈ {TEXT, GDAL_COLLECTION}`. For each cell, list the
  IDs of all adjacent cells. Triangle grids are not supported by DGGRID for this operation.
- **Hierarchical children** — `indexing_children_output_type`. Requires a `hier_indexing_system_type`.
- **Hierarchical parent** — `indexing_parent_output_type`. Requires a `hier_indexing_system_type`.

---

## 6. Grid Clipping and Region Subsetting

R's `dgrectgrid()` and `dgshptogrid()` clip by densely sampling points inside the bounding box,
converting to SEQNUMs, and deduplicating. This is an approximation. DGGRID's native clipping is
more accurate and offers options not exposed in R:

- **`clip_subset_type = GDAL`** — clip using an arbitrary GDAL vector polygon file, with proper
  polygon-intersection testing (`clip_type = POLY_INTERSECT`), polygon hole support
  (`clip_using_holes`), and geodetic densification of clipping edges (`geodetic_densify`).
- **`clip_subset_type = COARSE_CELLS`** — specify a list of cell addresses at a coarser resolution
  to define the region. Only supported for hexagon grids. Much faster than polygon clipping for
  hierarchically structured regions.
- **`clip_subset_type = ADDRESS_FILES`** — clip by providing an explicit list of cell addresses in
  a file.
- **`clip_cell_densification`** — add extra vertices along clipping-cell boundaries to improve
  accuracy.

---

## 7. Cell Output Options

### 7.1 Densification of Cell Boundaries

`densification` (integer, 0–500) adds extra interpolated vertices along each cell edge, producing
smoother boundaries for large cells. R always uses 0 (no densification). `dgcellstogrid()` takes
no densification argument.

### 7.2 Cell Center Points

`point_output_type` can write cell center coordinates as a separate file. In R, center retrieval
requires a separate `dgSEQNUM_to_GEO()` call. The native operation would return centers and
boundaries together.

### 7.3 Random Points Within Cells

`randpts_num_per_cell` / `randpts_output_type` generates uniformly distributed random sample
points inside each cell. No R equivalent.

### 7.4 Partial Grid Generation

`output_first_seqnum` / `output_last_seqnum` generate only a contiguous range of global sequence
numbers, enabling chunked or parallel grid generation. Not exposed in R.

### 7.5 Cell Label Types

`output_cell_label_type ∈ {GLOBAL_SEQUENCE, ENUMERATION, SUPERFUND}`. R only uses
`GLOBAL_SEQUENCE` (the standard SEQNUM). `ENUMERATION` renumbers output cells from 1 to N;
`SUPERFUND` uses the EPA numbering scheme.

### 7.6 Longitude Wrap Mode

`longitude_wrap_mode ∈ {WRAP, UNWRAP_EAST, UNWRAP_WEST}` controls vertex longitudes for cells
that straddle the antimeridian. R relies on downstream sf/s2 handling; the engine default (WRAP)
is always used.

---

## 8. Earth Datum / Radius

`proj_datum` controls the earth model used for cell-area calculations:

| Value | Description |
|---|---|
| `WGS84_AUTHALIC_SPHERE` | Default — equal-area sphere matching WGS84 (R uses this) |
| `WGS84_MEAN_SPHERE` | Mean-radius WGS84 sphere |
| `CUSTOM_SPHERE` | User-specified radius via `proj_datum_radius` (km) |

R always uses the authalic sphere with no way to change it, which affects cell-area computations
for applications requiring a different earth model.

---

## 9. Input/Output Formats

DGGRID supports rich file-based I/O not available through the R bridge:

**Cell boundary output (`cell_output_type`):**
AIGEN, SHAPEFILE, KML, GEOJSON, GDAL (any GDAL format via `cell_output_gdal_format`),
GDAL_COLLECTION. R can only return an sf data frame or write a Shapefile via `dgsavegrid()`.

**Cell point output (`point_output_type`):**
AIGEN, KML, SHAPEFILE, TEXT, GEOJSON, GDAL, GDAL_COLLECTION. Not accessible from R.

**Text output for binning operations (`output_file_type = TEXT`):**
BIN_POINT_VALS and BIN_POINT_PRESENCE can write a text file with one cell address + value(s) per
line, using any `output_address_type`. Not accessible from R (those operations are absent).

**GDAL point input (`point_input_file_type = GDAL`):**
GENERATE_GRID_FROM_POINTS and BIN_POINT_VALS accept GDAL vector files as input. Not accessible
from R.

---

## 10. Binning-Specific Parameters (BIN_POINT_VALS / BIN_POINT_PRESENCE)

These parameters are meaningful only for the two missing binning operations:

- `bin_coverage ∈ {GLOBAL, PARTIAL}` — memory/speed trade-off for point-set spatial extent.
- `cell_output_control ∈ {OUTPUT_ALL, OUTPUT_OCCUPIED}` — whether to emit all cells or only
  those with at least one input point.
- `input_value_field_name` — name of the value field when reading from GDAL format.
- `output_mean`, `output_count`, `output_total`, `output_presence_vector`,
  `output_num_classes` — per-cell statistics fields written to output.

---

## Potential Implementation Priorities

Roughly ordered by likely user demand:

1. **Aperture 7 and MIXED43 support** in `dgconstruct()` / `dgverify()` — low Rcpp work,
   mainly relax the validation check and pass the new parameters through the existing bridge.
2. **`dgbin_points()`** wrapping BIN_POINT_VALS — the most common analytical workflow
   (aggregate point data to a grid) currently requires two round-trips through R.
3. **`dgneighbors()`** wrapping neighbor_output_type — useful for spatial autocorrelation,
   graph-based smoothing, and adjacency matrix construction.
4. **`dgchildren()` / `dgparent()`** — hierarchical traversal via Z7/ZOrder/Z3 indexing.
5. **Densification parameter** in `dgcellstogrid()` / `dgearthgrid()` — improves boundary
   accuracy for coarse-resolution cells.
6. **REGION_CENTER / RANDOM orientation** in `dgconstruct()`.

---

## Implementation Status (v4.1.0)

All six priorities above were implemented in v4.1.0. The table below records what changed,
how it was done, and what limitations remain.

---

### Priority 1 — Aperture 7 and MIXED43 (§2.1–2.2)

**Status: Implemented.**

| Item | Change |
|---|---|
| Aperture 7 (ISEA7H, FULLER7H) | `dgconstruct(aperture=7)` accepted; `dgverify()` allows `aperture ∈ {3,4,7}` |
| MIXED43 (ISEA43H, FULLER43H) | `dgconstruct(aperture_type='MIXED43', num_aperture_4_res=N)` |
| C++ bridge | `DgParams` extended with `isMixed43` / `numAp4`; threaded through `GridThing`, both generators, `Transformer`, and all 30 generated coordinate-conversion functions |
| `func_gen.py` | `proj_arg` extended with `("bool","isMixed43","FALSE")` and `("int","numAp4","0L")`; `dgproj_args` generation uses null-safe accessors for backward compatibility; output path bug fixed (`../../R/cwrapper.R` → `../R/cwrapper.R`) |

**Remaining gaps:**
- Sequence aperture (`aperture_type = SEQUENCE`, preset PLANETRISK) — not implemented; requires `DgApSeq` string construction and a new R parameter.
- SUPERFUND preset (MIXED43 + special cell-label numbering) — not implemented.
- `dggetres()` / `GridStat_*` functions still use the 7-arg constructor with fixed `isMixed43=false`; MIXED43 grid statistics cannot be queried via `dggetres()`.

---

### Priority 2 — Point Aggregation (§1: GENERATE_GRID_FROM_POINTS, BIN_POINT_VALS)

**Status: Implemented in pure R using existing bridge calls.**

| Function | Wraps | Notes |
|---|---|---|
| `dgpoints_to_cells(dggs, lon, lat, return_count)` | GENERATE_GRID_FROM_POINTS | Calls `dgGEO_to_SEQNUM()` + `dgcellstogrid()`; returns sf grid with optional `count` column |
| `dgbin_points(dggs, lon, lat, values, output_count, output_mean, output_total)` | BIN_POINT_VALS | Pure collapse aggregation; returns a plain data frame (no geometry) |

**Remaining gaps:**
- GDAL vector file input for points — not supported (R-only path).
- `BIN_POINT_PRESENCE` (multi-class presence/absence binning) — not implemented.
- `cell_output_control = OUTPUT_ALL` (emit empty cells) — the R implementation only returns occupied cells.

---

### Priority 3 — Cell Neighbors (§5)

**Status: Implemented.**

New function `dgneighbors(dggs, cells)` → data frame with columns `seqnum` (input cell) and
`neighbor` (adjacent cell ID).

**C++ path:** `GridThing::getNeighbors(seqnum)` → `DgIDGGBase::setAddNeighbors(q2di, vec)` →
`GetNeighbors()` in `Rwrapper.cpp` → `RCPP_MODULE(gridgens)`.

**Remaining gaps:**
- Triangle grids: DGGRID itself does not support neighbor queries for TRIANGLE topology;
  `dgneighbors()` errors with a clear message.
- Diamond grids: `setAddNeighbors` is defined for `DgIDGGBase` and should work for DIAMOND,
  but this has not been tested.

---

### Priority 4 — Hierarchical Children / Parent (§4, §5)

**Status: Implemented via `DgDiscTopoRFS` public API (no HIERNDX address type).**

| Function | Returns |
|---|---|
| `dgchildren(dggs, cells)` | Data frame: `seqnum` (parent at `res`) + `child` (at `res+1`) |
| `dgparent(dggs, cells)` | Data frame: `seqnum` (child at `res`) + `parent` (at `res-1`) |

**C++ path:**
- `dgchildren` → `GetChildren()` constructs a `GridThing` with `res+1` (so `idggs` covers
  `res+2` levels) → `GridThing::getChildrenAt(seqnum, parentRes)` →
  `DgDiscTopoRFS::setAllChildren(parentRes, loc, vec)` →
  `DgHexIDGGS::setAddAllChildren()` (interior + boundary children).
- `dgparent` → `GetParent()` → `GridThing::getParentAt(seqnum, childRes)` →
  `DgDiscTopoRFS::setParents(childRes, loc, vec)` → `DgHexIDGGS::setAddParents()`.

**Design note — boundary children:** `setAddAllChildren` returns interior children (1 per
aperture-3 parent, 3 for aperture-4) plus boundary children shared with neighboring cells.
A boundary child's primary parent (as returned by `setParents`) may be a neighbor, not the
original parent. The R-level test reflects this: `any(prnt$parent == parent_cell)` rather
than `all(...)`.

**Remaining gaps:**
- Only immediate children (one resolution step) are supported; multi-step children require
  iterative calls.
- Only HEXAGON topology is supported (`dgverify`-level check); DIAMOND and TRIANGLE are not.
- The HIERNDX address system (Z7/ZOrder/Z3) is still not exposed in coordinate conversions;
  `dgchildren`/`dgparent` use the SEQNUM-based `DgDiscTopoRFS` public API instead.

---

### Priority 5 — Densification (§7.1)

**Status: Implemented.**

`dgearthgrid(dggs, densify=N)` and `dgcellstogrid(dggs, cells, densify=N)` add `N` extra
vertices along each cell edge (default 0). Threaded through `DgParams::densify` →
`GlobalGridGenerator` / `SeqNumGridGenerator` → `dgg.setVertices(*loc, verts, densify)`.

**Remaining gaps:** None for the core feature. DGGRID supports values 0–500; R passes the
integer through without clamping (out-of-range values will produce a C++ error).

---

### Priority 6 — Random Orientation (§3)

**Status: Implemented in pure R.**

`dgconstruct(orient='RANDOM')` draws a uniformly distributed random orientation:
```r
pole_lat_deg <- asin(runif(1, -1, 1)) * 180 / pi   # uniform on sphere
pole_lon_deg <- runif(1, -180, 180)
azimuth_deg  <- runif(1, 0, 360)
```
Use `set.seed()` before calling for reproducibility.

**Remaining gaps:**
- `REGION_CENTER` orientation (align icosahedron vertex 0 to a study-region centre) is not
  implemented — requires spherical trigonometry currently only inside the DGGRID CLI.
- `dggs_num_placements > 1` (multiple random placements in one call) is not implemented.
