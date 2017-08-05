////////////////////////////////////////////////////////////////////////////////
//
// DgIcosaMap.h: DgIcosaMap class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGICOSAMAP_H
#define DGICOSAMAP_H

////////////////////////////////////////////////////////////////////////////////

#include "DgConstants.h"
#include "DgDVec2D.h"

////////////////////////////////////////////////////////////////////////////////
class DgIcosaTri {

   public:

      DgIcosaTri (bool mapped = false, int rot60 = 0, 
                   const DgDVec2D& offset = DgDVec2D(M_ZERO, M_ZERO)) 
         : rot60_ (rot60), offset_ (offset), mapped_ (mapped) { }

      static const DgDVec2D origin_;

      int rot60_;

      DgDVec2D offset_;
      bool mapped_;

      void translate (DgDVec2D* pt) const;
      void untranslate (DgDVec2D* pt, bool shiftOrigin = true) const;

   friend class DgIcosaMap;

};

////////////////////////////////////////////////////////////////////////////////
class DgIcosaMap {

   public:

      static DgIcosaMap defIcosaMap;
      //static DgIcosaTri defIcosaTri[20];

      DgIcosaMap (const DgIcosaTri icosaTri[20]);

      void translate (int nTri, DgDVec2D* pt) const;
      void untranslate (int nTri, DgDVec2D* pt, bool shiftOrigin = true) const;
      DgDVec2D maxOffset (void);

      DgIcosaTri icosaTri_[20];

};

////////////////////////////////////////////////////////////////////////////////

#endif
