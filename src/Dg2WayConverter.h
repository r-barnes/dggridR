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
// Dg2WayConverter.h: Dg2WayConverter class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DG2WAYCONVERTER_H
#define DG2WAYCONVERTER_H

#include "DgBase.h"
#include "DgConverterBase.h"

class DgRFBase;

////////////////////////////////////////////////////////////////////////////////
class Dg2WayConverter {

   public:

      Dg2WayConverter (const DgConverterBase& forward,
                       const DgConverterBase& inverse);

      Dg2WayConverter (const Dg2WayConverter& con)
         : forward_ (&con.forward()), inverse_ (&con.inverse()) {}

      Dg2WayConverter& operator= (const Dg2WayConverter& con)
         { forward_ = &con.forward(); inverse_ = &con.inverse();
           return *this; }

      const DgConverterBase& forward (void) const { return *forward_; }
      const DgConverterBase& inverse (void) const { return *inverse_; }

   protected:

      const DgConverterBase* forward_;
      const DgConverterBase* inverse_;

};

////////////////////////////////////////////////////////////////////////////////
inline
Dg2WayConverter::Dg2WayConverter (const DgConverterBase& forward,
                                  const DgConverterBase& inverse)
{
   if (forward.fromFrame() != inverse.toFrame() ||
       forward.toFrame()   != inverse.fromFrame())
   {
      report("Dg2WayConverter::Dg2WayConverter() rf mismatch", DgBase::Fatal);
   }

   forward_ = &forward;
   inverse_ = &inverse;

} // Dg2WayConverter::Dg2WayConverter

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
