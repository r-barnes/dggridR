Structure
=============================================

dggridR consists of several parts connected together with a discomforting amount of duct tape and superglue. These parts are assembled into a directory called `src` which is designed to be nuke-able.

These parts are:
* `submodules/DGGRID` - The upstream engine for manipulating discrete global grids
* `copy_to_src/dglib.{cpp,h}` - A simplifying layer of C++
* `copy_to_src/Rwrapper.{h,cpp}` - Code that wraps `dglib.{cpp,h}` for use with R
* `copy_to_src/cgen_{head,body,funcs}.h` - Full-factorial coordinate conversion code generated with `copy_to_src/func_gen.py`
* `copy_to_src/func_gen.py` - Autogenerates full-factorial coordinate conversion code
* `./update_from_upstream.h` - Copies the appropriate files from `submodules/DGGRID` into `src/`

Why do we have to do this copying? Because R's build model is pretty terrible: it assumes all the code lives in the same directory. If you want a reasonable, subdirectory-based organization of your code things become very painful.

Making changes to the C++
==============================================

Updating the C++ code in dggridR is a multi-step process.

1. Pull code from https://github.com/sahrk/DGGRID into https://github.com/r-barnes/DGGRID
2. Update submodules/DGGRID from https://github.com/r-barnes/DGGRID
3. Run ./update_from_upstream.sh to update dggridR/src  from submodules/DGGRID

Making fixes to the C++ code requires running this process in reverse. The following workflow is suggested.

1. Make any changes you need to submodules/DGGRID. Use submodules/DGGRID/CMakeLists.txt and the usual CMake workflow to ensure these compile.
2. Use ./update_from_upstream.sh to import your changes into dggridR and test that they work.
3. Iterate on the above until everything is working.
4. If you need changes to CRAN soonish, collect changes to submodules/DGGRID into a branch on r-barnes/DGGRID that submodules/DGGRID can point to.
5. Break changes in submodules/DGGRID apart into small branches on r-barnes/DGGRID which can be made into pull requests to https://github.com/sahrk/DGGRID.
6. Get Kevin Sahr's to approve and merge the PR's into https://github.com/sahrk/DGGRID.
7. Update https://github.com/r-barnes/DGGRID
8. Point submodules/DGGRID to master on https://github.com/r-barnes/DGGRID
9. Run ./update_from_upstream.sh to update dggridR/src from submodules/DGGRID