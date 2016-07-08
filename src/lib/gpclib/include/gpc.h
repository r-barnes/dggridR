/*
===========================================================================

Project:   Generic Polygon Clipper

           A new algorithm for calculating the difference, intersection,
           exclusive-or or union of arbitrary polygon sets.

File:      gpc.h
Author:    Alan Murta (amurta@cs.man.ac.uk)
Version:   2.31
Date:      4th June 1999

Copyright: (C) 1997-1999, Advanced Interfaces Group,
           University of Manchester.

           This software is free for non-commercial use. It may be copied,
           modified, and redistributed provided that this copyright notice
           is preserved on all copies. The intellectual property rights of
           the algorithms used reside with the University of Manchester
           Advanced Interfaces Group.

           You may not use this software, in whole or in part, in support
           of any commercial product without the express consent of the
           author.

           There is no warranty or other guarantee of fitness of this
           software for any purpose. It is provided solely "as is".

===========================================================================

   2 January 2000, Kevin Sahr: added "extern C" ifdef for C++ 

===========================================================================
*/

#ifndef __gpc_h
#define __gpc_h

#include <stdio.h>


/*
===========================================================================
                               Constants
===========================================================================
*/

/* Increase GPC_EPSILON to encourage merging of near coincident edges    */

#define GPC_EPSILON (DBL_EPSILON)

#define GPC_VERSION "2.31"


/*
===========================================================================
                           Public Data Types
===========================================================================
*/

typedef enum                        /* Set operation type                */
{
  GPC_DIFF,                         /* Difference                        */
  GPC_INT,                          /* Intersection                      */
  GPC_XOR,                          /* Exclusive or                      */
  GPC_UNION                         /* Union                             */
} gpc_op;

typedef struct                      /* Polygon vertex structure          */
{
  long double              x;            /* Vertex x component                */
  long double              y;            /* vertex y component                */
} gpc_vertex;

typedef struct                      /* Vertex list structure             */
{
  int                 num_vertices; /* Number of vertices in list        */
  gpc_vertex         *vertex;       /* Vertex array pointer              */
} gpc_vertex_list;

typedef struct sbt_t_shape          /* Scanbeam tree                     */
{
  long double              y;            /* Scanbeam node y value             */
  struct sbt_t_shape *less;         /* Pointer to nodes with lower y     */
  struct sbt_t_shape *more;         /* Pointer to nodes with higher y    */

/* the next two fields added by K. Sahr */

  struct sbt_t_shape **parPtr;      /* Handle to parent's pointer to me  */
  int           firstPoly;          /* In first polygon? */

} sb_tree;

typedef struct bbox_shape           /* Contour axis-aligned bounding box */
{
  long double             xmin;          /* Minimum x coordinate              */
  long double             ymin;          /* Minimum y coordinate              */
  long double             xmax;          /* Maximum x coordinate              */
  long double             ymax;          /* Maximum y coordinate              */
} bbox;

typedef struct                      /* Polygon set structure             */
{
  int                 num_contours; /* Number of contours in polygon     */
  int                *hole;         /* Hole / external contour flags     */
  gpc_vertex_list    *contour;      /* Contour array pointer             */
  sb_tree            *sbtree;       /* associated sbtree or NULL: K. Sahr*/
  int                 sbtentries;   /* number in sbtree                  */
  bbox               *box;          /* bounding box                      */
} gpc_polygon;

typedef struct                      /* Tristrip set structure            */
{
  int                 num_strips;   /* Number of tristrips               */
  gpc_vertex_list    *strip;        /* Tristrip array pointer            */
} gpc_tristrip;


/*
===========================================================================
                       Public Function Prototypes
===========================================================================
*/

#ifdef __cplusplus
extern "C" {
#endif

/*=======================================================================*/
void gpc_read_polygon        (FILE            *infile_ptr, 
                              int              read_hole_flags,
                              gpc_polygon     *polygon);

void gpc_write_polygon       (FILE            *outfile_ptr,
                              int              write_hole_flags,
                              gpc_polygon     *polygon);

void gpc_add_contour         (gpc_polygon     *polygon,
                              gpc_vertex_list *contour,
                              int              hole);

void gpc_polygon_clip        (gpc_op           set_operation,
                              gpc_polygon     *subject_polygon,
                              gpc_polygon     *clip_polygon,
                              gpc_polygon     *result_polygon);

int gpc_polygon_intersect    (gpc_polygon     *subject_polygon,
                              gpc_polygon     *clip_polygon);

void gpc_tristrip_clip       (gpc_op           set_operation,
                              gpc_polygon     *subject_polygon,
                              gpc_polygon     *clip_polygon,
                              gpc_tristrip    *result_tristrip);

void gpc_polygon_to_tristrip (gpc_polygon     *polygon,
                              gpc_tristrip    *tristrip);

void gpc_free_polygon        (gpc_polygon     *polygon);

void gpc_free_tristrip       (gpc_tristrip    *tristrip);

/* added by K. Sahr */

void gpc_init_polygon        (gpc_polygon     *p);
void gpc_create_sbtree       (gpc_polygon     *p);

/*=======================================================================*/
#ifdef __cplusplus
}
#endif

#endif

/*
===========================================================================
                           End of file: gpc.h
===========================================================================
*/
