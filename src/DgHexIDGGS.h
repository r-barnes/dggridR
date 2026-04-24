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
// DgHexIDGGS.h: hexagonal IDGGS class
//
// Version 7.0 - Kevin Sahr,11/16/14
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHEXIDGGS_H
#define DGHEXIDGGS_H

#include <cmath>

#include "DgApSeq.h"
#include "DgHexIDGG.h"
#include "DgIDGGS.h"
#include "DgAddressType.h"
using namespace dgg::addtype;

////////////////////////////////////////////////////////////////////////////////
class DgHexIDGGS : public DgIDGGS {

   public:

      static const DgHexIDGGS* makeRF (DgRFNetwork& network, const DgGeoSphRF& backFrame,
               const DgGeoCoord& vert0, long double azDegs, unsigned int aperture = 4,
               int nRes = 1, const std::string& name = "IDGGS", const std::string& projType = "ISEA",
               bool isApSeq = false, const DgApSeq& apSeq = DgApSeq::defaultApSeq,
               bool isMixed43 = false, int numAp4 = 0, bool isSuperfund = false,
               const DgHierNdxSysType hierNdxSysType = InvalidHierNdxSysType);

      // copy constructor and operator= not implemented

      const DgHexIDGG& hexIdgg (int res) const
             { return static_cast<const DgHexIDGG&>(idggBase(res)); }

      const DgApSeq& apSeq (void) const { return apSeq_; }

   protected:

      DgHexIDGGS (DgRFNetwork& network, const DgGeoSphRF& backFrame, const DgGeoCoord& vert0,
               long double azDegs, unsigned int aperture = 4, int nRes = 1,
               const std::string& name = "IDGGS", const std::string& projType = "ISEA",
               bool isApSeq = false, const DgApSeq& apSeq = DgApSeq::defaultApSeq,
               bool isMixed43 = false, int numAp4 = 0, bool isSuperfund = false,
               DgHierNdxSysType hierNdxSysType = InvalidHierNdxSysType);

      // pure virtual functions remaining from above

      virtual void setAddParents (const DgResAdd<DgQ2DICoord>& add,
                                  DgLocVector& vec) const;

      virtual void setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const;

      virtual void setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const;

      // second order boundary children (only in aperture 7)
      virtual void setAddBoundary2Children (const DgResAdd<DgQ2DICoord>& add,
                                           DgLocVector& vec) const;

      virtual void setAddAllChildren (const DgResAdd<DgQ2DICoord>& add,
                                      DgLocVector& vec) const;

   private:

      DgApSeq apSeq_;

   friend std::ostream& operator<< (std::ostream& stream, const DgHexIDGGS& dgg);
};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, const DgHexIDGGS& dggs)
{
   stream << ">>> DgHexIDGGS: " << (const DgIDGGSBase&) dggs << std::endl;
   stream << "ApSeq: " << dggs.apSeq() << std::endl;
   for (int r = 0; r < dggs.nRes(); r++)
   {
      stream << ">>>>> res " << r << ": '" << std::endl;
      stream << dggs.hexIdgg(r) << std::endl;
   }

   return stream;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
