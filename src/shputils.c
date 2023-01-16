#ifndef DGGRIDR
#define DGGRIDR
#endif
/******************************************************************************
 * $Id: shputils.c,v 1.11 2016-12-05 12:44:06 erouault Exp $
 *
 * Project:  Shapelib
 * Purpose:
 *   Altered "shpdump" and "dbfdump" to allow two files to be appended.
 *   Other Functions:
 *     Selecting from the DBF before the write occurs.
 *     Change the UNITS between Feet and Meters and Shift X,Y.
 *     Clip and Erase boundary.  The program only passes thru the
 *     data once.
 *
 *   Bill Miller   North Carolina - Department of Transporation
 *   Feb. 1997 -- bmiller@dot.state.nc.us
 *         There was not a lot of time to debug hidden problems;
 *         And the code is not very well organized or documented.
 *         The clip/erase function was not well tested.
 *   Oct. 2000 -- bmiller@dot.state.nc.us
 *         Fixed the problem when select is using numbers
 *         larger than short integer.  It now reads long integer.
 *   NOTE: DBF files created using windows NT will read as a string with
 *         a length of 381 characters.  This is a bug in "dbfopen".
 *
 *
 * Author:   Bill Miller (bmiller@dot.state.nc.us)
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * This software is available under the following "MIT Style" license,
 * or at the option of the licensee under the LGPL (see COPYING).  This
 * option is discussed in more detail in shapelib.html.
 *
 * --
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log: shputils.c,v $
 * Revision 1.11  2016-12-05 12:44:06  erouault
 * * Major overhaul of Makefile build system to use autoconf/automake.
 *
 * * Warning fixes in contrib/
 *
 * Revision 1.10  2007-12-13 19:59:23  fwarmerdam
 * reindent code, avoid some warnings.
 *
 * Revision 1.9  2004/01/14 14:56:00  fwarmerdam
 * Some cleanlyness improvements.
 *
 * Revision 1.8  2004/01/14 14:40:22  fwarmerdam
 * Fixed exit() call to include code.
 *
 * Revision 1.7  2003/02/25 17:20:22  warmerda
 * Set psCShape to NULL after SHPDestroyObject() to avoid multi-frees of
 * the same memory ... as submitted by Fred Fox.
 *
 * Revision 1.6  2001/08/28 13:57:14  warmerda
 * fixed DBFAddField return value check
 *
 * Revision 1.5  2000/11/02 13:52:48  warmerda
 * major upgrade from Bill Miller
 *
 * Revision 1.4  1999/11/05 14:12:05  warmerda
 * updated license terms
 *
 * Revision 1.3  1998/12/03 15:47:39  warmerda
 * Did a bunch of rewriting to make it work with the V1.2 API.
 *
 * Revision 1.2  1998/06/18 01:19:49  warmerda
 * Made C++ compilable.
 *
 * Revision 1.1  1997/05/27 20:40:27  warmerda
 * Initial revision
 */

#include "shapefil.h"
#include "string.h"
#include <stdlib.h>

SHP_CVSID("$Id: shputils.c,v 1.11 2016-12-05 12:44:06 erouault Exp $")

#ifndef FALSE
#  define FALSE		0
#  define TRUE		1
#endif

char            infile[80], outfile[80], temp[400];

/* Variables for shape files */
SHPHandle	hSHP;
SHPHandle	hSHPappend;
int		nShapeType, nEntities, iPart;
int		nShapeTypeAppend, nEntitiesAppend;
SHPObject	*psCShape;
double		adfBoundsMin[4], adfBoundsMax[4];


/* Variables for DBF files */
DBFHandle	hDBF;
DBFHandle	hDBFappend;

DBFFieldType    iType;
DBFFieldType    jType;

char	iszTitle[12];
char	jszTitle[12];

int	*pt;
char	iszFormat[32], iszField[1024];
char	jszFormat[32], jszField[1024];
int	i, ti, iWidth, iDecimals, iRecord;
int	j, tj, jWidth, jDecimals, jRecord;


int clip_boundary(void);
double findunit(char *unit);
void openfiles(void);
void setext(char *pt, char *ext);
int strncasecmp2(char *s1, char *s2, int n);
void mergefields(void);
void findselect(void);
void showitems(void);
int selectrec(void);
void check_theme_bnd(void);
int clip_boundary(void);
void error(void);


