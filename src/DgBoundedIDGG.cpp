#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2018 Kevin Sahr

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
// DgBoundedIDGG.cpp: DgBoundedIDGG class implementation
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgBoundedIDGG.h"
#include "DgBoundedRF2D.h"
#include "DgBoundedHexC2RF2D.h"
#include "DgBoundedHexC3RF2D.h"
#include "DgBoundedHexC3C2RF2D.h"
#include "DgIDGGBase.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBoundedIDGG::DgBoundedIDGG (const DgIDGGBase& IDGGin)
   : DgBoundedRF<DgQ2DICoord, DgGeoCoord, long double> (IDGGin, IDGGin.firstAdd(),
          IDGGin.lastAdd(), IDGGin.undefAddress(), false), IDGG_ (IDGGin),
          bnd2D_ (0)
{
//cout << "********** DgBoundedIDGG::DgBoundedIDGG" << endl;
//cout << " -- idgg().gridStats().nCells(): " << idgg().gridStats().nCells() << endl;
//cout << "     offsetPerQuad_ = (idgg().gridStats().nCells() - 2) / 10;

   // calculate the offsetPerQuad
   if (idgg().gridTopo() == dgg::topo::Hexagon)
      offsetPerQuad_ = (idgg().gridStats().nCells() - 2) / 10;
   else
   {
      setFirstAdd(DgQ2DICoord(1, DgIVec2D(0, 0)));
      setLastAdd(DgQ2DICoord(10, DgIVec2D(idgg().maxI(), idgg().maxJ())));

      if (idgg().gridTopo() == dgg::topo::Triangle)
         offsetPerQuad_ = idgg().mag() * (idgg().maxJ() + 1);
      else
         offsetPerQuad_ = idgg().mag() * idgg().mag();
   }

   // create the surrogate boundedRF2D
   if (idgg().gridTopo() == dgg::topo::Hexagon)
   {
      if (idgg().isClassIII())
      {
         if (idgg().isClassI()) // Class I/III
            bnd2D_ = new DgBoundedHexC3RF2D(idgg().grid2D(), DgIVec2D(0, 0), 
                  DgIVec2D(idgg().maxI(), idgg().maxJ()), 
                  idgg().gridStats().nCells());
         else // Class II/III
            bnd2D_ = new DgBoundedHexC3C2RF2D(idgg().grid2D(), DgIVec2D(0, 0), 
                  DgIVec2D(idgg().maxI(), idgg().maxJ()), 
                  idgg().gridStats().nCells());
      }
      else if (!idgg().isClassI()) // Class II
         bnd2D_ = new DgBoundedHexC2RF2D(idgg().grid2D(), DgIVec2D(0, 0), 
              DgIVec2D(idgg().maxI(), idgg().maxJ()));
      else // Class I
         bnd2D_ = new DgBoundedRF2D(idgg().grid2D(), DgIVec2D(0, 0), 
                            DgIVec2D(idgg().maxI(), idgg().maxJ()));
   }
   else
      bnd2D_ = new DgBoundedRF2D(idgg().grid2D(), DgIVec2D(0, 0), 
                                 DgIVec2D(idgg().maxI(), idgg().maxJ()));

   // calculate the total size
   size_ = 10 * offsetPerQuad();
   if (firstAdd().quadNum() == 0) size_ += 2;

   // validate the size
   unsigned long long int tmpSize = size();
   if (firstAdd().quadNum() == 0) tmpSize -= 2;
/*
cout << "mag: " << idgg().mag() << ", /3: " << idgg().mag() / 3 << ", size: " << size() << ", tmpSize: " << tmpSize << ", tmpSize / 10: " << tmpSize/10 << endl;
cout  << "quadNum: " << firstAdd().quadNum() << ", offsetPerQuad: " << offsetPerQuad() << endl;
*/
   if ((tmpSize / 10) != offsetPerQuad())
   {
/*
       report("DgBoundedIDGG::DgBoundedIDGG() invalid size setting due to "
              "possible overflow", DgBase::Warning);
*/
       validSize_ = false;
   }
   else validSize_ = true;

} // DgBoundedIDGG::DgBoundedIDGG

