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
// DgIDGGS4H.cpp: DgIDGGS4H class implementation
//
// Version 7.0 - Kevin Sahr, 12/14/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgContCartRF.h"
#include "DgDiscRF.h"
#include "DgHexC1Grid2D.h"
#include "DgHexC2Grid2D.h"
#include "DgIDGGS4H.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIDGGS4H::DgIDGGS4H (const DgIDGGS4H& rf) 
  : DgHexIDGGS (rf)
{
   report("DgIDGGS4H::operator=() not implemented yet", DgBase::Fatal);

} // DgIDGGS4H::DgIDGGS4H

////////////////////////////////////////////////////////////////////////////////
DgIDGGS4H::~DgIDGGS4H (void)
{

} // DgIDGGS4H::~DgIDGGS4H

////////////////////////////////////////////////////////////////////////////////
DgIDGGS4H&
DgIDGGS4H::operator= (const DgIDGGS4H&)
{
   report("DgIDGGS4H::operator=() not implemented", DgBase::Fatal);

   return *this;

} // DgIDGGS4H& DgIDGGS4H::operator=

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4H::setAddParents (const DgResAdd<DgQ2DICoord>& add, 
                             DgLocVector& vec) const
{
   DgPolygon verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res()]->setVertices(*tmpLoc, verts);
   delete tmpLoc;

   // edge midpoints lie in parents

   DgLocVector midPts(verts.rf());
   for (int i = 0; i < verts.size(); i++)
   {
      DgDVec2D pt1 = *(grids()[add.res()]->backFrame().getAddress(verts[i]));

      DgDVec2D pt2 = *(grids()[add.res()]->backFrame().getAddress(
                                           verts[(i + 1) % verts.size()]));

      DgLocation* tmpLoc = 
         grids()[add.res()]->backFrame().makeLocation(
                                           DgDVec2D::midPoint(pt1, pt2));

      midPts.push_back(*tmpLoc);

      delete tmpLoc;
   }

   grids()[add.res() - 1]->convert(midPts);

   for (int i = 0; i < midPts.size(); i++)
   {
      // check if already present

      bool found = false;
      for (int j = 0; j < vec.size(); j++)
      {
         if (vec[j] == midPts[i])
         {
            found = true;
            break;
         }
      }

      if (!found) vec.push_back(midPts[i]);
   }

} // void DgIDGGS4H::setAddParents

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4H::setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   DgLocVector verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res() + 1]->convert(tmpLoc);
   vec.push_back(*tmpLoc);

   delete tmpLoc;

} // void DgIDGGS4H::setAddInteriorChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4H::setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   DgPolygon verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res()]->setVertices(*tmpLoc, verts);
   delete tmpLoc;

   // edge midpoints lie in children

   DgLocVector midPts(verts.rf());
   for (int i = 0; i < verts.size(); i++)
   {
      DgDVec2D pt1 = *(grids()[add.res()]->backFrame().getAddress(verts[i]));

      DgDVec2D pt2 = *(grids()[add.res()]->backFrame().getAddress(
                                           verts[(i + 1) % verts.size()]));
      DgLocation* tmpLoc = 
         grids()[add.res()]->backFrame().makeLocation(
                                              DgDVec2D::midPoint(pt1, pt2));

      midPts.push_back(*tmpLoc);

      delete tmpLoc;
   }

   grids()[add.res() + 1]->convert(midPts);

   for (int i = 0; i < midPts.size(); i++)
   {
      // check if already present

      bool found = false;
      for (int j = 0; j < vec.size(); j++)
      {
         if (vec[j] == midPts[i])
         {
            found = true;
            break;
         }
      }

      if (!found) vec.push_back(midPts[i]);
   }

} // void DgIDGGS4H::setAddBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4H::setAddAllChildren (const DgResAdd<DgQ2DICoord>& add, 
                                   DgLocVector& vec) const
{
   setAddInteriorChildren(add, vec);

   DgLocVector bndVec(vec.rf());
   setAddBoundaryChildren(add, bndVec);

   for (int i = 0; i < bndVec.size(); i++) vec.push_back(bndVec[i]);

} // void DgIDGGS4H::setAddAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