/* -------------------------------------------------------------------- */
/* Variables for the DESCRIBE function */
/* -------------------------------------------------------------------- */
   int       ilist = FALSE, iall = FALSE;
/* -------------------------------------------------------------------- */
/* Variables for the SELECT function */
/* -------------------------------------------------------------------- */
   int       found = FALSE, newdbf = FALSE;
   char      selectitem[40], *cpt;
   long int  selectvalues[150], selcount=0;
   int       iselect = FALSE, iselectitem = -1;
   int       iunselect = FALSE;

/* -------------------------------------------------------------------- */
/* Variables for the CLIP and ERASE functions */
/* -------------------------------------------------------------------- */
   double  cxmin, cymin, cxmax, cymax;
   int     iclip  = FALSE, ierase = FALSE;
   int     itouch = FALSE, iinside = FALSE, icut = FALSE;
   int     ibound = FALSE, ipoly = FALSE;
   char    clipfile[80];

/* -------------------------------------------------------------------- */
/* Variables for the FACTOR function */
/* -------------------------------------------------------------------- */
   double  infactor,outfactor,factor = 0;  /* NO FACTOR */
   int     iunit = FALSE;
   int     ifactor = FALSE;


/* -------------------------------------------------------------------- */
/* Variables for the SHIFT function */
/* -------------------------------------------------------------------- */
   double  xshift = 0, yshift = 0;  /* NO SHIFT */

/************************************************************************/
/*                             openfiles()                              */
/************************************************************************/

void openfiles(void) {
/* -------------------------------------------------------------------- */
/*      Open the DBF file.                                              */
/* -------------------------------------------------------------------- */
    setext(infile, "dbf");
    hDBF = DBFOpen( infile, "rb" );
    if( hDBF == NULL )
    {
	dgprintf( "ERROR: Unable to open the input DBF:%s\n", infile );
#ifndef DGGRIDR
	exit( 1 );
#endif
    }
/* -------------------------------------------------------------------- */
/*      Open the append DBF file.                                       */
/* -------------------------------------------------------------------- */
    if (strcmp(outfile,"")) {
        setext(outfile, "dbf");
        hDBFappend = DBFOpen( outfile, "rb+" );
        newdbf=0;
        if( hDBFappend == NULL )
        {
            newdbf=1;
            hDBFappend = DBFCreate( outfile );
            if( hDBFappend == NULL )
            {
                dgprintf( "ERROR: Unable to open the append DBF:%s\n", outfile );
#ifndef DGGRIDR
                exit( 1 );
#endif
            }
        }
    }
/* -------------------------------------------------------------------- */
/*      Open the passed shapefile.                                      */
/* -------------------------------------------------------------------- */
    setext(infile, "shp");
    hSHP = SHPOpen( infile, "rb" );

    if( hSHP == NULL )
    {
	dgprintf( "ERROR: Unable to open the input shape file:%s\n", infile );
#ifndef DGGRIDR
        exit( 1 );
#endif
    }

    SHPGetInfo( hSHP, &nEntities, &nShapeType, NULL, NULL );

/* -------------------------------------------------------------------- */
/*      Open the passed append shapefile.                               */
/* -------------------------------------------------------------------- */
    if (strcmp(outfile,"")) {
        setext(outfile, "shp");
        hSHPappend = SHPOpen( outfile, "rb+" );

        if( hSHPappend == NULL )
        {
            hSHPappend = SHPCreate( outfile, nShapeType );
            if( hSHPappend == NULL )
            {
                dgprintf( "ERROR: Unable to open the append shape file:%s\n",
                        outfile );
#ifndef DGGRIDR
                exit( 1 );
#endif
            }
        }
        SHPGetInfo( hSHPappend, &nEntitiesAppend, &nShapeTypeAppend,
                    NULL, NULL );

        if (nShapeType != nShapeTypeAppend)
        {
            dgprintf( "ERROR: Input and Append shape files are of different types.");
#ifndef DGGRIDR
        exit( 1 );
#endif
        }
    }
}

/* -------------------------------------------------------------------- */
/*	Change the extension.  If there is any extension on the 	*/
/*	filename, strip it off and add the new extension                */
/* -------------------------------------------------------------------- */
void setext(char *pt, char *ext)
{
int i;
    for( i = (int) strlen(pt)-1;
	 i > 0 && pt[i] != '.' && pt[i] != '/' && pt[i] != '\\';
	 i-- ) {}

    if( pt[i] == '.' )
        pt[i] = '\0';

    strcat(pt,".");
    strcat(pt,ext);
}



/* -------------------------------------------------------------------- */
/*	Find matching fields in the append file.                        */
/*      Output file must have zero records to add any new fields.       */
/* -------------------------------------------------------------------- */
void mergefields(void)
{
    int i,j;
    ti = DBFGetFieldCount( hDBF );
    tj = DBFGetFieldCount( hDBFappend );
    /* Create a pointer array for the max # of fields in the output file */
    pt = (int *) malloc( (ti+tj+1) * sizeof(int) );

    for( i = 0; i < ti; i++ )
    {
        pt[i]= -1;  /* Initial pt values to -1 */
    }
    /* DBF must be empty before adding items */
    jRecord = DBFGetRecordCount( hDBFappend );
    for( i = 0; i < ti; i++ )
    {
	iType = DBFGetFieldInfo( hDBF, i, iszTitle, &iWidth, &iDecimals );
        found=FALSE;
        {
      	    for( j = 0; j < tj; j++ )   /* Search all field names for a match */
    	    {
	        jType = DBFGetFieldInfo( hDBFappend, j, jszTitle, &jWidth, &jDecimals );
	        if (iType == jType && (strcmp(iszTitle, jszTitle) == 0) )
	        {
	            if (found || newdbf)
	            {
	                if (i == j)  pt[i]=j;
	                dgprintf("Warning: Duplicate field name found (%s)\n",iszTitle);
	                /* Duplicate field name
	                   (Try to guess the correct field by position) */
	            }
	            else
	            {
	            	pt[i]=j;  found=TRUE;
	            }
	        }
	    }
	}

	if (pt[i] == -1  && (! found) )  /* Try to force into an existing field */
	{                                /* Ignore the field name, width, and decimal places */
	    jType = DBFGetFieldInfo( hDBFappend, j, jszTitle, &jWidth, &jDecimals );
	    if (iType == jType)
	    {
	    	pt[i]=i;  found=1;
	    }
	}
	if ( (! found) &&  jRecord == 0)  /* Add missing field to the append table */
	{                 /* The output DBF must be is empty */
	    pt[i]=tj;
	    tj++;
	    if( DBFAddField( hDBFappend, iszTitle, iType, iWidth, iDecimals )
                == -1 )
	    {
		dgprintf( "Warning: DBFAddField(%s, TYPE:%d, WIDTH:%d  DEC:%d, ITEM#:%d of %d) failed.\n",
                        iszTitle, iType, iWidth, iDecimals, (i+1), (ti+1) );
		pt[i]=-1;
	    }
	}
    }
}


void findselect(void)
{
    /* Find the select field name */
    iselectitem = -1;
    for( i = 0; i < ti  &&  iselectitem < 0; i++ )
    {
	iType = DBFGetFieldInfo( hDBF, i, iszTitle, &iWidth, &iDecimals );
        if (strncasecmp2(iszTitle, selectitem, 0) == 0) iselectitem = i;
    }
    if (iselectitem == -1)
    {
        dgprintf("Warning: Item not found for selection (%s)\n",selectitem);
        iselect = FALSE;
        iall = FALSE;
	showitems();
        dgprintf("Continued... (Selecting entire file)\n");
    }
    /* Extract all of the select values (by field type) */

}

void showitems(void)
{
    char      stmp[40],slow[40],shigh[40];
    double    dtmp,dlow,dhigh,dsum,mean;
    long int  itmp,ilow,ihigh,isum;
    long int  maxrec;
    char      *pt;

    dgprintf("Available Items: (%d)",ti);
    maxrec = DBFGetRecordCount(hDBF);
    if (maxrec > 5000 && ! iall)
    { maxrec=5000; dgprintf("  ** ESTIMATED RANGES (MEAN)  For more records use \"All\""); }
    else  { dgprintf("          RANGES (MEAN)"); }

    for( i = 0; i < ti; i++ )
    {
        switch( DBFGetFieldInfo( hDBF, i, iszTitle, &iWidth, &iDecimals ) )
        {
          case FTString:
          case FTLogical:
            strcpy(slow, "~");
            strcpy(shigh,"\0");
            dgprintf("\n  String  %3d  %-16s",iWidth,iszTitle);
            for( iRecord = 0; iRecord < maxrec; iRecord++ ) {
                strncpy(stmp,DBFReadStringAttribute( hDBF, iRecord, i ),39);
                if (strcmp(stmp,"!!") > 0) {
                    if (strncasecmp2(stmp,slow,0)  < 0) memcpy(slow, stmp,39);
                    if (strncasecmp2(stmp,shigh,0) > 0) memcpy(shigh,stmp,39);
                }
            }
            pt=slow+strlen(slow)-1;
            while(*pt == ' ') { *pt='\0'; pt--; }
            pt=shigh+strlen(shigh)-1;
            while(*pt == ' ') { *pt='\0'; pt--; }
            if (strncasecmp2(slow,shigh,0) < 0)		dgprintf("%s to %s",slow,shigh);
            else if (strncasecmp2(slow,shigh,0) == 0)	dgprintf("= %s",slow);
            else	dgprintf("No Values");
            break;
          case FTInteger:
            dgprintf("\n  Integer %3d  %-16s",iWidth,iszTitle);
            ilow =  1999999999;
            ihigh= -1999999999;
            isum =  0;
            for( iRecord = 0; iRecord < maxrec; iRecord++ ) {
                itmp = DBFReadIntegerAttribute( hDBF, iRecord, i );
                if (ilow > itmp)  ilow = itmp;
                if (ihigh < itmp) ihigh = itmp;
                isum = isum + itmp;
            }
            mean=isum/maxrec;
            if (ilow < ihigh)       dgprintf("%ld to %ld \t(%.1f)",ilow,ihigh,mean);
            else if (ilow == ihigh) dgprintf("= %ld",ilow);
            else dgprintf("No Values");
            break;

          case FTDouble:
            dgprintf("\n  Real  %3d,%d  %-16s",iWidth,iDecimals,iszTitle);
            dlow =  999999999999999.0;
            dhigh= -999999999999999.0;
            dsum =  0;
            for( iRecord = 0; iRecord < maxrec; iRecord++ ) {
                dtmp = DBFReadDoubleAttribute( hDBF, iRecord, i );
                if (dlow > dtmp) dlow = dtmp;
                if (dhigh < dtmp) dhigh = dtmp;
                dsum = dsum + dtmp;
            }
            mean=dsum/maxrec;
            sprintf(stmp,"%%.%df to %%.%df \t(%%.%df)",iDecimals,iDecimals,iDecimals);
            if (dlow < dhigh)       dgprintf(stmp,dlow,dhigh,mean);
            else if (dlow == dhigh) {
                sprintf(stmp,"= %%.%df",iDecimals);
                dgprintf(stmp,dlow);
            }
            else dgprintf("No Values");
            break;

          case FTInvalid:
            break;

        }

    }
    dgprintf("\n");
}

int selectrec(void)
{
    long int value, ty;

    ty = DBFGetFieldInfo( hDBF, iselectitem, NULL, &iWidth, &iDecimals);
    switch(ty)
    {
      case FTString:
        dgprintf("Invalid Item");
        iselect=FALSE;
	break;
      case FTInteger:
        value = DBFReadIntegerAttribute( hDBF, iRecord, iselectitem );
        for (j = 0; j<selcount; j++)
        {
            if (selectvalues[j] == value)
            {
                if (iunselect) return(0);  /* Keep this record */
                else  return(1);  /* Skip this record */
            }
        }
	break;
      case FTDouble:
        dgprintf("Invalid Item");
        iselect=FALSE;
        break;
    }
    if (iunselect) return(1);  /* Skip this record */
    else  return(0);  /* Keep this record */
}


void check_theme_bnd(void)
{
    if ( (adfBoundsMin[0] >= cxmin) && (adfBoundsMax[0] <= cxmax) &&
         (adfBoundsMin[1] >= cymin) && (adfBoundsMax[1] <= cymax) )
    {   /** Theme is totally inside clip area **/
        if (ierase) nEntities=0; /** SKIP THEME  **/
        else   iclip=FALSE; /** WRITE THEME (Clip not needed) **/
    }

    if ( ( (adfBoundsMin[0] < cxmin) && (adfBoundsMax[0] < cxmin) ) ||
         ( (adfBoundsMin[1] < cymin) && (adfBoundsMax[1] < cymin) ) ||
         ( (adfBoundsMin[0] > cxmax) && (adfBoundsMax[0] > cxmax) ) ||
         ( (adfBoundsMin[1] > cymax) && (adfBoundsMax[1] > cymax) ) )
    {   /** Theme is totally outside clip area **/
        if (ierase) iclip=FALSE; /** WRITE THEME (Clip not needed) **/
             else   nEntities=0; /** SKIP THEME  **/
    }

    if (nEntities == 0)
        dgprintf("WARNING: Theme is outside the clip area."); /** SKIP THEME  **/
}

