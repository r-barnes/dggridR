////////////////////////////////////////////////////////////////////////////////
//
// DgRandom.h: DgRandom class definitions.
//
//    Contains two DgRandom sub-classes: DgRand is rand()-based, and
//      DgRandMother is based on George Marsaglia's multiply-with-carry
//      "mother" function.
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGRANDOM_H
#define DGRANDOM_H

#include <iostream>

using namespace std;

#include "DgEllipsoidRF.h"

////////////////////////////////////////////////////////////////////////////////
class DgRandom {

   public:

      DgRandom (const DgRandom& obj)
               : status_ (obj.status()), maxVal_ (obj.maxVal()) { }

      DgRandom (const unsigned long int statusIn,
                const unsigned long int maxValIn)
               : status_ (statusIn), maxVal_ (maxValIn) { }

      DgRandom& operator= (const DgRandom& obj)
               {
                  if (&obj != this)
                  {
                     status_ = obj.status();
                     maxVal_ = obj.maxVal();
                  }

                  return *this;

               } // DgRandom::operator=

      virtual ~DgRandom (void);

      // get methods

      unsigned long int status (void) const { return status_; }
      unsigned long int maxVal (void) const { return maxVal_; }

      // set methods

      void setStatus (const unsigned long int statusIn) { status_ = statusIn; }

      // random generation methods

      unsigned long int nextInt (void)
          { setStatus(nextRand()); return status(); }

      double nextDouble (void)  // return 0.0...1.0
          { 
             setStatus(nextRand()); 
             return (double) status() / (double) maxVal(); 
          }

      double randInRange (double min, double max) // inclusive
          { 
             return (min + (max - min) * nextDouble());
          }

      DgGeoCoord nextGeo (void) 
         { 
            return DgGeoCoord(randInRange(-M_PI, M_PI),
                              asin(randInRange(-1.0, 1.0)));
         }

   protected:

      virtual unsigned long int nextRand (void) = 0;

   private:

      unsigned long int status_;
      unsigned long int maxVal_;

}; // class DgRandom

////////////////////////////////////////////////////////////////////////////////
inline ostream&
operator<< (ostream& stream, const DgRandom& obj)
{
   return stream << obj.status();

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgRand : public DgRandom {

   public:

      DgRand (const DgRand& obj) : DgRandom (obj) { srand((unsigned) status()); }

      DgRand (const unsigned long int statusIn = 1)
         : DgRandom (statusIn, RAND_MAX) { srand((unsigned) status()); }

   protected:

      virtual unsigned long int nextRand (void) { return rand(); }

}; // class DgRand

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define m32Double  4294967295.0  /* 2^32-1 */

class DgRandMother : public DgRandom {

   public:

      DgRandMother (const DgRandMother& obj) : DgRandom (obj) { srand((unsigned) status()); }

      DgRandMother (const unsigned long int statusIn = 1)
         : DgRandom (statusIn, (unsigned long int) m32Double) { }

   protected:

      virtual unsigned long int nextRand (void);

}; // class DgRandMother

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
