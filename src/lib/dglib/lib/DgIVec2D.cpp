////////////////////////////////////////////////////////////////////////////////
//
// DgIVec2D.cpp: DgIVec2D class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <climits>

#include "DgBase.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////

const DgIVec2D& DgIVec2D::undefDgIVec2D = DgIVec2D(INT_MAX, INT_MAX);

////////////////////////////////////////////////////////////////////////////////
const char*
DgIVec2D::fromString (const char* str, char delimiter)
{
   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   // Get i and j:
   char* tok;

   long long int	iIn(0),
	   		jIn(0);

   try
    {
   	tok = strtok(tmpStr, delimStr);
   	iIn = dgg::util::from_string<long long int>(tok);
	
   	tok = strtok(NULL, delimStr);
   	jIn = dgg::util::from_string<long long int>(tok);
    }  
   catch(...)
    {
      ::report("DgIVec2D::fromString() invalid value in string " + string(tok), 
               DgBase::Fatal);
    }

   setI(iIn);
   setJ(jIn);

   unsigned long long int offset = (tok - tmpStr) + strlen(tok) + 1;
   if (offset >= strlen(str)) 
    return 0;

   return &str[offset];

} // const char* DgIVec2D::fromString

////////////////////////////////////////////////////////////////////////////////
