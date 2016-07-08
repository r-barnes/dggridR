////////////////////////////////////////////////////////////////////////////////
//
// DgHexSF.h: DgHexSF class definitions
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGHEXSF_H
#define DGHEXSF_H

#include <string>
#include <iostream>
#include "DgIVec3D.h"

using namespace std;

class GridGenParam;
class DgIDGG;
class DgContCartRF;
class DgEvalData;

////////////////////////////////////////////////////////////////////////////////
class DgHexSF {

   public:

      static const string all;
      static const string cs4;
      static const string cs3A;
      static const string cs3B;
      static const string cs3rA;
      static const string cs3rB;
      static const string cs3rC;
      static const string cs3rD;
      static const string cs3rE;
      static const string cs3rF;

      DgHexSF (void) 
         : type_ ('A'), res_ (0), ijkCoord_(0, 0, 0), classI_ (true), 
           quadNum_ (0) { }

      DgHexSF (const DgHexSF& h) 
         : type_ (h.type_), res_ (h.res_), ijkCoord_(0, 0, 0), classI_ (true), 
           quadNum_ (h.quadNum_) { }

      DgHexSF (const DgIVec3D& ijk, int res = 0, bool classI = true, 
               int quadNum = 0) 
         : type_ ('A'), res_ (res), ijkCoord_ (ijk), classI_ (classI), 
           quadNum_ (quadNum) { }

      DgHexSF (int i, int j, int k, int res = 0, bool classI = true, 
               int quadNum = 0)
         : type_ ('A'), res_ (res), ijkCoord_ (i, j, k), classI_ (classI), 
           quadNum_ (quadNum) { }

     ~DgHexSF (void) { }

      inline operator string (void) const;

      inline string superFundIndex (void) const;

      const DgIVec3D& ijkCoord (void) const { return ijkCoord_; }
      char            type     (void) const { return type_; }
      int             res      (void) const { return res_; }
      bool            classI   (void) const { return classI_; }
      const string&   ciNdx    (void) const { return ciNdx_; }
      const string&   sfNdx    (void) const { return sfNdx_; }
      int             quadNum  (void) const { return quadNum_; }

      void setIjkCoord (const DgIVec3D& c) { ijkCoord_ = c; }
      void setType     (char c)            { type_ = c; }
      void setRes      (int r)             { res_ = r; }
      void setClassI   (bool classI)       { classI_ = classI; }
      void setCiNdx    (string ciNdx)      { ciNdx_ = ciNdx; }
      void setSfNdx    (string sfNdx)      { sfNdx_ = sfNdx; }
      void setQuadNum  (int quadNum)       { quadNum_ = quadNum; }

      string cpiNdx (void) const;

      void addSf3Digit (int digit);

      DgHexSF& operator= (const DgHexSF& h);

      bool operator== (const DgHexSF& h) const
            { return (ijkCoord() == h.ijkCoord() && type() == h.type()
                      && res() == h.res()) && classI() == h.classI()
                      && quadNum() == h.quadNum(); }

      bool operator!= (const DgHexSF& h) const
            { return !operator==(h); }

      unsigned long long int depthFirstTraversal (GridGenParam& dp, 
                  const DgIDGG& dgg, const DgContCartRF& deg, int numAp4Res,
                  DgEvalData* ed = NULL);

      unsigned long long int visitMe (GridGenParam& dp, const DgIDGG& dgg,
                          const DgContCartRF& deg, DgEvalData* ed);

      DgHexSF downAp4 (void);
      DgHexSF downAp3 (void);
      DgHexSF downAp3r (void);

      DgHexSF dirFromCenter (int digit);

      friend ostream& operator<< (ostream& stream, const DgHexSF& h);

   private:

      char type_;
      int res_;
      DgIVec3D ijkCoord_;
      bool classI_;
      string ciNdx_;
      string sfNdx_;
      int quadNum_;
};

////////////////////////////////////////////////////////////////////////////////
inline string
DgHexSF::superFundIndex (void) const
{
   // build the res 1 base tile index out of the quad number and res 1 digit
   int res1tile = 0;
   if (quadNum_ == 0)
   {
      res1tile = 10;
   }
   else
   {
      int res1 = 0;
      if (sfNdx_.length() > 0) res1 = sfNdx_[0] - '1'; // assumes startDigit is 1
      res1tile = 11 + (quadNum_ - 1) * 4 + res1;
   }
   
   //string completeNdx = dgg::util::to_string(quadNum_, 2) + sfNdx_;
   
   string remainder = "";
   if (sfNdx_.length() > 1) remainder = sfNdx_.substr(1, sfNdx_.length() - 1);

   string completeNdx = dgg::util::to_string(res1tile, 2) + remainder;

   return completeNdx;

} // DgHexSF::superFundIndex

////////////////////////////////////////////////////////////////////////////////
inline void
DgHexSF::addSf3Digit (int digit)
// assumes the previous res and this res are both aperture 3
{ 
   string dStr = dgg::util::to_string(digit);
   if (!classI_ || sfNdx_.length() < 1)
      sfNdx_ += dStr;
   else
   {
      string pairs[] = { 
        "11", "12", "13", "21", "22", "23", "31", "32", "33" };
        //"00", "01", "02", "10", "11", "12", "20", "21", "22" };
      string digits[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

      int lastNdx = (int) sfNdx_.length() - 1;
      sfNdx_.append(dStr);
      string lastTwo = sfNdx_.substr(lastNdx, 2);
      string newDigit = "";
      for (int i = 0; i < 9; i++)
         if (!lastTwo.compare(pairs[i])) newDigit = digits[i];

      sfNdx_ = sfNdx_.substr(0, lastNdx) + newDigit;
   }
}

////////////////////////////////////////////////////////////////////////////////
inline DgHexSF& 
DgHexSF::operator= (const DgHexSF& h)
{
   if (this != &h)
   {
      ijkCoord_ = h.ijkCoord();
      type_ = h.type();
      res_ = h.res();
      classI_ = h.classI();
      quadNum_ = h.quadNum();
   }

   return *this;

} // DgHexSF& DgHexSF::operator=

////////////////////////////////////////////////////////////////////////////////
inline DgHexSF::operator string (void) const
{
   return string("{ ") + type_ + ", " + ((classI_) ? "I" : "II") + ", res " + 
                 dgg::util::to_string(res_) + ", quad " + 
                 dgg::util::to_string(quadNum_) + string(ijkCoord_) + " } " +
                 ciNdx_ + " " + sfNdx_;

} // DgHexSF::operator string

////////////////////////////////////////////////////////////////////////////////
inline ostream& operator<< (ostream& stream, const DgHexSF& h)
            { return stream << string(h); }

////////////////////////////////////////////////////////////////////////////////

#endif
