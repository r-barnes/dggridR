////////////////////////////////////////////////////////////////////////////////
//
// DgInputStream.C: DgInputStream class implementation
//
//   This class provides wrappers around some basic input stream functionality
//   to increase ease of use. 
//
// Version 6.2 - Kevin Sahr, 9/10/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgInputStream.h"

string DgInputStream::defaultDirectory_;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgInputStream::DgInputStream (const string& fileNameIn, const string& suffixIn,
                              DgReportLevel failLevel)
   : DgBase ("DgInputStream:" + fileNameIn), suffix_ (suffixIn)
{
   if (!open(fileNameIn, DgBase::Silent))
   {
      report("DgInputStream::DgInputStream() unable to open file " +
             fileNameIn, failLevel);
   }

} // DgInputStream::DgInputStream

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool 
DgInputStream::open (string fileNameIn, DgReportLevel failLevel)
//
// Open fileName as an input file. Report with a report level of failLevel
// if the open is unsuccessful.
//
// Returns true if successful and false if unsuccessful.
//
{
   // make sure we are not already open

   if ((rdbuf())->is_open()) close();
   
   // get all the possible name variations

   string names[4];

   names[0] = fileNameIn;
   names[1] = fileNameIn + "." + suffix_;
   names[2] = defaultDirectory_ + "/" + names[0];
   names[3] = names[2] + "." + suffix_;

   int i;
   for (i = 0; i < 4; i++)
   {
      ifstream::open(names[i].c_str(), ios::in);
      if (good())
      {
         fileName_ = names[i];
         break;
      }
      else
      {
         close();
         clear();
      }
   } 

   if (i == 4) 
   {
      report("DgInputStream::open() unable to open file " + fileNameIn, 
             failLevel);
      return false;
   }
   else
   {
      debug("DgInputStream::open() opened file " + fileName_);
      return true;
   }
   
} // DgInputStream::open

////////////////////////////////////////////////////////////////////////////////
