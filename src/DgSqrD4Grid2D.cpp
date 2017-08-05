////////////////////////////////////////////////////////////////////////////////
//
// DgSqrD4Grid2D.cpp: DgSqrD4Grid2D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgSqrD4Grid2D.h"
#include "DgPolygon.h"

////////////////////////////////////////////////////////////////////////////////
void
DgSqrD4Grid2D::setAddVertices (const DgIVec2D& add, DgPolygon& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   // find the center

   DgLocation tmpLoc;
   setAddPoint(add, tmpLoc);
   const DgDVec2D& cent = *(backFrame().getAddress(tmpLoc));

   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-0.5, -0.5)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( 0.5, -0.5)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( 0.5,  0.5)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-0.5,  0.5)));

} // void DgSqrD4Grid2D::setAddVertices

////////////////////////////////////////////////////////////////////////////////
void
DgSqrD4Grid2D::setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() + 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j())));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j())));

} // void DgSqrD4Grid2D::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
