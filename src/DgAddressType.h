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
// DgAddressType.h: discrete grid address types enum definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGADDRESSTYPE_H
#define DGADDRESSTYPE_H

#include <ostream>
#include "DgUtil.h"

namespace dgg { namespace addtype {

using namespace dgg::util;

enum DgAddressType { Geo, Plane, ProjTri, Q2DD, Q2DI, SeqNum, Vertex2DD, HierNdx,
    InvalidAddressType
};

static const std::string addTypeStrings[] = { "GEO", "PLANE", "PROJTRI", "Q2DD",
    "Q2DI", "SEQNUM", "VERTEX2DD", "HIERNDX",
    "NONE"
};

enum DgHierNdxSysType { Z7, ZOrder, Z3, InvalidHierNdxSysType };
static const std::string hierNdxSysTypeStrings[] = {
    "Z7", "ZORDER", "Z3", "NONE"
};

enum DgHierNdxFormType { Int64, DigitString, InvalidHierNdxFormType };
static const std::string hierNdxFormTypeStrings[] = {
    "INT64", "DIGIT_STRING", "NONE"
};

DgAddressType stringToAddressType (const std::string& str);
const std::string& to_string (DgAddressType t);

DgHierNdxSysType stringToHierNdxSysType (const std::string& str);
const std::string& to_string (DgHierNdxSysType t);

DgHierNdxFormType stringToHierNdxFormType (const std::string& str);
const std::string& to_string (DgHierNdxFormType  t);

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, DgAddressType obj)
{
   return stream << to_string(obj);

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, DgHierNdxSysType obj)
{
   return stream << to_string(obj);

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, DgHierNdxFormType obj)
{
   return stream << to_string(obj);

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

}} // namespace dgg::addtype

#endif
