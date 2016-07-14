LIB = gpclib.a 
INCDIRS = -I../include -I../../dglib/include
CCFLAGS = ${INCDIRS} ${ADD_CC_FLAGS} 

.PHONY: clean all

all:
	$(CXX) -c gpc.cpp -I../include -I../../dglib/include -O3
	$(AR) rcs gpclib.a gpc.o

clean:
	rm -rf gpclib.a gpc.o
