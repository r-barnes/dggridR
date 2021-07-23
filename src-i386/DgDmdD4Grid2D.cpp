////////////////////////////////////////////////////////////////////////////////
//
// DgDmdD4Grid2D.cpp: DgDmdD4Grid2D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgDmdD4Grid2D.h"
#include "DgPolygon.h"

const long double DgDmdD4Grid2D::yOff_ = sqrt(3.0) / 4.0;
const long double DgDmdD4Grid2D::yOff2_ = yOff_ * 2.0;
const long double DgDmdD4Grid2D::xOff_ = 0.25;
const long double DgDmdD4Grid2D::xOffComp_ = 1.0 - xOff_;
const long double DgDmdD4Grid2D::skewFac_ = sqrt(3.0) / 3.0;

////////////////////////////////////////////////////////////////////////////////
void
DgDmdD4Grid2D::setAddVertices (const DgIVec2D& add, DgPolygon& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   // find the center

   DgLocation tmpLoc;
   setAddPoint(add, tmpLoc);
   const DgDVec2D& cent = *(backFrame().getAddress(tmpLoc));

   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-xOff_,     -yOff_)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( xOffComp_, -yOff_)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( xOff_,      yOff_)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-xOffComp_,  yOff_)));

} // void DgDmdD4Grid2D::setAddVertices

////////////////////////////////////////////////////////////////////////////////
void
DgDmdD4Grid2D::setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() + 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j())));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i(), add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j())));

} // void DgDmdD4Grid2D::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
