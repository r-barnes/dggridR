LIB = gpclib.a 
SRCS = $(wildcard *.cpp) 
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.depends)
include ../../../MakeIncludes
INCDIRS = -I../include -I../../dglib/include
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