int clip_boundary(void)
{
    int  inside;
    int  prev_outside;
    int  i2;
    int  j2;

    /*** FIRST check the boundary of the feature ***/
    if ( ( (psCShape->dfXMin < cxmin) && (psCShape->dfXMax < cxmin) ) ||
         ( (psCShape->dfYMin < cymin) && (psCShape->dfYMax < cymin) ) ||
         ( (psCShape->dfXMin > cxmax) && (psCShape->dfXMax > cxmax) ) ||
         ( (psCShape->dfYMin > cymax) && (psCShape->dfYMax > cymax) ) )
    {   /** Feature is totally outside clip area **/
        if (ierase) return(1); /** WRITE RECORD **/
        else   return(0); /** SKIP  RECORD **/
    }

    if ( (psCShape->dfXMin >= cxmin) && (psCShape->dfXMax <= cxmax) &&
         (psCShape->dfYMin >= cymin) && (psCShape->dfYMax <= cymax) )
    {   /** Feature is totally inside clip area **/
        if (ierase) return(0); /** SKIP  RECORD **/
        else   return(1); /** WRITE RECORD **/
    }

    if (iinside)
    { /** INSIDE * Feature might touch the boundary or could be outside **/
        if (ierase)  return(1); /** WRITE RECORD **/
        else    return(0); /** SKIP  RECORD **/
    }

    if (itouch)
    {   /** TOUCH **/
        if ( ( (psCShape->dfXMin <= cxmin) || (psCShape->dfXMax >= cxmax) ) &&
             (psCShape->dfYMin >= cymin) && (psCShape->dfYMax <= cymax)    )
        {   /** Feature intersects the clip boundary only on the X axis **/
            if (ierase) return(0); /** SKIP  RECORD **/
            else   return(1); /** WRITE RECORD **/
        }

        if (   (psCShape->dfXMin >= cxmin) && (psCShape->dfXMax <= cxmax)   &&
               ( (psCShape->dfYMin <= cymin) || (psCShape->dfYMax >= cymax) )  )
        {   /** Feature intersects the clip boundary only on the Y axis **/
            if (ierase) return(0); /** SKIP  RECORD **/
            else   return(1); /** WRITE RECORD **/
        }

        for( j2 = 0; j2 < psCShape->nVertices; j2++ )
        {   /** At least one vertex must be inside the clip boundary **/
            if ( (psCShape->padfX[j2] >= cxmin  &&  psCShape->padfX[j2] <= cxmax) ||
                 (psCShape->padfY[j2] >= cymin  &&  psCShape->padfY[j2] <= cymax)  )
            {
                if (ierase) return(0); /** SKIP  RECORD **/
                else   return(1); /** WRITE RECORD **/
            }
        }

        /** All vertices are outside the clip boundary **/
        if (ierase) return(1); /** WRITE RECORD **/
        else   return(0); /** SKIP  RECORD **/
    }   /** End TOUCH **/

    if (icut)
    {   /** CUT **/
        /*** Check each vertex in the feature with the Boundary and "CUT" ***/
        /*** THIS CODE WAS NOT COMPLETED!  READ NOTE AT THE BOTTOM ***/
        i2=0;
        prev_outside=FALSE;
        for( j2 = 0; j2 < psCShape->nVertices; j2++ )
        {
            inside = psCShape->padfX[j2] >= cxmin  &&  psCShape->padfX[j2] <= cxmax  &&
                psCShape->padfY[j2] >= cymin  &&  psCShape->padfY[j2] <= cymax ;

            if (ierase) inside=(! inside);
            if (inside)
            {
                if (i2 != j2)
                {
                    if (prev_outside)
                    {
                        /*** AddIntersection(i2); ***/  /*** Add intersection ***/
                        prev_outside=FALSE;
                    }
                    psCShape->padfX[i2]=psCShape->padfX[j2];     /** move vertex **/
                    psCShape->padfY[i2]=psCShape->padfY[j2];
                }
                i2++;
            } else {
                if ( (! prev_outside) && (j2 > 0) )
                {
                    /*** AddIntersection(i2); ***//*** Add intersection (Watch out for j2==i2-1) ***/
                    /*** Also a polygon may overlap twice and will split into a several parts  ***/
                    prev_outside=TRUE;
                }
            }
        }

        dgprintf("Vertices:%d   OUT:%d   Number of Parts:%d\n",
               psCShape->nVertices,i2, psCShape->nParts );

        psCShape->nVertices = i2;

        if (i2 < 2) return(0); /** SKIP RECORD **/
        /*** (WE ARE NOT CREATING INTERESECTIONS and some lines could be reduced to one point) **/

        if (i2 == 0) return(0); /** SKIP  RECORD **/
        else    return(1); /** WRITE RECORD **/
    }  /** End CUT **/

    return 0;
}


