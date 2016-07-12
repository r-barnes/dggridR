export ADD_CC_FLAGS = -O3
DGGRID_DIR = ./apps/dggrid
APPEX_DIR = ./apps/appex
DG_DIR = ./lib/dglib
GPC_DIR = ./lib/gpclib
PROJ4_DIR = ./lib/proj4lib
SHPLIB_HDR_DIR = ./lib/shapelib-1.3.0
LIBRARIES = $(DG_DIR)/lib $(GPC_DIR)/lib $(PROJ4_DIR)/lib $(SHPLIB_HDR_DIR)

export

.PHONY: all $(DGGRID_DIR) $(LIBRARIES)

all: $(DGGRID_DIR) $(APPEX_DIR)

$(DGGRID_DIR) $(APPEX_DIR) $(LIBRARIES):
	$(MAKE) --directory=$@

$(DGGRID_DIR) $(APPEX_DIR): $(LIBRARIES)

clean:
	cd $(DGGRID_DIR); $(MAKE) clean
	cd $(APPEX_DIR); $(MAKE) clean
	cd $(GPC_DIR)/lib; $(MAKE) clean
	cd $(PROJ4_DIR)/lib; $(MAKE) clean
	cd $(DG_DIR)/lib; $(MAKE) clean
	cd $(SHPLIB_HDR_DIR); $(MAKE) clean
