////////////////////////////////////////////////////////////////////////////////
//
// DgIDGG.cpp: DgIDGG class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <limits>
#include <cstdint>

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

const DgQ2DICoord DgQ2DICoord::undefDgQ2DICoord(-1, 
                                     DgIVec2D(std::numeric_limits<std::int64_t>::max(), std::numeric_limits<std::int64_t>::max()));
const DgQ2DDCoord DgQ2DDCoord::undefDgQ2DDCoord(-1, 
                                     DgDVec2D(std::numeric_limits<long double>::max(), std::numeric_limits<long double>::max()));

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
const DgQuadEdgeCells DgIDGG::edgeTable_[12] = {

   DgQuadEdgeCells(0,  true,  0,  0, 0), // quad 0 should never occur
   DgQuadEdgeCells(1,  true,  0,  2, 6),
   DgQuadEdgeCells(2,  true,  0,  3, 7),
   DgQuadEdgeCells(3,  true,  0,  4, 8),
   DgQuadEdgeCells(4,  true,  0,  5, 9),
   DgQuadEdgeCells(5,  true,  0,  1, 10),
   DgQuadEdgeCells(6,  false, 11, 2, 7),
   DgQuadEdgeCells(7,  false, 11, 3, 8),
   DgQuadEdgeCells(8,  false, 11, 4, 9),
   DgQuadEdgeCells(9,  false, 11, 5, 10),
   DgQuadEdgeCells(10, false, 11, 1, 6),
   DgQuadEdgeCells(11, false, 11, 0, 0)  // quad 11 should never occur

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
   int q;
   if (sscanf(tok, "%d", &q) != 1)
   {
      ::report("DgQ2DDRF::fromString() invalid value in string " + string(tok), 
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
   int vNum;
   if (sscanf(tok, "%d", &vNum) != 1)
   {
      ::report("DgVertex2DDCoord::fromString() invalid value in string " +
               string(tok), DgBase::Fatal);
   }

   // get the triNum

   tok = strtok(NULL, delimStr);
   int tNum;
   if (sscanf(tok, "%d", &tNum) != 1)
   {
      ::report("DgVertex2DDCoord::fromString() invalid value in string " +
               string(tok), DgBase::Fatal);
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
               string(tok), DgBase::Fatal);
   }

   const char* tmp = &(str[(tok - tmpStr) + strlen(tok) + 1]);
   DgDVec2D vec;
   tmp = vec.fromString(tmp, delimiter);

   *add = DgVertex2DDCoord(keep, vNum, tNum, vec);

   return tmp;

} // const char* DgVertex2DDRF::str2add

////////////////////////////////////////////////////////////////////////////////
const char*
DgInterleaveRF::str2add (DgInterleaveCoord* add, const char* str, 
                         char delimiter) const
{
   if (!add) add = new DgInterleaveCoord();

   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);
   char* tok = strtok(tmpStr, delimStr);

   add->setValString(tok);

   delete[] tmpStr;

   unsigned int offset = strlen(tok) + 1;
   if (offset >= strlen(str)) return 0;
   else return &str[offset];

} // const char* DgInterleaveRF::str2add

////////////////////////////////////////////////////////////////////////////////
const char*
DgIDGG::str2add (DgQ2DICoord* add, const char* str, char delimiter) const
{
   if (!add) add = new DgQ2DICoord();

   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   // get the quadNum

   char* tok = strtok(tmpStr, delimStr);
   int q;
   if (sscanf(tok, "%d", &q) != 1)
   {
      ::report("DgQ2DIRF::fromString() invalid value in string " +
               string(tok), DgBase::Fatal);
   }

   const char* tmp = &(str[strlen(tok) + 1]);
   DgIVec2D vec;
   tmp = vec.fromString(tmp, delimiter);

   *add = DgQ2DICoord(q, vec);

   return tmp;

} // const char* DgIDGG::str2add

