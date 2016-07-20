////////////////////////////////////////////////////////////////////////////////
//
// DgRFBase.h: DgRFBase class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRFBASE_H
#define DGRFBASE_H

#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>

#include "DgRFNetwork.h"
#include "DgLocBase.h"
#include "DgDVec2D.h"
#include "DgUtil.h"

using namespace std;

class DgConverterBase;
class DgLocation;
class DgLocVector;
class DgDistanceBase;
class DgAddressBase;

////////////////////////////////////////////////////////////////////////////////
class DgRFBase {

   public:

      virtual ~DgRFBase (void);

      int id (void) const { return id_; }

      string name (void) const { return name_; }

      bool isConnected (void) const { return connectTo_ && connectFrom_; }

      DgRFNetwork& network (void) const { return *network_; }

      ostream& traceToGround (ostream& stream = cout) const;

      const DgRFBase* connectTo   (void) const { return connectTo_; }
      const DgRFBase* connectFrom (void) const { return connectFrom_; }
      
      DgLocation* convert (DgLocation* loc) const; // converts in place

      DgLocVector& convert (DgLocVector& vec) const; // converts in place

      void convert (DgLocBase* loc) const // converts in place
                { loc->convertTo(*this); }

      virtual DgDistanceBase* distance (const DgLocation& loc1, 
                                        const DgLocation& loc2,
                                        bool convert = false) const = 0;

      DgLocation* createLocation (void) const; // creates 0 address

      virtual DgLocation* createLocation (const DgLocation& loc,
                                          bool convert = false) const = 0;

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

      virtual DgAddressBase* vecAddress (const DgDVec2D& v) const
                    { return 0; }

      virtual DgLocation* vecLocation (const DgDVec2D& v) const
                    { return 0; }

      virtual DgDVec2D getVecAddress (const DgAddressBase& add) const
                    { return DgDVec2D::undefDgDVec2D; }

      virtual DgDVec2D getVecLocation (const DgLocation& loc) const
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

   private:
   
      void setFormatStr (void)
            { sprintf(formatStr_, "%%#.%dLF", precision()); }

      string toString        (const DgLocBase& lb) const;
      string toString        (const DgLocBase& lb, char delimiter) const;
      string toAddressString (const DgLocBase& lb) const;
      string toAddressString (const DgLocBase& lb, char delimiter) const;

      virtual string toString (const DgLocation&     loc)  const = 0;
      virtual string toString (const DgLocVector&    loc)  const = 0;
      virtual string toString (const DgDistanceBase& dist) const = 0;
      virtual long double toDouble (const DgDistanceBase& dist) const = 0;

      virtual uint64_t toInt (const DgDistanceBase& dist) const = 0;

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

      char formatStr_[20];

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
