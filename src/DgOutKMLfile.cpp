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
// DgOutKMLfile.cpp: DgOutKMLfile class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iostream>

#include "DgOutKMLfile.h"
#include "DgLocList.h"
#include "DgPolygon.h"
#include "DgLocation.h"
#include "DgCell.h"
#include "DgGeoSphRF.h"

DgOutKMLfile::DgOutKMLfile(const DgGeoSphDegRF& rf, const std::string& filename,
    int precision, bool isPointFile, const string& colorIn, int widthIn,
    const string& nameIn, const string& descIn, DgReportLevel failLevel)
   : DgOutLocTextFile (filename, rf, isPointFile, "kml", precision, failLevel)
{
   // test for override of vecAddress
   DgAddressBase* dummy = rf.vecAddress(DgDVec2D(M_ZERO, M_ZERO));
   if (!dummy)
      DgOutputStream::report("DgOutKMLfile::DgOutKMLfile(): RF " + rf.name() +
             " must override the vecAddress() method", DgBase::Fatal);
   delete dummy;

   setColor(colorIn);
   setWidth(widthIn);
   setName(nameIn);
   setDescription(descIn);

   setFormatStr();

   preamble();
}

void
DgOutKMLfile::setColor (const string& colorIn)
{
   if (colorIn.length() != 8)
      DgOutputStream::report("DgOutKMLfile::setColor(): invalid KML color " + colorIn,
             DgBase::Fatal);

/* can't find regex so not compiling...
   std::regex e("[0-9a-fA-F]*");
   if (!std::regex_match(colrIn, e))
      DgOutputStream::report("DgOutKMLfile::setColor(): invalid KML color " + colorIn,
             DgBase::Fatal);
*/

   for (unsigned int i = 0; i < colorIn.length(); i++)
   {
      char c = colorIn[i];
      if (!(c >= '0' && c <= '9') &&
          !(c >= 'a' && c <= 'f') &&
          !(c >= 'A' && c <= 'F'))
         DgOutputStream::report("DgOutKMLfile::setColor(): invalid KML color " + colorIn,
             DgBase::Fatal);
   }

   color_ = colorIn;
}

void
DgOutKMLfile::setWidth (int widthIn)
{
   if (widthIn < 1)
      DgOutputStream::report("DgOutKMLfile::setColor(): invalid negative width",
             DgBase::Fatal);

   width_ = widthIn;
}

void
DgOutKMLfile::setName (const string& nameIn)
{
   name_ = nameIn;
}

void
DgOutKMLfile::setDescription (const string& descIn)
{
   description_ = descIn;
}

DgOutKMLfile::~DgOutKMLfile()
{
   postamble();
   close();
}

void DgOutKMLfile::preamble()
{
   DgOutKMLfile& o(*this);   // make life a bit easier

   o << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
     << "<kml xmlns=\"http://earth.google.com/kml/2.1\">\n";

   o << "<Folder>\n";

   o << "   <name>";
   if (name_ == "")
      o << o.DgOutputStream::fileName();
   else
      o << name_;
   o << "</name>\n";

   o << "   <description>";
   o << description_;
   o << "</description>\n";

   o << "   <Style id=\"lineStyle1\">\n";
   o << "      <LineStyle id=\"lineStyle2\">\n";
   o << "         <color>" << color_ << "</color>\n";
   o << "         <width>" << width_ << "</width>\n";
   o << "      </LineStyle>\n";
   o << "   </Style>\n";

   o.flush();
}

void DgOutKMLfile::postamble()
{
   DgOutKMLfile& o(*this);

   o << "</Folder>\n" << "</kml>\n" ;
   o.flush();
}

DgOutLocFile&
DgOutKMLfile::insert(const DgDVec2D& pt)
{
   DgOutKMLfile& o(*this);

   const int maxBuffSize = 200;
   char buff[maxBuffSize];

   snprintf(buff, maxBuffSize, formatStr(), pt.x(), pt.y());

   o << "            " << buff;

   o.flush();

   return o;
}

DgOutLocFile&
DgOutKMLfile::insert (DgLocation& loc, const string* label)
{
   DgOutKMLfile& o(*this);

   rf().convert(&loc);

   o << "   <Placemark>\n";

   if (label)
      o << "      <name>" << *label << "</name>\n";

   o << "      <Point>\n";
   o << "         <coordinates>\n";

   o.insert(rf().getVecLocation(loc));

   o << "         </coordinates>\n";
   o << "      </Point>\n";
   o << "   </Placemark>\n";

   return *this;
}

DgOutLocFile&
DgOutKMLfile::insert (DgLocVector& vec, const string* label, const DgLocation* /* cent */)
{
   DgOutKMLfile& o(*this);

   rf().convert(vec);

   o << "<Placemark>\n";

   if (label)
      o << "   <name>" << *label << "</name>\n";

   o << "      <styleUrl>#lineStyle1</styleUrl>\n";
   o << "      <LineString>\n";
   o << "         <tessellate>1</tessellate>\n";
   o << "         <coordinates>\n";

   vector<DgAddressBase *>& v = vec.addressVec();
   for(vector<DgAddressBase *>::iterator i = v.begin(); v.end() != i; ++i)
         o.insert(rf().getVecAddress(*(*i)));

   o << "         </coordinates>\n";
   o << "      </LineString>\n";
   o << "</Placemark>\n";

   o.flush();

   return *this;
}

DgOutLocFile&
DgOutKMLfile::insert (DgPolygon& poly, const string* label, const DgLocation* /* cent */)
{
   DgOutKMLfile& o(*this);

   rf().convert(poly);

   o << "<Placemark>\n";

   if (label)
      o << "<name>" << *label << "</name>\n";

   o << "      <styleUrl>#lineStyle1</styleUrl>\n";
   o << "      <LineString>\n";
   o << "         <tessellate>1</tessellate>\n";
   o << "         <coordinates>\n";

   vector<DgAddressBase *>& v = poly.addressVec();
   for(vector<DgAddressBase *>::iterator i = v.begin(); v.end() != i; ++i)
         o.insert(rf().getVecAddress(*(*i)));

   // rewrite first vertex:
   o.insert(rf().getVecAddress(*v[0]));

   o << "         </coordinates>\n";
   o << "      </LineString>\n";
   o << "</Placemark>\n";

   o.flush();

   return *this;
}
