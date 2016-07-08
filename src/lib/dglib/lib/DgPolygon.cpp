////////////////////////////////////////////////////////////////////////////////
//
// DgPolygon.cpp: DgPolygon class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgPolygon.h"
#include "DgContCartRF.h"

////////////////////////////////////////////////////////////////////////////////
void 
DgPolygon::densify (int ptsPerEdge)
{ 
   if (ptsPerEdge <= 0) return;

   const DgContCartRF* cc = dynamic_cast<const DgContCartRF*>(&rf());
   if (cc == 0) report("DgPolygon::densify() with non-CCRF", DgBase::Fatal);

   DgPolygon densVerts(*cc);

   vector<DgAddressBase*>& v0 = addressVec();
   vector<DgAddressBase*>& v1 = densVerts.addressVec();

   // for each edge
   for (unsigned long i = 0; i < v0.size(); i++)
   {
      DgDVec2D p1 = dynamic_cast< DgAddress<DgDVec2D>& >(*v0[i]).address();
      DgDVec2D p2 = dynamic_cast< DgAddress<DgDVec2D>& >( 
                                          *v0[(i + 1) % v0.size()]).address();

      long double fracStep = 1.0 / (ptsPerEdge + 1);
      long double frac = fracStep;

      // add the first point

      v1.push_back(new DgAddress<DgDVec2D>(p1));

      // add each densification point

      for (int j = 0; j < ptsPerEdge; j++)
      {
         DgDVec2D p3 = DgDVec2D::fracPoint(p1, p2, frac);
         frac += fracStep;

         v1.push_back(new DgAddress<DgDVec2D>(p3));
      }

      // p2 should get pushed next time
   }

   // now replace the original with the densified version

   *this = densVerts;

} // DgPolygon::densify

////////////////////////////////////////////////////////////////////////////////
