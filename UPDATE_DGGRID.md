Here is a detailed plan markdown based on the repository's architecture and documented workflows:

---

```markdown
# Plan: Update dggridR C++ Engine to Latest DGGRID

## Background

`dggridR` bundles a vendored copy of the upstream [DGGRID C++ library](https://github.com/sahrk/DGGRID)
(by Kevin Sahr) via a Git submodule at `submodules/DGGRID`, which is a fork maintained at
`https://github.com/r-barnes/DGGRID`. The `src/` directory is **generated** — it is assembled
by `update_from_upstream.sh` and is safe to nuke and rebuild. The bridge between DGGRID and R
lives in `copy_to_src/` and is **not** auto-generated (except for the `cgen_*.h` files).

Current bundled version: **DGGRID ~7.0** (based on file headers in `src/`).  
Target: latest release on `https://github.com/sahrk/DGGRID`.

---

## Phase 1: Sync the Upstream Fork

> Work happens in the `r-barnes/DGGRID` fork, not in `dggridR` itself.

### 1.1 Fetch latest upstream into the fork

```bash
# In your local clone of r-barnes/DGGRID
git remote add upstream https://github.com/sahrk/DGGRID
git fetch upstream
git checkout main          # or master, depending on branch name
git merge upstream/main    # resolve any conflicts
git push origin main
```

### 1.2 Review the upstream changelog

Before merging, read the DGGRID release notes / git log for:

- **New source files** added to `src/lib/dglib/` or `src/lib/proj4lib/` or `src/lib/shapelib/`
- **Removed or renamed files** (these will break `update_from_upstream.sh`)
- **API changes** to classes used by `copy_to_src/dglib.cpp`:
  - `DgIDGGSBase::makeRF(...)` — constructor signature
  - `DgGeoSphRF::makeRF(...)` — constructor signature
  - `DgGeoSphDegRF::makeRF(...)`
  - `DgIDGGBase` methods: `bndRF()`, `projTriRF()`, `q2ddRF()`, `planeRF()`, `setVertices()`
  - `DgGridStats` methods: `nCells()`, `cellAreaKM()`, `cellDistKM()`, `cls()`
  - `stringToGridTopology()` — free function used in `dglib.cpp`
  - `dgg::topo::D6` — enum value used in `dglib.cpp`
- **New compile-time requirements** (C++ standard, new system dependencies)
- **New constants** that may conflict with R's headers (like the existing `M_2PI` issue)

---

## Phase 2: Update the Submodule

```bash
# In the dggridR repo root
cd submodules/DGGRID
git fetch origin
git checkout main          # point to updated r-barnes/DGGRID
git pull
cd ../..
git add submodules/DGGRID
git commit -m "Update DGGRID submodule to latest upstream"
```

---

## Phase 3: Update `update_from_upstream.sh`

File: `update_from_upstream.sh`

Check whether the upstream directory layout has changed. The current script copies from:

| Source path in submodule | Destination |
|---|---|
| `src/apps/dggrid/{dggrid,util}.h` | `src/` |
| `src/apps/dggrid/util.cpp` | `src/` |
| `src/lib/dglib/include/dglib/*` | `src/` |
| `src/lib/dglib/lib/*` | `src/` |
| `src/lib/proj4lib/include/*` | `src/` |
| `src/lib/proj4lib/lib/*` | `src/` |
| `src/lib/shapelib/include/shapelib/*` | `src/` |
| `src/lib/shapelib/lib/*` | `src/` |

**Actions:**
- If DGGRID reorganized its directory structure, update the `cp -f` lines accordingly.
- If new subdirectories were added (e.g., a new library), add corresponding `cp -f` lines.
- If the `#include "dglib/DgBase.h"` fix (line 18, marked with a TODO) was merged upstream,
  remove that `sed` workaround.
- Check whether any new global constants conflict with R headers and add `sed` removals as needed
  (following the pattern of the existing `M_2PI` removal on line 34).

---

## Phase 4: Rebuild `src/` and Verify Compilation

```bash
# Nuke and rebuild src/
bash update_from_upstream.sh

# Verify the flat src/ directory looks correct
ls src/ | wc -l   # sanity check file count

# Test compilation via CMake (standalone, no R needed)
mkdir -p build && cd build
cmake ..
make -j$(nproc)
cd ..
```

**If CMake fails:**
- New `.cpp` files from DGGRID will need to be added to `CMakeLists.txt` (the `add_executable` block).
- Missing symbols usually mean a new `.cpp` was not copied by `update_from_upstream.sh`.

---

## Phase 5: Adapt the Bridge Layer (`copy_to_src/`)

File: `copy_to_src/dglib.cpp` and `copy_to_src/dglib.h`

This is the most likely source of breakage. For each API change identified in Phase 1.2:

### 5.1 Constructor signature changes

The `DgIDGGSBase::makeRF(...)` call in `dglib.cpp` (lines 29–44 and 68–83) passes a fixed
set of arguments. If the upstream added or reordered parameters, update both call sites.

```cpp
// Current call (copy_to_src/dglib.cpp ~line 29)
idggs(DgIDGGSBase::makeRF(
  net0, *geoRF,
  DgGeoCoord(dp.pole_lon_deg, dp.pole_lat_deg, false),
  dp.azimuth_deg, dp.aperture, dp.res + 1,
  stringToGridTopology(dp.topology),
  dgg::topo::D6, "DDG", dp.projection,
  false, 0, false, false
)),
```

