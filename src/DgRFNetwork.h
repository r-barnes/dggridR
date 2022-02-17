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
// DgRFNetwork.h: DgRFNetwork class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRFNETWORK_H
#define DGRFNETWORK_H

#include "DgUtil.h"

#include <vector>

using namespace std;

class DgLocation;
class DgRFBase;
class DgConverterBase;

////////////////////////////////////////////////////////////////////////////////
class DgRFNetwork {

   public:

      DgRFNetwork (int chunkSize = 10)
         : nextId_ (0), chunkSize_ (chunkSize) { reserve(chunkSize_); }

      DgRFNetwork (const DgRFNetwork& net) { operator=(net); }

     ~DgRFNetwork (void);

      unsigned long long int size (void) const { return frames_.size(); }

      bool existsConverter (const DgRFBase& fromFrame,
                            const DgRFBase& toFrame) const;

      const DgConverterBase* getConverter (const DgLocation& fromLoc,
                                       const DgRFBase& toFrame) const;

      const DgConverterBase* getConverter (const DgRFBase& fromFrame,
                                       const DgRFBase& toFrame) const;

      void update (void);

      DgRFNetwork& operator= (const DgRFNetwork& net)
            { chunkSize_ = net.chunkSize_; return *this; }

      bool operator== (const DgRFNetwork& net) const
                           { return (this == &net); }

      bool operator!= (const DgRFNetwork& net) const
                           { return (this != &net); }

      // memory management methods

      size_t capacity (void) const { return frames_.capacity(); }
      void reserve (const size_t& capacity);

      int chunkSize (void) const { return chunkSize_; }
      void setChunkSize (int chunkSize) { chunkSize_ = chunkSize; }

   private:

      int nextId_;

      int chunkSize_;

      vector< DgRFBase* > frames_;

      vector< vector<DgConverterBase*> > matrix_;

      int generateId (DgRFBase* frame);

   friend class DgConverterBase;
   friend class DgRFBase;
   friend class DgSeriesConverter;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