////////////////////////////////////////////////////////////////////////////////
DgQ2DDtoIConverter::DgQ2DDtoIConverter (
   const DgRF<DgQ2DDCoord, long double>& from, const DgRF<DgQ2DICoord, std::int64_t>& to)
  : DgConverter<DgQ2DDCoord, long double, DgQ2DICoord, std::int64_t>(from, to),
    pIDGG_ (0) 
{ 
   pIDGG_ = dynamic_cast<const DgIDGG*>(&toFrame());

   if (!pIDGG_)
   {
      report("DgQ2DDtoIConverter::DgQ2DDtoIConverter(): "
      " toFrame not of type DgIDGG", DgBase::Fatal);
   }

} // DgQ2DDtoIConverter::DgQ2DDtoIConverter

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord 
DgQ2DDtoIConverter::convertTypedAddress (const DgQ2DDCoord& addIn) const
{
   DgLocation* loc = IDGG().ccFrame().makeLocation(addIn.coord());

#if DGDEBUG
cout << "DgQ2DDtoIConverter::convertTypedAddress loc: " << *loc << endl;
#endif

    IDGG().grid2D().convert(loc);

#if DGDEBUG
cout << " ---> " << *loc << endl;
#endif

   DgIVec2D coord = *IDGG().grid2D().getAddress(*loc);
   delete loc;

   int quadNum = addIn.quadNum();

   const long double nudge = 0.0000001L;
   if (coord.i() < 0 || coord.j() < 0) // maybe round-off error?
   {
      DgDVec2D tmp(addIn.coord());

      tmp.setX(tmp.x() + nudge);
      tmp.setY(tmp.y() + nudge);

      loc = IDGG().ccFrame().makeLocation(tmp);
      IDGG().grid2D().convert(loc);
      coord = *IDGG().grid2D().getAddress(*loc);
      delete loc;
   }

   std::int64_t edgeI = IDGG().maxI() + 1;
   std::int64_t edgeJ = IDGG().maxJ() + 1;
   if (coord.i() > edgeI || coord.j() > edgeJ) // maybe round-off error?
   {
      DgDVec2D tmp(addIn.coord());

      tmp.setX(tmp.x() - nudge);
      tmp.setY(tmp.y() - nudge);

      loc = IDGG().ccFrame().makeLocation(tmp);
      IDGG().grid2D().convert(loc);
      coord = *IDGG().grid2D().getAddress(*loc);
      delete loc;
   }

   if (coord.i() < 0 || coord.j() < 0 || 
       coord.i() > edgeI || coord.j() > edgeJ)
   {
      report("DgQ2DDtoIConverter::convertTypedAddress(): "
             " coordinate out of range: " + (string) coord, DgBase::Fatal);
   }
   else if (coord.i() == edgeI || coord.j() == edgeJ)
   {
      const DgQuadEdgeCells& ec = IDGG().edgeTable(quadNum);

      if (ec.isType0())
      {
         if (coord.j() == edgeJ)
         {
            if (coord.i() == 0)
            {
               quadNum = ec.loneVert();
               coord = DgIVec2D(0, 0);
            }
            else
            {
               quadNum = ec.upQuad();
               coord = DgIVec2D(0, edgeI - coord.i());
            }
         }
         else // i == edgeI
         {
            quadNum = ec.rightQuad();
            coord.setI(0);
         }
      }
      else // type 1
      {
         if (coord.i() == edgeI)
         {
            if (coord.j() == 0)
            {
               quadNum = ec.loneVert();
               coord = DgIVec2D(0, 0);
            }
            else
            {
               quadNum = ec.rightQuad();
               coord = DgIVec2D(edgeJ - coord.j(), 0);
            }
         }
         else // j == edgeJ
         {
            quadNum = ec.upQuad();
            coord.setJ(0);
         }
      }
   } 

   DgQ2DICoord result(quadNum, coord);

   return result;

} // DgQ2DICoord DgQ2DDtoIConverter::convertTypedAddress 

