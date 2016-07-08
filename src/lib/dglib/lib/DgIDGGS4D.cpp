////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS4D.cpp: DgIDGGS4D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgContCartRF.h"
#include "DgDiscRF.h"
#include "DgIDGGS4D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIDGGS4D::DgIDGGS4D (const DgIDGGS4D& rf) 
  : DgIDGGS (rf)
{
   report("DgIDGGS4D::operator=() not implemented yet", DgBase::Fatal);

} // DgIDGGS4D::DgIDGGS4D

////////////////////////////////////////////////////////////////////////////////
DgIDGGS4D::~DgIDGGS4D (void)
{
   for (unsigned int i = 0; i < grids().size(); i++) 
    delete (*grids_)[i]; 

   delete grids_;

} // DgIDGGS4D::~DgIDGGS4D

////////////////////////////////////////////////////////////////////////////////
DgIDGGS4D&
DgIDGGS4D::operator= (const DgIDGGS4D& rf)
{
   report("DgIDGGS4D::operator=() not implemented", DgBase::Fatal);

   return *this;

} // DgIDGGS4D& DgIDGGS4D::operator=

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4D::setAddParents (const DgResAdd<DgQ2DICoord>& add, 
                               DgLocVector& vec) const
{
//cout << "   setAddParents: " << add << endl;
   if (isCongruent() || radix() == 3)
   {
      DgLocation* tmpLoc = makeLocation(add);
      grids()[add.res() - 1]->convert(tmpLoc);
      convert(tmpLoc);

      vec.push_back(*tmpLoc);

      delete tmpLoc;
   }
   else // must be aligned aperture 4
   {
      // vertices lie in parents

      DgLocation* tmpLoc = makeLocation(add);
      DgPolygon* verts = makeVertices(*tmpLoc);
      delete tmpLoc;

//cout << "   verts 1: " << *verts << endl;

      grids()[add.res() - 1]->convert(*verts);
//cout << "   verts 2: " << *verts << endl;
      convert(*verts);
//cout << "   verts 3: " << *verts << endl;

      for (int i = 0; i < verts->size(); i++)
      {
         bool found = false;

         for (int j = 0; j < vec.size(); j++) 
         {
//cout << "  " << i << " " << j << " " << (*verts)[i] << " " << vec[j];
            if ((*verts)[i] == vec[j]) 
            {
//cout << " YES" << endl;
               found = true;
               break;
            }
//cout << " NO" << endl;
         }
         
         if (!found) vec.push_back((*verts)[i]);
      }
//cout << "   parents: " << vec << endl;

      delete verts;
   }

} // void DgIDGGS4D::setAddParents

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4D::setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   if (isCongruent() || radix() == 3)
   {
      const DgIVec2D& lowerLeft = add.address().coord() * radix();

      vector<DgAddressBase*>& v = vec.addressVec();
      for (int i = 0; i < radix(); i++)
      {
         for (int j = 0; j < radix(); j++)
         {
            v.push_back(new DgAddress< DgResAdd<DgQ2DICoord> >(
             DgResAdd<DgQ2DICoord>(DgQ2DICoord(add.address().quadNum(),
                       DgIVec2D(lowerLeft.i() + i, lowerLeft.j() + j)), 
                               add.res() + 1)));
         }
      }
   }
   else // must be aligned aperture 4
   {
/*
      // only center square is interior

      DgLocation* tmpLoc = makeLocation(add);
      grids()[add.res() + 1]->convert(tmpLoc);
      vec.push_back(*tmpLoc);

      delete tmpLoc;
*/
   }
   
} // void DgIDGGS4D::setAddInteriorChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4D::setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   if (isCongruent() || radix() == 3)
   {
      // no boundary children in this topology; leave vec empty
   }
   else // must be aligned aperture 4
   {
/*
      DgLocation* tmpLoc = makeLocation(add);

      // D8 neighbors is what we want

      DgDmdD8Grid2D d8(network(), grids()[add.res() + 1]->backFrame(),
                       "dummyD8");
      d8.convert(tmpLoc);
      d8.setNeighbors(*tmpLoc, vec);

      grids()[add.res() + 1]->convert(vec);
      convert(vec);

      delete tmpLoc;
*/
   }

} // void DgIDGGS4D::setAddBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4D::setAddAllChildren (const DgResAdd<DgQ2DICoord>& add, 
                                   DgLocVector& vec) const
{
   setAddInteriorChildren(add, vec);

/*
   DgLocVector bndVec(vec.rf());
   setAddBoundaryChildren(add, bndVec);

   for (int i = 0; i < bndVec.size(); i++) vec.push_back(bndVec[i]);
*/

} // void DgIDGGS4D::setAddAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
