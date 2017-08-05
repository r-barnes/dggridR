////////////////////////////////////////////////////////////////////////////////
//
// DgSeriesConverter.h: DgSeriesConverter class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGSERIESCONVERTER_H
#define DGSERIESCONVERTER_H

#include <iostream>

#include "DgConverterBase.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
class DgSeriesConverter : public DgConverterBase {

   public:

      DgSeriesConverter (const vector<const DgConverterBase*>& series,
                         bool userGenerated = false);
                         
      DgSeriesConverter (const DgRFBase& fromFrame, 
                         const DgRFBase& toFrame,
                         bool userGenerated = false);

      DgSeriesConverter (const DgSeriesConverter& con) 
         : DgConverterBase (con) { operator=(con); }

      virtual ~DgSeriesConverter (void);
      
      int size (void) const { return series_.size(); }

      const DgConverterBase& converter (int ndx) const;

      DgSeriesConverter& operator= (const DgSeriesConverter& con)
         { copy(con.series_.begin(), con.series_.end(), series_.begin()); 
           return *this; }
   
   private:
   
      vector<const DgConverterBase*> series_;

      virtual DgAddressBase* createConvertedAddress
                              (const DgAddressBase& addIn) const;

};

////////////////////////////////////////////////////////////////////////////////
ostream& operator<< (ostream& stream, const DgSeriesConverter& con);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
