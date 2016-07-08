
PREFIX	=	/usr/local
CFLAGS	=	-g -Wall -fPIC
#CFLAGS  =       -g -DUSE_CPL
#CC = g++

LIBOBJ	=	shpopen.o dbfopen.o safileio.o shptree.o 
SHPBIN	=	shpcreate shpadd shpdump shprewind dbfcreate dbfadd dbfdump \
		shptreedump 

default:	all

all:	$(SHPBIN) shptest lib

shpopen.o:	shpopen.c shapefil.h
	$(CC) $(CFLAGS) -c shpopen.c

shptree.o:	shptree.c shapefil.h
	$(CC) $(CFLAGS) -c shptree.c

dbfopen.o:	dbfopen.c shapefil.h
	$(CC) $(CFLAGS) -c dbfopen.c

safileio.o:	safileio.c shapefil.h
	$(CC) $(CFLAGS) -c safileio.c

shpcreate:	shpcreate.c shpopen.o safileio.o 
	$(CC) $(CFLAGS) shpcreate.c shpopen.o safileio.o $(LINKOPT) -o shpcreate

shpadd:		shpadd.c shpopen.o safileio.o
	$(CC) $(CFLAGS) shpadd.c shpopen.o safileio.o $(LINKOPT) -o shpadd

shpdump:	shpdump.c shpopen.o safileio.o
	$(CC) $(CFLAGS) shpdump.c shpopen.o safileio.o $(LINKOPT) -o shpdump

shprewind:	shprewind.c shpopen.o safileio.o
	$(CC) $(CFLAGS) shprewind.c shpopen.o safileio.o $(LINKOPT) -o shprewind

dbfcreate:	dbfcreate.c dbfopen.o safileio.o
	$(CC) $(CFLAGS) dbfcreate.c dbfopen.o safileio.o $(LINKOPT) -o dbfcreate

dbfadd:		dbfadd.c dbfopen.o safileio.o
	$(CC) $(CFLAGS) dbfadd.c dbfopen.o safileio.o $(LINKOPT) -o dbfadd

dbfdump:	dbfdump.c dbfopen.o safileio.o
	$(CC) $(CFLAGS) dbfdump.c dbfopen.o safileio.o $(LINKOPT) -o dbfdump

shptest:	shptest.c shpopen.o safileio.o
	$(CC) $(CFLAGS) shptest.c shpopen.o safileio.o $(LINKOPT) -o shptest

shputils:	shputils.c shpopen.o safileio.o dbfopen.o 
	$(CC) $(CFLAGS) shputils.c shpopen.o safileio.o dbfopen.o  $(LINKOPT) -o shputils

shptreedump:	shptreedump.c shptree.o shpopen.o safileio.o
	$(CC) $(CFLAGS) shptreedump.c shptree.o shpopen.o safileio.o $(LINKOPT) \
		-o shptreedump

clean:
	rm -f *.o shptest $(SHPBIN) libshp.a 

test:	test2 test3

#
#	Note this stream only works if example data is accessable.
#	Fetch ftp://gdal.velocet.ca/pub/outgoing/shape_eg_data.zip
#
test1:
	@./stream1.sh > s1.out
	@if test "`diff s1.out stream1.out`" = '' ; then \
	    echo "******* Stream 1 Succeeded *********"; \
	    rm s1.out; \
	else \
	    echo "******* Stream 1 Failed *********"; \
	    diff s1.out stream1.out; \
	fi

test2:
	@./stream2.sh > s2.out
	@if test "`diff s2.out stream2.out`" = '' ; then \
	    echo "******* Stream 2 Succeeded *********"; \
	    rm s2.out; \
	    rm test*.s??; \
	else \
	    echo "******* Stream 2 Failed *********"; \
	    diff s2.out stream2.out; \
	fi

test3:
	@./makeshape.sh > s3.out
	@if test "`diff s3.out stream3.out`" = '' ; then \
	    echo "******* Stream 3 Succeeded *********"; \
	    rm s3.out; \
	    rm test.*; \
	else \
	    echo "******* Stream 3 Failed *********"; \
	    diff s3.out stream3.out; \
	fi


lib:	libshp.a

libshp.a:	$(LIBOBJ)
	ar r libshp.a $(LIBOBJ)

lib_install:	libshp.a
	cp libshp.a $(PREFIX)/lib
	cp shapefil.h $(PREFIX)/include

bin_install:	$(SHPBIN)
	cp $(SHPBIN) $(PREFIX)/bin

install:	lib_install bin_install

