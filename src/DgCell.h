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
// DgCell.h: DgCell class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGCELL_H
#define DGCELL_H

#include <string>

#include "DgLocation.h"
#include "DgLocBase.h"
#include "DgPolygon.h"

////////////////////////////////////////////////////////////////////////////////
class DgCell : public DgLocBase {

   public:

      DgCell (void) : region_ (0) {}

      DgCell (const DgRFBase& rfIn, const string& labelIn,
              const DgLocation& nodeIn, DgPolygon* regionIn = 0)
         : DgLocBase(rfIn), label_ (labelIn), node_ (nodeIn),
           region_ (regionIn)
      { rf().convert(&node_); if (hasRegion()) rf().convert(region_); }

     ~DgCell (void) { delete region_; }

      const string&     label  (void) const { return label_; }
      const DgLocation& node   (void) const { return node_; }
      const DgPolygon&  region (void) const { return *region_; }

      string&     label  (void) { return label_; }
      DgLocation& node   (void) { return node_; }
      DgPolygon&  region (void) { return *region_; }

      bool hasRegion (void) const { return (region_ != 0); }

      void setLabel (const string& labelIn) { label_ = labelIn; }

      virtual void setNode (const DgLocation& nodeIn)
           { node_ = nodeIn; if (node_.rf() != rf()) rf().convert(&node_); }

      void setRegion (DgPolygon* regionIn)
           {
              delete region_;
              region_ = regionIn;
              if (hasRegion() && region_->rf() != rf()) rf().convert(region_);
           }

      DgCell& operator= (const DgCell& cell) // deep copy
      {
         if (this != &cell)
         {
            setLabel(cell.label());
            setNode(cell.node());
            if (cell.hasRegion())
            {
               DgPolygon* poly = new DgPolygon(cell.region());
               setRegion(poly);
            }
            else
               setRegion(0);
         }

	 return *this;
      }

      bool operator== (const DgCell& cell) const
           { return (cell.label() == label() && cell.node() == node() &&
              ((cell.hasRegion() && hasRegion() && cell.region() == region())
               || (!cell.hasRegion() && !hasRegion()))); }

      bool operator!= (const DgCell& cell) const
           { return !operator==(cell); }

      virtual string asString (void) const;
      virtual string asString (char delimiter) const;
      virtual string asAddressString (void) const;
      virtual string asAddressString (char delimiter) const;

      virtual const char* fromString (const char* str, char delimiter);

      virtual int cardinality (void) const
           { return 1 + region().cardinality(); }

      virtual void clearAddress (void)
           { node_.clearAddress(); if (hasRegion()) region_->clearAddress(); }

   protected:

      virtual void convertTo (const DgRFBase& rf);

   private:

      string label_;
      DgLocation node_;
      DgPolygon* region_;

   friend class DgInArcGen;
};

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgCell& cell)
            {
              stream << "[" << cell.label() << ":" << cell.node();
              if (cell.hasRegion()) stream << ":" << cell.region();
              return stream << endl;
            }

////////////////////////////////////////////////////////////////////////////////

#endif
