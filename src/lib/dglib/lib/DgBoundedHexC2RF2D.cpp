////////////////////////////////////////////////////////////////////////////////
//
// DgBoundedHexC2RF2D.cpp: DgBoundedHexC2RF2D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <climits>
#include <cstdint>

#include "DgBoundedHexC2RF2D.h"
#include "DgDiscRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBoundedHexC2RF2D::DgBoundedHexC2RF2D 
           (const DgDiscRF<DgIVec2D, DgDVec2D, long double>& rf, 
            const DgIVec2D& lowerLeft, const DgIVec2D& upperRight)
   : DgBoundedRF2D (rf, lowerLeft, upperRight) 
{ 
   DgIVec2D magVec = upperRight - lowerLeft + DgIVec2D(1, 1);

   size_ = magVec.i() / 3 * magVec.j();
   if (magVec.i() != static_cast<int>((3 * size_ / magVec.j())))
   {
       report("DgBoundedHexC2RF2D::DgBoundedHexC2RF2D() invalid size setting "
              "due to possible overflow", DgBase::Warning);
       validSize_ = false;
   }
   else validSize_ = true;
  
   setLastAdd(addFromSeqNum(size() - 1));

} // DgBoundedHexC2RF2D::DgBoundedHexC2RF2D

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIVec2D&
DgBoundedHexC2RF2D::incrementAddress (DgIVec2D& add) const
{
   if (!validAddress(add)) return add = invalidAdd();
   else if (add == upperRight() || add == endAdd()) return add = endAdd();

   do {

      if (add.j() == upperRight().j())
      {
         add = DgIVec2D(add.i() + 1, lowerLeft().j());
      }
      else
      {
         add.setJ(add.j() + 1);
      }

   } while (((add.i() + add.j()) % 3) && add != upperRight());

   if (add == upperRight() && ((add.i() + add.j()) % 3)) add = endAdd();

   return add;

} // DgIVec2D& DgBoundedHexC2RF2D::incrementAddress

////////////////////////////////////////////////////////////////////////////////
DgIVec2D&
DgBoundedHexC2RF2D::decrementAddress (DgIVec2D& add) const
{
   if (!validAddress(add) || add == lowerLeft()) return add = invalidAdd();
   
   do {

      if (add.j() == lowerLeft().j())
      {
         add = DgIVec2D(add.i() - 1, upperRight().j());
      }
      else
      {
         add.setJ(add.j() - 1);
      }

   } while (((add.i() + add.j()) % 3) && add != lowerLeft());

   if (add == lowerLeft() && ((add.i() + add.j()) % 3)) add = invalidAdd();

   return add;

} // DgIVec2D& DgBoundedHexC2RF2D::decrementAddress

////////////////////////////////////////////////////////////////////////////////
std::uint64_t
DgBoundedHexC2RF2D::seqNumAddress (const DgIVec2D& add) const
{
   DgIVec2D tVec = add - lowerLeft();
   std::uint64_t sNum = tVec.i() * numI() / 3;

   switch (tVec.i() % 3)
   {
      case 0: sNum += tVec.j() / 3;
              break;

      case 1: sNum += (tVec.j() - 2) / 3;
              break;

      case 2: sNum += (tVec.j() - 1) / 3;
              break;
   }

   if (!zeroBased()) sNum++;

   return sNum;

} 

////////////////////////////////////////////////////////////////////////////////
DgIVec2D
DgBoundedHexC2RF2D::addFromSeqNum (std::uint64_t sNum) const
{
   DgIVec2D res;

   if (!zeroBased()) 
    sNum--;

   res.setI((sNum * 3) / numI());
   res.setJ((sNum * 3) % numI());

   switch (res.i() % 3)
   {
      case 0: break;

      case 1: res.setJ(res.j() + 2);
              break;

      case 2: res.setJ(res.j() + 1);
              break;
   }

   res += lowerLeft();

   return res;

} // DgIVec2D DgBoundedHexC2RF2D::addFromSeqNum

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


