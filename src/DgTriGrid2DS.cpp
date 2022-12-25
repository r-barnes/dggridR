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
// DgTriGrid2DS.cpp: DgTriGrid2DS class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgContCartRF.h"
#include "DgIVec2D.h"
#include "DgDiscRF.h"
#include "DgTriGrid2D.h"
#include "DgTriGrid2DS.h"

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
DgTriGrid2DS::DgTriGrid2DS (DgRFNetwork& networkIn, 
               const DgRF<DgDVec2D, long double>& backFrameIn, int nResIn, 
               unsigned int apertureIn, bool isCongruentIn, bool isAlignedIn,
               const string& nameIn)
        : DgDiscRFS2D (networkIn, backFrameIn, nResIn, apertureIn, Triangle, D3,
                       isCongruentIn, isAlignedIn, nameIn) 
{ 
   if (!isCongruent())
   {
      report("DgTriGrid2DS::DgTriGrid2DS() only congruent triangle grid "
             "systems implemented", DgBase::Fatal);
   }

   // determine the radix

   radix_ = static_cast<int>(sqrt(static_cast<float>(aperture())));
   if (static_cast<unsigned int>(radix() * radix()) != aperture())
   {
      report( "DgTriGrid2DS::DgTriGrid2DS() aperture must be a perfect square",
       DgBase::Fatal);
   }

   // do the grids

   long double fac = M_SQRT3; // scale so e == 1

   DgDVec2D trans;
   trans = DgDVec2D(-M_SQRT3_2, -0.5L);

   for (int i = 0; i < nRes(); i++)
   {
      string newName = name() + "_" + dgg::util::to_string(i);

      //cout << newName << " " << fac << ' ' << trans << endl;

      const DgContCartRF* ccRF = DgContCartRF::makeRF(network(), newName + string("bf"));

      Dg2WayContAffineConverter(backFrame(), *ccRF, (long double) fac, 0.0, trans); 

      (*grids_)[i] = DgTriGrid2D::makeRF(network(), *ccRF, newName);
      Dg2WayResAddConverter<DgIVec2D, DgDVec2D, long double>(*this, *(grids()[i]), i);

      fac *= radix();
   }

} // DgTriGrid2DS::DgTriGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgTriGrid2DS::DgTriGrid2DS (const DgTriGrid2DS& rf) 
  : DgDiscRFS2D (rf)
{
   report("DgTriGrid2DS::operator=() not implemented yet", DgBase::Fatal);

} // DgTriGrid2DS::DgTriGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgTriGrid2DS::~DgTriGrid2DS (void)
{ } // DgTriGrid2DS::~DgTriGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgTriGrid2DS&
DgTriGrid2DS::operator= (const DgTriGrid2DS&)
{
   report("DgTriGrid2DS::operator=() ", DgBase::Fatal);

   return *this;

} // DgTriGrid2DS& DgTriGrid2DS::operator=

////////////////////////////////////////////////////////////////////////////////
void 
DgTriGrid2DS::setAddParents (const DgResAdd<DgIVec2D>& add, 
                               DgLocVector& vec) const
{
//cout << "   setAddParents: " << add << endl;
   if (isCongruent())
   {
      DgLocation* tmpLoc = makeLocation(add);
      grids()[add.res() - 1]->convert(tmpLoc);
      convert(tmpLoc);

      vec.push_back(*tmpLoc);

      delete tmpLoc;
   }
   else
   {
      report("DgTriGrid2DS::DgTriGrid2DS() only congruent triangle grid "
             "systems implemented", DgBase::Fatal);
   }

} // void DgTriGrid2DS::setAddParents

////////////////////////////////////////////////////////////////////////////////
void 
DgTriGrid2DS::setAddInteriorChildren (const DgResAdd<DgIVec2D>& add, 
                                        DgLocVector& vec) const
{
   if (isCongruent())
   {
//cout << "Children: " << add << " " << lowerLeft << endl;

      vector<DgAddressBase*>& v = vec.addressVec();

      if (DgTriGrid2D::isUp(add.address()))
      {
         const DgIVec2D lowerLeft((add.address().i() * radix()),
                                  (add.address().j() * radix()));

         long long int maxJ = 0;
         for (int i = 0; i < radix(); i++)
         {
            for (long long int j = 0; j <= maxJ; j++)
            {
               v.push_back(new DgAddress< DgResAdd<DgIVec2D> >(
                           DgResAdd<DgIVec2D>(DgIVec2D(lowerLeft.i() + i, 
                                lowerLeft.j() + j), add.res() + 1)));
            }
            maxJ += 2;
         }
      }
      else // down pointing
      {
         const DgIVec2D upperRight((add.address().i() * radix() + radix() - 1),
                                   (add.address().j() * radix() + radix() - 1));

         long long int maxJ = 0;
         for (int i = 0; i < radix(); i++)
         {
            for (long long int j = 0; j <= maxJ; j++)
            {
               v.push_back(new DgAddress< DgResAdd<DgIVec2D> >(
                           DgResAdd<DgIVec2D>(DgIVec2D(upperRight.i() - i, 
                                upperRight.j() - j), add.res() + 1)));
            }
            maxJ += 2;
         }
      }
   }
   else
   {
      report("DgTriGrid2DS::DgTriGrid2DS() only congruent triangle grid "
             "systems implemented", DgBase::Fatal);
   }
//cout << vec << endl;
   
} // void DgTriGrid2DS::setAddInteriorChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgTriGrid2DS::setAddBoundaryChildren (const DgResAdd<DgIVec2D>&, DgLocVector&) const
{
   if (isCongruent()) {
      // no boundary children in this topology; leave vec empty
   } else {
      report("DgTriGrid2DS::DgTriGrid2DS() only congruent triangle grid "
             "systems implemented", DgBase::Fatal);
   }

} // void DgTriGrid2DS::setAddBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgTriGrid2DS::setAddAllChildren (const DgResAdd<DgIVec2D>& add, 
                                   DgLocVector& vec) const
{
   setAddInteriorChildren(add, vec);

} // void DgTriGrid2DS::setAddAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
