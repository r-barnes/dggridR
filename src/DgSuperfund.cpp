#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

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
// DgSuperfund.cpp: DgSuperfund class implementation
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