Update both the `DgParams`-based constructor and the explicit-argument constructor to match
the new signature.

### 5.2 New coordinate systems or output types

If DGGRID added new coordinate reference frames (e.g., a new address type beyond GEO, PROJTRI,
Q2DD, Q2DI, SEQNUM, PLANE), consider whether to expose them:

- Add new entries to `in_arg` / `out_arg` in `copy_to_src/func_gen.py`
- Add corresponding `inXXX` / `outXXX` methods to `dglib::Transformer` in `dglib.h` / `dglib.cpp`
- Re-run `func_gen.py` to regenerate `cgen_head.h`, `cgen_body.h`, `cgen_funcs.h`, and `R/cwrapper.R`

```bash
cd copy_to_src
python3 func_gen.py
cd ..
```

### 5.3 Removed or renamed methods

If methods like `bndRF()`, `projTriRF()`, `q2ddRF()`, `planeRF()`, or `setVertices()` were
renamed or removed, update the corresponding `Transformer::inXXX` / `outXXX` methods in
`copy_to_src/dglib.cpp`.

---

## Phase 6: Test with R

```bash
# Full R package check
devtools::document()
devtools::check()
```

Run the existing test suite:

```bash
devtools::test()
```

Pay particular attention to:
- `dgGEO_to_SEQNUM` / `dgSEQNUM_to_GEO` round-trip accuracy
- `dgearthgrid()` and `dgcellstogrid()` polygon generation
- Apple Silicon numeric precision (regression from v3.1.0 fix, `#63`)

---

## Phase 7: Update Package Metadata

### 7.1 `DESCRIPTION`

- Bump `Version:` (e.g., `3.2.0`)
- Update `Date:`
- If new system dependencies were introduced by the new DGGRID, add `SystemRequirements:`

### 7.2 `NEWS`

Add a new entry at the top:

```
dggridR vX.Y.Z
===============
* Updated bundled DGGRID C++ engine to vX.Y (upstream sahrk/DGGRID)
* [List any API additions, bug fixes inherited from upstream]
```

### 7.3 `README.md`

Update the version reference on line 124:

```markdown
The code in the 'src' directory is based off of
[DGGRIDvX.Y](https://discreteglobal.wpengine.com/) by Kevin Sahr.
```

### 7.4 `vignettes/dggridR.Rmd`

Update the version reference near line 499:

```markdown
Large portions of the above documentation are drawn from the DGGRID version X.Y
User Documentation...
```

---

## Phase 8: CRAN Submission Checks

```bash
# Set stack size (may be needed for deep recursion in DGGRID)
ulimit -s 32768

# Local check
devtools::check()

# Cross-platform check via R-hub
devtools::check_rhub()

# Windows builder
# echo put dggridR_X.Y.Z.tar.gz | ftp win-builder.r-project.org

# Release
devtools::release()
```

---

## Key Files Reference

| File | Role | Action Needed |
|---|---|---|
| `submodules/DGGRID` | Git submodule pointing to r-barnes/DGGRID fork | Update commit pointer |
| `update_from_upstream.sh` | Assembles `src/` from submodule | Update paths if DGGRID reorganized |
| `src/` | Generated — nuke-able | Rebuilt by script |
| `copy_to_src/dglib.h` | C++ API surface for Rcpp | Update if DGGRID API changed |
| `copy_to_src/dglib.cpp` | Implementation of bridge layer | Update constructor calls |
| `copy_to_src/Rwrapper.cpp` | Rcpp module definitions | Rarely needs changes |
| `copy_to_src/func_gen.py` | Generates coordinate conversion boilerplate | Update if new coord types added |
| `copy_to_src/cgen_*.h` | Generated by func_gen.py | Re-run func_gen.py |
| `R/cwrapper.R` | Generated by func_gen.py | Re-run func_gen.py |
| `CMakeLists.txt` | Standalone CMake build for testing | Add new `.cpp` files |
| `DESCRIPTION` | Package metadata | Bump version + date |
| `NEWS` | Changelog | Add entry |
| `README.md` | User docs | Update DGGRID version string |

---

## Risk Areas

1. **`DgIDGGSBase::makeRF` signature** — this is the most fragile coupling point between
   `dglib.cpp` and DGGRID. Any parameter addition/reordering breaks compilation silently
   if types are compatible.

2. **New files not copied** — if `update_from_upstream.sh` misses new `.cpp` files, the
   linker will fail with undefined symbol errors. Cross-reference `CMakeLists.txt` in the
   submodule against the copy script.

3. **CRAN `.hpp` rename** — the `rename` command used in `update_from_upstream.sh` (line 30)
   is GNU `rename` (Perl-based). On macOS it is `rename` from `util-linux` which has different
   syntax. Ensure CI runs on Linux or adjust the script.

4. **`M_2PI` and similar constants** — check the new DGGRID for any other constants defined
   without namespaces that collide with R's math headers.

5. **Apple Silicon precision** — the fix for issue `#63` lives in `copy_to_src/dglib.cpp`.
   Verify it still applies cleanly after the merge.
```