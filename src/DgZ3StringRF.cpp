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
// DgZ3StringRF.cpp: DgZ3StringRF class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "DgZ3StringRF.h"
#include "DgIVec2D.h"
#include "DgRadixString.h"

////////////////////////////////////////////////////////////////////////////////
//const DgZ3StringCoord DgZ3StringCoord::undefDgZ3StringCoord(0xffffffffffffffff);

////////////////////////////////////////////////////////////////////////////////
DgZ3StringRF::DgZ3StringRF (const DgHierNdxSystemRFBase& sysIn, int resIn,
              const std::string& nameIn)
   : DgHierNdxStringRF(sysIn, resIn, nameIn), unitScaleClassIres_ (0)
{
    //number of Class I resolutions
    effRes_ = (dgg().res() + 1) / 2;
    effRadix_ = 3;
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxStringCoord
DgZ3StringRF::quantify (const DgQ2DICoord& addIn) const
{
    std::string qstr = dgg::util::to_string(addIn.quadNum(), 2);
    std::string addstr = qstr;

    if (effRes_ > 0) {
       DgRadixString rs1(effRadix_, (int) addIn.coord().i(), effRes_);
       DgRadixString rs2(effRadix_, (int) addIn.coord().j(), effRes_);

       std::string digits1 = rs1.digits();
       std::string digits2 = rs2.digits();

       int n1 = (int) digits1.length();
       int n2 = (int) digits2.length();

       if (n2 < n1) {
          for (int i = 0; i < (n1 - n2); i++) digits2 = "0" + digits2;
       } else {
          for (int i = 0; i < (n2 - n1); i++) digits1 = "0" + digits1;
       }

       static const std::string z3digits[3][3] = {
           {"00", "22", "21"},
           {"01", "02", "20"},
           {"12", "10", "11"}
       };

       for (unsigned int i = 0; i < digits1.length(); i++) {
          addstr = addstr +
               z3digits[digits1[i] - '0'][digits2[i] - '0'];
       }

       if (!dgg().isClassI() && addstr.length())
          addstr.pop_back();
    }

    DgHierNdxStringCoord c;
    c.setValue(addstr);
    return c;
}

////////////////////////////////////////////////////////////////////////////////
DgQ2DICoord
DgZ3StringRF::invQuantify (const DgHierNdxStringCoord& addIn) const
{
    std::string addstr = addIn.valString();

    // first get the quad number
    std::string qstr = addstr.substr(0, 2);
    if (qstr[0] == '0') // leading 0
       qstr = qstr.substr(1, 1);
    int quadNum = std::stoi(qstr);

    // res 0 is just the quad number
    if (effRes_ == 0)
       return DgQ2DICoord(quadNum, DgIVec2D(0, 0));

    int index = 2; // skip the two quad digits

    // the rest is the Z3 digit string
    std::string z3str = addstr.substr(index);

 //dgcout << "z3str in: " << z3str;

    // adjust if Class II (odd res)
    if (z3str.length() % 2)
       z3str += "0";
 //dgcout << " adjusted: " << z3str << std::endl;

    // build the digit string for i and j from the two-digit
    // z3 codes
    std::string radStr1 = "";
    std::string radStr2 = "";
    for (int i = 0; i < z3str.length(); i += 2) {
       std::string z3code = z3str.substr(i, 2);
       if (z3code == "00") {
          radStr1 += "0";
          radStr2 += "0";
       } else if (z3code == "22") {
          radStr1 += "0";
          radStr2 += "1";
       } else if (z3code == "21") {
          radStr1 += "0";
          radStr2 += "2";
       } else if (z3code == "01") {
          radStr1 += "1";
          radStr2 += "0";
       } else if (z3code == "02") {
          radStr1 += "1";
          radStr2 += "1";
       } else if (z3code == "20") {
          radStr1 += "1";
          radStr2 += "2";
       } else if (z3code == "12") {
          radStr1 += "2";
          radStr2 += "0";
       } else if (z3code == "10") {
          radStr1 += "2";
          radStr2 += "1";
       } else if (z3code == "11") {
          radStr1 += "2";
          radStr2 += "2";
       }
 //      dgcout << "z3code: " << z3code << " radStr1: " << radStr1
 //             << " radStr2: " << radStr2 << std::endl;
    }

    DgRadixString rad1(effRadix_, radStr1);
    DgRadixString rad2(effRadix_, radStr2);

 //   dgcout << "qstr: " << qstr << " rad1: " << rad1 << " rad2: " << rad2 << std::endl;

    DgQ2DICoord q2di(quadNum, DgIVec2D(rad1.value(), rad2.value()));
    //dgcout << "q2di: " << q2di << std::endl;

    return q2di;
}

////////////////////////////////////////////////////////////////////////////////
