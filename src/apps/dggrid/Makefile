TARGET = dggrid
SRCS = dglib.cpp     param.cpp  table.cpp  util.cpp dglib_gridgen.cpp Rwrapper.cpp
OBJS = dglib.o       param.o    table.o    util.o   dglib_gridgen.o   Rwrapper.o
#DEPS = binpres.depends  dggrid.depends   gridgen.depends  quadclip.depends  transform.depends binvals.depends  DgHexSF.depends  param.depends    table.depends     util.depends
DGLIBDIR = ../../lib/dglib
CLIPPERDIR = ../../lib/clipper
PROJ4LIBDIR = ../../lib/proj4lib
SHAPELIBDIR = ../../lib/shapelib-1.3.0
INCDIRS = -I$(DGLIBDIR)/include -I$(CLIPPERDIR) \
   -I$(PROJ4LIBDIR)/include -I$(SHAPELIBDIR) -I. 
#-I$(HOME)/.R/x86_64-pc-linux-gnu-library/3.4/Rcpp/include/ -I/usr/share/R/include
LDLIBS= $(DGLIBDIR)/lib/dglib.a \
        $(CLIPPERDIR)/libclipper.a \
        $(PROJ4LIBDIR)/lib/proj4lib.a $(SHAPELIBDIR)/libshp.a

.PHONY: clean all

all: $(TARGET)

${TARGET}: $(SRC) $(OBJS) $(LDLIBS) #$(DEPS)
	$(CXX) $(CXXFLAGS) $(CLINK_CPPFLAGS) $(CXX11PICFLAGS) -o ${TARGET} ${OBJS} $(LDLIBS) $(INCDIRS)

lib:	../../dggridlib.so

../../dggridlib.so:	$(OBJS)
	$(CXX) $(CXXFLAGS) $(CLINK_CPPFLAGS) -shared -o ../../dggridlib.so $(OBJS)

%.o: %.cpp
	$(CXX) -c $< $(CLINK_CPPFLAGS) $(CXXFLAGS) $(INCDIRS) -o $@

link:
	$(CXX) $(CXXFLAGS) $(CLINK_CPPFLAGS) -o ${TARGET} ${OBJS} $(LDLIBS) $(INCDIRS)

#%.depends: %.cpp
#	$(CXX) -MM $(CXXFLAGS) $(INCDIRS) $< > $@

$(LDLIBS):
	cd $(CLIPPERDIR);      ${MAKE}
	cd $(PROJ4LIBDIR)/lib; ${MAKE}
	cd $(DGLIBDIR)/lib;    ${MAKE}

clean:
	rm -rf $(OBJS) ${TARGET} core ti_files ii_files #$(DEPS)

#-include $(DEPS)

