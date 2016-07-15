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
CCFLAGS  += ${INCDIRS}
LDLIBS= $(DGLIBDIR)/lib/dglib.a \
        $(CLIPPERDIR)/libclipper.a \
        $(PROJ4LIBDIR)/lib/proj4lib.a $(SHAPELIBDIR)/libshp.a

.PHONY: clean all

all: $(TARGET)

${TARGET}: $(SRC) $(OBJS) $(DEPS) $(LDLIBS)
	$(CXX) $(CCFLAGS) $(EXPORT) -o ${TARGET} ${OBJS} $(LDLIBS)

%.o: %.cpp
	$(CXX) -c $< $(CCFLAGS) -o $@

link:
	$(CXX) $(CCFLAGS) $(EXPORT) -o ${TARGET} ${OBJS} $(LDLIBS)

%.depends: %.cpp
	$(CXX) -M $(CCFLAGS) $< > $@

$(LDLIBS):
	cd $(CLIPPERDIR);      ${MAKE}
	cd $(PROJ4LIBDIR)/lib; ${MAKE}
	cd $(DGLIBDIR)/lib;    ${MAKE}

clean:
	rm -rf $(OBJS) $(DEPS) ${TARGET} core ti_files ii_files

-include $(DEPS)

