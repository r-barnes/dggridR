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
// DgRadixString.h: DgRadixString class definitions
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRADIXSTRING_H
#define DGRADIXSTRING_H

#include "DgBase.h"
#include "DgString.h"

#include <ostream>

////////////////////////////////////////////////////////////////////////////////
class DgRadixString {

   public:

      static std::string digitInterleave (const DgRadixString& s1,
                                     const DgRadixString& s2,
                                     bool combineDigits = true)
          {
             if (s1.base() != s2.base()) {
                report("DgRadixString::digitInterleave(): "
                       "bases do not match", DgBase::Fatal);
             }

             if (s1.digits()[0] == '-' || s2.digits()[0] == '-') {
                report("DgRadixString::digitInterleave(): "
                       "negative value input", DgBase::Fatal);
             }

             std::string tmp1 = s1.digits();
             std::string tmp2 = s2.digits();

             int n1 = (int) tmp1.length();
             int n2 = (int) tmp2.length();

             // pad shorter one with 0's

             if (n2 < n1) {
                for (int i = 0; i < (n1 - n2); i++) tmp2 = "0" + tmp2;
             } else {
                for (int i = 0; i < (n2 - n1); i++) tmp1 = "0" + tmp1;
             }

             // now interleave

             std::string result;
             char c1[] = " ";
             char c2[] = " ";
             if (!combineDigits) {
                for (unsigned int i = 0; i < tmp1.length(); i++) {
                   c1[0] = tmp1[i];
                   c2[0] = tmp2[i];

                   result += std::string(c1) + std::string(c2);
                }
             } else {
                for (unsigned int i = 0; i < tmp1.length(); i++) {
                   c1[0] = tmp1[i];
                   c2[0] = tmp2[i];

                   int d1 = atoi(c1);
                   int d2 = atoi(c2);

                   result += dgg::util::to_string(d1 * s1.base() + d2);
                }
             }

             return result;
          }

      DgRadixString (void) : base_ (10), digits_ ("") { }

      DgRadixString (const DgRadixString& obj)
               : base_ (obj.base()), digits_ (obj.digits()) { }

      DgRadixString (unsigned int baseIn, const std::string& digitsIn)
               : base_ (baseIn), digits_ (digitsIn) { }

      DgRadixString (unsigned int baseIn, int digitsIn, int padWidth = -1)
               : base_ (baseIn), digits_ ("") { convert(digitsIn, padWidth); }

      DgRadixString& operator= (const DgRadixString& obj)
               {
                  if (&obj != this)
                  {
                     base_ = obj.base();
                     digits_ = obj.digits();
                  }

                  return *this;

               } // DgRadixString::operator=

      bool operator== (const DgRadixString& obj) const
               {
                  return (base() == obj.base() && digits() == obj.digits());

               } // bool DgRadixString::operator==

      bool operator!= (const DgRadixString& obj) const
               {
                  return !operator==(obj);

               } // bool DgRadixString::operator!=

      // get methods

      unsigned int base (void) const { return base_; }

      const std::string& digits (void) const { return digits_; }

      long long int value (void) const {

         long long int val = 0;
         int exponent = 1;
         for (int index = (int) digits().size() - 1;
              index >= 0; index--) {

            int d = digits()[index] - '0'; // convert digit to int
            val += d * exponent;
//dgcout << " ==== d: " << d << " exp: " << exponent << " val: " << val << std::endl;
            exponent *= base();
         }

         return val;
      }

      // set methods

      void setBase (unsigned int baseIn) { base_ = baseIn; }

      void setDigits (const std::string& digitsIn) { digits_ = digitsIn; }

      // conversion methods

      void convert (int val, int padWidth = -1)
         //
         // Algorithm from Knuth, Vol 2, with padding code by K. Sahr
         //
         // padWidth of -1 indicates no padding
         //
         {
            int pval = std::abs(val);

            int nd = pval % base();
            setDigits(dgg::util::to_string(nd));

            pval = pval / base();
            while (pval > 0)
            {
               nd = pval % base();

               setDigits(dgg::util::to_string(nd) + digits());

               pval = pval / base();
            }

            // pad to specified width

            int nPad = padWidth - (int) digits().length();
            for (int i = 0; i < nPad; i++) setDigits("0" + digits());

            // adjust if negative

            if (val < 0) setDigits("-" + digits());

         } // DgRadixString::convert()

   private:

      unsigned int base_;

      std::string digits_;

}; // class DgRadixString

////////////////////////////////////////////////////////////////////////////////
inline std::ostream&
operator<< (std::ostream& stream, const DgRadixString& obj)
{
   return stream << "base: " << obj.base() << " val: " << obj.digits();

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
