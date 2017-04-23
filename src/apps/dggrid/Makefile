TARGET = dggrid
SRCS = binpres.cpp  binvals.cpp  dggrid.cpp  DgHexSF.cpp  gridgen.cpp  param.cpp  quadclip.cpp  table.cpp  transform.cpp  util.cpp
OBJS = binpres.o  binvals.o  dggrid.o  DgHexSF.o  gridgen.o  param.o  quadclip.o  table.o  transform.o  util.o
#DEPS = binpres.depends  dggrid.depends   gridgen.depends  quadclip.depends  transform.depends binvals.depends  DgHexSF.depends  param.depends    table.depends     util.depends
DGLIBDIR = ../../lib/dglib
CLIPPERDIR = ../../lib/clipper
PROJ4LIBDIR = ../../lib/proj4lib
SHAPELIBDIR = ../../lib/shapelib-1.3.0
INCDIRS = -I$(DGLIBDIR)/include -I$(CLIPPERDIR) \
   -I$(PROJ4LIBDIR)/include -I$(SHAPELIBDIR) -I.
LDLIBS= $(DGLIBDIR)/lib/dglib.a \
        $(CLIPPERDIR)/libclipper.a \
        $(PROJ4LIBDIR)/lib/proj4lib.a $(SHAPELIBDIR)/libshp.a

.PHONY: clean all

all: $(TARGET)

${TARGET}: $(SRC) $(OBJS) $(LDLIBS) #$(DEPS)
	$(CXX) $(CXXFLAGS) -o ${TARGET} ${OBJS} $(LDLIBS) $(INCDIRS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) $(INCDIRS) -o $@

link:
	$(CXX) $(CXXFLAGS) -o ${TARGET} ${OBJS} $(LDLIBS) $(INCDIRS)

#%.depends: %.cpp
#	$(CXX) -MM $(CXXFLAGS) $(INCDIRS) $< > $@

$(LDLIBS):
	cd $(CLIPPERDIR);      ${MAKE}
	cd $(PROJ4LIBDIR)/lib; ${MAKE}
	cd $(DGLIBDIR)/lib;    ${MAKE}

clean:
	rm -rf $(OBJS) ${TARGET} core ti_files ii_files #$(DEPS)

#-include $(DEPS)

