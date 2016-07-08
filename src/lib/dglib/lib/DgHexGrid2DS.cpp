////////////////////////////////////////////////////////////////////////////////
//
// DgHexGrid2DS.cpp: DgHexGrid2DS class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgContCartRF.h"
#include "DgDiscRF.h"
#include "DgHexC1Grid2D.h"
#include "DgHexC2Grid2D.h"
#include "DgHexGrid2DS.h"

////////////////////////////////////////////////////////////////////////////////
DgHexGrid2DS::DgHexGrid2DS (DgRFNetwork& networkIn, 
               const DgRF<DgDVec2D, long double>& backFrameIn, int nResIn, 
               unsigned int apertureIn, bool isCongruentIn, bool isAlignedIn,
               const string& nameIn, bool isMixed43In, int numAp4In, 
               bool isSuperfundIn)
        : DgDiscRFS2D (networkIn, backFrameIn, nResIn, apertureIn, isCongruentIn, 
                       isAlignedIn, nameIn), 
          isMixed43_ (isMixed43In), numAp4_ (numAp4In), isSuperfund_ (isSuperfundIn) 
{ 
   if (aperture() != 3 && aperture() != 4)
   {
      report(
      "DgHexGrid2DS::DgHexGrid2DS() only apertures 3 and 4 implemented",
       DgBase::Fatal);
   }

   if (isCongruent())
   {
      report(
      "DgHexGrid2DS::DgHexGrid2DS() hex grids cannot be congruent",
       DgBase::Fatal);
   }

   if (!isAligned())
   {
      report(
      "DgHexGrid2DS::DgHexGrid2DS() hex grids must be aligned",
       DgBase::Fatal);
   }

   frequency_ = sqrt((long double) aperture());

   // do the grids

   long double fac = M_ONE;

   for (int i = 0; i < nRes(); i++)
   {
      string newName = name() + "_" + dgg::util::to_string(i);

      //cout << newName << " " << fac << ' ' << trans << endl;

      DgContCartRF* ccRF = new DgContCartRF(network(), newName + string("bf"));

      new Dg2WayContAffineConverter(backFrame(), *ccRF, (long double) fac, M_ZERO, 
                                    DgDVec2D(M_ZERO, M_ZERO)); 

      bool isClassI;
      if (!isMixed43())
      {
         if (aperture() == 4) 
            isClassI = true;
         else // ap 3
            isClassI = !(i % 2);
      }
      else // mixed43
      {
         if (i <= numAp4())
            isClassI = true;
         else
            isClassI = !((i - numAp4()) % 2);
      }

      if (isClassI)
      {
         (*grids_)[i] = new DgHexC1Grid2D(network(), *ccRF, newName);
      }
      else
      {
         (*grids_)[i] = new DgHexC2Grid2D(network(), *ccRF, newName);
      }

      new Dg2WayResAddConverter<DgIVec2D, DgDVec2D, long double>
                                                  (*this, *(grids()[i]), i);
      if (isMixed43())
      {
         if (i < numAp4())
            fac *= 2.0L;
         else
            fac *= M_SQRT3;
      }
      else
         fac *= frequency();
//cout << "i: " << i << " fac: " << fac << endl;
   }
//cout << "** final fac: " << fac << endl;

} // DgHexGrid2DS::DgHexGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgHexGrid2DS::DgHexGrid2DS (const DgHexGrid2DS& rf) 
  : DgDiscRFS2D (rf)
{
   report("DgHexGrid2DS::operator=() not implemented yet", DgBase::Fatal);

} // DgHexGrid2DS::DgHexGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgHexGrid2DS::~DgHexGrid2DS (void)
{
   for (unsigned long i = 0; i < grids().size(); i++) 
    delete (*grids_)[i]; 

   delete grids_;

} // DgHexGrid2DS::~DgHexGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgHexGrid2DS&
DgHexGrid2DS::operator= (const DgHexGrid2DS& rf)
{
   report("DgHexGrid2DS::operator=() ", DgBase::Fatal);

   return *this;

} // DgHexGrid2DS& DgHexGrid2DS::operator=

////////////////////////////////////////////////////////////////////////////////
void 
DgHexGrid2DS::setAddParents (const DgResAdd<DgIVec2D>& add, 
                             DgLocVector& vec) const
{
   DgPolygon verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res()]->setVertices(*tmpLoc, verts);
   delete tmpLoc;

   int effectiveAperture = aperture();
   if (isMixed43() && add.res() <= numAp4()) effectiveAperture = 4;

   if (effectiveAperture == 3)
   {
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
   }
   else // must be aligned aperture 4
   {
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
   }

} // void DgHexGrid2DS::setAddParents

////////////////////////////////////////////////////////////////////////////////
void 
DgHexGrid2DS::setAddInteriorChildren (const DgResAdd<DgIVec2D>& add, 
                                        DgLocVector& vec) const
{
   DgLocVector verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res() + 1]->convert(tmpLoc);
   vec.push_back(*tmpLoc);

   delete tmpLoc;

} // void DgHexGrid2DS::setAddInteriorChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgHexGrid2DS::setAddBoundaryChildren (const DgResAdd<DgIVec2D>& add, 
                                        DgLocVector& vec) const
{
   DgPolygon verts;
   DgLocation* tmpLoc = grids()[add.res()]->makeLocation(add.address());
   grids()[add.res()]->setVertices(*tmpLoc, verts);
   delete tmpLoc;

   int effectiveAperture = aperture();
   if (isMixed43() && add.res() <= numAp4()) effectiveAperture = 4;

   if (effectiveAperture == 3)
   {
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
   }
   else // must be aligned aperture 4
   {
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
   }

} // void DgHexGrid2DS::setAddBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgHexGrid2DS::setAddAllChildren (const DgResAdd<DgIVec2D>& add, 
                                   DgLocVector& vec) const
{
   setAddInteriorChildren(add, vec);

   DgLocVector bndVec(vec.rf());
   setAddBoundaryChildren(add, bndVec);

   for (int i = 0; i < bndVec.size(); i++) vec.push_back(bndVec[i]);

} // void DgHexGrid2DS::setAddAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
