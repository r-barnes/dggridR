////////////////////////////////////////////////////////////////////////////////
//
// DgCell.cpp: DgCell class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////
//
//  The DgCell class defines an object with a point node, a region, and a 
//  label.
//
////////////////////////////////////////////////////////////////////////////////

#include "DgBase.h"
#include "DgCell.h"
#include "DgLocation.h"
#include "DgPolygon.h"

////////////////////////////////////////////////////////////////////////////////
void 
DgCell::convertTo (const DgRFBase& rfIn) 
{ 
   if (!rf_ || rf() != rfIn)
   {
      rf_ = &rfIn;
      rfIn.convert(&node_); 
      if (hasRegion()) rfIn.convert(*region_);
   }

} // DgCell::convertTo

////////////////////////////////////////////////////////////////////////////////
string 
DgCell::asString (void) const
{ 
   string tmp = "[" + node().asString();
   if (hasRegion()) tmp += ":" + region().asString();
   tmp += "]"; 

   return tmp;

} // DgCell::asString

////////////////////////////////////////////////////////////////////////////////
string 
DgCell::asString (char delimiter) const
{ 
   string tmp = node().asString(delimiter);
   if (hasRegion()) tmp += delimiter + region().asString(delimiter);

   return tmp;

} // DgCell::asString

////////////////////////////////////////////////////////////////////////////////
string 
DgCell::asAddressString (void) const
{ 
   string tmp = "[" + node().asAddressString();
   if (hasRegion()) tmp += ":" + region().asAddressString();
   tmp += "]"; 

   return tmp;

} // DgCell::asAddressString

////////////////////////////////////////////////////////////////////////////////
string 
DgCell::asAddressString (char delimiter) const
{ 
   string tmp = node().asAddressString(delimiter);
   if (hasRegion()) tmp += delimiter + region().asAddressString(delimiter);

   return tmp;

} // DgCell::asAddressString

////////////////////////////////////////////////////////////////////////////////
const char*
DgCell::fromString (const char* str, char delimiter)
{
   clearAddress();

   DgLocation tloc(rf());
   const char* tmp = tloc.fromString(str, delimiter);
   setNode(tloc);

   if (*tmp == delimiter) tmp++;

   DgPolygon* reg = new DgPolygon(rf());
   tmp = reg->fromString(tmp, delimiter);
   setRegion(reg);

   return tmp;

} // DgCell::fromString

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
