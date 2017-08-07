////////////////////////////////////////////////////////////////////////////////
//
// DgBase.h: DgBase class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGBASE_H
#define DGBASE_H

#include <iostream>
#include <string>

using namespace std;

#define DGDEBUG         0
#define DGGRID_VERSION  "6.2b1" 

////////////////////////////////////////////////////////////////////////////////
class DgBase {

   public:

      enum DgReportLevel { Debug1, Debug0, Info, Warning, Fatal, Silent };

   private:

      static const string defaultName;
      static DgReportLevel minReportLevel_;

   public:

      virtual ~DgBase (void);

      static void setMinReportLevel (DgReportLevel newRLevel)
                   { minReportLevel_ = newRLevel; }

      static DgReportLevel minReportLevel (void) { return minReportLevel_; }

      static bool testArgEqual (int argc, int expected, 
                     const string& message = string("invalid argument count"),
                     DgReportLevel level = Fatal);

      static bool testArgEqual (int argc, char* argv[], int expected, 
                     const string& message = string("invalid argument count"));

      static bool testArgMin (int argc, int minExpected, 
                     const string& message = string("invalid argument count"),
                     DgReportLevel level = Fatal);

      static bool testArgMin (int argc, char* argv[], int minExpected, 
                     const string& message = string("invalid argument count"));

      DgBase (const string& instanceName = defaultName);

      DgBase (const string* instanceName = NULL);
      
      void setInstanceName (const string& instanceName)
              { instanceName_ = instanceName; }

      const string& instanceName (void) const { return instanceName_; }

   protected:

      void report (const string& message, DgReportLevel level = Info) const;
      void debug  (const string& message) const;

   private:

      // data members

      string instanceName_;
};

////////////////////////////////////////////////////////////////////////////////
extern "C" void report (const string& message, 
                        DgBase::DgReportLevel level = DgBase::Info);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
inline void
DgBase::debug (const string& message) const
//
// Print-out a debugging message if the DGDEBUG flag is set. Otherwise this
// is a null operation.
//
////////////////////////////////////////////////////////////////////////////////
{

// #if DGDEBUG

//    cout << "DEBUG: [" << instanceName_ << "] " << message << endl;

// #endif

} // void DgBase::debug

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgBase& b)
            { return stream << b.instanceName(); }

#endif
