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
// DgZ3System.cpp: DgZ3System class implementation
//
////////////////////////////////////////////////////////////////////////////////

//#define __STDC_FORMAT_MACROS
//#include <inttypes.h>
//#include <cmath>
//#include <climits>
//#include <cfloat>
//#include <string.h>

#include "DgHierNdx.h"
#include "DgZ3System.h"
#include "DgHierNdxRF.h"
#include "DgHierNdxSystemRF.h"

class DgIDGGSBase;

/** max Z3 resolution */
#define MAX_Z3_RES 30

/** The number of bits in a Z3 index. */
#define Z3_NUM_BITS 64

/** The bit offset of the max resolution digit in a Z3 index. */
#define Z3_MAX_OFFSET 63

/** The bit offset of the quad number in a Z3 index. */
#define Z3_QUAD_OFFSET 60

/** 1's in the 4 quad number bits, 0's everywhere else. */
#define Z3_QUAD_MASK ((uint64_t)(15) << Z3_QUAD_OFFSET)

/** 0's in the 4 mode bits, 1's everywhere else. */
#define Z3_QUAD_MASK_NEGATIVE (~Z3_QUAD_MASK)

/** The number of bits in a single Z3 resolution digit. */
#define Z3_PER_DIGIT_OFFSET 2

/** 1's in the 2 bits of highest res digit bits, 0's everywhere else. */
#define Z3_DIGIT_MASK ((uint64_t)(3))

/** Gets the integer quad number of a Z3 index. */
#define Z3_GET_QUADNUM(z) ((int)((((z)&Z3_QUAD_MASK) >> Z3_QUAD_OFFSET)))

/** Sets the integer mode of z to v. */
#define Z3_SET_QUADNUM(z, v) \
    (z) = (((z)&Z3_QUAD_MASK_NEGATIVE) | (((uint64_t)(v)) << Z3_QUAD_OFFSET))

/** Gets the resolution res integer digit of z. */
#define Z3_GET_INDEX_DIGIT(z, res)                                        \
    ((int)((((z) >> ((MAX_Z3_RES - (res)) * Z3_PER_DIGIT_OFFSET)) & \
                  Z3_DIGIT_MASK)))

/** Sets the resolution res digit of z to the integer digit */
#define Z3_SET_INDEX_DIGIT(z, res, digit)                                  \
    (z) = (((z) & ~((Z3_DIGIT_MASK                                        \
                       << ((MAX_Z3_RES - (res)) * Z3_PER_DIGIT_OFFSET)))) | \
            (((uint64_t)(digit))                                            \
             << ((MAX_Z3_RES - (res)) * Z3_PER_DIGIT_OFFSET)))

////////////////////////////////////////////////////////////////////////////////
int DgZ3System::defaultInvalidDigit = 3;

