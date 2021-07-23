////////////////////////////////////////////////////////////////////////////////
//
// DgInAIGenFile.h: DgInAIGenFile class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGINAIGENFILE_H
#define DGINAIGENFILE_H

#include "DgInLocTextFile.h"

class DgPolygon;

////////////////////////////////////////////////////////////////////////////////
class DgInAIGenFile : public DgInLocTextFile {

   public:

      DgInAIGenFile (const DgRFBase& rfIn, const string* fileNameIn = NULL,
                        DgReportLevel failLevel = DgBase::Fatal);

      bool forcePolyLine (void) const { return forcePolyLine_; }
      bool forceCells    (void) const { return forceCells_; }

      void setForcePolyLine (bool forcePolyLine = false)
                 { forcePolyLine_ = forcePolyLine; }

      void setForceCells (bool forceCells = false)
                 { forceCells_ = forceCells; }

      virtual DgInLocFile& extract (DgLocList& list);
      virtual DgInLocFile& extract (DgLocVector& vec);
      virtual DgInLocFile& extract (DgPolygon& poly);
      virtual DgInLocFile& extract (DgLocation& loc);
      virtual DgInLocFile& extract (DgCell& cell);

   private:
      
      bool forcePolyLine_;
      bool forceCells_;

};

////////////////////////////////////////////////////////////////////////////////

#endif
