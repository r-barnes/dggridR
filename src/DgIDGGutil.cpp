#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGutil.cpp: common classes used by IDGG classes
//
// Version 7.0 - Kevin Sahr, 11/16/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <climits>
#include <cfloat>
#include <string.h>

#include "DgIDGGutil.h"
#include "DgIDGGBase.h"
#include "DgUtil.h"
#include "DgIDGG.h"
#include "DgDmdD4Grid2DS.h"
#include "DgHexGrid2DS.h"
#include "DgTriGrid2DS.h"
#include "DgSeriesConverter.h"
#include "DgRadixString.h"
#include "DgBoundedIDGG.h"
#include "DgProjISEA.h"
#include "DgProjFuller.h"
#include "DgString.h"

const DgQ2DICoord DgQ2DICoord::undefDgQ2DICoord(-1,
                                     DgIVec2D(LLONG_MAX, LLONG_MAX));
const DgQ2DDCoord DgQ2DDCoord::undefDgQ2DDCoord(-1,
                                     DgDVec2D(LDBL_MAX, LDBL_MAX));

////////////////////////////////////////////////////////////////////////////////
DgVertTriVals DgVertex2DDRF::vertTable_[12][6] = {

   {  // vert 0

      DgVertTriVals(  2,  1, 0, true,  DgDVec2D(-M_HALF, -M_SIN60),  3 ),
      DgVertTriVals(  1,  0, 1, true,  DgDVec2D(-M_ONE,   M_ZERO ),  2 ),
      DgVertTriVals(  5,  4, 2, true,  DgDVec2D(-M_HALF,  M_SIN60),  1 ),
      DgVertTriVals( -1, -1, 3, false, DgDVec2D(-M_HALF,  M_SIN60),  1 ),
      DgVertTriVals(  4,  3, 4, true,  DgDVec2D( M_ONE,   M_ZERO ), -1 ),
      DgVertTriVals(  3,  2, 5, true,  DgDVec2D( M_HALF, -M_SIN60), -2 )
   },

   {  // vert 1

      DgVertTriVals(  1,  0, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  1,  5, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals( 10, 14, 2, true,  DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals( -1, -1, 3, false, DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals(  5,  9, 4, true,  DgDVec2D( M_ZERO,  M_ZERO),  3 ),
      DgVertTriVals(  5,  4, 5, true,  DgDVec2D( M_ONE,   M_ZERO),  0 )
   },

   {  // vert 2

      DgVertTriVals(  2,  1, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  2,  6, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals( 10, 10, 2, true,  DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals( -1, -1, 3, false, DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals(  1,  5, 4, true,  DgDVec2D( M_ZERO,  M_ZERO),  3 ),
      DgVertTriVals(  1,  0, 5, true,  DgDVec2D( M_ONE,   M_ZERO),  0 )
   },

   {  // vert 3

      DgVertTriVals(  3,  2, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  3,  7, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  7, 11, 2, true,  DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals( -1, -1, 3, false, DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals(  2,  6, 4, true,  DgDVec2D( M_ZERO,  M_ZERO),  3 ),
      DgVertTriVals(  2,  1, 5, true,  DgDVec2D( M_ONE,   M_ZERO),  0 )
   },

   {  // vert 4

      DgVertTriVals(  4,  3, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  4,  8, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  8, 12, 2, true,  DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals( -1, -1, 3, false, DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals(  3,  7, 4, true,  DgDVec2D( M_ZERO,  M_ZERO),  3 ),
      DgVertTriVals(  3,  2, 5, true,  DgDVec2D( M_ONE,   M_ZERO),  0 )
   },

   {  // vert 5

      DgVertTriVals(  5,  4, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  5,  9, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  9, 13, 2, true,  DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals( -1, -1, 3, false, DgDVec2D(-M_HALF,  M_SIN60), 1 ),
      DgVertTriVals(  4,  8, 4, true,  DgDVec2D( M_ZERO,  M_ZERO),  3 ),
      DgVertTriVals(  4,  3, 5, true,  DgDVec2D( M_ONE,   M_ZERO),  0 )
   },

   {  // vert 6

      DgVertTriVals(  6, 10, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),   1 ),
      DgVertTriVals(  6, 15, 1, true,  DgDVec2D(-M_HALF, -M_SIN60),  4 ),
      DgVertTriVals( 10, 19, 2, true,  DgDVec2D( M_ZERO,  M_ZERO),  -1 ),
      DgVertTriVals( 10, 14, 3, true,  DgDVec2D(-M_HALF,  M_SIN60),  2 ),
      DgVertTriVals( -1, -1, 4, false, DgDVec2D(-M_HALF,  M_SIN60),  1 ),
      DgVertTriVals(  1,  5, 5, true,  DgDVec2D( M_HALF, -M_SIN60),  4 )
   },

   {  // vert 7

      DgVertTriVals(  7, 11, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),   1 ),
      DgVertTriVals(  7, 16, 1, true,  DgDVec2D(-M_HALF, -M_SIN60),  4 ),
      DgVertTriVals(  6, 15, 2, true,  DgDVec2D( M_ZERO,  M_ZERO),  -1 ),
      DgVertTriVals(  6, 10, 3, true,  DgDVec2D(-M_HALF,  M_SIN60),  2 ),
      DgVertTriVals( -1, -1, 4, false, DgDVec2D(-M_HALF,  M_SIN60),  1 ),
      DgVertTriVals(  2,  6, 5, true,  DgDVec2D( M_HALF, -M_SIN60),  4 )
   },

   {  // vert 8

      DgVertTriVals(  8, 12, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),   1 ),
      DgVertTriVals(  8, 17, 1, true,  DgDVec2D(-M_HALF, -M_SIN60),  4 ),
      DgVertTriVals(  7, 16, 2, true,  DgDVec2D( M_ZERO,  M_ZERO),  -1 ),
      DgVertTriVals(  7, 11, 3, true,  DgDVec2D(-M_HALF,  M_SIN60),  2 ),
      DgVertTriVals( -1, -1, 4, false, DgDVec2D(-M_HALF,  M_SIN60),  1 ),
      DgVertTriVals(  3,  7, 5, true,  DgDVec2D( M_HALF, -M_SIN60),  4 )
   },

   {  // vert 9

      DgVertTriVals(  9, 13, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),   1 ),
      DgVertTriVals(  9, 18, 1, true,  DgDVec2D(-M_HALF, -M_SIN60),  4 ),
      DgVertTriVals(  8, 17, 2, true,  DgDVec2D( M_ZERO,  M_ZERO),  -1 ),
      DgVertTriVals(  8, 12, 3, true,  DgDVec2D(-M_HALF,  M_SIN60),  2 ),
      DgVertTriVals( -1, -1, 4, false, DgDVec2D(-M_HALF,  M_SIN60),  1 ),
      DgVertTriVals(  4,  8, 5, true,  DgDVec2D( M_HALF, -M_SIN60),  4 )
   },

   {  // vert 10

      DgVertTriVals( 10, 14, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),   1 ),
      DgVertTriVals( 10, 19, 1, true,  DgDVec2D(-M_HALF, -M_SIN60),  4 ),
      DgVertTriVals(  9, 18, 2, true,  DgDVec2D( M_ZERO,  M_ZERO),  -1),
      DgVertTriVals(  9, 13, 3, true,  DgDVec2D(-M_HALF,  M_SIN60),  2 ),
      DgVertTriVals( -1, -1, 4, false, DgDVec2D(-M_HALF,  M_SIN60),  1 ),
      DgVertTriVals(  5,  9, 5, true,  DgDVec2D( M_HALF, -M_SIN60),  4 )
   },

   {  // vert 11

      DgVertTriVals(  8, 17, 0, true,  DgDVec2D(-M_HALF, -M_SIN60),  3 ),
      DgVertTriVals(  9, 18, 1, true,  DgDVec2D(-M_ONE,   M_ZERO),   2 ),
      DgVertTriVals( 10, 19, 2, true,  DgDVec2D(-M_HALF,  M_SIN60),  1 ),
      DgVertTriVals(  6, 15, 3, true,  DgDVec2D( M_HALF,  M_SIN60),  0 ),
      DgVertTriVals( -1, -1, 4, false, DgDVec2D( M_ZERO,  M_ZERO),   0 ),
      DgVertTriVals(  7, 16, 5, true,  DgDVec2D( M_HALF, -M_SIN60), -2 )
   }

};

////////////////////////////////////////////////////////////////////////////////
const DgVertTriVals DgVertex2DDRF::triTable_[20] = {

      DgVertTriVals(  1,  0, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  2,  1, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  3,  2, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  4,  3, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  5,  4, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  1,  5, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  2,  6, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  3,  7, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  4,  8, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  5,  9, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  6, 10, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  7, 11, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  8, 12, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  9, 13, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals( 10, 14, 0, true,  DgDVec2D( M_ZERO,  M_ZERO),  1 ),
      DgVertTriVals(  6, 15, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  7, 16, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  8, 17, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals(  9, 18, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
      DgVertTriVals( 10, 19, 1, true,  DgDVec2D(-M_HALF, -M_SIN60), 4 ),
};

////////////////////////////////////////////////////////////////////////////////
const char*
DgQ2DDRF::str2add (DgQ2DDCoord* add, const char* str, char delimiter) const
{
   if (!add) add = new DgQ2DDCoord();

   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   // get the quadNum

   char* tok = strtok(tmpStr, delimStr);
   int q = -1;
   if (!tok || sscanf(tok, "%d", &q) != 1)
   {
      ::report("DgQ2DDRF::fromString() invalid value in string " + ((tok) ? std::string(tok) : std::string("\"\"")),
               DgBase::Fatal);
   }

   const char* tmp = &(str[strlen(tok) + 1]);
   DgDVec2D vec;
   tmp = vec.fromString(tmp, delimiter);

   *add = DgQ2DDCoord(q, vec);

   return tmp;

} // const char* DgQ2DDRF::str2add

////////////////////////////////////////////////////////////////////////////////
const char*
DgVertex2DDRF::str2add (DgVertex2DDCoord* add, const char* str, char delimiter)                                                                           const
{
   if (!add) add = new DgVertex2DDCoord();

   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   // get the vertNum

   char* tok = strtok(tmpStr, delimStr);
   int vNum = -1;
   if (!tok || sscanf(tok, "%d", &vNum) != 1)
   {
      ::report("DgVertex2DDCoord::fromString() invalid value in string " +
               ((tok) ? std::string(tok) : std::string("\"\"")), DgBase::Fatal);
   }

   // get the triNum

   tok = strtok(NULL, delimStr);
   int tNum = -1;
   if (!tok || sscanf(tok, "%d", &tNum) != 1)
   {
      ::report("DgVertex2DDCoord::fromString() invalid value in string " +
               ((tok) ? std::string(tok) : std::string("\"\"")), DgBase::Fatal);
   }

   // get the keeper

   tok = strtok(NULL, delimStr);

   bool keep = true;;

   if (strcmp(tok, "keep") == 0)
      keep = true;
   else if (strcmp(tok, "nokeep") == 0)
      keep = false;
   else
   {
      ::report("DgVertex2DDCoord::fromString() invalid value in string " +
               std::string(tok), DgBase::Fatal);
   }

   const char* tmp = &(str[(tok - tmpStr) + strlen(tok) + 1]);
   DgDVec2D vec;
   tmp = vec.fromString(tmp, delimiter);

   *add = DgVertex2DDCoord(keep, vNum, tNum, vec);

   return tmp;

} // const char* DgVertex2DDRF::str2add

////////////////////////////////////////////////////////////////////////////////
DgQ2DDtoIConverter::DgQ2DDtoIConverter (
   const DgRF<DgQ2DDCoord, long double>& from, const DgRF<DgQ2DICoord, long long int>& to)
  : DgConverter<DgQ2DDCoord, long double, DgQ2DICoord, long long int>(from, to),
    pIDGG_ (0)
{
   pIDGG_ = dynamic_cast<const DgIDGGBase*>(&toFrame());

   if (!pIDGG_)
   {
      report("DgQ2DDtoIConverter::DgQ2DDtoIConverter(): "
      " toFrame not of type DgIDGGBase", DgBase::Fatal);
   }

} // DgQ2DDtoIConverter::DgQ2DDtoIConverter

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord
DgQ2DDtoIConverter::convertTypedAddress (const DgQ2DDCoord& addIn) const
{
   DgLocation* loc = IDGG().ccFrame().makeLocation(addIn.coord());

#if DGDEBUG
dgcout << "\nDgQ2DDtoIConverter::convertTypedAddress loc: " << *loc << std::endl;
#endif

    IDGG().grid2D().convert(loc);

//dgcout << "XX " << addIn << " " << *loc << std::endl;

#if DGDEBUG
dgcout << " ---> A. " << *loc << std::endl;
#endif

   DgIVec2D coord = *IDGG().grid2D().getAddress(*loc);
   delete loc;

   int quadNum = addIn.quadNum();

#if DGDEBUG
dgcout << " ---> B. " << coord << std::endl;
#endif

    long long int maxI = IDGG().maxI();
    long long int maxJ = IDGG().maxJ();
    long long int topEdgeI = maxI + 1;
    long long int topEdgeJ = maxJ + 1;
    /*
    long long int maxTopOverageI = (IDGG().aperture() == 7 && IDGG().isClassI()) ? topEdgeI + 1 : topEdgeI;
    long long int maxTopOverageJ = (IDGG().aperture() == 7 && IDGG().isClassI()) ? topEdgeJ + 1 : topEdgeJ;
    long long int minBottomI = (IDGG().aperture() == 7 && IDGG().isClassI()) ? -1 : 0;
    long long int minBottomJ = (IDGG().aperture() == 7 && IDGG().isClassI()) ? -1 : 0;
     */
    /*
     long long int maxTopOverageI = (IDGG().aperture() == 7 && ! ? topEdgeI + 2 : topEdgeI;
    long long int maxTopOverageJ = (IDGG().aperture() == 7) ? topEdgeJ + 2 : topEdgeJ;
     */
    /*
    long long int maxTopOverageI = (IDGG().aperture() == 7 && IDGG().isClassI()) ? topEdgeI + 2 : topEdgeI;
    long long int maxTopOverageJ = (IDGG().aperture() == 7 && IDGG().isClassI()) ? topEdgeJ + 2 : topEdgeJ;
     */

    long long int maxTopOverageI = (IDGG().aperture() == 7) ? topEdgeI + 2 : topEdgeI;
    long long int maxTopOverageJ = (IDGG().aperture() == 7) ? topEdgeJ + 2 : topEdgeJ;

    long long int minBottomI = (IDGG().aperture() == 7) ? -2 : 0;
    long long int minBottomJ = (IDGG().aperture() == 7) ? -2 : 0;

#if DGDEBUG
    std::cout << maxI << " " << maxJ << " " << topEdgeI << " " << topEdgeJ << std::endl;
    std::cout << minBottomI << " " << minBottomJ << " " << maxTopOverageI << " " << maxTopOverageJ << std::endl;
#endif

   // if out of range check for possible round-off error
   const long double nudge = 0.0000001L;
   bool underI, underJ, overI, overJ;
   underI = coord.i() < minBottomI;
   underJ = coord.j() < minBottomJ;
   overI = coord.i() > maxTopOverageI;
   overJ = coord.j() > maxTopOverageJ;

   DgDVec2D tmp(addIn.coord());
   bool overage = false;
   if (underI || underJ) {
      overage = true;
      tmp.setX(tmp.x() + nudge);
      tmp.setY(tmp.y() + nudge);
   } else if (overI || overJ) {
      overage = true;
      tmp.setX(tmp.x() - nudge);
      tmp.setY(tmp.y() - nudge);
   }

   if (overage) {
      // nudge the incoming point and try again
      loc = IDGG().ccFrame().makeLocation(tmp);
      IDGG().grid2D().convert(loc);
      coord = *IDGG().grid2D().getAddress(*loc);
      delete loc;

      // reset the overage conditions
      underI = coord.i() < minBottomI;
      underJ = coord.j() < minBottomJ;
      overI = coord.i() > maxTopOverageI;
      overJ = coord.j() > maxTopOverageJ;

      // are we good?
      if (underI || underJ || overI || overJ)
         report("DgQ2DDtoIConverter::convertTypedAddress(): "
                "coordinate out of range: " + std::to_string(quadNum) + " " + (std::string) coord, DgBase::Fatal);
   }

#if DGDEBUG
dgcout << " ---> C. " << coord << std::endl;
#endif
   // we'll reuse the booleans above set to based on the actual quad i,j ranges
   underI = coord.i() < 0;
   underJ = coord.j() < 0;
   overI = coord.i() > maxI;
   overJ = coord.j() > maxJ;
   int numOver = underI + underJ + overI + overJ; // works because bool is an int

    if (numOver) {
        //if (overI || overJ) {
        const DgQuadEdgeCells& ec = IDGG().edgeTable(quadNum);

        // special case first
        if (overI && overJ) {
#if DGDEBUG
            std::cout << "BOTH OVER: " << !ec.isType0() << " " << quadNum << " " << coord.i() << " " << coord.j();
#endif
            // must be upper right corner
            if (ec.isType0()) {
                quadNum = ec.upQuad();
                coord = DgIVec2D(0, 0);
            } else { // TypeI
                quadNum = ec.rightQuad();
                coord = DgIVec2D(0, 0);
            }
        } else if (numOver > 1) {
            report("DgQ2DDtoIConverter::convertTypedAddress(): "
                   "coordinate has multiple overages: " + std::to_string(quadNum) + " " + (std::string) coord, DgBase::Fatal);
        } else if (underI) {
#if DGDEBUG
            std::cout << "UNDER I: " << !ec.isType0() << " " << quadNum << " " << coord.i() << " " << coord.j();
#endif
            quadNum = ec.leftQuad();
            if (ec.isType0()) {
                coord = DgIVec2D(topEdgeJ - coord.j() + coord.i(), topEdgeJ + coord.i());
            } else { // TypeI
                coord.setI(topEdgeI + coord.i());
            }
        } else if (underJ) {
#if DGDEBUG
            std::cout << "UNDER J: " << !ec.isType0() << " " << quadNum << " " << coord.i() << " " << coord.j();
#endif
            quadNum = ec.downQuad();
            if (ec.isType0()) {
                coord = DgIVec2D(coord.i(), topEdgeJ + coord.j());
            } else { // TypeI
                coord = DgIVec2D(topEdgeJ + coord.j(), (topEdgeI - coord.i()) + coord.j());
            }
        } else if (overI) {
#if DGDEBUG
            std::cout << "OVER I: " << !ec.isType0() << " " << quadNum << " " << coord.i() << " " << coord.j();
#endif
            if (ec.isType0()) {
                quadNum = ec.rightQuad();
                coord.setI(coord.i() - topEdgeI);
            } else { // TypeI
                if (coord.j() == 0) {
                    quadNum = ec.loneVert();
                    coord = DgIVec2D(0, 0);
                } else {
                    quadNum = ec.rightQuad();
                    long long int iOverage = coord.i() - topEdgeI;
                    coord = DgIVec2D((topEdgeJ - coord.j()) + iOverage, iOverage);
                }
            }
        } else if (overJ) {
#if DGDEBUG
            std::cout << "OVER J: " << !ec.isType0() << " " << quadNum << " " << coord.i() << " " << coord.j();
#endif
            if (ec.isType0()) {
                if (coord.i() == 0) {
                    quadNum = ec.loneVert();
                    coord = DgIVec2D(0, 0);
                } else {
                    quadNum = ec.upQuad();
                    long long int jOverage = coord.j() - topEdgeJ;
                    coord = DgIVec2D(jOverage, topEdgeI - coord.i() + jOverage);
                }
            } else { // TypeI
                quadNum = ec.upQuad();
                coord.setJ(coord.j() - topEdgeJ);
            }
        }
    }

#if DGDEBUG
    if (underI || underJ || overI || overJ)
        std::cout << " -> " << quadNum << " " << coord.i() << " " << coord.j() << std::endl;
#endif

#if DGDEBUG
dgcout << " ---> D. " << coord << std::endl;
#endif

    // reset the overage conditions
    underI = coord.i() < 0;
    underJ = coord.j() < 0;
    overI = coord.i() > maxI;
    overJ = coord.j() > maxJ;

    // are we good?
    if (underI || underJ || overI || overJ)
       report("DgQ2DDtoIConverter::convertTypedAddress(): "
           "final coordinate out of range: " + std::to_string(quadNum) + " " + (std::string) coord, DgBase::Fatal);

   DgQ2DICoord result(quadNum, coord);

   return result;

} // DgQ2DICoord DgQ2DDtoIConverter::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
DgQ2DItoDConverter::DgQ2DItoDConverter (const DgRF<DgQ2DICoord, long long int>& from,
                                        const DgRF<DgQ2DDCoord, long double>& to)
        : DgConverter<DgQ2DICoord, long long int, DgQ2DDCoord, long double> (from, to),
          pIDGG_ (NULL)
{
   pIDGG_ = dynamic_cast<const DgIDGGBase*>(&fromFrame());

   if (!pIDGG_)
   {
      report("DgQ2DItoDConverter::DgQ2DItoDConverter(): "
         " fromFrame not of type DgIDGGBase", DgBase::Fatal);
   }

} // DgQ2DItoDConverter::DgQ2DItoDConverter

////////////////////////////////////////////////////////////////////////////////
DgQ2DDCoord
DgQ2DItoDConverter::convertTypedAddress (const DgQ2DICoord& addIn) const
{
   DgLocation* loc = IDGG().grid2D().makeLocation(addIn.coord());
    IDGG().ccFrame().convert(loc);
    DgDVec2D coord = *IDGG().ccFrame().getAddress(*loc);
   delete loc;

   DgQ2DDCoord result(addIn.quadNum(), coord);

   return result;

} // DgQ2DDCoord DgQ2DItoDConverter::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
DgVertex2DDToQ2DDConverter::DgVertex2DDToQ2DDConverter (
   const DgRF<DgVertex2DDCoord, long double>& from,
   const DgRF<DgQ2DDCoord, long double>& to)
  : DgConverter<DgVertex2DDCoord, long double, DgQ2DDCoord, long double>(from, to)
{

} // DgVertex2DDToQ2DDConverter::DgVertex2DDToQ2DDConverter

////////////////////////////////////////////////////////////////////////////////
DgQ2DDCoord
DgVertex2DDToQ2DDConverter::convertTypedAddress
                                      (const DgVertex2DDCoord& addIn) const
{
   DgQ2DDCoord result(addIn.vertNum(), addIn.coord());

   return result;

} // DgQ2DDCoord DgVertex2DDToQ2DDConverter::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
DgQ2DDtoVertex2DDConverter::DgQ2DDtoVertex2DDConverter
          (const DgRF<DgQ2DDCoord, long double>& from,
           const DgRF<DgVertex2DDCoord, long double>& to)
  : DgConverter<DgQ2DDCoord, long double, DgVertex2DDCoord, long double>(from, to)
{

} // DgQ2DDtoVertex2DDConverter::DgQ2DDtoVertex2DDConverter

////////////////////////////////////////////////////////////////////////////////
// Private helper method to compute subtriangle region:
int
DgQ2DDtoVertex2DDConverter::compute_subtriangle
                    (const long double& x, const long double& y) const
{
 const long double tol = 0.000000000000001L;

 long double xs = M_SQRT3 * x;

 long double xpp =	 xs + tol,
	xmp =	-xs + tol,
	xpm =	 xs - tol,
	xmm =	-xs - tol;

   if (y >= xmm && y > xpp)
   {
	return 0;
   }

   if ((fabsl(y) <= tol && fabsl(x) <= tol)  ||
       (y <= xpp && y >= (0.0 - tol)))
   {
      return 1;
   }

   if (y < (0.0 - tol) && y > xmp)
   {
      return 2;
   }

   if (y <= xmp && y < xpm)
   {
      return 3;
   }

   if (y >= xpm && y < (0.0 - tol))
   {
      return 4;
   }

   if (y >= (0.0 - tol) && y < xmm)
   {
      return 5;
   }

   report("Dg2DDtoVertex2DDConverter value out of hex", DgBase::Fatal);

   return -1;
}

////////////////////////////////////////////////////////////////////////////////
DgVertex2DDCoord
DgQ2DDtoVertex2DDConverter::convertTypedAddress (const DgQ2DDCoord& addIn) const
{
   int subTri = compute_subtriangle(addIn.coord().x(), addIn.coord().y());

   const DgVertTriVals& st = DgVertex2DDRF::vertTable(addIn.quadNum(), subTri);

   DgVertex2DDCoord newCoord(st.keep());
   if (newCoord.keep())
   {
      newCoord.setVertNum(addIn.quadNum());
      newCoord.setTriNum(subTri);
      newCoord.setCoord(addIn.coord());
   }
   else
   {
      newCoord.setVertNum(-1);
      newCoord.setTriNum(-1);
   }

   return newCoord;

} // DgVertex2DDCoord DgQ2DDtoVertex2DDConverter::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
DgVertex2DDCoord
DgProjTriToVertex2DD::convertTypedAddress (const DgProjTriCoord& addIn) const
{
   const DgVertTriVals& triVals = DgVertex2DDRF::triTable(addIn.triNum());

   DgVertex2DDCoord newCoord(true, triVals.quadNum(), addIn.triNum());

   DgDVec2D tmpCoord(addIn.coord());
   tmpCoord.rotate(triVals.rot60() * 60.0L);
   tmpCoord -= triVals.trans();

   newCoord.setCoord(tmpCoord);

   return newCoord;

} // DgVertex2DDCoord DgProjTriToVertex2DD::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
DgProjTriCoord
DgVertex2DDtoProjTri::convertTypedAddress (const DgVertex2DDCoord& addIn) const
{
   const DgVertTriVals& triVal =
                   DgVertex2DDRF::vertTable(addIn.vertNum(), addIn.triNum());

   //const DgVertTriVals& triVals = DgVertex2DDRF::triTable(addIn.triNum());

   DgDVec2D tmpCoord(addIn.coord());
   tmpCoord += triVal.trans();
   tmpCoord.rotate(triVal.rot60() * -60.0L);

   DgProjTriCoord newCoord(triVal.triNum(), tmpCoord);

   return newCoord;

} // DgProjTriCoord DgVertex2DDtoProjTri::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
DgPlaneTriProj::DgPlaneTriProj (const DgRF<DgProjTriCoord, long double>& from,
                                const DgRF<DgDVec2D, long double>& to)
         : DgConverter<DgProjTriCoord, long double, DgDVec2D, long double>(from, to),
           pPlaneTriRF_ (0)
{
   pPlaneTriRF_= dynamic_cast<const DgPlaneTriRF*>(&toFrame());

   if (!pPlaneTriRF_)
   {
      report("DgPlaneTriProj::DgPlaneTriProj(): "
        " toFrame not of type DgPlaneTriRF", DgBase::Fatal);
   }

} // DgPlaneTriProj::DgPlaneTriProj

////////////////////////////////////////////////////////////////////////////////
DgDVec2D
DgPlaneTriProj::convertTypedAddress (const DgProjTriCoord& addIn) const
{
   //DgDVec2D scaledPt(addIn.coord().x() / snyedglen,
                           //addIn.coord().y() / snyedglen);
   DgDVec2D newPt(addIn.coord());
   planeTriRF().icosaMap().translate(addIn.triNum(), &newPt);

   return newPt;

} // DgDVec2D DgPlaneTriProj::convertTypedAddress

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
