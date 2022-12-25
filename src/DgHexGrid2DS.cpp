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
// DgHexGrid2DS.cpp: DgHexGrid2DS class implementation
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
#include "DgHexC3Grid2D.h"
#include "DgHexGrid2DS.h"

static const string& emptyStr = "";

using namespace dgg::topo;

////////////////////////////////////////////////////////////////////////////////
DgHexGrid2DS::DgHexGrid2DS (DgRFNetwork& networkIn, 
               const DgRF<DgDVec2D, long double>& backFrameIn, int nResIn, 
               const DgApSeq& apSeqIn, const string& nameIn) 
        : DgDiscRFS2D (networkIn, backFrameIn, nResIn, 4,
                       //apSeqIn.getAperture(apSeqIn.lastRes()).aperture(), 
                       Hexagon, D6, 0, 1, nameIn), 
          apSeq_ (apSeqIn),
          // these are deprecated and not used by this constructor
          isMixed43_ (0), numAp4_ (0), isSuperfund_ (0), isApSeq_ (0)
{ 
   //frequency_ = sqrt((long double) aperture());

   // do the grids

   long double fac = M_ONE;

   //cout << "*** CREATING AFFINE CONVERTERS:" << endl;

   bool isClassI = true;
   bool isClassIII = false;
   int ap = 4;
   for (int r = 0; r < nRes(); r++)
   {
      string newName = name() + "_" + dgg::util::to_string(r);

      //cout << "  r: " + dgg::util::to_string(r) + " grid: " + newName << " fac: " << fac << endl;

      const DgContCartRF* ccRF = DgContCartRF::makeRF(network(), newName + string("bf"));

      // this creates the forward and inverse converters, which are memory
      // managed by the DgRFNetwork. but 2Way converters  are not themselves 
      // converters
      Dg2WayContAffineConverter(backFrame(), *ccRF, (long double) fac, M_ZERO, 
                                    DgDVec2D(M_ZERO, M_ZERO)); 

      if (isClassIII)
         (*grids_)[r] = DgHexC3Grid2D::makeRF(network(), *ccRF, isClassI, newName);
      else
      {
         if (isClassI)
            (*grids_)[r] = DgHexC1Grid2D::makeRF(network(), *ccRF, newName);
         else
            (*grids_)[r] = DgHexC2Grid2D::makeRF(network(), *ccRF, newName);
      }

      Dg2WayResAddConverter<DgIVec2D, DgDVec2D, long double>(*this, *(grids()[r]), r);
      // setup for next res
      if (r < apSeq().lastRes())
         ap = apSeq().getAperture(r + 1).aperture();
      // else use aperture from last res

      if (ap == 4)
         fac *= 2.0L;
      else if (ap == 3)
      {
         fac *= M_SQRT3;
         isClassI = !isClassI;
      }
      else if (ap == 7)
      {
         fac *= M_SQRT7;
         isClassIII = !isClassIII;
      }

//cout << "i: " << i << " fac: " << fac << endl;
   }
//cout << "** final fac: " << fac << endl;

} // DgHexGrid2DS::DgHexGrid2DS

////////////////////////////////////////////////////////////////////////////////
// this constructor is deprecated
DgHexGrid2DS::DgHexGrid2DS (DgRFNetwork& networkIn, 
               const DgRF<DgDVec2D, long double>& backFrameIn, int nResIn, 
               unsigned int apertureIn, bool isCongruentIn, bool isAlignedIn,
               const string& nameIn, bool isMixed43In, int numAp4In, 
               bool isSuperfundIn, bool isApSeqIn, const DgApSeq& apSeqIn)
        : DgDiscRFS2D (networkIn, backFrameIn, nResIn, apertureIn, Hexagon, D6,
              isCongruentIn, isAlignedIn, nameIn), 
          apSeq_ (apSeqIn),
          isMixed43_ (isMixed43In), numAp4_ (numAp4In), isSuperfund_ (isSuperfundIn),
          isApSeq_ (isApSeqIn)
{ 
   if (isApSeq())
      report("DgHexGrid2DS::DgHexGrid2DS() isApSeq should be false in this constructor",
       DgBase::Fatal);

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

   long double frequency = sqrtl((long double) aperture());

   // do the grids

   long double fac = M_ONE;

   for (int i = 0; i < nRes(); i++)
   {
      string newName = name() + "_" + dgg::util::to_string(i);

      //cout << newName << " " << fac << ' ' << trans << endl;

      const DgContCartRF* ccRF = DgContCartRF::makeRF(network(), newName + string("bf"));

      Dg2WayContAffineConverter(backFrame(), *ccRF, (long double) fac, M_ZERO, 
                                    DgDVec2D(M_ZERO, M_ZERO)); 

      bool isClassI;
      if (!isMixed43()) {
         if (aperture() == 4) 
            isClassI = true;
         else // ap 3
            isClassI = !(i % 2);
      } else { // mixed43 
         if (i <= numAp4())
            isClassI = true;
         else
            isClassI = !((i - numAp4()) % 2);
      }

      if (isClassI) {
         (*grids_)[i] = DgHexC1Grid2D::makeRF(network(), *ccRF, newName);
      } else {
         (*grids_)[i] = DgHexC2Grid2D::makeRF(network(), *ccRF, newName);
      }

      Dg2WayResAddConverter<DgIVec2D, DgDVec2D, long double>(*this, *(grids()[i]), i);
      if (isMixed43())
      {
         if (i < numAp4())
            fac *= 2.0L;
         else
            fac *= M_SQRT3;
      }
      else
         fac *= frequency;
//cout << "i: " << i << " fac: " << fac << endl;
   }
//cout << "** final fac: " << fac << endl;

} // DgHexGrid2DS::DgHexGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgHexGrid2DS::DgHexGrid2DS (const DgHexGrid2DS& rf) 
  : DgDiscRFS2D (rf), apSeq_ (DgApSeq::defaultApSeq)
{
   report("DgHexGrid2DS::operator=() not implemented yet", DgBase::Fatal);

} // DgHexGrid2DS::DgHexGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgHexGrid2DS::~DgHexGrid2DS (void)
{ } // DgHexGrid2DS::~DgHexGrid2DS

////////////////////////////////////////////////////////////////////////////////
DgHexGrid2DS&
DgHexGrid2DS::operator= (const DgHexGrid2DS&)
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
