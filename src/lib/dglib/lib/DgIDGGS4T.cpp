////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS4T.cpp: DgIDGGS4T class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "DgContCartRF.h"
#include "DgDiscRF.h"
#include "DgIDGGS4T.h"
#include "DgTriGrid2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIDGGS4T::DgIDGGS4T (const DgIDGGS4T& rf) 
  : DgIDGGS (rf)
{
   report("DgIDGGS4T::operator=() not implemented yet", DgBase::Fatal);

} // DgIDGGS4T::DgIDGGS4T

////////////////////////////////////////////////////////////////////////////////
DgIDGGS4T::~DgIDGGS4T (void)
{
   for (unsigned int i = 0; i < grids().size(); i++) 
    delete (*grids_)[i]; 

   delete grids_;

} // DgIDGGS4T::~DgIDGGS4T

////////////////////////////////////////////////////////////////////////////////
DgIDGGS4T&
DgIDGGS4T::operator= (const DgIDGGS4T& rf)
{
   report("DgIDGGS4T::operator=() not implemented", DgBase::Fatal);

   return *this;

} // DgIDGGS4T& DgIDGGS4T::operator=

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4T::setAddParents (const DgResAdd<DgQ2DICoord>& add, 
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
      report("DgIDGGS4T::DgIDGGS4T() only congruent triangle grid "
             "systems implemented", DgBase::Fatal);
   }

} // void DgIDGGS4T::setAddParents

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4T::setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   if (isCongruent())
   {
//cout << "Children: " << add << " " << lowerLeft << endl;

      vector<DgAddressBase*>& v = vec.addressVec();

      if (DgTriGrid2D::isUp(add.address().coord()))
      {
         const DgIVec2D lowerLeft((add.address().coord().i() * radix()),
                                  (add.address().coord().j() * radix()));

         int64_t maxJ = 0;
         for (int i = 0; i < radix(); i++)
         {
            for (int64_t j = 0; j <= maxJ; j++)
            {
               v.push_back(new DgAddress< DgResAdd<DgQ2DICoord> >(
                  DgResAdd<DgQ2DICoord>(DgQ2DICoord(add.address().quadNum(),
                       DgIVec2D(lowerLeft.i() + i, lowerLeft.j() + j)),
                               add.res() + 1)));
            }
            maxJ += 2;
         }
      }
      else // down pointing
      {
         const DgIVec2D upperRight(
                         (add.address().coord().i() * radix() + radix() - 1),
                         (add.address().coord().j() * radix() + radix() - 1));

         int64_t maxJ = 0;
         for (int i = 0; i < radix(); i++)
         {
            for (int64_t j = 0; j <= maxJ; j++)
            {
               v.push_back(new DgAddress< DgResAdd<DgQ2DICoord> >(
                  DgResAdd<DgQ2DICoord>(DgQ2DICoord(add.address().quadNum(),
                       DgIVec2D(upperRight.i() - i, upperRight.j() - j)),
                               add.res() + 1)));
            }
            maxJ += 2;
         }
      }
   }
   else
   {
      report("DgIDGGS4T::DgIDGGS4T() only congruent triangle grid "
             "systems implemented", DgBase::Fatal);
   }
//cout << vec << endl;
   
} // void DgIDGGS4T::setAddInteriorChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4T::setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   if (isCongruent())
   {
      // no boundary children in this topology; leave vec empty
   }
   else
   {
      report("DgIDGGS4T::DgIDGGS4T() only congruent triangle grid "
             "systems implemented", DgBase::Fatal);
   }

} // void DgIDGGS4T::setAddBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS4T::setAddAllChildren (const DgResAdd<DgQ2DICoord>& add, 
                                   DgLocVector& vec) const
{
   setAddInteriorChildren(add, vec);

} // void DgIDGGS4T::setAddAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
