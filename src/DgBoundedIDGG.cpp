////////////////////////////////////////////////////////////////////////////////
//
// DgBoundedIDGG.cpp: DgBoundedIDGG class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cstdint>

#include "DgBoundedIDGG.h"
#include "DgBoundedRF2D.h"
#include "DgBoundedHexC2RF2D.h"
#include "DgIDGG.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgBoundedIDGG::DgBoundedIDGG (const DgIDGG& IDGGin)
   : DgBoundedRF<DgQ2DICoord, DgGeoCoord, long double> (IDGGin, IDGGin.firstAdd(), 
          IDGGin.lastAdd(), IDGGin.undefAddress(), false), IDGG_ (IDGGin), 
          bnd2D_ (0)
{
   // create the surrogate boundedRF2D

   if (idgg().gridTopo() == "HEXAGON" && !idgg().isClassI())
   {
      bnd2D_ = new DgBoundedHexC2RF2D(idgg().grid2D(), DgIVec2D(0, 0), 
                                      DgIVec2D(idgg().maxI(), idgg().maxJ()));
   }
   else
   {
      bnd2D_ = new DgBoundedRF2D(idgg().grid2D(), DgIVec2D(0, 0), 
                                 DgIVec2D(idgg().maxI(), idgg().maxJ()));
   }

   // calculate the offsetPerQuad

   if (idgg().gridTopo() == "TRIANGLE")
   {
      offsetPerQuad_ = idgg().mag() * (idgg().maxJ() + 1);
   }
   else if (idgg().gridTopo() == "HEXAGON" && idgg().aperture() == 3)
   {
      if (!idgg().isClassI()) 
         offsetPerQuad_ = (idgg().mag() / 3) * idgg().mag();
      else 
         offsetPerQuad_ = idgg().mag() * idgg().mag();
   }
   else
   {
      offsetPerQuad_ = idgg().mag() * idgg().mag();
   }

   // calculate the total size

   size_ = 10 * offsetPerQuad();
   if (firstAdd().quadNum() == 0) size_ += 2;

   // validate the size

   std::uint64_t tmpSize = size();
   if (firstAdd().quadNum() == 0) tmpSize -= 2;
/*
cout << "mag: " << idgg().mag() << ", /3: " << idgg().mag() / 3 << ", size: " << size() << ", tmpSize: " << tmpSize << ", tmpSize / 10: " << tmpSize/10 << endl;
cout  << "quadNum: " << firstAdd().quadNum() << ", offsetPerQuad: " << offsetPerQuad() << endl;
*/
   if ((tmpSize / 10) != offsetPerQuad())
   {
       report("DgBoundedIDGG::DgBoundedIDGG() invalid size setting due to "
              "possible overflow", DgBase::Warning);
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
std::uint64_t 
DgBoundedIDGG::seqNumAddress (const DgQ2DICoord& add) const
{
   std::uint64_t offset = 0;
   int q = add.quadNum();

   // get the quad offset

   if (q > 0)
   {
      if (firstAdd().quadNum() == 0) offset++;
      offset += (q - 1) * offsetPerQuad();
   }

   // now account for seqNum within the quad

   std::uint64_t sNum = offset + bnd2D().seqNumAddress(add.coord());

   if (!zeroBased()) 
    sNum++;

   return sNum;

} 

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord 
DgBoundedIDGG::addFromSeqNum (std::uint64_t sNum) const
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

   std::uint64_t q = sNum / offsetPerQuad() + 1; // note int division
   sNum -= (q - 1) * offsetPerQuad();
   
   return DgQ2DICoord((int) q, bnd2D().addFromSeqNum(sNum));
} 

