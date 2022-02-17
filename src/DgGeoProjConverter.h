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
// DgGeoProjConverter.h: DgGeoProjConverter class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGGEOPROJCONVERTER_H
#define DGGEOPROJCONVERTER_H

#include "DgEllipsoidRF.h"
#include "DgConverter.h"
#include "Dg2WayConverter.h"
#include "DgDVec2D.h"
#include "DgGeoProjRF.h"

#define EPS 1.0e-12L

////////////////////////////////////////////////////////////////////////////////
class DgGeoProjConverter :
              public DgConverter<DgGeoCoord, long double, DgDVec2D, long double> {

   public:

      DgGeoProjConverter (const DgRF<DgGeoCoord, long double>& fromFrameIn,
                          const DgRF<DgDVec2D, long double>& toFrameIn)
         : DgConverter<DgGeoCoord, long double, DgDVec2D, long double>
                  (fromFrameIn, toFrameIn), geoProjRF_ (0), ellipsoidRF_ (0)
        {
           geoProjRF_ = dynamic_cast<const DgGeoProjRF*>(&toFrame());
           if (!geoProjRF_)
           {
              report("DgGeoProjConverter::DgGeoProjConverter(): "
                 " toFrame not of type DgGeoProjRF", DgBase::Fatal);
           }

           ellipsoidRF_ = dynamic_cast<const DgEllipsoidRF*>(&fromFrame());
           if (!ellipsoidRF_)
           {
              report("DgGeoProjConverter::DgGeoProjConverter(): "
                 " toFrame not of type DgEllipsoidRF", DgBase::Fatal);
           }
        }

      DgGeoProjConverter (const DgGeoProjConverter& con)
         : DgConverter<DgGeoCoord, long double, DgDVec2D, long double> (con),
           geoProjRF_ (&con.geoProjRF()), ellipsoidRF_ (&con.ellipsoidRF()) { }

      virtual DgDVec2D convertTypedAddress (const DgGeoCoord& addIn) const
        {
           const DgEllipsoidRF& e = ellipsoidRF();
           const DgGeoProjRF& p = geoProjRF();

//cout << "cta: proj0: " << p.proj0() << " addIn: " << addIn << endl;
           DgGeoCoord lp(addIn);
           DgDVec2D xy;
           long double t;

           // check for forward and latitude or longitude overange

           if ((t = fabsl(lp.lat())-M_PI_2) > EPS || fabsl(lp.lon()) > 10.0L) {
                xy = DgDVec2D::undefDgDVec2D;
                report("DgGeoProjConverter::convertTypedAddress(): "
                   " lat or lon out of range", DgBase::Fatal);
           } else { /* proceed with projection */
              if (fabsl(t) <= EPS)
                 lp.setLat(lp.lat() < 0.0L ? -M_PI_2 : M_PI_2);
              else if (p.geoc())
                 lp.setLat(atanl(e.rone_es() * tanl(lp.lat())));
//cout << "cta: lp: " << lp << endl;
              lp.setLon(lp.lon() - p.lam0());  /* compute del lp.lam */
//cout << "cta: lp: " << lp << endl;
              if (!p.over()) {
                 if (lp.lon() < 0.0)
                    lp.setLon(lp.lon() + M_2PI);
                 if (lp.lon() > M_2PI)
                    lp.setLon(lp.lon() - M_2PI);
              }

//cout << "cta: lp: " << lp << endl;

              xy = p.projForward(lp, e);

              // adjust for major axis and easting/northings

              xy = DgDVec2D(p.fr_meter() * (e.a() * xy.x() + p.x0()),
              p.fr_meter() * (e.a() * xy.y() + p.y0()));
           }
           return xy;
        }

      // get methods

      const DgGeoProjRF& geoProjRF (void) const { return *geoProjRF_; }

      const DgEllipsoidRF& ellipsoidRF (void) const { return *ellipsoidRF_; }

   protected:

      const DgGeoProjRF* geoProjRF_;

      const DgEllipsoidRF* ellipsoidRF_;

   friend class Dg2WayGeoProjConverter;

};

