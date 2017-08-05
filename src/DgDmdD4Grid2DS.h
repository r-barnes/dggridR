////////////////////////////////////////////////////////////////////////////////
//
// DgDmdD4Grid2DS.h: DgDmdD4Grid2DS class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGDMDD4GRID2DS_H 
#define DGDMDD4GRID2DS_H 

#include "DgRF.h"
#include "DgLocVector.h"
#include "DgDiscRFS2D.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgDmdD4Grid2DS : public DgDiscRFS2D {

   public:

      DgDmdD4Grid2DS (DgRFNetwork& networkIn, 
                 const DgRF<DgDVec2D, long double>& backFrameIn,
                 int nRes = 1, unsigned int apertureIn = 4, 
                 bool isCongruentIn = true, bool isAlignedIn = false,
                 const string& nameIn = "4D4");

      DgDmdD4Grid2DS (const DgDmdD4Grid2DS& rf);

     ~DgDmdD4Grid2DS (void);

      DgDmdD4Grid2DS& operator= (const DgDmdD4Grid2DS& rf);

      int radix (void) const { return radix_; }

   protected:

      int radix_;

      // pure virtual functions from DgDiscRFS

      virtual void setAddParents (const DgResAdd<DgIVec2D>& add,
                                  DgLocVector& vec) const;

      virtual void setAddInteriorChildren (const DgResAdd<DgIVec2D>& add,
                                           DgLocVector& vec) const;

      virtual void setAddBoundaryChildren (const DgResAdd<DgIVec2D>& add,
                                           DgLocVector& vec) const;

      virtual void setAddAllChildren (const DgResAdd<DgIVec2D>& add,
                                      DgLocVector& vec) const;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
