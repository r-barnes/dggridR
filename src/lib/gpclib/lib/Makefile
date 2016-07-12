LIB = gpclib.a 
INCDIRS = -I../include -I../../dglib/include
CCFLAGS = ${INCDIRS} ${ADD_CC_FLAGS} 

.PHONY: clean all

all:
	$(CXX) -c gpc.cpp -I../include -I../../dglib/include -O3
	$(AR) ruv gpclib.a gpc.o
	$(RANLIB) gpclib.a

clean:
	rm -rf gpclib.a gpc.o
