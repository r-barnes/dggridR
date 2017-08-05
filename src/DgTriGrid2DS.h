////////////////////////////////////////////////////////////////////////////////
//
// DgTriGrid2DS.h: DgTriGrid2DS class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGTRIGRID2DS_H 
#define DGTRIGRID2DS_H 

#include "DgRF.h"
#include "DgLocVector.h"
#include "DgDiscRFS2D.h"
#include "DgIVec2D.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgTriGrid2DS : public DgDiscRFS2D {

   public:

      DgTriGrid2DS (DgRFNetwork& networkIn, 
                    const DgRF<DgDVec2D, long double>& backFrameIn,
                    int nRes = 1, unsigned int apertureIn = 4, 
                    bool isCongruentIn = true, bool isAlignedIn = false,
                    const string& nameIn = "3T4");

      DgTriGrid2DS (const DgTriGrid2DS& rf);

     ~DgTriGrid2DS (void);

      DgTriGrid2DS& operator= (const DgTriGrid2DS& rf);

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