////////////////////////////////////////////////////////////////////////////////
DgZ3System::DgZ3System (const DgIDGGSBase& dggsIn, bool extModeIntIn, const std::string& nameIn)
   : DgHierNdxSystemRFS<DgZ3RF, DgZ3StringRF>(dggsIn, extModeIntIn, nameIn)
{

}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxIntCoord
DgZ3System::toIntCoord (const DgHierNdxStringCoord& addIn, int gridRes) const
{
    std::string addstr = addIn.valString();
    if (addstr.size() - 2 > MAX_Z3_RES) {
       report("DgZ3System::toIntCoord(): "
        " input resolution exceeds max Z3 resolution of 30", DgBase::Fatal);
    }

    // initialize the Z3 index so it is padded with the correct value
    static uint64_t fillDigit[4] = {
        UINT64_C(0x0000000000000000), // index 0: base-4 digit 0 repeated (00)
        UINT64_C(0x5555555555555555), // index 1: base-4 digit 1 repeated (01)
        UINT64_C(0xAAAAAAAAAAAAAAAA), // index 2: base-4 digit 2 repeated (10)
        UINT64_C(0xFFFFFFFFFFFFFFFF)  // index 3: base-4 digit 3 repeated (11)
    };

    uint64_t z = fillDigit[DgZ3System::defaultInvalidDigit];

    // first get the quad number and add to the val
    std::string qstr = addstr.substr(0, 2);
    if (qstr[0] == '0') // leading 0
       qstr = qstr.substr(1, 1);
    int quadNum = std::stoi(qstr);
    Z3_SET_QUADNUM(z, quadNum);

    int index = 2; // skip the two quad digits

    // the rest is the radix string — length must match grid resolution
    std::string radStr = addstr.substr(index);
    if ((int) radStr.size() != gridRes) {
       report("DgZ3System::toIntCoord(): Z3 digit string must have exactly "
              "gridRes radix digits after the base cell (2 digits); got " +
              dgg::util::to_string((int) radStr.size()) + " expected " +
              dgg::util::to_string(gridRes),
              DgBase::Fatal);
    }

    int r = 1;
    for (const char& digit: radStr) {
       int d = digit - '0'; // convert to int
       Z3_SET_INDEX_DIGIT(z, r, d);
       r++;
    }
 /*
    // pad as needed
    if (DgZ3RF::defaultInvalidDigit != 0) {
       while (r <= MAX_Z3_RES) {
          Z3_SET_INDEX_DIGIT(z, r, DgZ3RF::defaultInvalidDigit);
          r++;
       }
    }
  */

    DgHierNdxIntCoord coord;
    coord.setValue(z);

    return coord;

}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxStringCoord
DgZ3System::toStringCoord (const DgHierNdxIntCoord& addIn, int gridRes) const
{
    uint64_t z = addIn.value();

    int quadNum = Z3_GET_QUADNUM(z);
    std::string s = dgg::util::to_string(quadNum, 2);

    for (int r = 1; r <= gridRes; r++) {
       // get the integer digit
       char d = Z3_GET_INDEX_DIGIT(z, r);
       // convert to char
       d += '0';
       // append to index string
       s += d;
    }

    DgHierNdxStringCoord zStr;
    zStr.setValue(s);

    return zStr;
}

////////////////////////////////////////////////////////////////////////////////
void
DgZ3System::setAddNdxParent (const DgResAdd<DgHierNdx>& add,
                                   DgLocation& parent) const
{
    // res has already been verified by the caller
    int pRes = add.res() - 1;
    std::string addStr = add.address().strNdx().value();
    std::string pStr = addStr.substr(0, addStr.size() - 1);

    // build the parent address
    DgResAdd<DgHierNdx> pAdd;
    initNdxFromString(pAdd, pRes, pStr);
    forceAddress(&parent, pAdd);
}

////////////////////////////////////////////////////////////////////////////////
void
DgZ3System::setAddNdxChildren (const DgResAdd<DgHierNdx>& add,
                                     DgLocVector& children) const
{
    int chdRes = add.res() + 1;
    std::string valStr = add.address().strNdx().value();

    // first get the base cell number
    std::string quadStr = valStr.substr(0, 2);
    if (quadStr[0] == '0') // leading 0
          quadStr = quadStr.substr(1, 1);
      int quadNum = std::stoi(quadStr);
      if (quadNum < 0 || quadNum > 11) {
           report("DgZ3System::setAddNdxChildren(): "
              "index has invalid base cell number", DgBase::Fatal);
     }

    std::string addStr = valStr.substr(2);

    // KEVIN: this should all be done with integers and c_str's
    // assume no skip digit
    int skipDigit = -1;
    // check if current address is all zeros
    size_t pos = addStr.find_first_not_of('0');
    if (pos == std::string::npos) { // all digits are zero
        skipDigit = (quadNum <= 5) ? 2 : 5;
    }

    children.clearAddress();
    std::vector<DgAddressBase*>& v = children.addressVec();
    for (int i = 0; i <= 6; i++) {
        // KEVIN pentagon sub-sequence check

        // skip pentagon sub-sequence digits as per above
        if (i == skipDigit)
            continue;

        // build the child address
        std::string chdStr = valStr + std::to_string(i);
        DgResAdd<DgHierNdx> chdAdd;
        initNdxFromString(chdAdd, chdRes, chdStr);

        v.push_back(new DgAddress<DgResAdd<DgHierNdx>>(chdAdd));
    }
}

////////////////////////////////////////////////////////////////////////////////
