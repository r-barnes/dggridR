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
// DgContCartRF.h: DgContCartRF class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGCONTCARTRF_H
#define DGCONTCARTRF_H

#include "Dg2WayConverter.h"
#include "DgConverter.h"
#include "DgDVec2D.h"
#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
class DgContCartRF : public DgRF<DgDVec2D, long double> {

   public:

      static const DgContCartRF* makeRF (DgRFNetwork& networkIn,
                const string& nameIn = "ContCart")
      { return new DgContCartRF(networkIn, nameIn); }

      DgContCartRF& operator= (const DgContCartRF& rf)
         { DgRF<DgDVec2D, long double>::operator=(rf); return *this; }

      virtual long double dist (const DgDVec2D& add1, const DgDVec2D& add2) const
                       { return add1.distance(add2); }

      virtual string add2str (const DgDVec2D& add) const
                       { return string(add); }

      virtual string add2str (const DgDVec2D& add, char delimiter) const
                  { return dgg::util::to_string(add.x(), formatStr()) + delimiter +
			   dgg::util::to_string(add.y(), formatStr()); }

      virtual const char* str2add (DgDVec2D* add, const char* str,
                                   char delimiter) const
                  {    if (!add) add = new DgDVec2D();
                       return add->fromString(str, delimiter); }

      virtual string dist2str (const long double& dist) const
                       { return dgg::util::to_string(dist, formatStr()); }

      virtual long double dist2dbl (const long double& dist) const
                       { return dist; }

      virtual unsigned long long int dist2int (const long double& dist) const
                       { return static_cast<unsigned long long int>(dist); }

      virtual const DgDVec2D& undefAddress (void) const
                            { return DgDVec2D::undefDgDVec2D; }

      // the following routines are "back-doors" included for speed;
      // use with care!

      virtual DgAddressBase* vecAddress (const DgDVec2D& v) const
                    { return new DgAddress<DgDVec2D>(v); }

      virtual DgLocation* vecLocation (const DgDVec2D& v) const
                    { return makeLocation(v); }

      virtual DgDVec2D getVecAddress (const DgAddressBase& add) const
         { return static_cast< const DgAddress<DgDVec2D> &>(add).address(); }

      virtual DgDVec2D getVecLocation (const DgLocation& loc) const
                    { return *getAddress(loc); }

   protected:

      DgContCartRF (DgRFNetwork& networkIn, const string& nameIn = "ContCart")
         : DgRF<DgDVec2D, long double> (networkIn, nameIn)
           { setUndefLoc(makeLocation(undefAddress())); }

      DgContCartRF (const DgContCartRF& rf)
         : DgRF<DgDVec2D, long double>(rf)
           { setUndefLoc(makeLocation(undefAddress())); }
};

////////////////////////////////////////////////////////////////////////////////
class DgContAffineConverter :
              public DgConverter<DgDVec2D, long double, DgDVec2D, long double> {

   public:

      DgContAffineConverter (const DgRF<DgDVec2D, long double>& fromFrame,
                         const DgRF<DgDVec2D, long double>& toFrame,
                         long double scaleFac = 1.0L, long double rotDeg = 0.0L,
                         const DgDVec2D& translate = DgDVec2D(0.0L, 0.0L))
         : DgConverter<DgDVec2D, long double, DgDVec2D, long double> (fromFrame, toFrame),
           scaleFac_ (scaleFac), rotDeg_ (rotDeg), translate_ (translate) {}

      DgContAffineConverter (const DgContAffineConverter& con)
         : DgConverter<DgDVec2D, long double, DgDVec2D, long double> (con) {}

      long double scaleFac (void) const { return scaleFac_; }
      long double rotDeg   (void) const { return rotDeg_; }

      const DgDVec2D& translate (void) const { return translate_; }

      virtual DgDVec2D convertTypedAddress (const DgDVec2D& addIn) const
                {
                   DgDVec2D newAdd(addIn);
                   newAdd.scale(scaleFac_, scaleFac_);
                   newAdd.rotate(rotDeg_);
                   newAdd += translate_;
                   return newAdd;
                }

   protected:

      long double scaleFac_;
      long double rotDeg_;
      DgDVec2D translate_;

};

////////////////////////////////////////////////////////////////////////////////
class DgContInvAffineConverter : public DgContAffineConverter {

   public:

      DgContInvAffineConverter (const DgContAffineConverter& fwd)
         : DgContAffineConverter (
              static_cast<const DgRF<DgDVec2D, long double>&>(fwd.toFrame()),
              static_cast<const DgRF<DgDVec2D, long double>&>(fwd.fromFrame()),
              fwd.scaleFac(), fwd.rotDeg(), fwd.translate()) {}

      DgContInvAffineConverter (const DgRF<DgDVec2D, long double>& fromFrame,
                                const DgRF<DgDVec2D, long double>& toFrame,
                                long double scaleFac = 1.0L, long double rotDeg = 0.0L,
                                const DgDVec2D& translate = DgDVec2D(0.0L, 0.0L))
         : DgContAffineConverter (toFrame, fromFrame, scaleFac, rotDeg,
                                  translate) {}

      long double scaleFac (void) const { return scaleFac_; }
      long double rotDeg   (void) const { return rotDeg_; }

      const DgDVec2D& translate (void) const { return translate_; }

      virtual DgDVec2D convertTypedAddress (const DgDVec2D& addIn) const
                {
                   DgDVec2D newAdd(addIn);
                   newAdd -= translate_;
                   newAdd.rotate(-1.0L * rotDeg_);
                   newAdd.scale(1.0L / scaleFac_, 1.0L / scaleFac_);
                   return newAdd;
                }

};

////////////////////////////////////////////////////////////////////////////////
class Dg2WayContAffineConverter : public Dg2WayConverter {

   public:

      Dg2WayContAffineConverter (const DgRF<DgDVec2D, long double>& fromFrame,
                                 const DgRF<DgDVec2D, long double>& toFrame,
                                 long double scaleFac = 1.0L, long double rotDeg = 0.0L,
                                 const DgDVec2D& translate = DgDVec2D(0.0L, 0.0L))
         : Dg2WayConverter (*(new DgContAffineConverter(fromFrame, toFrame,
                                             scaleFac, rotDeg, translate)),
                            *(new DgContInvAffineConverter(fromFrame, toFrame,
                                             scaleFac, rotDeg, translate))) {}

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
