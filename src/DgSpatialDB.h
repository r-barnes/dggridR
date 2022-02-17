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
// DgSpatialDB.h: DgSpatialDB class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGSPATIALDB_H
#define DGSPATIALDB_H

#include "DgPhysicalRF.h"

class DgLocation;

////////////////////////////////////////////////////////////////////////////////
template<class C> class DgSpatialDB {

   public:

      class iterator : public DgLocation {

         public:

            iterator (DgSpatialDB<C>& db)
               : DgLocation (db.rf()), db_ (&db) {}

            iterator (DgSpatialDB<C>& db, const DgLocation& loc)
               : DgLocation (db.rf()), db_ (&db) { *this = loc; }

            iterator (const iterator& it)
               : DgLocation (it), db_ (&it.db()) { }

            iterator& operator++ (void)
                { db().boundedRF().incrementLocation(*this, true);
                  return *this; }

            iterator& operator-- (void)
                { db().boundedRF().decrementLocation(*this, true);
                  return *this; }

            C* operator* (void) const
                { return db_->getContents(*this, false); }

            C& operator-> (void) { return *(db_->getContents(*this, true)); }

            const DgSpatialDB<C>& db (void) const { return *db_; }

            iterator& operator= (const iterator& it)
                { DgLocation::operator=(it); db_ = &it.db(); return *this; }

            iterator& operator= (const DgLocation& loc)
                { DgLocation tmpLoc(loc); rf().convert(&tmpLoc);
                  DgLocation::operator=(tmpLoc); return *this; }

            bool operator== (const iterator& it) const
                   { return (DgLocation::operator==(it) && it.db() == db()); }

            bool operator!= (const iterator& it) const
                   { return !operator==(it); }

         protected:

            const DgSpatialDB<C>* db_;
      };

      DgSpatialDB<C> (DgPhysicalRFBase<C>& rfIn)
         : physicalRF_ (rfIn), begin_ (*this), end_ (*this)
         {  begin_ = boundedRF().first(); end_ = boundedRF().end();
            rfIn.initAllCells(); }

      virtual ~DgSpatialDB (void) { }

      const DgPhysicalRFBase<C>& physRF (void) const { return physicalRF_; }

      const DgBoundedRFBase& boundedRF (void) const
                     { return physicalRF_.boundedRFBase(); }

      const DgRFBase& rf (void) const { return boundedRF().rf(); }

      bool operator== (const DgSpatialDB<C>& dbIn) const
         { return physicalRF_ == dbIn.physicalRF_; }

      bool operator!= (const DgSpatialDB<C>& dbIn) const
         { return !operator==(dbIn.physicalRF_); }

      C* getContents (const DgLocation& loc, bool convert = true,
                      bool allocate = false) const
            { return physicalRF_.getContents(loc, convert, allocate); }

      void replaceContents (const DgLocation& loc, C* cont,
                            bool convert = true) // no copy
            { physicalRF_.replaceContents(loc, cont, convert); }

      void setContents (const DgLocation& loc, const C& cont,
                        bool convert = true) // makes copy
            { physicalRF_.setContents(loc, cont, convert); }

      void deleteAddContents (const DgLocation& loc, bool convert = true)
            { physicalRF_.deleteAddContents(loc, convert); }

      bool validAddress (const DgLocation& loc, bool convert = true) const
            { return rf().validAddress(loc, convert); }

      virtual const iterator& begin (void) const { return begin_; }
      virtual const iterator& end   (void) const { return end_; }

   protected:

      DgPhysicalRFBase<C>& physicalRF_;
      iterator begin_;
      iterator end_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
