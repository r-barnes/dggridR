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