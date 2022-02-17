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
// DgBoundedHexC3C2RF2D.cpp: DgBoundedHexC3C2RF2D class implementation
//
// Version 7.0 - Kevin Sahr, 12/15/14
//
////////////////////////////////////////////////////////////////////////////////

#include <climits>

#include "DgBoundedHexC3C2RF2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
unsigned long long int
DgBoundedHexC3C2RF2D::seqNumAddress (const DgIVec2D& add) const
{
   DgIVec2D tVec = add - lowerLeft();
   unsigned long long int sNum = tVec.i() * numI() / 7;

   switch (tVec.i() % 7)
   {
      case 0: sNum += tVec.j() / 7;
              break;

      case 1: sNum += (tVec.j() - 5) / 7;
              break;

      case 2: sNum += (tVec.j() - 3) / 7;
              break;

      case 3: sNum += (tVec.j() - 1) / 7;
              break;

      case 4: sNum += (tVec.j() - 6) / 7;
              break;

      case 5: sNum += (tVec.j() - 4) / 7;
              break;

      case 6: sNum += (tVec.j() - 2) / 7;
              break;

   }

   if (!zeroBased()) sNum++;

   sNum = sNum / 3;

   return sNum;
} 
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


