////////////////////////////////////////////////////////////////////////////////
//
// DgRFNetwork.h: DgRFNetwork class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRFNETWORK_H
#define DGRFNETWORK_H

#include <vector>

#include "DgUtil.h"

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
