export ADD_CC_FLAGS = -O3
DGGRID_DIR = ./apps/dggrid
APPEX_DIR = ./apps/appex
DG_DIR = ./lib/dglib
PROJ4_DIR = ./lib/proj4lib
CLIPPER_DIR = ./lib/clipper
SHPLIB_HDR_DIR = ./lib/shapelib-1.3.0
LIBRARIES = $(DG_DIR)/lib $(PROJ4_DIR)/lib $(SHPLIB_HDR_DIR) $(CLIPPER_DIR)

export RANLIB = ranlib

.PHONY: all $(DGGRID_DIR) $(LIBRARIES) clean

all: $(DGGRID_DIR) $(APPEX_DIR)

$(DGGRID_DIR) $(APPEX_DIR) $(LIBRARIES):
	$(MAKE) --directory=$@

$(DGGRID_DIR) $(APPEX_DIR): $(LIBRARIES)

clean:
	cd $(DGGRID_DIR);     $(MAKE) clean
	cd $(APPEX_DIR);      $(MAKE) clean
	cd $(CLIPPER_DIR);    $(MAKE) clean
	cd $(PROJ4_DIR)/lib;  $(MAKE) clean
	cd $(DG_DIR)/lib;     $(MAKE) clean
	cd $(SHPLIB_HDR_DIR); $(MAKE) clean
