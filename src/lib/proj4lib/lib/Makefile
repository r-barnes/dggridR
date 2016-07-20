LIB = proj4lib.a 
SRCS = adjlon.cpp  emess.cpp  hypot.cpp  pj_auth.cpp  pj_mlfn.cpp  pj_msfn.cpp  pj_phi2.cpp  pj_qsfn.cpp  pj_tsfn.cpp
OBJS = adjlon.o  emess.o  hypot.o  pj_auth.o  pj_mlfn.o  pj_msfn.o  pj_phi2.o  pj_qsfn.o  pj_tsfn.o
DEPS = adjlon.depends  hypot.depends    pj_mlfn.depends  pj_phi2.depends  pj_tsfn.depends emess.depends   pj_auth.depends  pj_msfn.depends  pj_qsfn.depends
INCDIRS = -I../include

.PHONY: clean all

all: $(LIB)

$(LIB): $(SRC) $(OBJS) $(DEPS)
	$(AR) rcs $(LIB) $(OBJS)

%.o: %.cpp 
	$(CXX) -c $< $(CXXFLAGS) $(INCDIRS)

%.depends: %.cpp
	$(CXX) -MM $(CXXFLAGS) $(INCDIRS) $< > $@

clean:
	rm -rf $(OBJS) $(LIB) $(DEPS) ii_files ti_files make.out

-include $(DEPS)
