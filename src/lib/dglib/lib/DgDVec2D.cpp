////////////////////////////////////////////////////////////////////////////////
//
// DgDVec2D.cpp: DgDVec2D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cfloat>

#include "DgBase.h"
#include "DgDVec2D.h"
#include "DgDVec3D.h"

////////////////////////////////////////////////////////////////////////////////

const DgDVec2D& DgDVec2D::undefDgDVec2D = DgDVec2D(DBL_MAX, DBL_MAX);

////////////////////////////////////////////////////////////////////////////////
DgDVec2D::DgDVec2D (const DgDVec3D& pt) : x_ (pt.x()), y_ (pt.y()) {}

////////////////////////////////////////////////////////////////////////////////
DgDVec2D&
DgDVec2D::operator= (const DgDVec3D& pt)
{
   x_ = pt.x();
   y_ = pt.y();

   return *this;

} // DgDVec2D& DgDVec2D::operator=

////////////////////////////////////////////////////////////////////////////////
const char* 
DgDVec2D::fromString (const char* str, char delimiter)
{
   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   char* tok;

   // get the x

   tok = strtok(tmpStr, delimStr);
   long double xIn;
   if (sscanf(tok, "%LF", &xIn) != 1)
   {
      ::report("DgDVec2D::fromString() invalid value in string " + string(tok), 
               DgBase::Fatal);
   }

   // get the y

   tok = strtok(NULL, delimStr);
   long double yIn;
   if (sscanf(tok, "%LF", &yIn) != 1)
   {
      ::report("DgDVec2D::fromString() invalid value in string " + string(tok), 
               DgBase::Fatal);
   }

   setX(xIn);
   setY(yIn);

   unsigned int offset = (tok - tmpStr) + strlen(tok) + 1;
   if (offset >= strlen(str)) 
    return 0;
   else 
    return &str[offset];
} // const char* DgDVec2D::fromString

////////////////////////////////////////////////////////////////////////////////
