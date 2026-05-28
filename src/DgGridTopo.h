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
// DgGridTopo.h: discrete grid topology and metric enum definitions
//
// Version 7.1 - Kevin Sahr, 3/11/21
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGGRIDTOPO_H
#define DGGRIDTOPO_H

#include <ostream>

#include "DgUtil.h"

namespace dgg { namespace topo {

enum DgGridTopology { Hexagon, Triangle, Square, Diamond, InvalidTopo };
enum DgGridMetric { D3, D4, D6, D8, InvalidMetric };

DgGridTopology stringToGridTopology (const std::string& str);
DgGridMetric stringToGridMetric (const std::string& str);
const std::string& to_string (DgGridTopology t);
const std::string& to_string (DgGridMetric m);

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, DgGridTopology obj)
{
   return stream << to_string(obj);

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, DgGridMetric obj)
{
   return stream << to_string(obj);

} // std::ostream& operator<<

}} // end namespace

////////////////////////////////////////////////////////////////////////////////

#endif
