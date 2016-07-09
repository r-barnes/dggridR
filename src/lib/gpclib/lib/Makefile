LIB = gpclib.a 
include ../../../MakeIncludes
INCDIRS = -I../include -I../../dglib/include
CCFLAGS = ${INCDIRS} ${ADD_CC_FLAGS} 

.PHONY: clean all

all:
	$(CPPCOMP) -c gpc.cpp -I../include -I../../dglib/include -O3
	ar ruv gpclib.a gpc.o
	ranlib gpclib.a

clean:
	rm -rf gpclib.a gpc.o
