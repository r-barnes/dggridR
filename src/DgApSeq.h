#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2023 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//
// DgApSeq.h: aperture sequence class
//
// Version 7.0 - Kevin Sahr,11/16/14
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DGAPSEQ_H
#define DGAPSEQ_H

#include "DgBase.h"
#include "DgUtil.h"

#include <algorithm>
#include <ostream>
#include <vector>

class DgAperture {

   public:

      const static int defaultAperture;

      DgAperture (void) : a_ (defaultAperture) { }

      DgAperture (int a) { setAperture(a); }

      DgAperture (char c) { setAperture(c); }

      DgAperture (const DgAperture& ap) { setAperture(ap.aperture()); }

      void setAperture (int a)
            {
               if (a == 3 || a == 4 || a == 7)
                  a_ = a;
               else
               {
                  a_ = -1;
                  report("DgAperture::setAperture() invalid aperture " +
                          //dgg::util::to_std::string(a), DgBase::Fatal);
                          std::to_string(a), DgBase::Fatal);
               }
            }

      void setAperture (char c) { setAperture(c - '0'); }

      int aperture (void) const { return a_; }

      DgAperture& operator= (const DgAperture& ap)
            {
               if (ap != *this)
                      setAperture(ap.aperture());

               return *this;
            }

      bool operator== (const DgAperture& ap) const
             { return aperture() == ap.aperture(); }

      bool operator!= (const DgAperture& ap) const
             { return !operator==(ap); }

      operator std::string (void) const
             //{ return dgg::util::to_string(a_); }
             { return std::to_string(a_); }

   private:

      int a_;
};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream& operator<< (std::ostream& stream, const DgAperture& ap)
{
   stream << std::string(ap);
   return stream;

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class DgApSeq : public DgBase {

   public:

      const static DgApSeq defaultApSeq;
      const static std::string defaultEmptyApSeqStr;

      DgApSeq (void) : DgBase ("EMPTY_APSEQ") { }

      DgApSeq (const std::vector<DgAperture>& ap, const std::string& name = "")
         : DgBase (name) { setApSeq(ap); }

      DgApSeq (const std::string& apSeqStr, const std::string& name = "")
         : DgBase (name) { setApSeq(apSeqStr); }

      DgApSeq (const DgApSeq& apSeq, const std::string& name = "")
         : DgBase (name) { setApSeq(apSeq.seqVec_); }

      // note that the resolution 1 aperture is vector index 0
      const DgAperture& getAperture (int res) const { return seqVec_[res - 1]; }

      void setAperture (int res, const DgAperture& ap) { seqVec_[res - 1] = ap; }

      void setApSeq (const std::vector<DgAperture>& apSeq)
            {
               seqVec_.clear();
               //copy(apSeq.begin(), apSeq.end(), seqVec_.begin());
               for(auto &a: apSeq)
                  addAperture(a);
            }

      void setApSeq (const std::string& seqStr)
            {
               seqVec_.clear();
               for (auto &ss: seqStr)
                  addAperture(DgAperture(ss));
            }

      void addAperture (const DgAperture& ap) { seqVec_.push_back(ap); }

      int nRes (void) const { return (int) seqVec_.size(); }

      int lastRes (void) const { return nRes(); }

      bool isEmpty (void) const { return nRes() == 0; }

      DgApSeq& operator= (const DgApSeq& ap)
            {
               if (ap != *this)
                      setApSeq(ap.seqVec_);

               return *this;
            }

      bool operator== (const DgApSeq& ap) const
             { return std::string(*this) == std::string(ap); }

      bool operator!= (const DgApSeq& ap) const
             { return !operator==(ap); }

      operator std::string (void) const
             {
                std::string seqStr;
                if (isEmpty())
                   seqStr = defaultEmptyApSeqStr;
                else
                   for (std::vector<DgAperture>::const_iterator it = seqVec_.begin();
                        it != seqVec_.end(); ++it)
                      seqStr += std::string(*it);

                return seqStr;
             }

   private:

      std::vector<DgAperture> seqVec_;
};

////////////////////////////////////////////////////////////////////////////////
inline std::ostream& operator<< (std::ostream& stream, const DgApSeq& ap)
{
   stream << ap.instanceName() << std::string(ap);
   return stream;

} // std::ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif
