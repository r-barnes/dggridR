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
// DgAddressType.cpp: DgAddressType enum function implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgAddressType.h"

#include <cmath>

namespace dgg { namespace addtype {

////////////////////////////////////////////////////////////////////////////////
DgAddressType stringToAddressType (const std::string& str) {
   for (int i = 0; i < InvalidAddressType; i++)
      if (str == addTypeStrings[i]) return static_cast<DgAddressType>(i);

   return InvalidAddressType;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& to_string (DgAddressType t) {
   if (t <= InvalidAddressType) return addTypeStrings[t];
   return addTypeStrings[InvalidAddressType];
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxSysType stringToHierNdxSysType (const std::string& str) {
   for (int i = 0; i < static_cast<int>(DgHierNdxSysType::InvalidHierNdxSysType); i++)
      if (str == hierNdxSysTypeStrings[i]) return static_cast<DgHierNdxSysType>(i);

   return DgHierNdxSysType::InvalidHierNdxSysType;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& to_string (DgHierNdxSysType t) {
   if (t <= DgHierNdxSysType::InvalidHierNdxSysType) return hierNdxSysTypeStrings[static_cast<int>(t)];
   return hierNdxSysTypeStrings[static_cast<int>(DgHierNdxSysType::InvalidHierNdxSysType)];
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxFormType stringToHierNdxFormType (const std::string& str) {
   for (int i = 0; i < InvalidHierNdxFormType; i++)
      if (str == hierNdxFormTypeStrings[i]) return static_cast<DgHierNdxFormType>(i);

   return InvalidHierNdxFormType;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& to_string (DgHierNdxFormType t) {
   if (t <= InvalidHierNdxFormType) return hierNdxFormTypeStrings[t];
   return hierNdxFormTypeStrings[InvalidHierNdxFormType];
}

}} // namespace dgg::addtype
