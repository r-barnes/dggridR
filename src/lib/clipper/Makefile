LIBOBJ	=	clipper.o dbfopen.o safileio.o shptree.o 
SHPBIN	=	shpcreate shpadd shpdump shprewind dbfcreate dbfadd dbfdump shptreedump 

default:	all

all: clipper.o libclipper.a

libclipper.a: clipper.o
	$(AR) rcs libclipper.a clipper.o 

clipper.o: clipper.cpp clipper.hpp
	$(CXX) $(CFLAGS) -c clipper.cpp -o clipper.o

clean:
	rm -f *.o libclipper.a