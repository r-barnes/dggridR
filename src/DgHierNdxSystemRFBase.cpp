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
// DgHierNdxSystemRFBase.cpp: DgHierNdxSystemRFBase class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgConverter.h"
#include "Dg2WayConverter.h"

#include "DgHierNdxSystemRFBase.h"
#include "DgHierNdx.h"
#include "DgHierNdxIntRF.h"
#include "DgHierNdxStringRF.h"
#include "DgHierNdxSystemRFSBase.h"

////////////////////////////////////////////////////////////////////////////////
DgHierNdxStringToIntConverter::DgHierNdxStringToIntConverter (
                  const DgHierNdxStringRF& from, const DgHierNdxIntRF& to)
         : DgConverter<DgHierNdxStringCoord, long long int,
                    DgHierNdxIntCoord, long long int> (from, to),
           sys(from.system())
      { }

////////////////////////////////////////////////////////////////////////////////
DgHierNdxIntCoord
DgHierNdxStringToIntConverter::convertTypedAddress
                                (const DgHierNdxStringCoord& addIn) const
{
   return sys.toIntCoord(addIn);
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxIntToStringConverter::
      DgHierNdxIntToStringConverter (const DgHierNdxIntRF& from,
            const DgHierNdxStringRF& to)
         : DgConverter<DgHierNdxIntCoord, long long int,
                    DgHierNdxStringCoord, long long int> (from, to),
           sys(from.system())
      { }

////////////////////////////////////////////////////////////////////////////////
DgHierNdxStringCoord
DgHierNdxIntToStringConverter::convertTypedAddress
                                (const DgHierNdxIntCoord& addIn) const
{
   return sys.toStringCoord(addIn);
}

//////////////////////////////////////////////////////////////////////////////////////
DgHierNdx2WayIntToStringConverter::DgHierNdx2WayIntToStringConverter (const DgHierNdxSystemRFBase& sys)
   : Dg2WayConverter (*(new DgHierNdxStringToIntConverter(
                               *sys.strRF(), *sys.intRF())),
                      *(new DgHierNdxIntToStringConverter(
                               *sys.intRF(), *sys.strRF())))
{
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxSystemRFBase::DgHierNdxSystemRFBase (
         const DgHierNdxSystemRFSBase& hierNdxRFSIn, int resIn, const std::string& nameIn)
   : DgDiscRF<DgHierNdx, DgQ2DICoord, long long int>(hierNdxRFSIn.dggs().network(),
              hierNdxRFSIn.dggs()[resIn], nameIn),
     hierNdxRFS_ (hierNdxRFSIn), dggs_ (hierNdxRFSIn.dggs()), res_ (resIn),
     aperture_ (hierNdxRFSIn.dggs().aperture()),
     pRes_ {nullptr, nullptr, nullptr}, curRes_ {nullptr, nullptr, nullptr},
     chRes_ {nullptr, nullptr, nullptr}
{
   // sub-classes need to assign appropriate RF's to curRes_
   // RFS has to call initialize to set up the parent and child systems
   // after the grids_ are all initialized
}

////////////////////////////////////////////////////////////////////////////////
std::string
DgHierNdxSystemRFBase::add2str (const DgHierNdx& add) const
{
    return std::string(add);
}

////////////////////////////////////////////////////////////////////////////////
std::string
DgHierNdxSystemRFBase::add2str (const DgHierNdx& add, char delimiter) const
{
    return std::string(add);
}

////////////////////////////////////////////////////////////////////////////////
const char*
DgHierNdxSystemRFBase::str2add (DgHierNdx* ndx, const char* str, char delimiter) const
{
  const char* newS = str;
  if (extModeInt()) {
     DgHierNdxIntCoord intNdx;
     newS = intRF()->str2add(&intNdx, str, delimiter);
     ndx->setIntNdx(intNdx);
     setStringFromIntCoord(*ndx);
  } else {
     DgHierNdxStringCoord strNdx;
     newS = strRF()->str2add(&strNdx, str, delimiter);
     ndx->setStrNdx(strNdx);
     setIntFromStringCoord(*ndx);
  }

  return newS;
}

////////////////////////////////////////////////////////////////////////////////
const DgHierNdx&
DgHierNdxSystemRFBase::undefAddress (void) const
{
   return DgHierNdx::undefCoord;
}

////////////////////////////////////////////////////////////////////////////////
bool
DgHierNdxSystemRFBase::extModeInt (void) const
{
   return hierNdxRFS().extModeInt();
}

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFBase::setIntFromStringCoord (DgHierNdx& hn) const
{
   hn.intNdx_ = this->toIntCoord(hn.strNdx());
}

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFBase::setStringFromIntCoord (DgHierNdx& hn) const
{
   hn.strNdx_ = this->toStringCoord(hn.intNdx());
}

////////////////////////////////////////////////////////////////////////////////
int
DgHierNdxSystemRFBase::setSystemSet (DgSystemSet& set, std::vector<DgHierNdxSystemRFBase*>& rfGrids, int res)
{
   set.dgg_ = nullptr;
   set.intRF_ = nullptr;
   set.strRF_ = nullptr;
   if (res < 0 || res >= hierNdxRFS_.nRes())
      return 1;

   // if we're here res is valid
   set.dgg_ = rfGrids[res]->dgg();
   set.intRF_ = rfGrids[res]->intRF();
   set.strRF_ = rfGrids[res]->strRF();

   return 0;
}

////////////////////////////////////////////////////////////////////////////////
void
DgHierNdxSystemRFBase::initialize (std::vector<DgHierNdxSystemRFBase*>& rfGrids)
{
   // current res set in the sub-class constructor
   //setSystemSet(curRes_, res_);

   setSystemSet(pRes_, rfGrids, res_ - 1);
   setSystemSet(chRes_, rfGrids, res_ + 1);

}

////////////////////////////////////////////////////////////////////////////////
DgHierNdx
DgHierNdxSystemRFBase::quantify (const DgQ2DICoord& point) const
{
   DgHierNdx ndx(extModeInt());

   ndx.strNdx_ = strRF()->quantify(point);
   setIntFromStringCoord(ndx);

   //DgResAdd<DgHierNdx> add(ndx, res());
   //return add;
   return ndx;
}

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord
DgHierNdxSystemRFBase::invQuantify (const DgHierNdx& ndx) const
{
   DgQ2DICoord point(strRF()->invQuantify(ndx.strNdx_));
   //std::cout << "KEVIN: " << ndx.strNdx_ << " " << point << std::endl;
   return point;
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxIntCoord
DgHierNdxSystemRFBase::toIntCoord (const DgHierNdxStringCoord& c) const
{
    return hierNdxRFS().toIntCoord(c, res_);
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxStringCoord
DgHierNdxSystemRFBase::toStringCoord (const DgHierNdxIntCoord& c) const
{
    return hierNdxRFS().toStringCoord(c, res_);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///

