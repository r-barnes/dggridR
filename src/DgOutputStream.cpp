////////////////////////////////////////////////////////////////////////////////
//
// DgOutputStream.C: DgOutputStream class implementation
//
//   This class provides wrappers around some basic output stream functionality
//   to increase ease of use. 
//
// Version 6.2 - Kevin Sahr, 9/10/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgOutputStream.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgOutputStream::DgOutputStream (const string& fileName, const string& suffix,
                                DgReportLevel failLevel)
   : DgBase ("DgOutputStream:" + fileName), suffix_ (suffix)
{
   if (!open(fileName, DgBase::Silent))
   {
      report("DgOutputStream::DgOutputStream() unable to open file " +
             fileName, failLevel);
   }

} // DgOutputStream::DgOutputStream

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool 
DgOutputStream::open (string fileName, DgReportLevel failLevel)
//
// Open fileName as an output file. Report with a report level of failLevel
// if the open is unsuccessful.
//
// Returns true if successful and false if unsuccessful.
//
{
   // make sure we are not already open

   if ((rdbuf())->is_open()) close();
   
   // get all the possible name variations

   fileName_ 
      = (suffix() == string("")) ? fileName : 
                                     fileName + string(".") + suffix();

   ofstream::open(fileName_.c_str(), ios::out);
   if (!good()) 
   {
      report("DgOutputStream::open() unable to open file " + fileName_, 
             failLevel);
      return false;
   }
   else
   {
      debug("DgOutputStream::open() opened file " + fileName_);
      return true;
   }
   
} // DgOutputStream::open

////////////////////////////////////////////////////////////////////////////////
