TARGET = appex
SRCS = appex.cpp
OBJS = appex.o
#DEPS = appex.depends
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

${TARGET}: $(SRC) $(OBJS) $(LDLIBS) #$(DEPS)
	$(CXX) $(CXX1XFLAGS) -o ${TARGET} ${OBJS} $(LDLIBS) $(INCDIRS)

%.o: %.cpp
	$(CXX1X) -c $< $(CXX1XFLAGS) $(INCDIRS) -o $@

link:
	$(CXX1X) $(CXX1XFLAGS) $(INCDIRS) -o ${TARGET} ${OBJS} $(LDLIBS)

#%.depends: %.cpp
#	$(CXX) -M $(CXXFLAGS) $(INCDIRS) $< > $@

$(LDLIBS):
	cd $(CLIPPERDIR);      ${MAKE}
	cd $(PROJ4LIBDIR)/lib; ${MAKE}
	cd $(DGLIBDIR)/lib;    ${MAKE}

clean:
	rm -rf $(OBJS) ${TARGET} core ti_files ii_files #$(DEPS)

#-include $(DEPS)

