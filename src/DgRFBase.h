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
// DgRFBase.h: DgRFBase class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRFBASE_H
#define DGRFBASE_H

#include "DgBase.h"
#include "DgDVec2D.h"
#include "DgLocBase.h"
#include "DgRFNetwork.h"
#include "DgUtil.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class DgConverterBase;
class DgLocation;
class DgLocVector;
class DgPolygon;
class DgDistanceBase;
class DgAddressBase;
class NuCell;

////////////////////////////////////////////////////////////////////////////////
class DgRFBase {

   public:

      static const unsigned int maxFmtStr = 50;

      virtual ~DgRFBase (void);

      int id (void) const { return id_; }

      string name (void) const { return name_; }

      bool isConnected (void) const { return connectTo_ && connectFrom_; }

      DgRFNetwork& network (void) const { return *network_; }

      ostream& traceToGround (ostream& stream = dgcout) const;

      const DgRFBase* connectTo   (void) const { return connectTo_; }
      const DgRFBase* connectFrom (void) const { return connectFrom_; }

      DgLocation* convert (DgLocation* loc) const; // converts in place

      DgPolygon& convert (DgPolygon& poly) const; // converts in place

      DgLocVector& convert (DgLocVector& vec) const; // converts in place

      void convert (DgLocBase* loc) const // converts in place
                { loc->convertTo(*this); }

      virtual DgDistanceBase* distance (const DgLocation& loc1,
                                        const DgLocation& loc2,
                                        bool convert = false) const = 0;

      DgLocation* createLocation (void) const; // creates 0 address

      virtual DgLocation* createLocation (const DgLocation& loc,
                                          bool convert = false) const = 0;

// USE_NUCELL is set in MakeIncludes
#ifdef USE_NUCELL
      virtual NuCell* createCell (const NuCell& cell,
                                          bool convert = false) const = 0;
#endif

      void setName (const string& nameIn) { name_ = nameIn; }

      virtual bool operator== (const DgRFBase& frame) const
               { return (id() == frame.id() && network() == frame.network()); }

      virtual bool operator!= (const DgRFBase& frame) const
               { return !operator==(frame); }

      DgRFBase& operator= (const DgRFBase& frame);

      int precision (void) { return precision_; }

      void setPrecision (int precIn) { precision_ = precIn; setFormatStr(); }

      const char* formatStr (void) const { return formatStr_; }

      virtual const DgLocation& undefLoc (void) const { return *undefLoc_; }

      virtual void jumpRF (DgLocation* loc) = 0;

      // the following routines are "back-doors" included for speed;
      // use with care!

      virtual DgAddressBase* vecAddress (const DgDVec2D&) const
                    { return 0; }

      virtual DgLocation* vecLocation (const DgDVec2D&) const
                    { return 0; }

      virtual DgDVec2D getVecAddress (const DgAddressBase&) const
                    { return DgDVec2D::undefDgDVec2D; }

      virtual DgDVec2D getVecLocation (const DgLocation&) const
                    { return DgDVec2D::undefDgDVec2D; }

   protected:

      DgRFBase (DgRFNetwork& networkIn, const string& nameIn)
         : network_ (&networkIn), name_ (nameIn), precision_ (6),
           connectTo_ (0), connectFrom_ (0), undefLoc_ (0)
           { id_ = networkIn.generateId(this); setFormatStr(); }

      DgRFBase (const DgRFBase& rf)
         : network_ (&rf.network()), precision_ (6),
           connectTo_ (0), connectFrom_ (0), undefLoc_ (0)
           { id_ = network_->generateId(this); setFormatStr(); }

      virtual DgLocation* buildLocation (DgAddressBase* addIn) const;
                             // does not make copy

      virtual DgAddressBase* createAddress (void) const = 0;

      virtual DgAddressBase* createAddress
                                 (const DgAddressBase& addIn) const = 0;

      virtual void copyAddress (const DgAddressBase& from,
                                      DgAddressBase* to) const = 0;

      virtual bool equalAddress (const DgAddressBase& add1,
                                 const DgAddressBase& add2) const = 0;

      void forceRF (DgLocBase* loc) { loc->rf_ = this; }

      void setUndefLoc (DgLocation* undefLoc);

   private:

      void setFormatStr (void)
            { snprintf(formatStr_, DgRFBase::maxFmtStr, "%%#.%dLF", precision()); }

      string toString        (const DgLocBase& lb) const;
      string toString        (const DgLocBase& lb, char delimiter) const;
      string toAddressString (const DgLocBase& lb) const;
      string toAddressString (const DgLocBase& lb, char delimiter) const;

      virtual string toString (const DgLocation&     loc)  const = 0;
      virtual string toString (const DgLocVector&    loc)  const = 0;
      virtual string toString (const DgDistanceBase& dist) const = 0;
      virtual long double toDouble (const DgDistanceBase& dist) const = 0;

      virtual unsigned long long int toInt (const DgDistanceBase& dist) const = 0;

      virtual string toAddressString (const DgLocation& loc)     const = 0;
      virtual string toAddressString (const DgLocVector& locVec) const = 0;

      virtual string toString (const DgLocation& loc, char delimiter) const = 0;
      virtual string toString (const DgLocVector& loc, char delimiter)
                                                                      const = 0;
      virtual string toAddressString (const DgLocation& loc, char delimiter)
                                                                      const = 0;
      virtual string toAddressString (const DgLocVector& locVec, char delimiter)
                                                                      const = 0;
      virtual const char* fromString (DgLocation& loc, const char* str,
                                      char delimiter) const = 0;

      int id_;

      char formatStr_[maxFmtStr];

      DgRFNetwork* network_;

      string name_;

      int precision_;

      DgRFBase* connectTo_;
      DgRFBase* connectFrom_;

   protected:
      DgLocation* undefLoc_;

   private:

   friend class DgLocation;
   friend class DgLocVector;
   friend class DgDistanceBase;
   friend class DgConverterBase;
   friend class DgRFNetwork;
   friend class DgIdentityConverter;
   friend class NuCell;
};

////////////////////////////////////////////////////////////////////////////////
inline DgRFBase&
DgRFBase::operator= (const DgRFBase& frame)
{
   network_ = &frame.network();
   name_ = frame.name();
   id_ = frame.id();

   return *this;

} // inline DgRFBase& DgRFBase::operator=

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgRFBase& frame)
            { return stream << frame.name(); }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
