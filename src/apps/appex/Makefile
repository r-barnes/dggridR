TARGET = appex
SRCS = appex.cpp
OBJS = appex.o
DEPS = appex.depends
DGLIBDIR = ../../lib/dglib
CLIPPERDIR = ../../lib/clipper
PROJ4LIBDIR = ../../lib/proj4lib
SHAPELIBDIR = ../../lib/shapelib-1.3.0
INCDIRS = -I$(DGLIBDIR)/include -I$(CLIPPERDIR) \
   -I$(PROJ4LIBDIR)/include
LDLIBS= $(DGLIBDIR)/lib/dglib.a \
        $(CLIPPERDIR)/libclipper.a \
        $(PROJ4LIBDIR)/lib/proj4lib.a $(SHAPELIBDIR)/libshp.a

.PHONY: clean all

all: $(TARGET)

${TARGET}: $(SRC) $(OBJS) $(DEPS) $(LDLIBS)
	$(CXX) $(CXXFLAGS) -o ${TARGET} ${OBJS} $(LDLIBS) $(INCDIRS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) $(INCDIRS) -o $@

link:
	$(CXX) $(CXXFLAGS) $(INCDIRS) -o ${TARGET} ${OBJS} $(LDLIBS)

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $(INCDIRS) $< > $@

$(LDLIBS):
	cd $(CLIPPERDIR);      ${MAKE}
	cd $(PROJ4LIBDIR)/lib; ${MAKE}
	cd $(DGLIBDIR)/lib;    ${MAKE}

clean:
	rm -rf $(OBJS) $(DEPS) ${TARGET} core ti_files ii_files

-include $(DEPS)

