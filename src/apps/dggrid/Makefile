TARGET = dggrid
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.depends)
include ../../MakeIncludes
DGLIBDIR = ../../lib/dglib
GPCLIBDIR = ../../lib/gpclib
PROJ4LIBDIR = ../../lib/proj4lib
SHAPELIBDIR = ../../lib/shapelib-1.3.0
INCDIRS = -I$(DGLIBDIR)/include -I$(GPCLIBDIR)/include \
   -I$(PROJ4LIBDIR)/include -I$(SHAPELIBDIR)
CCFLAGS  = ${INCDIRS} ${ADD_CC_FLAGS} 
LDLIBS= $(DGLIBDIR)/lib/dglib.a \
        $(GPCLIBDIR)/lib/gpclib.a \
        $(PROJ4LIBDIR)/lib/proj4lib.a $(SHAPELIBDIR)/libshp.a

.PHONY: clean all

all: $(TARGET)

${TARGET}: $(SRC) $(OBJS) $(DEPS) $(LDLIBS)
	${CPPCOMP} $(CCFLAGS) $(EXPORT) -o ${TARGET} ${OBJS} $(LDLIBS)

%.o: %.cpp
	$(CPPCOMP) -c $< $(CCFLAGS) -o $@

link:
	${CPPCOMP} $(CCFLAGS) $(EXPORT) -o ${TARGET} ${OBJS} $(LDLIBS)

%.depends: %.cpp
	$(CPPCOMP) -M $(CCFLAGS) $< > $@

$(LDLIBS):
	cd $(GPCLIBDIR)/lib; ${MAKE}
	cd $(PROJ4LIBDIR)/lib; ${MAKE}
	cd $(DGLIBDIR)/lib; ${MAKE}

clean:
	rm -rf $(OBJS) $(DEPS) ${TARGET} core ti_files ii_files

-include $(DEPS)

