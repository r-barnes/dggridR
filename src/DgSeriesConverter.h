#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

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
// DgSeriesConverter.h: DgSeriesConverter class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGSERIESCONVERTER_H
#define DGSERIESCONVERTER_H

#include "DgConverterBase.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
class DgSeriesConverter : public DgConverterBase {

   public:

      DgSeriesConverter (const std::vector<const DgConverterBase*>& series,
                         bool userGenerated = false);

      DgSeriesConverter (const DgRFBase& fromFrame,
                         const DgRFBase& toFrame,
                         bool userGenerated = false);

      DgSeriesConverter (const DgSeriesConverter& con)
         : DgConverterBase (con) { operator=(con); }

      virtual ~DgSeriesConverter (void);

      int size (void) const { return (int) series_.size(); }

      const DgConverterBase& converter (int ndx) const;

      DgSeriesConverter& operator= (const DgSeriesConverter& con)
         { copy(con.series_.begin(), con.series_.end(), series_.begin());
           return *this; }

   private:

      std::vector<const DgConverterBase*> series_;

      virtual DgAddressBase* createConvertedAddress
                              (const DgAddressBase& addIn) const;

};

////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& stream, const DgSeriesConverter& con);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
