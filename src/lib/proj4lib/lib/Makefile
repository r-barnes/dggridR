LIB = proj4lib.a 
SRCS = adjlon.cpp  emess.cpp  hypot.cpp  pj_auth.cpp  pj_mlfn.cpp  pj_msfn.cpp  pj_phi2.cpp  pj_qsfn.cpp  pj_tsfn.cpp
OBJS = adjlon.o  emess.o  hypot.o  pj_auth.o  pj_mlfn.o  pj_msfn.o  pj_phi2.o  pj_qsfn.o  pj_tsfn.o
DEPS = adjlon.depends  hypot.depends    pj_mlfn.depends  pj_phi2.depends  pj_tsfn.depends emess.depends   pj_auth.depends  pj_msfn.depends  pj_qsfn.depends
SHAPELIBDIR = ../../shapelib-1.3.0
INCDIRS = -I../include -I../../dglib/include -I$(SHAPELIBDIR)
CCFLAGS += ${INCDIRS}

.PHONY: clean all

all: $(LIB)

$(LIB): $(SRC) $(OBJS) $(DEPS)
	$(AR) rcs $(LIB) $(OBJS)

%.o: %.cpp 
	$(CXX) -c $< $(CCFLAGS)

%.depends: %.cpp
	$(CXX) -MM $(CCFLAGS) $< > $@

clean:
	rm -rf $(OBJS) $(LIB) $(DEPS) ii_files ti_files make.out

-include $(DEPS)
