////////////////////////////////////////////////////////////////////////////////
//
// DgAddress.h: DgAddress class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGADDRESS_H
#define DGADDRESS_H

#include "DgAddressBase.h"

class DgDistanceBase;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <class A> class DgAddress : public DgAddressBase {

   public:

      DgAddress (void) {}
      DgAddress (const DgAddress<A>& add) : address_ (add.address()) {}
      DgAddress (const A& address) : address_ (address) {}

      void setAddress (const A& address) { address_ = address; }

      A& address (void) { return address_; }

      const A& address (void) const { return address_; }

      DgAddress<A>& operator= (const DgAddress<A>& add) 
                                    { address_ = add.address(); return *this; }

   protected:

      virtual ostream& writeTo (ostream& stream) const
                                    { return stream << address_; }

   private:

      A address_;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
