////////////////////////////////////////////////////////////////////////////////
//
// DgTriGrid2DS.cpp: DgTriGrid2DS class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdint>

#include "DgContCartRF.h"
#include "DgIVec2D.h"
#include "DgDiscRF.h"
#include "DgTriGrid2D.h"
#include "DgTriGrid2DS.h"

////////////////////////////////////////////////////////////////////////////////
DgTriGrid2DS::DgTriGrid2DS (DgRFNetwork& networkIn, 
               const DgRF<DgDVec2D, long double>& backFrameIn, int nResIn, 
               unsigned int apertureIn, bool isCongruentIn, bool isAlignedIn,
               const string& nameIn)
        : DgDiscRFS2D (networkIn, backFrameIn, nResIn, apertureIn, 
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

   long double fac = sqrt(3.0); // scale so e == 1

   DgDVec2D trans;
   trans = DgDVec2D(-sqrt(3.0) / 2.0, -0.5);

   for (int i = 0; i < nRes(); i++)
   {
      string newName = name() + "_" + dgg::util::to_string(i);

      //cout << newName << " " << fac << ' ' << trans << endl;

      DgContCartRF* ccRF = new DgContCartRF(network(), newName + string("bf"));

      new Dg2WayContAffineConverter(backFrame(), *ccRF, (long double) fac, 0.0, 
                                    trans); 

      (*grids_)[i] = new DgTriGrid2D(network(), *ccRF, newName);
      new Dg2WayResAddConverter<DgIVec2D, DgDVec2D, long double>
                                                  (*this, *(grids()[i]), i);

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
{
   for (unsigned long i = 0; i < grids().size(); i++) 
    delete (*grids_)[i]; 

   delete grids_;

} // DgTriGrid2DS::~DgTriGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgTriGrid2DS&
DgTriGrid2DS::operator= (const DgTriGrid2DS& rf)
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

         std::int64_t maxJ = 0;
         for (int i = 0; i < radix(); i++)
         {
            for (std::int64_t j = 0; j <= maxJ; j++)
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

         std::int64_t maxJ = 0;
         for (int i = 0; i < radix(); i++)
         {
            for (std::int64_t j = 0; j <= maxJ; j++)
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
DgTriGrid2DS::setAddBoundaryChildren (const DgResAdd<DgIVec2D>& add, 
                                        DgLocVector& vec) const
{
   if (isCongruent())
   {
      // no boundary children in this topology; leave vec empty
   }
   else
   {
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
