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
// DgZOrderSystem.cpp: DgZOrderSystem class implementation
//
////////////////////////////////////////////////////////////////////////////////

//#define __STDC_FORMAT_MACROS
//#include <inttypes.h>
//#include <cmath>
//#include <climits>
//#include <cfloat>
//#include <string.h>

#include "DgHierNdx.h"
#include "DgZOrderSystem.h"
#include "DgHierNdxRF.h"
#include "DgHierNdxSystemRF.h"
#include "DgIVec3D.h"

class DgIDGGSBase;

/** max ZORDER resolution */
#define MAX_ZORDER_RES 30

/** The number of bits in a ZOrder index. */
#define ZORDER_NUM_BITS 64

/** The bit offset of the max resolution digit in a ZOrder index. */
#define ZORDER_MAX_OFFSET 63

/** The bit offset of the quad number in a ZOrder index. */
#define ZORDER_QUAD_OFFSET 60

/** 1's in the 4 quad number bits, 0's everywhere else. */
#define ZORDER_QUAD_MASK ((uint64_t)(15) << ZORDER_QUAD_OFFSET)

/** 0's in the 4 mode bits, 1's everywhere else. */
#define ZORDER_QUAD_MASK_NEGATIVE (~ZORDER_QUAD_MASK)

/** The number of bits in a single ZORDER resolution digit. */
#define ZORDER_PER_DIGIT_OFFSET 2

/** 1's in the 2 bits of highest res digit bits, 0's everywhere else. */
#define ZORDER_DIGIT_MASK ((uint64_t)(3))

/** Gets the integer quad number of a ZOrder index. */
#define ZORDER_GET_QUADNUM(z) ((int)((((z)&ZORDER_QUAD_MASK) >> ZORDER_QUAD_OFFSET)))

/** Sets the integer mode of z to v. */
#define ZORDER_SET_QUADNUM(z, v) \
    (z) = (((z)&ZORDER_QUAD_MASK_NEGATIVE) | (((uint64_t)(v)) << ZORDER_QUAD_OFFSET))

/** Gets the resolution res integer digit of z. */
#define ZORDER_GET_INDEX_DIGIT(z, res)                                        \
    ((int)((((z) >> ((MAX_ZORDER_RES - (res)) * ZORDER_PER_DIGIT_OFFSET)) & \
                  ZORDER_DIGIT_MASK)))

/** Sets the resolution res digit of z to the integer digit */
#define ZORDER_SET_INDEX_DIGIT(z, res, digit)                                  \
    (z) = (((z) & ~((ZORDER_DIGIT_MASK                                        \
                       << ((MAX_ZORDER_RES - (res)) * ZORDER_PER_DIGIT_OFFSET)))) | \
            (((uint64_t)(digit))                                            \
             << ((MAX_ZORDER_RES - (res)) * ZORDER_PER_DIGIT_OFFSET)))

////////////////////////////////////////////////////////////////////////////////
DgZOrderSystem::DgZOrderSystem (const DgIDGGSBase& dggsIn, bool extModeIntIn, const std::string& nameIn)
   : DgHierNdxSystemRFS<DgZOrderRF, DgZOrderStringRF>(dggsIn, extModeIntIn, nameIn)
{

}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxIntCoord
DgZOrderSystem::toIntCoord (const DgHierNdxStringCoord& addIn, int gridRes) const
{
    std::string addstr = addIn.valString();
    if (addstr.size() - 2 > MAX_ZORDER_RES) {
     report("DgZOrderSystem::toIntCoord(): "
        " input resolution exceeds max ZOrder resolution of 30", DgBase::Fatal);
    }

    uint64_t z = 0;

    // first get the quad number and add to the val
    std::string qstr = addstr.substr(0, 2);
    if (qstr[0] == '0') // leading 0
       qstr = qstr.substr(1, 1);
    int quadNum = std::stoi(qstr);
    ZORDER_SET_QUADNUM(z, quadNum);

    int index = 2; // skip the two quad digits

    // the rest is the radix string — length must match grid resolution
    std::string radStr = addstr.substr(index);
    if ((int) radStr.size() != gridRes) {
       report("DgZOrderSystem::toIntCoord(): Z-order digit string must have "
              "exactly gridRes radix digits after the base cell (2 digits); "
              "got " + dgg::util::to_string((int) radStr.size()) +
              " expected " + dgg::util::to_string(gridRes),
              DgBase::Fatal);
    }

    int r = 1;
    for (const char& digit: radStr) {
       int d = digit - '0'; // convert to int
       ZORDER_SET_INDEX_DIGIT(z, r, d);
       r++;
    }

    DgHierNdxIntCoord coord;
    coord.setValue(z);

    return coord;
}

////////////////////////////////////////////////////////////////////////////////
DgHierNdxStringCoord
DgZOrderSystem::toStringCoord (const DgHierNdxIntCoord& addIn, int gridRes) const
{
    uint64_t z = addIn.value();

    int quadNum = ZORDER_GET_QUADNUM(z);
    std::string s = dgg::util::to_string(quadNum, 2);

    for (int r = 1; r <= gridRes; r++) {
       // get the integer digit
       char d = ZORDER_GET_INDEX_DIGIT(z, r);
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
DgZOrderSystem::setAddNdxParent (const DgResAdd<DgHierNdx>& add,
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
DgZOrderSystem::setAddNdxChildren (const DgResAdd<DgHierNdx>& add,
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
           report("DgZOrderSystem::setAddNdxChildren(): "
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