////////////////////////////////////////////////////////////////////////////////
class DgGeoInvProjConverter :
              public DgConverter<DgDVec2D, long double, DgGeoCoord, long double> {

   public:

      DgGeoInvProjConverter (const DgRF<DgDVec2D, long double>& fromFrameIn,
                             const DgRF<DgGeoCoord, long double>& toFrameIn)
         : DgConverter<DgDVec2D, long double, DgGeoCoord, long double>
                  (fromFrameIn, toFrameIn), geoProjRF_ (0), ellipsoidRF_ (0)
        {
           geoProjRF_ = dynamic_cast<const DgGeoProjRF*>(&fromFrame());
           if (!geoProjRF_)
           {
              report("DgGeoInvrojConverter::DgGeoInvrojConverter(): "
                 " toFrame not of type DgGeoProjRF", DgBase::Fatal);
           }

           ellipsoidRF_ = dynamic_cast<const DgEllipsoidRF*>(&toFrame());
           if (!ellipsoidRF_)
           {
              report("DgGeoInvrojConverter::DgGeoInvrojConverter(): "
                 " toFrame not of type DgEllipsoidRF", DgBase::Fatal);
           }
      }

      DgGeoInvProjConverter (const DgGeoInvProjConverter& con)
         : DgConverter<DgDVec2D, long double, DgGeoCoord, long double> (con),
           geoProjRF_ (&con.geoProjRF()), ellipsoidRF_ (&con.ellipsoidRF()) { }

      virtual DgGeoCoord convertTypedAddress (const DgDVec2D& addIn) const
        {
           const DgGeoProjRF& p = geoProjRF();
           const DgEllipsoidRF& e = ellipsoidRF();

           DgDVec2D xy(addIn);
           DgGeoCoord lp;

           // can't do as much preliminary checking as with forward

           if (xy.x() == HUGE_VAL || xy.y() == HUGE_VAL) {
                lp = DgGeoCoord::undefGeoCoord;
                report("DgGeoInvrojConverter::convertTypedAddress(): "
                   " x or y out of range", DgBase::Fatal);
           }

           // descale and de-offset */

           xy = DgDVec2D((xy.x() * p.to_meter() - p.x0()) * e.ra(),
                    (xy.y() * p.to_meter() - p.y0()) * e.ra());

           lp = p.projInverse(xy, e);

           lp.setLon(lp.lon() + p.lam0()); // reduce from del lp.lam
           if (!p.over()) {
              if (lp.lon() < 0.0)
                 lp.setLon(lp.lon() + M_2PI);
              if (lp.lon() > M_2PI)
                 lp.setLon(lp.lon() - M_2PI);
           }

           if (p.geoc() && fabsl(fabsl(lp.lat())-M_PI_2) > EPS)
              lp.setLat(atanl(e.one_es() * tanl(lp.lat())));

           lp.normalize();

           return lp;
        }

      // get methods

      const DgGeoProjRF& geoProjRF (void) const { return *geoProjRF_; }

      const DgEllipsoidRF& ellipsoidRF (void) const { return *ellipsoidRF_; }

   protected:

      const DgGeoProjRF* geoProjRF_;

      const DgEllipsoidRF* ellipsoidRF_;

   friend class Dg2WayGeoProjConverter;

};

////////////////////////////////////////////////////////////////////////////////
class Dg2WayGeoProjConverter : public Dg2WayConverter {

   public:

      Dg2WayGeoProjConverter (const DgEllipsoidRF& fromFrame,
                              const DgGeoProjRF& toFrame)
         : Dg2WayConverter (*(new DgGeoProjConverter(fromFrame, toFrame)),
                            *(new DgGeoInvProjConverter(toFrame, fromFrame)))
         { }
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
