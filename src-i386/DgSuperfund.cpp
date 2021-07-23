////////////////////////////////////////////////////////////////////////////////
//
// DgSuperfund.cpp: DgSuperfund class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgSuperfund.h"
#include "DgBase.h"

////////////////////////////////////////////////////////////////////////////////
int sfRes2actualRes (int sfRes)
{
   int actRes[] = { 1, 2, 4, 6, 8, 10, 12, 14, 16, 17 }; 

   int res = 0;
   if (sfRes < 0 || sfRes > 9)
      ::report( "sfRes2actualRes(): invalid sfRes", DgBase::Fatal);
   else
      res = actRes[sfRes];

   return res;
}

////////////////////////////////////////////////////////////////////////////////
int actualRes2sfRes (int actualRes)
{
   // res #         0  1  2   3  4   5  6   7  8   9 10  11 12  13 14  15 16 17 }; 
   int sfRes[] = { -1, 0, 1, -1, 2, -1, 3, -1, 4, -1, 5, -1, 6, -1, 7, -1, 8, 9 }; 

   int res = 0;
   if (actualRes < 0 || actualRes > 17)
      ::report( "actualRes2sfRes(): invalid actualRes", DgBase::Fatal);
   else
      res = sfRes[actualRes];

   return res;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

