
#LINKOPT	=	/usr/local/lib/libdbmalloc.a
#CFLAGS	=	-g

# Endian: define either _LITTLE_ENDIAN or _BIG_ENDIAN
ENDIAN	=	-D_LITTLE_ENDIAN

CFLAGS	=	-g -I.. -I$(HOME)/bld/include -DPROJ4 $(ENDIAN) -DDEBUG -DDEBUG2 

SHPOBJ	=	../shpopen.o ../dbfopen.o ../safileio.o

SHPGOBJ =	../shpopen.o ../dbfopen.o ../safileio.o shpgeo.o

GEOOBJ	=	./shpgeo.o -lm -L$(HOME)/bld/lib -lproj

default:	all

all:	shpdxf shpproj dbfinfo shpcentrd shpdata shpwkb dbfinfo dbfcat shpinfo shpfix shpcat Shape_PointInPoly shpsort

clean:  
	rm -f shpdxf shpproj dbfinfo shpcentrd shpdata shpwkb dbfcat dbfinfo shpinfo shpfix shpcat *.o

shpgeo.o:	shpgeo.c shpgeo.h
	$(CC) $(CFLAGS) -c shpgeo.c

shpdxf:		shpdxf.c $(SHPOBJ)
	$(CC) $(CFLAGS) shpdxf.c ${SHPOBJ} $(LINKOPT) -o shpdxf

shpcentrd:	shpcentrd.c $(SHPGOBJ)
	$(CC) $(CFLAGS) shpcentrd.c ${SHPOBJ} $(LINKOPT) $(GEOOBJ) -o shpcentrd

shpdata:	shpdata.c $(SHPGOBJ)
	$(CC) $(CFLAGS) shpdata.c ${SHPOBJ} $(LINKOPT) $(GEOOBJ) -o shpdata

shpinfo:	shpinfo.c $(SHPOBJ)
	$(CC) $(CFLAGS) shpinfo.c ${SHPOBJ} $(LINKOPT) $(GEOOBJ) -o shpinfo

shpfix:		shpfix.c $(SHPOBJ)
	$(CC) $(CFLAGS) shpfix.c ${SHPOBJ} $(LINKOPT) -o shpfix

shpcat:		shpcat.c $(SHPOBJ)
	$(CC) $(CFLAGS) shpcat.c ${SHPOBJ} $(LINKOPT) -o shpcat

shpwkb:		shpwkb.c $(SHPGOBJ)
	$(CC) $(CFLAGS) shpwkb.c ${SHPOBJ} $(LINKOPT) $(GEOOBJ) -o shpwkb

shpproj: 	shpproj.c $(SHPGOBJ)
	$(CC) $(CFLAGS) shpproj.c  $(SHPOBJ) $(GEOOBJ) -lm -lproj $(LINKOPT) -o shpproj

shpsort:		shpsort.c $(SHPOBJ)
	$(CC) $(CFLAGS) shpsort.c ${SHPOBJ} $(LINKOPT) -lm -o shpsort

dbfinfo: 	dbfinfo.c $(SHPOBJ)
	$(CC) $(CFLAGS) dbfinfo.c $(SHPOBJ) $(LINKOPT) -o dbfinfo

dbfcat:		dbfcat.c $(SHPOBJ)
	$(CC) $(CFLAGS) dbfcat.c $(SHPOBJ) $(LINKOPT) -o dbfcat

Shape_PointInPoly:	Shape_PointInPoly.cpp $(SHPOBJ)
	$(CXX) $(CFLAGS) Shape_PointInPoly.cpp $(SHPOBJ) $(LINKOPT) \
		-o Shape_PointInPoly

check:	testproj

testproj: 
	tests/shpproj.sh
