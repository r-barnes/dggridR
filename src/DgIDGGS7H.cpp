#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

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
// DgIDGGS7H.cpp: DgIDGGS7H class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgContCartRF.h"
#include "DgDiscTopoRF.h"
#include "DgHexC1Grid2D.h"
#include "DgHexC2Grid2D.h"
#include "DgIDGGS7H.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIDGGS7H::DgIDGGS7H (const DgIDGGS7H& rf)
  : DgHexIDGGS (rf)
{
   report("DgIDGGS7H::operator=() not implemented yet", DgBase::Fatal);

} // DgIDGGS7H::DgIDGGS7H

////////////////////////////////////////////////////////////////////////////////
DgIDGGS7H::~DgIDGGS7H (void)
{

} // DgIDGGS7H::~DgIDGGS7H

////////////////////////////////////////////////////////////////////////////////
DgIDGGS7H&
DgIDGGS7H::operator= (const DgIDGGS7H&)
{
   report("DgIDGGS7H::operator=() not implemented", DgBase::Fatal);

   return *this;

} // DgIDGGS7H& DgIDGGS7H::operator=

////////////////////////////////////////////////////////////////////////////////
void
DgIDGGS7H::setAddParents (const DgResAdd<DgQ2DICoord>& add,
                             DgLocVector& vec) const
{
   DgPolygon verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res()]->setVertices(*tmpLoc, verts);
   delete tmpLoc;

   // vertices lie in parents

   grids()[add.res() - 1]->convert(verts);

   for (int i = 0; i < verts.size(); i++)
   {
      // check if already present

      bool found = false;
      for (int j = 0; j < vec.size(); j++)
      {
         if (vec[j] == verts[i])
         {
            found = true;
            break;
         }
      }

      if (!found) vec.push_back(verts[i]);
   }

} // void DgIDGGS7H::setAddParents

////////////////////////////////////////////////////////////////////////////////
void
DgIDGGS7H::setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add,
                                        DgLocVector& vec) const
{
   DgLocVector verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res() + 1]->convert(tmpLoc);
   vec.push_back(*tmpLoc);

   delete tmpLoc;

} // void DgIDGGS7H::setAddInteriorChildren

////////////////////////////////////////////////////////////////////////////////
void
DgIDGGS7H::setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add,
                                        DgLocVector& vec) const
{
   DgPolygon verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res()]->setVertices(*tmpLoc, verts);
   delete tmpLoc;

   // vertices lie in children

   grids()[add.res() + 1]->convert(verts);

   for (int i = 0; i < verts.size(); i++)
   {
      // check if already present

      bool found = false;
      for (int j = 0; j < vec.size(); j++)
      {
         if (vec[j] == verts[i])
         {
            found = true;
            break;
         }
      }

      if (!found) vec.push_back(verts[i]);
   }

} // void DgIDGGS7H::setAddBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
void
DgIDGGS7H::setAddAllChildren (const DgResAdd<DgQ2DICoord>& add,
                                   DgLocVector& vec) const
{
   setAddInteriorChildren(add, vec);

   DgLocVector bndVec(vec.rf());
   setAddBoundaryChildren(add, bndVec);

   for (int i = 0; i < bndVec.size(); i++) vec.push_back(bndVec[i]);

} // void DgIDGGS7H::setAddAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
