LIB = dglib.a 
SRCS = $(wildcard *.cpp) 
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.depends)
include ../../../MakeIncludes
SHAPELIBDIR = ../../shapelib-1.3.0
PROJ4LIBDIR = ../../proj4lib
INCDIRS = -I../include -I$(PROJ4LIBDIR)/include -I$(SHAPELIBDIR)
CCFLAGS = ${INCDIRS} ${ADD_CC_FLAGS} 

.PHONY: clean all

all: $(LIB)

$(LIB): $(SRC) $(OBJS) $(DEPS)
	ar ruv $(LIB) $(OBJS)
	ranlib $(LIB)

%.o: %.cpp 
	$(CPPCOMP) -c $< $(CCFLAGS)

%.depends: %.cpp
	$(CPPCOMP) -M $(CCFLAGS) $< > $@

clean:
	rm -rf $(OBJS) $(LIB) $(DEPS) ii_files ti_files make.out

-include $(DEPS)