/************************************************************************/
/*                            strncasecmp2()                            */
/*                                                                      */
/*      Compare two strings up to n characters                          */
/*      If n=0 then s1 and s2 must be an exact match                    */
/************************************************************************/

int strncasecmp2(char *s1, char *s2, int n)

{
int j,i;
   if (n<1) n= (int) strlen(s1)+1;
   for (i=0; i<n; i++)
   {
      if (*s1 != *s2)
      {
         if (*s1 >= 'a' && *s1 <= 'z') {
            j=*s1-32;
            if (j != *s2) return(*s1-*s2);
         } else {
            if (*s1 >= 'A' && *s1 <= 'Z') { j=*s1+32; }
                                   else   { j=*s1;    }
            if (j != *s2) return(*s1-*s2);
         }
      }
      s1++;
      s2++;
   }
   return(0);
}


#define  NKEYS (sizeof(unitkeytab) / sizeof(struct unitkey))
double findunit(char *unit)
   {
   struct unitkey {
     char   *name;
     double value;
   } unitkeytab[] = {
     {"CM",            39.37},
     {"CENTIMETER",    39.37},
     {"CENTIMETERS",   39.37},  /** # of inches * 100 in unit **/
     {"METER",          3937},
     {"METERS",         3937},
     {"KM",          3937000},
     {"KILOMETER",   3937000},
     {"KILOMETERS",  3937000},
     {"INCH",            100},
     {"INCHES",          100},
     {"FEET",           1200},
     {"FOOT",           1200},
     {"YARD",           3600},
     {"YARDS",          3600},
     {"MILE",        6336000},
     {"MILES",       6336000}
   };

   double unitfactor=0;
   for (unsigned int j = 0; j < NKEYS; j++) {
    if (strncasecmp2(unit, unitkeytab[j].name, 0) == 0) unitfactor=unitkeytab[j].value;
   }
   return(unitfactor);
}

