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
// DgGridTopo.cpp: DgGridTopo enum function implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgGridTopo.h"

#include <cmath>

namespace dgg { namespace topo {

static const string topoStrings[] =
               { "HEXAGON", "TRIANGLE", "SQUARE", "DIAMOND", "INVALID" };
static const string metricStrings[] =
               { "D3", "D4", "D6", "D8", "INVALID" };

DgGridTopology stringToGridTopology (const string& str) {
   for (int i = 0; i < InvalidTopo; i++)
      if (str == topoStrings[i]) return static_cast<DgGridTopology>(i);

   return InvalidTopo;
}

DgGridMetric stringToGridMetric (const string& str) {
   for (int i = 0; i < InvalidMetric; i++)
      if (str == metricStrings[i]) return static_cast<DgGridMetric>(i);

   return InvalidMetric;
}

const string& to_string (DgGridTopology t) {
   if (t <= InvalidTopo) return topoStrings[t];
   return topoStrings[InvalidTopo];
}

const string& to_string (DgGridMetric m) {
   if (m <= InvalidMetric) return metricStrings[m];
   return metricStrings[InvalidMetric];
}

}} // namespace dgg::topo