////////////////////////////////////////////////////////////////////////////////
bool
DgBoundedIDGG::validAddress (const DgQ2DICoord& add) const
{ 
   if (add == invalidAdd()) return false;

   if (add.quadNum() == 0 || add.quadNum() == 11)
   {
      if (firstAdd().quadNum() == 0)
      {
         if (add.coord() == DgIVec2D(0, 0)) return true;
         else return false; 
      }
      else
         return false;
   }
   else
   {
      return add.coord().i() >= 0 && add.coord().i() <= idgg().maxI() &&
             add.coord().j() >= 0 && add.coord().j() <= idgg().maxJ();

   }

} // DgBoundedIDGG::validAddress

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord&
DgBoundedIDGG::incrementAddress (DgQ2DICoord& add) const
{
   /* cout << "+++ DgBoundedIDGG::incrementAddress(): add: " << add
        << " add.coord(): " << add.coord()
        << " bnd2D().lastAdd(): " << bnd2D().lastAdd() << endl; */

   if (!validAddress(add)) return add = invalidAdd();
   else if (add == lastAdd() || add == endAdd()) return add = endAdd();

   if (add.quadNum() == 0) return add = DgQ2DICoord(1, DgIVec2D(0, 0));

   if (add.coord() == bnd2D().lastAdd())
      return add = DgQ2DICoord(add.quadNum() + 1, DgIVec2D(0, 0));
   else
   {
      DgIVec2D tmpCoord(add.coord());
      bnd2D().incrementAddress(tmpCoord);
      add.setCoord(tmpCoord);
   }

   return add;

} // DgQ2DICoord& DgBoundedIDGG::incrementAddress

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord&
DgBoundedIDGG::decrementAddress (DgQ2DICoord& add) const
{
   if (!validAddress(add) || add == firstAdd()) return add = invalidAdd();

   if (add.coord() == DgIVec2D(0, 0))
   {
      if (add.quadNum() == 1)
         return add = DgQ2DICoord(0, DgIVec2D(0, 0));

      return add = DgQ2DICoord(add.quadNum() - 1, 
                               DgIVec2D(idgg().maxI(), idgg().maxJ()));
   }
   
   DgIVec2D tmpCoord(add.coord());
   bnd2D().decrementAddress(tmpCoord);
   add.setCoord(tmpCoord);

   return add;

} // DgQ2DICoord& DgBoundedIDGG::decrementAddress

////////////////////////////////////////////////////////////////////////////////
unsigned long long int 
DgBoundedIDGG::seqNumAddress (const DgQ2DICoord& add) const
{
   unsigned long long int offset = 0;
   int q = add.quadNum();

   // get the quad offset

   if (q > 0)
   {
      if (firstAdd().quadNum() == 0) offset++;
      offset += (q - 1) * offsetPerQuad();
   }

   // now account for seqNum within the quad

   unsigned long long int sNum = offset + bnd2D().seqNumAddress(add.coord());

   if (!zeroBased()) 
    sNum++;

   return sNum;

} 

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord 
DgBoundedIDGG::addFromSeqNum (unsigned long long int sNum) const
{
   if (!zeroBased()) 
    sNum--;

   if (sNum >= size()) 
    return invalidAdd();

   if (sNum == 0) 
    return firstAdd();

   // adjust for quad 0 if applicable

   if (firstAdd().quadNum() == 0) 
    sNum--;

   // determine quad and adjust accordingly

   unsigned long long int q = sNum / offsetPerQuad() + 1; // note int division
   sNum -= (q - 1) * offsetPerQuad();
   
   return DgQ2DICoord((int) q, bnd2D().addFromSeqNum(sNum));
} 