/* -------------------------------------------------------------------- */
/*      Display a usage message.                                        */
/* -------------------------------------------------------------------- */
void error(void)
{
    dgprintf( "The program will append to an existing shape file or it will" );
    dgprintf( "create a new file if needed." );
    dgprintf( "Only the items in the first output file will be preserved." );
    dgprintf( "When an item does not match with the append theme then the item");
    dgprintf( "might be placed to an existing item at the same position and type." );
    dgprintf( "  OTHER FUNCTIONS:" );
    dgprintf( "  - Describe all items in the dbase file (Use ALL for more than 5000 recs.)");
    dgprintf( "  - Select a group of shapes from a comma separated selection list.");
    dgprintf( "  - UnSelect a group of shapes from a comma separated selection list.");
    dgprintf( "  - Clip boundary extent or by theme boundary." );
    dgprintf( "      Touch writes all the shapes that touch the boundary.");
    dgprintf( "      Inside writes all the shapes that are completely within the boundary.");
    dgprintf( "      Boundary clips are only the min and max of a theme boundary." );
    dgprintf( "  - Erase boundary extent or by theme boundary." );
    dgprintf( "      Erase is the direct opposite of the Clip function." );
    dgprintf( "  - Change coordinate value units between meters and feet.");
    dgprintf( "      There is no way to determine the input unit of a shape file.");
    dgprintf( "      Skip this function if the shape file is already in the correct unit.");
    dgprintf( "      Clip and Erase will be done before the unit is changed.");
    dgprintf( "      A shift will be done after the unit is changed.");
    dgprintf( "  - Shift X and Y coordinates.\n" );
    dgprintf( "Finally, There can only be one select or unselect in the command line.");
    dgprintf( "         There can only be one clip or erase in the command line.");
    dgprintf( "         There can only be one unit and only one shift in the command line.\n");
    dgprintf( "Ex: shputils in.shp out.shp   SELECT countycode 3,5,9,13,17,27");
    dgprintf( "    shputils in.shp out.shp   CLIP   10 10 90 90 Touch   FACTOR Meter Feet");
    dgprintf( "    shputils in.shp out.shp   FACTOR Meter 3.0");
    dgprintf( "    shputils in.shp out.shp   CLIP   clip.shp Boundary Touch   SHIFT 40 40");
    dgprintf( "    shputils in.shp out.shp   SELECT co 112   CLIP clip.shp Boundary Touch\n");
    dgprintf( "USAGE: shputils  <DescribeShape>   {ALL}");
    dgprintf( "USAGE: shputils  <InputShape>  <OutShape|AppendShape>" );
    dgprintf( "   { <FACTOR>       <FEET|MILES|METERS|KM> <FEET|MILES|METERS|KM|factor> }" );
    dgprintf( "   { <SHIFT>        <xshift> <yshift> }" );
    dgprintf( "   { <SELECT|UNSEL> <Item> <valuelist> }" );
    dgprintf( "   { <CLIP|ERASE>   <xmin> <ymin> <xmax> <ymax> <TOUCH|INSIDE|CUT> }" );
    dgprintf( "   { <CLIP|ERASE>   <theme>      <BOUNDARY>     <TOUCH|INSIDE|CUT> }" );
    dgprintf( "     Note: CUT is not complete and does not create intersections.");
    dgprintf( "           For more information read programmer comment.");

    /****   Clip functions for Polygon and Cut is not supported
            There are several web pages that describe methods of doing this function.
            It seem easy to impliment until you start writting code.  I don't have the
            time to add these functions but a did leave a simple cut routine in the
            program that can be called by using CUT instead of TOUCH in the
            CLIP or ERASE functions.  It does not add the intersection of the line and
            the clip box, so polygons could look incomplete and lines will come up short.

            Information about clipping lines with a box:
            http://www.csclub.uwaterloo.ca/u/mpslager/articles/sutherland/wr.html
            Information about finding the intersection of two lines:
            http://www.whisqu.se/per/docs/math28.htm

            THE CODE LOOKS LIKE THIS:
            ********************************************************
            void Intersect_Lines(float x0,float y0,float x1,float y1,
            float x2,float y2,float x3,float y3,
            float *xi,float *yi)
            {
//  this function computes the intersection of the sent lines
//  and returns the intersection point, note that the function assumes
//  the lines intersect. the function can handle vertical as well
//  as horizontal lines. note the function isn't very clever, it simply
//  applies the math, but we don't need speed since this is a
//  pre-processing step
//  The Intersect_lines program came from (http://www.whisqu.se/per/docs/math28.htm)

float a1,b1,c1, // constants of linear equations
a2,b2,c2,
det_inv,  // the inverse of the determinant of the coefficientmatrix
m1,m2;    // the slopes of each line

// compute slopes, note the cludge for infinity, however, this will
// be close enough
if ((x1-x0)!=0)
m1 = (y1-y0)/(x1-x0);
else
m1 = (float)1e+10;  // close enough to infinity


if ((x3-x2)!=0)
m2 = (y3-y2)/(x3-x2);
else
m2 = (float)1e+10;  // close enough to infinity

// compute constants
a1 = m1;
a2 = m2;
b1 = -1;
b2 = -1;
c1 = (y0-m1*x0);
c2 = (y2-m2*x2);
// compute the inverse of the determinate
det_inv = 1/(a1*b2 - a2*b1);
// use Kramers rule to compute xi and yi
*xi=((b1*c2 - b2*c1)*det_inv);
*yi=((a2*c1 - a1*c2)*det_inv);
} // end Intersect_Lines
    **********************************************************/

#ifndef DGGRIDR
     exit( 1 );
#endif
}
