////////////////////////////////////////////////////////////////////////////////
//
// DgConverterBase.h: DgConverterBase class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGCONVERTERBASE_H
#define DGCONVERTERBASE_H

#include <vector>

using namespace std;

#include "DgRFBase.h"
#include "DgLocation.h"

////////////////////////////////////////////////////////////////////////////////
class DgConverterBase {

   public:
      virtual ~DgConverterBase (void);
     
      static bool isTraceOn (void) { return isTraceOn_; }
      static bool setTraceOn (bool traceOnIn) 
                     { isTraceOn_ = traceOnIn; return isTraceOn(); }
      //static void setTraceStream (ostream& stream = cout) 
      //                     { traceStream_ = &stream; }
      //static ostream& traceStream (void) { return *traceStream_; }

      virtual DgLocation* convert (DgLocation* loc) const; // convert in place

      void forceConnectFrom (bool verify = true) const;
      void forceConnectTo   (bool verify = true) const;

      const DgRFBase& fromFrame (void) const { return *fromFrame_; }
      const DgRFBase& toFrame   (void) const { return *toFrame_; }
      
      bool userGenerated (void) const { return userGenerated_; }

   protected:

      DgConverterBase (const DgRFBase& fromFrame, 
                       const DgRFBase& toFrame, bool userGenerated = true);

      DgConverterBase (const DgConverterBase& con) { operator=(con); }

      DgConverterBase& operator= (const DgConverterBase& con);

      virtual DgAddressBase* createConvertedAddress 
                              (const DgAddressBase& addIn) const = 0;

      static bool isTraceOn_;
      //static ostream* traceStream_;

      DgRFBase* fromFrame_;
      DgRFBase* toFrame_;
      
      bool userGenerated_;

   friend class DgRFBase;
   friend class DgSeriesConverter;
};

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgConverterBase& con)
            { return stream << "{" << con.fromFrame().name()
                            << "->" << con.toFrame().name() << "}"; }

////////////////////////////////////////////////////////////////////////////////
class DgIdentityConverter : public DgConverterBase {

   public:

      DgIdentityConverter (const DgRFBase& frame)
          : DgConverterBase(frame, frame, false) {}

      virtual DgLocation* convert (DgLocation* loc) const { return loc; }

   protected:

      virtual DgAddressBase* createConvertedAddress 
                                  (const DgAddressBase& addIn) const
              { return fromFrame().createAddress(addIn); }

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
