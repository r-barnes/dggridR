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
// DgGridTopo.h: discrete grid topology and metric enum definitions
//
// Version 7.1 - Kevin Sahr, 3/11/21
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGGRIDTOPO_H
#define DGGRIDTOPO_H

#include <iostream>

using namespace std;

namespace dgg { namespace topo {

enum DgGridTopology { Hexagon, Triangle, Square, Diamond, InvalidTopo };
enum DgGridMetric { D3, D4, D6, D8, InvalidMetric };

DgGridTopology stringToGridTopology (const string& str);
DgGridMetric stringToGridMetric (const string& str);
const string& to_string (DgGridTopology t);
const string& to_string (DgGridMetric m);

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, DgGridTopology obj)
{
   return stream << to_string(obj);

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, DgGridMetric obj)
{
   return stream << to_string(obj);

} // ostream& operator<<

}} // end namespace

////////////////////////////////////////////////////////////////////////////////

#endif
