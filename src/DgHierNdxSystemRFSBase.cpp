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
// DgHierNdxSystemRFSBase.cpp: DgHierNdxSystemRFSBase class definition.
//
////////////////////////////////////////////////////////////////////////////////

#include "DgHierNdxSystemRFSBase.h"
#include "DgHierNdxRF.h"
#include "DgDiscRFSGrids.h"
#include "DgHierNdx.h"
#include "DgZ3System.h"
#include "DgZ7System.h"
#include "DgZOrderSystem.h"

////////////////////////////////////////////////////////////////////////////////
const DgResAdd<DgHierNdx>
DgHierNdxSystemRFSBase::undefCoord(DgHierNdx::undefCoord, -1);

////////////////////////////////////////////////////////////////////////////////
DgHierNdxSystemRFSBase*
DgHierNdxSystemRFSBase::makeSystem (const DgIDGGSBase& dggsIn,
               DgHierNdxSysType sysType, DgHierNdxFormType form,
               const std::string& nameIn)
{
   DgHierNdxSystemRFSBase* sys = nullptr;

   // do some checks
   if (sysType == InvalidHierNdxSysType || form == InvalidHierNdxFormType) {
        ::report("DgHierNdxSystemRFSBase::makeSystem() invalid sytem type or form", DgBase::Fatal);
        return sys;
   }

   bool extModeInt = (form == Int64);
   switch (sysType) {
      case dgg::addtype::Z7:
         if (!dggsIn.isPure() || dggsIn.aperture() != 7) {
            ::report("DgHexIDGGS::makeRF() Z7 only available for pure aperture 7 grids", DgBase::Fatal);
         }
         sys = DgZ7System::makeSystem(dggsIn, extModeInt, "Z7");
         break;
       case dgg::addtype::ZOrder:
         if (!dggsIn.isPure() || (dggsIn.aperture() != 3 && dggsIn.aperture() != 4)) {
            ::report("DgHexIDGGS::makeRF() ZOrder only available for pure aperture 3 or 4 grids", DgBase::Fatal);
         }
         sys = DgZOrderSystem::makeSystem(dggsIn, extModeInt, "ZOrder");
         break;
       case dgg::addtype::Z3:
         if (!dggsIn.isPure() || dggsIn.aperture() != 3) {
            ::report("DgHexIDGGS::makeRF() Z3 only available for pure aperture 3 grids", DgBase::Fatal);
         }
         sys = DgZ3System::makeSystem(dggsIn, extModeInt, "Z3");
         break;
       case dgg::addtype::InvalidHierNdxSysType: // shouldn't be possible
         ::report("DgHexIDGGS::makeRF() invalid hierarchical system type", DgBase::Fatal);
         break;
   }

   return sys;
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxSystemRFSBase::DgHierNdxSystemRFSBase (const DgIDGGSBase& dggsIn,
         bool extModeIntIn, const std::string& nameIn)
   : DgDiscRFS<DgHierNdx, DgQ2DICoord, long long int>
                         (dggsIn.network(), dggsIn, dggsIn.nRes(), nameIn),
     dggs_ (dggsIn), extModeInt_ (extModeIntIn)
{
   //grids_ = new std::vector<const DgHierNdxSystemRFBase*>(nRes_, nullptr);
}

////////////////////////////////////////////////////////////////////////////////
const DgResAdd<DgHierNdx>&
DgHierNdxSystemRFSBase::undefAddress (void) const
{
    return undefCoord;
}

////////////////////////////////////////////////////////////////////////////////
const DgHierNdxSystemRFBase&
DgHierNdxSystemRFSBase::sysRF (int res) const
{
    return *(static_cast<const DgHierNdxSystemRFBase*>(grids()[res]));
}

////////////////////////////////////////////////////////////////////////////////
const DgIDGGBase&
DgHierNdxSystemRFSBase::dgg (int res) const
{
    return dggs().idggBase(res);
}

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFSBase::setNdxParent (int res, const DgLocation& loc,
                    DgLocation& parent) const
{
   parent.clearAddress();
   convert(&parent);

   if (res > 0 && res < nRes()) {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      convert(&tmpLoc);
      setAddNdxParent(*(getAddress(tmpLoc)), parent);
   }

} // void DgHierNdxSystemRFSBase::setNdxParent

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFSBase::setNdxParent (const DgResAdd<DgHierNdx>& add,
         const DgRFBase& rf, DgLocation& parent) const
{
  setNdxParent(add, parent);
  rf.convert(&parent);
}

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFSBase::setNdxParent (const DgResAdd<DgHierNdx>& add,
                         DgLocation& parent) const
{
  //add.clearAddress();
  convert(&parent);
  if (add.res() > 0 && add.res() < nRes())
	         setAddNdxParent(add, parent);
}

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFSBase::setNdxParent (const DgResAdd<DgQ2DICoord>& add,
                         DgLocation& parent) const
{
    DgResAdd<DgHierNdx> hierNdx = quantify(add);
    setNdxParent(hierNdx, parent);
}

////////////////////////////////////////////////////////////////////////////////
DgLocation*
DgHierNdxSystemRFSBase::makeNdxParent (int res, const DgLocation& loc) const
{
  DgLocation* parent = new DgLocation(*this);
  setNdxParent(res, loc, *parent);

  return parent;
}

////////////////////////////////////////////////////////////////////////////////
DgLocation*
DgHierNdxSystemRFSBase::makeNdxParent (const DgResAdd<DgHierNdx>& add) const
{
  DgLocation* parent = new DgLocation(*this);
  setNdxParent(add, *parent);
  return parent;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFSBase::setNdxChildren (int res, const DgLocation& loc,
                                   DgLocVector& children) const
{
   children.clearAddress();
   convert(children);

   if (res >= 0 && res < (nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      convert(&tmpLoc);
      setAddNdxChildren(*(getAddress(tmpLoc)), children);
   }

} // void DgHierNdxSystemRFSBase::setNdxChildren

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFSBase::setNdxChildren (const DgResAdd<DgHierNdx>& add,
       const DgRFBase& rf, DgLocVector& chld) const
{
   setNdxChildren(add, chld);
   rf.convert(chld);
}

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFSBase::setNdxChildren (const DgResAdd<DgHierNdx>& add,
       DgLocVector& chld) const
{
  chld.clearAddress();
  convert(chld);
  if (add.res() >= 0 && add.res() < (nRes() - 1)) {
     setAddNdxChildren(add, chld);
  }
  //std::cout << add << ": " << chld << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFSBase::setNdxChildren (const DgResAdd<DgQ2DICoord>& add,
       DgLocVector& chld) const
{
  DgResAdd<DgHierNdx> hierNdx = quantify(add);
  setNdxChildren(hierNdx, chld);
}

////////////////////////////////////////////////////////////////////////////////
DgLocVector*
DgHierNdxSystemRFSBase::makeNdxChildren (int res,
     const DgLocation& loc) const
{
  DgLocVector* chld = new DgLocVector(*this);
  setNdxChildren(res, loc, *chld);

  return chld;
}

////////////////////////////////////////////////////////////////////////////////
DgLocVector*
DgHierNdxSystemRFSBase::makeNdxChildren (const DgResAdd<DgHierNdx>& add) const
{
  DgLocVector* chld = new DgLocVector(*this);
  setNdxChildren(add, *chld);

  return chld;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
