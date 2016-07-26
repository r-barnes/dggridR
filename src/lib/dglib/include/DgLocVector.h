////////////////////////////////////////////////////////////////////////////////
//
// DgLocVector.h: DgLocVector class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGLOCVECTOR_H
#define DGLOCVECTOR_H

#include <string>
#include <vector>
#include <iostream>

#include "DgLocation.h"

using namespace std;

class DgAddressBase;

////////////////////////////////////////////////////////////////////////////////
class DgLocVector : public DgLocBase {

   public:

      DgLocVector (void) { }

      DgLocVector (const DgLocVector& vec)    // deep copy
         : DgLocBase (vec.rf()) { *this = vec; }

      DgLocVector (const DgRFBase& rfIn, int sizeIn = 0);

     ~DgLocVector (void) { clearAddress(); }

      DgLocVector& operator= (const DgLocVector& vec); // deep copy

      bool operator== (const DgLocVector& vec) const;

      bool operator!= (const DgLocVector& vec) const
            { return !operator==(vec); }

      void setLoc (int ndx, const DgLocation& loc, bool conv = true);

      int size (void) const { return (int) vec_.size(); }

      void resize (int n) { vec_.resize(n, 0); }

      int capacity (void) const { return (int) vec_.capacity(); }

      void reserve (int n) { vec_.reserve(n); }

      const DgLocation& operator[] (int ndx) const
       { tmpLoc_.rf_ = rf_; tmpLoc_.address_ = vec_[ndx]; return tmpLoc(); }

      void push_back (const DgLocation& loc, bool conv = true);

      vector<DgAddressBase*>& addressVec (void) { return vec_; }

      const vector<DgAddressBase*>& addressVec (void) const { return vec_; }

      const DgLocation& tmpLoc (void) const { return tmpLoc_; }

      virtual string asString (void) const
               { return rf().toString(*this); }

      virtual string asString (char delimiter) const
               { return rf().toString(*this, delimiter); }

      virtual string asAddressString (void) const
               { return rf().toAddressString(*this); }

      virtual string asAddressString (char delimiter) const
               { return rf().toAddressString(*this, delimiter); }

      virtual const char* fromString (const char* str, char delimiter);

      virtual void clearAddress (void);

      virtual int cardinality (void) const { return size(); } 

   protected:

      mutable DgLocation tmpLoc_;

      virtual void convertTo (const DgRFBase& rf) { rf.convert(*this); }

   private:

      vector<DgAddressBase*> vec_;

   friend class DgRFBase;
   friend class DgConverterBase;
   friend class DgPoly;
   template <class A, class D> friend class DgRF;

};

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgLocVector& vec)
{
   stream << vec.rf().name() << " {\n";

/* ccw
   for (int i = 0; i < vec.size(); i++) stream << vec[i] << "\n";
*/
   for (int i = vec.size() - 1; i >= 0; i--) stream << vec[i] << "\n";

   return stream << "}" << endl;

} // inline ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
inline DgLocVector& 
DgLocVector::operator= (const DgLocVector& vec)
{
   if (this != &vec)
   {
      clearAddress();
      
      rf_ = &vec.rf();
      resize(vec.size());

      for (int i = 0; i < size(); i++) 
      {
         vec_[i] = rf().createAddress(*vec.vec_[i]);
      }
   }

   return *this;

} // DgLocVector& DgLocVector::operator=

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
