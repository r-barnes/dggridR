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
// DgPolygon.h: DgPolygon class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGPOLYGON_H
#define DGPOLYGON_H

#include <vector>
#include "DgLocVector.h"

////////////////////////////////////////////////////////////////////////////////
class DgPolygon : public DgLocVector {

   public:

      DgPolygon (void) { }

      DgPolygon (const DgLocVector& poly)  // deep copy
         : DgLocVector (poly) { }

      DgPolygon (const DgPolygon& poly);   // deep copy

      DgPolygon (const DgRFBase& rfIn, int sizeIn = 0)
         : DgLocVector (rfIn, sizeIn) { }

     ~DgPolygon (void);

      const DgPolygon& operator= (const DgPolygon& vec); // deep copy

      bool operator== (const DgPolygon& vec) const;

      bool operator!= (const DgPolygon& vec) const
            { return !operator==(vec); }

      void densify (int ptsPerEdge);

      void clearHoles (void);

      bool hasHoles (void) const { return holes_.size() > 0; }

      void addHole (DgPolygon* hole); // does not make copy

      const vector<DgPolygon*>& holes (void) const { return holes_; }

   protected:

      // takes ownership of holes' memory
      vector<DgPolygon*> holes_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgPolygon& poly)
{
   stream << poly.rf().name() << " {\n";

   if (poly.hasHoles())
      stream << "[\n";

   for (int i = 0; i < poly.size(); i++) stream << poly[i] << "\n";

   if (poly.hasHoles()) {
      stream << "][\n";
      for (unsigned long i = 0; i < poly.holes().size(); i++) 
         stream << *poly.holes()[i];
      stream << "]\n";
   }

   return stream << "}" << endl;

} // inline ostream& operator<<

#endif