////////////////////////////////////////////////////////////////////////////////
/// Take a point that may lie off it's quad and get it on the correct quad
DgQ2DICoord 
DgBoundedIDGG::q2dixToQ2di (const DgQ2DICoord& add) const
{
   int q = add.quadNum();
   DgIVec2D c = add.coord();
   bool good = false;
   DgQ2DICoord result(add);
   DgQ2DICoord last(add);
   long long int numD = bnd2D().numJ();
   long long int numDminusOne = numD - 1;
   //cout << " q2dixToQ2di: addIn: " << add << " numD: " << numD << endl;

   bool hexGrid = (idgg().gridTopo() == dgg::topo::Hexagon);

   int numTries = 0;
   while (!good)
   {
      long long int i = c.i();
      long long int j = c.j();
      DgIVec2D origin(0, 0);
      //DgIVec2D offset(0, 0);
      DgIVec2D offset(0, 0);

      if (q == 0) {
         if (i > 0) {
            q = 2;
            origin = DgIVec2D(0, numD);
            offset = i * DgIVec2D(0, -1) + j * DgIVec2D(1, 1);
         } else if (j > 0) {
            q = 3;
            origin = DgIVec2D(0, numD);
            offset = i * DgIVec2D(-1, -1) + j * DgIVec2D(1, 0);
         } else if (i < 0) {
            q = 5;
            origin = DgIVec2D(0, numD);
            offset = i * DgIVec2D(0, 1) + j * DgIVec2D(-1, -1);
         } else if (j < 0) {
            q = 1;
            origin = DgIVec2D(0, numD);
            offset = DgIVec2D(i, j);
         } else
            good = true;
   
      } // q == 0
      else if (q <= 5)
      {
         int case2adjTbl[][4] = {
                {5, 2, 6, 10},  // q == 1
                {1, 3, 7, 6},   // q == 2
                {2, 4, 8, 7},   // q == 3
                {3, 5, 9, 8},   // q == 4
                {4, 1, 10, 9}}; // q == 5
         int A = case2adjTbl[q-1][0];
         int B = case2adjTbl[q-1][1];
         int C = case2adjTbl[q-1][2];
         int D = case2adjTbl[q-1][3];

         if (hexGrid && i == 0 && j == numD)
         {
//cout << "one" << endl;
            q = 0;
            c = DgIVec2D(0, 0);
            good = true;
         }
         else if (i >= numD)
         {
//cout << "two" << endl;
            q = C;
            origin = DgIVec2D(-numD, 0);
            offset = DgIVec2D(i, j);
         }
         else if (j >= numD)
         {
//cout << "three" << endl;
            q = B;
            if (hexGrid) {
               origin = DgIVec2D(-numD, 0);
               offset = i * DgIVec2D(0, -1) + j * DgIVec2D(1, 1);
            } else {
               origin = DgIVec2D(0, 0);
               offset = DgIVec2D(j - numD, numDminusOne - i);
            }
         }
         else if (i < 0)
         {
//cout << "four" << endl;
            q = A;
            if (hexGrid) {
                origin = DgIVec2D(numD, numD);
                offset = i * DgIVec2D(1, 1) + j * DgIVec2D(-1, 0);
            } else {
                origin = DgIVec2D(0, 0);
                offset = DgIVec2D(numDminusOne - j, numD + i);
            }
         }
         else if (j < 0)
         {
//cout << "five" << endl;
            q = D;
            origin = DgIVec2D(0, numD);
            if (hexGrid)
               offset = DgIVec2D(i, j);
            else {
                origin = DgIVec2D(0, 0);
                offset = DgIVec2D(i, j + numD);
            }
/*
            else
               offset = DgIVec2D(i, j);
*/
         }
         else
            good = true;

      } // q <= 5
      else if (q <= 10)
      {
         int case2adjTbl[][4] = {
                {1, 2, 7, 10},  // q == 6
                {2, 3, 8, 6},   // q == 7
                {3, 4, 9, 7},   // q == 8
                {4, 5, 10, 8},  // q == 9
                {5, 1, 6, 9}};  // q == 10
         int A = case2adjTbl[q-6][0];
         int B = case2adjTbl[q-6][1];
         int C = case2adjTbl[q-6][2];
         int D = case2adjTbl[q-6][3];

         if (hexGrid && i == numD && j == 0)
         {
//cout << "six" << endl;
            q = 11;
            c = DgIVec2D(0, 0);
            good = true;
         }
         else if (i < 0)
         {
//cout << "seven" << endl;
            q = A;
            origin = DgIVec2D(numD, 0);
            offset = DgIVec2D(i, j);
         }
         else if (j >= numD)
         {
//cout << "eight" << endl;
            q = B;
            origin = DgIVec2D(0, -numD);
            offset = DgIVec2D(i, j);
         }
         else if (i >= numD)
         {
//cout << "nine" << endl;
            q = C;
            if (hexGrid) {
                origin = DgIVec2D(0, -numD);
                offset = i * DgIVec2D(1, 1) + j * DgIVec2D(-1, 0);
            } else {
                origin = DgIVec2D(0, 0);
                offset = DgIVec2D(numDminusOne - j, i - numD);
            }
         }
         else if (j < 0)
         {
//cout << "ten" << endl;
            q = D;
            if (hexGrid) {
               origin = DgIVec2D(numD, numD);
               offset = i * DgIVec2D(0, -1) + j * DgIVec2D(1, 1);
            } else {
               origin = DgIVec2D(0, 0);
               offset = DgIVec2D(j + numD, numDminusOne - i);
            }
         }
         else
            good = true;

      } // q <= 10
      else if (hexGrid) // q == 11
      {
         if (i > 0)
         {
            q = 7;
            origin = DgIVec2D(numD, 0);
            offset = i * DgIVec2D(0, 1) + j * DgIVec2D(-1, -1);
         }
         else if (j > 0)
         {
            q = 6;
            origin = DgIVec2D(numD, 0);
            offset = i * DgIVec2D(1, 1) + j * DgIVec2D(-1, 0);
         }
         else if (i < 0)
         {
            q = 10;
            origin = DgIVec2D(numD, 0);
            offset = DgIVec2D(i, j);
         }
         else if (j < 0)
         {
            q = 9;
            origin = DgIVec2D(numD, 0);
            offset = i * DgIVec2D(-1, -1) + j * DgIVec2D(1, 0);
         }
         else
            good = true;
   
      } // q == 11

//offset = c;

      if (!good)
      {
         c = origin + offset;
         //cout << " q: " << q << " orig: " << origin << " off: " << offset << " c: " << c << endl;
      }

      result = DgQ2DICoord(q, c);
      if (!good && result == last)
         report("DgBoundedIDGG::q2dix2q2di() infinite loop", 
                DgBase::Fatal);
      else
      {
         last = result;
         if (++numTries >= 10)
            report("DgBoundedIDGG::q2dix2q2di() ABORT", 
                DgBase::Fatal);
      }

   } // while !good

   //cout << " result: " << add << " -> " << result << endl;
/*
   if (result == add)
      cout << " ===>> SAME" << endl;
   else
      cout << "==>> CHANGE: " << result << endl;
*/

   return result;
}




