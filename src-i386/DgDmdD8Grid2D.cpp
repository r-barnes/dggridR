////////////////////////////////////////////////////////////////////////////////
//
// DgDmdD8Grid2D.cpp: DgDmdD8Grid2D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgDmdD8Grid2D.h"
#include "DgLocVector.h"

////////////////////////////////////////////////////////////////////////////////
void
DgDmdD8Grid2D::setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() + 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j() + 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j())));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j())));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j() + 1)));

} // void DgDmdD8Grid2D::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
