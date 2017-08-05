////////////////////////////////////////////////////////////////////////////////
//
// DgConverter.h: DgConverter class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGCONVERTER_H
#define DGCONVERTER_H

#include <vector>

using namespace std;

#include "DgRF.h"
#include "DgConverterBase.h"
#include "DgAddress.h"

////////////////////////////////////////////////////////////////////////////////
template <class AFrom, class DFrom, class ATo, class DTo> class DgConverter : 
public DgConverterBase {

   public:

      DgConverter (const DgRF<AFrom, DFrom>& fromFrame, 
                   const DgRF<ATo, DTo>& toFrame)
          : DgConverterBase(fromFrame, toFrame) {}

      DgConverter (const DgConverter& con) : DgConverterBase(con) {}

      DgConverter& operator= (const DgConverter& con)
         { DgConverterBase::operator=(con); return *this; }

      virtual ATo convertTypedAddress (const AFrom& addIn) const = 0;

   protected:

      virtual DgAddressBase* createConvertedAddress 
                                  (const DgAddressBase& addIn) const
#if DGDEBUG
          { 
            const DgAddress<AFrom>& addFrom = 
                static_cast<const DgAddress<AFrom>&>(addIn).address();
            DgAddress<ATo> *addTo = 
                //new DgAddress<ATo>(convertTypedAddress(addFrom));
                new DgAddress<ATo>(convertTypedAddress(
                static_cast<const DgAddress<AFrom>&>(addIn).address()));
            cout << " " << addFrom.address() << "->" << addTo->address() << endl;
            return addTo;
          }
#else
          { return new DgAddress<ATo>(convertTypedAddress(
                static_cast<const DgAddress<AFrom>&>(addIn).address())); }
#endif

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
