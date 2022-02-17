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
// DgConverter.h: DgConverter class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGCONVERTER_H
#define DGCONVERTER_H

#include <vector>

using namespace std;

#include "DgAddress.h"
#include "DgConverterBase.h"
#include "DgRF.h"

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
            dgcout << " " << addFrom.address() << "->" << addTo->address() << endl;
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
