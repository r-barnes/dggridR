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
// DgZOrderStringRF.cpp: DgZOrderStringRF class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "DgZOrderStringRF.h"
#include "DgIVec3D.h"
#include "DgRadixString.h"

////////////////////////////////////////////////////////////////////////////////
DgZOrderStringRF::DgZOrderStringRF (const DgHierNdxSystemRFBase& sysIn, int resIn,
              const std::string& nameIn)
   : DgHierNdxStringRF(sysIn, resIn, nameIn), unitScaleClassIres_ (0)
{
    effRes_ = res();       // effective resolution
    effRadix_ = dgg().radix();   // effective radix
    if (aperture() == 3) {
        effRadix_ = 3;
        // effRes_ is the number of Class I resolutions
        effRes_ = (res() + 1) / 2;
    }
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxStringCoord
DgZOrderStringRF::quantify (const DgQ2DICoord& addIn) const
{
    std::string qstr = dgg::util::to_string(addIn.quadNum(), 2);
    std::string addstr = qstr;

    if (effRes() > 0) {
 //dgcout << "** addIn " << addIn << std::endl;
       DgRadixString rs1(effRadix(), (int) addIn.coord().i(), effRes());
       DgRadixString rs2(effRadix(), (int) addIn.coord().j(), effRes());

 //dgcout << "rs1 " << rs1 << std::endl;
 //dgcout << "rs2 " << rs2 << std::endl;

 /*
    if (IDGG().aperture() == 3) {
       dgcout << "Class " << ((IDGG().isClassI()) ? "I" : "II") << std::endl;
    }
 */
       addstr = addstr +
           DgRadixString::digitInterleave(rs1, rs2, !((dgg().aperture() == 3)));

 //dgcout << "addstr " << addstr << std::endl;
    // trim last digit if Class II
       if (aperture() == 3 && !dgg().isClassI() && addstr.length()) {
          addstr.pop_back();
       }
 //dgcout << "trimmed " << addstr << std::endl;
    }

    DgHierNdxStringCoord c;
    c.setValue(addstr);

   return c;
}

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord
DgZOrderStringRF::invQuantify (const DgHierNdxStringCoord& addIn) const
{
    std::string addstr = addIn.valString();

    // first get the quad number
    std::string qstr = addstr.substr(0, 2);
    if (qstr[0] == '0') // leading 0
       qstr = qstr.substr(1, 1);
    int quadNum = std::stoi(qstr);

    int index = 2; // skip the two quad digits

    // the rest is the radix string
    std::string radStr = addstr.substr(index);

    // split out the interleaved digits
    std::string radStr1 = "";
    std::string radStr2 = "";

    if (aperture() == 3) {
       bool isIdigit = true; // first char is an i digit
       int lastIdigit = 0;
       for (const char& digit: radStr) {
          if (isIdigit) {
             radStr1 += dgg::util::to_string(digit);
             lastIdigit = digit - '0';
          } else
             radStr2 += dgg::util::to_string(digit);

          isIdigit = !isIdigit;
       }

       if (!dgg().isClassI()) {
          // add the last j digit based on the last i digit
          std::string jDigits[] = { "0", "2", "1" };
          radStr2 += jDigits[lastIdigit];
       }

    } else {

       for (const char& digit: radStr) {

          // break out the interleaved digits
          int d = digit - '0'; // convert to int
          int c1 = (int) (d / effRadix_);

          int c2 = (d % effRadix_);

          radStr1 += dgg::util::to_string(c1);
          radStr2 += dgg::util::to_string(c2);
       }
    }

    DgRadixString rad1(effRadix_, radStr1);
    DgRadixString rad2(effRadix_, radStr2);

    //dgcout << "qstr: " << qstr << " rad1: " << rad1 << " rad2: " << rad2 << std::endl;

    DgQ2DICoord q2di(quadNum, DgIVec2D(rad1.value(), rad2.value()));
    //dgcout << "q2di: " << q2di << std::endl;

    return q2di;
}

////////////////////////////////////////////////////////////////////////////////