////////////////////////////////////////////////////////////////////////////////
DgQ2DItoDConverter::DgQ2DItoDConverter (const DgRF<DgQ2DICoord, std::int64_t>& from,
                                        const DgRF<DgQ2DDCoord, long double>& to)
        : DgConverter<DgQ2DICoord, std::int64_t, DgQ2DDCoord, long double> (from, to),
          pIDGG_ (NULL)
{ 
   pIDGG_ = dynamic_cast<const DgIDGG*>(&fromFrame());

   if (!pIDGG_)
   {
      report("DgQ2DItoDConverter::DgQ2DItoDConverter(): "
         " fromFrame not of type DgIDGG", DgBase::Fatal);
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
DgQ2DItoInterleaveConverter::DgQ2DItoInterleaveConverter 
                (const DgRF<DgQ2DICoord, std::int64_t>& from,
                 const DgRF<DgInterleaveCoord, std::int64_t>& to)
        : DgConverter<DgQ2DICoord, std::int64_t, DgInterleaveCoord, std::int64_t> (from, to),
          pIDGG_ (NULL)
{ 
   pIDGG_ = dynamic_cast<const DgIDGG*>(&fromFrame());

   if (!pIDGG_)
   {
      report("DgQ2DItoInterleaveConverter::DgQ2DItoInterleaveConverter(): "
         " fromFrame not of type DgIDGG", DgBase::Fatal);
   }

} // DgQ2DItoInterleaveConverter::DgQ2DItoInterleaveConverter 

////////////////////////////////////////////////////////////////////////////////
DgInterleaveCoord 
DgQ2DItoInterleaveConverter::convertTypedAddress 
                                               (const DgQ2DICoord& addIn) const
{
   string qstr = dgg::util::to_string(addIn.quadNum(), 2);

   int effRes = IDGG().res();       // effective resolution
   int effRadix = IDGG().radix();   // effective radix
   if (IDGG().aperture() == 3)    
   {
       effRadix = 3;
       effRes = (effRes + 1) / 2;
   }

   if (IDGG().gridTopo() == string("TRIANGLE")) effRes++; // adjust for long double j

//cout << "addIn " << addIn << endl;
   DgRadixString rs1(effRadix, addIn.coord().i(), effRes);
   DgRadixString rs2(effRadix, addIn.coord().j(), effRes);

//cout << "rs1 " << rs1 << endl;
//cout << "rs2 " << rs2 << endl;

   string addstr = qstr;
   if (IDGG().aperture() == 3)
   {
     if (IDGG().isClassI()) addstr = addstr + string("0");
     else addstr = addstr + string("1");
   }

   addstr = addstr + DgRadixString::digitInterleave(rs1, rs2);

//cout << "addstr " << addstr << endl;

   DgInterleaveCoord res;
   res.setValString(addstr);
//cout << "res " << res << endl;

   return res;

} // DgInterleaveCoord DgQ2DItoInterleaveConverter::convertTypedAddress 

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

   if ((fabs(y) <= tol && fabs(x) <= tol)  ||
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
DgIDGG::DgIDGG (const DgGeoSphRF& geoRF, const DgGeoCoord& vert0, 
       long double azDegs, unsigned int aperture, int res, const string& name,
       const string& gridTopo, const string& projType, bool isMixed43, 
       int numAp4, bool isSuperfund, int sfRes, unsigned int precision)
   : 
     DgDiscRF<DgQ2DICoord, DgGeoCoord, long double>(geoRF.network(), geoRF, name),
     sphIcosa_(0), geoRF_(geoRF), vert0_(vert0), azDegs_(azDegs),
     gridTopo_(gridTopo), projType_(projType), aperture_(aperture), res_(res),
     isMixed43_(isMixed43), numAp4_(numAp4), isSuperfund_(isSuperfund),
     sfRes_ (sfRes), precision_(precision), grid2D_(0), grid2DS_(0), ccFrame_(0),
     projTriRF_(0), vertexRF_(0), q2ddRF_(0), bndRF_(0), intRF_(0), planeRF_(0)
{ 
   initialize();

} // DgIDGG::DgIDGG

////////////////////////////////////////////////////////////////////////////////
DgIDGG::DgIDGG (const DgIDGG& rfIn)
   : DgDiscRF<DgQ2DICoord, DgGeoCoord, long double> (rfIn), 
        sphIcosa_(0), geoRF_(rfIn.geoRF()), vert0_(rfIn.vert0()), 
        azDegs_(rfIn.azDegs()), gridTopo_(rfIn.gridTopo()), 
        projType_ (rfIn.projType()), aperture_(rfIn.aperture()), 
        res_(rfIn.res()), isMixed43_(rfIn.isMixed43()), numAp4_(rfIn.numAp4()), 
        isSuperfund_(rfIn.isSuperfund()), sfRes_(rfIn.sfRes()), 
        precision_(rfIn.precision()), grid2D_(0), grid2DS_(0), ccFrame_(0), 
        projTriRF_(0), vertexRF_(0), q2ddRF_(0), bndRF_(0), intRF_(0), 
        planeRF_(0)
{ 
   initialize();

} // DgIDGG::DgIDGG

DgIDGG::~DgIDGG()
{
 /* Notice that we do not delete:
	grid2D_, grid2DS_
    ...this is because they are freed elsewhere (by DgHexGrid2DS.C), and a segfault
    will result. It would be good to resolve the "ambiguity" -JFW */

 delete sphIcosa_;

 delete ccFrame_;

 delete projTriRF_;
 delete vertexRF_;
 delete q2ddRF_;
 delete bndRF_;
 delete intRF_;
 delete planeRF_;
}

////////////////////////////////////////////////////////////////////////////////
void
DgIDGG::initialize (void)
{
   // verify parameter validity

   string apErrStr = string("DgIDGG::initialize(): invalid aperture " + 
                         dgg::util::to_string(aperture()) + 
                         string(" for grid topo ") + gridTopo());

   if (gridTopo() == "HEXAGON")
   {
      if (aperture() != 3 && aperture() != 4) 
         report(apErrStr, DgBase::Fatal);
   }
   else if (gridTopo() == "TRIANGLE" || gridTopo() == "DIAMOND")
   {
      if (aperture() != 4) report(apErrStr, DgBase::Fatal);
   }
   else
      report("DgIDGG::initialize(): invalid grid topo " + gridTopo(), 
             DgBase::Fatal);

   if (isMixed43())
      aperture_ = (res() > numAp4()) ? 3 : 4;

   // create some internal data structures

   undefLoc_ = makeLocation(undefAddress());
   sphIcosa_ = new DgSphIcosa(vert0(), azDegs());

   // setup some parameters

   isClassI_ = true; // default
   radix_ = (int) sqrt((long double) aperture());
   adjRes_ = res();

   if (gridTopo() == "HEXAGON")
   {
      isAligned_ = true;
      isCongruent_ = false;

      if (!isMixed43())
      {
         if (aperture() == 4)
         {
            isClassI_ = true;
            maxD_ = (std::int64_t) pow(2.0L, res()) - 1;
         }
         else // aperture 3
         {
            isClassI_ = !(res() % 2);
            if (!isClassI()) adjRes_ = res() + 1;
            maxD_ = (std::int64_t) pow(3.0L, (adjRes() / 2)) - 1;
         }
      }
      else // mixed43
      {
         if (res() <= numAp4())
         {
            isClassI_ = true;
            maxD_ = (std::int64_t) pow(2.0L, res()) - 1;
         }
         else
         {
            isClassI_ = !((res() - numAp4()) % 2);
            if (!isClassI()) adjRes_ = res() + 1;

            maxD_ = (std::int64_t) (pow(2.0L, numAp4()) *
                      pow(3.0L, ((adjRes() - numAp4()) / 2))) - 1;
         }
         // cout << "MAXD: " << maxD_ << endl;
      }
      maxI_ = maxD();
      maxJ_ = maxD();
      mag_ = maxD() + 1;
      firstAdd_ = DgQ2DICoord(0, DgIVec2D(0, 0));
      lastAdd_ = DgQ2DICoord(11, DgIVec2D(0, 0));
   }
   else
   {
      maxD_ = (std::int64_t) pow((long double) radix(), res()) - 1;
      maxI_ = maxD();
      maxJ_ = maxD();
      mag_ = maxD() + 1;
      if (gridTopo() == "TRIANGLE") 
      {
         isAligned_ = true; // only for aperture 4
         isCongruent_ = true;
         maxJ_ = (mag() * 2) - 1;
      }
      else // topo is DIAMOND
      {
         isAligned_ = false;
         isCongruent_ = true;
      }
      firstAdd_ = DgQ2DICoord(1, DgIVec2D(0, 0));
      lastAdd_ = DgQ2DICoord(10, DgIVec2D(maxI(), maxJ()));
   }

   // set-up local network to scale so that quad (and consequently tri) edge 
   // length is 1.0

   ccFrame_ = new DgContCartRF(locNet_, name() + "CC1");

   if (gridTopo() == "DIAMOND")
   {
      grid2DS_ = new DgDmdD4Grid2DS(locNet_, ccFrame(), res() + 1, aperture(), 
                                    isCongruent(), isAligned());
   }
   else if (gridTopo() == "HEXAGON")
   {
      grid2DS_ = new DgHexGrid2DS(locNet_, ccFrame(), adjRes() + 1, aperture(), 
        isCongruent(), isAligned(), "H2DS", isMixed43(), numAp4(), isSuperfund());
   }
   else if (gridTopo() == "TRIANGLE")
   {
      grid2DS_ = new DgTriGrid2DS(locNet_, ccFrame(), res() + 1, aperture(), 
                                  isCongruent(), isAligned());
   }

   grid2D_ = dynamic_cast<DgDiscRF2D*>(grid2DS().grids()[res()]);
   bndRF_ = new DgBoundedIDGG(*this);

   // create the intermediate RFs

   projTriRF_ = new DgProjTriRF(network(), name() + string("projTri"), 
                sphIcosa_);
   vertexRF_ = new DgVertex2DDRF(network(), name() + string("vertex"));
   q2ddRF_ = new DgQ2DDRF(network(), name() + string("q2dd"));
   intRF_ = new DgInterleaveRF(network(), name() + string("int"));
   planeRF_ = new DgPlaneTriRF(network(), name() + string("plane"));

   // create the converters; for convenience use where they are in overall
   // sequence for name

   DgIcosaProj* icosaProj = NULL; 
   if (projType() == "ISEA")
      icosaProj = new DgProjISEA(geoRF(), projTriRF());
   else if (projType() == "FULLER")
      icosaProj = new DgProjFuller(geoRF(), projTriRF());
   else
      report("DgIDGG::initialize(): invalid projection type " + projType(), 
             DgBase::Fatal);

   const DgConverterBase* c1to2 = &(icosaProj->forward());
   const DgConverterBase* c2to3 = new DgProjTriToVertex2DD(projTriRF(), vertexRF());
   const DgConverterBase* c3to4 = new DgVertex2DDToQ2DDConverter(vertexRF(), q2ddRF());
   const DgConverterBase* c4to5 = new DgQ2DDtoIConverter(q2ddRF(), *this);
   
   const DgConverterBase* c5to4 = new DgQ2DItoDConverter(*this, q2ddRF());
   const DgConverterBase* c4to3 = new DgQ2DDtoVertex2DDConverter(q2ddRF(), vertexRF());
   const DgConverterBase* c3to2 = new DgVertex2DDtoProjTri(vertexRF(), projTriRF());
   const DgConverterBase* c2to1 = &(icosaProj->inverse());

   DgConverterBase* toInt = new DgQ2DItoInterleaveConverter(*this, intRF());
   DgConverterBase* toPlane = new DgPlaneTriProj(projTriRF(), planeRF());

   // create the series converters that will replace the default DgDiscRF
   // converters

   vector<const DgConverterBase*> sc;
   sc.push_back(c1to2);
   sc.push_back(c2to3);
   sc.push_back(c3to4);
   sc.push_back(c4to5);
   new DgSeriesConverter(sc, true);
   sc.resize(0);
   
   sc.push_back(c5to4);
   sc.push_back(c4to3);
   sc.push_back(c3to2);
   sc.push_back(c2to1);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   //// now fill in the other connections; begin with from vertexRF and then
   //// connect to it as needed

   // vertexRF -> geoRF
   sc.push_back(c3to2);
   sc.push_back(c2to1);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // vertexRF -> Q2DI
   sc.push_back(c3to4);
   sc.push_back(c4to5);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // vertexRF -> projTriRF is c3to2 above

   // vertexRF -> planeRF
   sc.push_back(c3to2);
   sc.push_back(toPlane);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // vertexRF -> Q2DD is c3to4 above

   // vertexRF -> intRF
   sc.push_back(c3to4);
   sc.push_back(c4to5);
   sc.push_back(toInt);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   /// now do from projTriRF
   
   // projTriRF -> geoRF is c2to1 above

   // projTriRF -> Q2DI
   sc.push_back(c2to3);
   sc.push_back(network().getConverter(vertexRF(), *this));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // projTriRF -> vertexRF is c2to3 above
   // projTriRF -> planeRF is toPlane above

   // projTriRF -> Q2DD
   sc.push_back(c2to3);
   sc.push_back(c3to4);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // projTriRF -> intRF
   sc.push_back(c2to3);
   sc.push_back(network().getConverter(vertexRF(), intRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   /// do from Q2DD

   // Q2DD -> geoRF
   sc.push_back(c4to3);
   sc.push_back(network().getConverter(vertexRF(), geoRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DD -> Q2DI is c4to5 above
   // Q2DD -> vertexRF is c4to3 above

   // Q2DD -> projTriRF
   sc.push_back(c4to3);
   sc.push_back(network().getConverter(vertexRF(), projTriRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DD -> planeRF
   sc.push_back(c4to3);
   sc.push_back(network().getConverter(vertexRF(), planeRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DD -> intRF
   sc.push_back(c4to3);
   sc.push_back(network().getConverter(vertexRF(), intRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   /// do from Q2DI

   // Q2DI -> geoRF is series converter given above

   // Q2DI -> vertexRF
   sc.push_back(c5to4);
   sc.push_back(c4to3);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DI -> projTriRF
   sc.push_back(c5to4);
   sc.push_back(network().getConverter(q2ddRF(), projTriRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DI -> planeRF
   sc.push_back(c5to4);
   sc.push_back(network().getConverter(q2ddRF(), planeRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // Q2DI -> Q2DD is c5to4 above
   // Q2DI -> intRF is toInt above

   /// finally from geoRF

   // geoRF -> Q2DI is series converter given above

   // geoRF -> vertexRF
   sc.push_back(c1to2);
   sc.push_back(c2to3);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // geoRF -> projTriRF is c1to2 above

   // geoRF -> planeRF
   sc.push_back(network().getConverter(geoRF(), vertexRF()));
   sc.push_back(network().getConverter(vertexRF(), planeRF()));
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // geoRF -> Q2DD
   sc.push_back(c1to2);
   sc.push_back(c2to3);
   sc.push_back(c3to4);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   // geoRF -> intRF
   sc.push_back(network().getConverter(geoRF(), *this));
   sc.push_back(toInt);
   new DgSeriesConverter(sc, true);
   sc.resize(0);

   ///// calculate the statistics /////

   gridStats_.setPrecision(precision());
   gridStats_.setNCells(bndRF().size());

   long double tmpLen = DgGeoSphRF::icosaEdgeKM();
   if (gridTopo() == "TRIANGLE") tmpLen /= M_SQRT3;
   if (!isMixed43())
      gridStats_.setCellDistKM(tmpLen / pow(sqrt((long double) aperture()), res()));
   else // mixed43
   {
      if (res() < numAp4())
      {
         tmpLen = tmpLen / pow((long double) 2.0, (long double) res());
      }
      else
      {
         tmpLen = tmpLen / pow((long double) 2.0, (long double) numAp4());
         if (res() > numAp4())
            tmpLen = tmpLen / pow(M_SQRT3, (long double) (res() - numAp4()));
      }
      gridStats_.setCellDistKM(tmpLen);
   }

   if (gridTopo() == "HEXAGON")
   {
      // a = globeArea / ((#cells - 12) + (12 * 5/6))
      //   = globeArea / (#cells - 2);
      gridStats_.setCellAreaKM(DgGeoSphRF::totalAreaKM() / 
                       (gridStats_.nCells() - 2));
   }
   else if (gridTopo() == "DIAMOND")
      gridStats_.setCellAreaKM(DgGeoSphRF::totalAreaKM() / gridStats_.nCells());
   else if (gridTopo() == "TRIANGLE")
      gridStats_.setCellAreaKM(DgGeoSphRF::totalAreaKM() / gridStats_.nCells());

   gridStats_.setCLS(2.0L * 2.0L * DgGeoSphRF::earthRadiusKM() * 
                     asin(sqrt(gridStats_.cellAreaKM() / dgM_PI) / 
                     (2.0L * DgGeoSphRF::earthRadiusKM())));

} // DgIDGG::initialize

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGG::setVertices (const DgLocation& loc, DgPolygon& vec, 
                     int densify) const
{
   vec.clearAddress();
   backFrame().convert(vec);
  
   DgLocation tLoc(loc);
//cout << "*** " << loc << endl;
   convert(&tLoc);
//cout << "**** " << tLoc << endl;

   setAddVertices(*getAddress(tLoc), vec, densify);
   
} // void DgDiscRF::setVertices

////////////////////////////////////////////////////////////////////////////////
void
DgIDGG::setAddVertices (const DgQ2DICoord& add, DgPolygon& vec,
                        int densify) const
{
   DgLocation* tmpLoc = grid2D().makeLocation(add.coord());
//cout << "a: " << *tmpLoc << endl;
    DgPolygon dummy(ccFrame());
    vec = dummy;  // force empty RF to allow for network change
    grid2D().setVertices(*tmpLoc, vec);
   delete tmpLoc;

//cout << "A: " << vec << endl;
   ccFrame().convert(vec);
//cout << "B: " << vec << endl;

   // densify
   vec.densify(densify);
//cout << "C: " << vec << endl;
   
   // kludge to jump nets and add the quad number

   DgPolygon tmpVec(q2ddRF());
   vector<DgAddressBase*>& v = tmpVec.addressVec();
   for (int i = 0; i < vec.size(); i++)
   {
      v.push_back(new DgAddress<DgQ2DDCoord>(DgQ2DDCoord(add.quadNum(),
                       *(ccFrame().getAddress(vec[i])))));
   }
   vec = tmpVec;

//cout << "D: " << vec << endl;

   vertexRF().convert(vec);

//cout << "E: " << vec << endl;
   // do we need to look for non-keepers?

   if (!isCongruent() && add.coord() == DgIVec2D(0, 0))
   {
      // we need to explicitly go to vertexRF to look for non-keepers
      // to clip

      // JFW: possible overflow: keepers is smaller than v.size() below:
      unsigned int keepers = 0;

      vector<DgAddressBase*>& v = vec.addressVec();
      for (unsigned long i = 0; i < v.size(); i++)
      {
         DgAddress<DgVertex2DDCoord>& fullAdd =
                   dynamic_cast< DgAddress<DgVertex2DDCoord>& >(*v[i]);
         DgVertex2DDCoord& add = fullAdd.address();
         if (add.keep())
         {
            if (i > keepers)
            {
               DgAddress<DgVertex2DDCoord>& fullAdd2 =
                   dynamic_cast< DgAddress<DgVertex2DDCoord>& >(*v[keepers]);
               DgVertex2DDCoord& add2 = fullAdd2.address();
               add2 = add;
            }
            keepers++;
         }
      }

      v.resize(keepers);
   }

   // now convert to the geoRF

//cout << "F: " << vec << endl;
   geoRF().convert(&vec);
//cout << "G: " << vec << endl;

   // Release the Kraken... I mean, the vector's pointers:
   dgg::util::release(v);

} // DgIDGG::setAddVertices

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
