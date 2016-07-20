////////////////////////////////////////////////////////////////////////////////
//
// DgHexC1Grid2D.cpp: DgHexC1Grid2D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <stdint.h>

#include "DgHexC1Grid2D.h"
#include "DgPolygon.h"

////////////////////////////////////////////////////////////////////////////////
void
DgHexC1Grid2D::setAddVertices (const DgIVec2D& add, DgPolygon& vec) const
{

   vector<DgAddressBase*>& v = vec.addressVec();

   // find the center

#if DGDEBUG
cout << setprecision(20) << "^^^^^^^^^^^^^^^^^\nDgHexC1Grid2D::setAddVertices"
     << endl; 
cout << "^^ input: " << add << endl;
cout.flush();
#endif

   DgLocation tmpLoc;
   setAddPoint(add, tmpLoc);

   const DgDVec2D& cent = *(backFrame().getAddress(tmpLoc));

   long double r2 = r_ / 2.0L;
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( 0.0L,  r_)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-0.5L,  r2)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D(-0.5L, -r2)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( 0.0L, -r_)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( 0.5L, -r2)));
   v.push_back(new DgAddress<DgDVec2D>(cent + DgDVec2D( 0.5L,  r2)));

} // void DgHexC1Grid2D::setAddVertices

////////////////////////////////////////////////////////////////////////////////
void
DgHexC1Grid2D::setAddNeighbors (const DgIVec2D& add, DgLocVector& vec) const
{
   vector<DgAddressBase*>& v = vec.addressVec();

   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1, add.j() + 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(    add.i(), add.j() + 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1,     add.j())));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() - 1, add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(    add.i(), add.j() - 1)));
   v.push_back(new DgAddress<DgIVec2D>(DgIVec2D(add.i() + 1,     add.j())));

} // void DgHexC1Grid2D::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
DgIVec2D 
DgHexC1Grid2D::quantify (const DgDVec2D& point) const
{
   long double a1, a2;
   long double x1, x2;
   int64_t m1, m2;
   long double r1, r2;
   DgIVec2D add;

   a1 = fabs(point.x());
   a2 = fabs(point.y());

   /* first do a reverse conversion */

   x2 = a2 / M_SIN60;
   x1 = a1 + x2 / 2.0L;

   /* check if we have the center of a hex */

   m1 = static_cast<int64_t>(x1);
   m2 = static_cast<int64_t>(x2);

   /* otherwise round correctly */

   r1 = x1 - m1;
   r2 = x2 - m2;

   if (r1 < 0.5L)
   {
      if (r1 < 1.0L/3.0L)
      {
         if (r2 < (1.0L + r1) / 2.0L)
         {
            add.setI(m1);
            add.setJ(m2);
         }
         else
         {
            add.setI(m1);
            add.setJ(m2 + 1);
         }
      }
      else
      {
         if (r2 < (1.0L - r1))
         {
            add.setJ(m2);
         }
         else
         {
            add.setJ(m2 + 1);
         }

         if ((1.0L - r1) <= r2 && r2 < (2.0 * r1))
         {
            add.setI(m1 + 1);
         }
         else
         {
            add.setI(m1);
         }
      }
   }
   else
   {
      if (r1 < 2.0L/3.0L)
      {
         if (r2 < (1.0L - r1))
         {
            add.setJ(m2);
         }
         else
         {
            add.setJ(m2 + 1);
         }

         if ((2.0L * r1 - 1.0L) < r2 && r2 < (1.0L - r1))
         {
            add.setI(m1);
         }
         else
         {
            add.setI(m1 + 1);
         }
      }
      else
      {
         if (r2 < (r1 / 2.0L))
         {
            add.setI(m1 + 1);
            add.setJ(m2);
         }
         else
         {
            add.setI(m1 + 1);
            add.setJ(m2 + 1);
         }
      }
   }

   // now fold across the axes if necessary

   if (point.x() < 0.0L)
   {
      if ((add.j() % 2) == 0) // even
      {
         int64_t axisi = add.j() / 2;
         int64_t diff = add.i() - axisi;
         add.setI(add.i() - 2 * diff);
      }
      else
      {
         int64_t axisi = (add.j() + 1) / 2;
         int64_t diff = add.i() - axisi;
         add.setI(add.i() - (2 * diff + 1));
      }
   }

   if (point.y() < 0.0L) 
   {
      add.setI(add.i() - (2 * add.j() + 1) / 2);
      add.setJ(-1 * add.j());
   }

   return add;

} // DgIVec2D DgHexC1Grid2D::quantify

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
