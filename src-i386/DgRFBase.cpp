////////////////////////////////////////////////////////////////////////////////
//
// DgRFBase.cpp: DgRFBase class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <list>

using namespace std;

#include "DgBase.h"
#include "DgRFBase.h"
#include "DgLocBase.h"
#include "DgLocation.h"
#include "DgLocVector.h"
#include "DgConverterBase.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgRFBase::~DgRFBase (void)
{
   /* JFW: We don't delete connectTo and connectFrom (we don't really own them);
   for that matter, I'm not sure we actually ever touch undefLoc_, below: */
   delete undefLoc_;

} // DgRFBase::~DgRFBase

////////////////////////////////////////////////////////////////////////////////
DgLocation* 
DgRFBase::convert (DgLocation* loc) const
{
   if (loc->rf_ == 0)
   {
      loc->rf_ = this;
      return loc;
   }
   
   if (network() != loc->rf().network())
   {
      report("DgRFBase::convert() from/to network mismatch",
             DgBase::Fatal);
      return 0;
   }

   if (loc->rf() == *this) return loc;

   if (loc->address() == 0)
   {
      loc->rf_ = this;
      return loc;
   }
   
   // if we're here we need to convert

   const DgConverterBase* conv = network().getConverter(loc->rf(), *this);
   if (!conv)
   {
      report("DgRFBase::convert() getConverter error", DgBase::Fatal);
      return 0;
   }

   return conv->convert(loc);

} // DgLocation* DgRFBase::convert

////////////////////////////////////////////////////////////////////////////////
DgLocVector& 
DgRFBase::convert (DgLocVector& vec) const
{
   if (vec.rf_ == 0)
   {
      vec.rf_ = this;
      return vec;
   }
   
   if (network() != vec.rf().network())
   {
      report("DgRFBase::convert() from/to network mismatch",
             DgBase::Fatal);
      return vec;
   }

   if (vec.rf() == *this) return vec;

   if (vec.size() == 0)
   {
      vec.rf_ = this;
      return vec;
   }
   
   // if we're here we need to convert

   const DgConverterBase* conv = network().getConverter(vec.rf(), *this);
   if (!conv)
   {
      report("DgRFBase::convert() getConverter error", DgBase::Fatal);
      return vec;
   }

   vector<DgAddressBase*>& v = vec.addressVec();
   for (unsigned long i = 0; i < v.size(); i++)
   {
      DgAddressBase* addIn = v[i];

      if (addIn) 
       v[i] = conv->createConvertedAddress(*addIn);

      delete addIn;
   }

   vec.rf_ = this;
   vec.tmpLoc_.rf_ = this;

   return vec;

} // DgLocVector& DgRFBase::convert

////////////////////////////////////////////////////////////////////////////////
DgLocation* 
DgRFBase::createLocation (void) const
{
   return new DgLocation(*this, 0);

} // DgLocation* DgRFBase::createLocation

////////////////////////////////////////////////////////////////////////////////
DgLocation* 
DgRFBase::buildLocation (DgAddressBase* addIn) const
{
   return new DgLocation(*this, addIn);

} // DgLocation* DgRFBase::buildLocation

////////////////////////////////////////////////////////////////////////////////
ostream&
DgRFBase::traceToGround (ostream& stream) const
{
   stream << "tracing " << name() << " {";

   if (id() == 0) return stream << " at ground }" << endl;

   if (!connectTo()) return stream << " => NULL }" << endl;

   stream << "\n";

   const DgRFBase* to = connectTo();
   while (true)
   {
      if (!to) return stream << "  => NULL }" << endl;

      // at ground?

      if (to->id() == 0)
      {
         stream << "  => at ground: " << to->name() << "\n";
         break;
      }

      // otherwise continue

      stream << "  => " << to->name() << "\n";
      to = to->connectTo();
   }

   // now go back from ground

   list<const DgRFBase*> rev;
   const DgRFBase* from = connectFrom();
   while (true)
   {
      if (!from) return stream << "  => NULL }" << endl;

      // at ground?

      if (from->id() == 0) break;

      // otherwise continue

      rev.push_front(from);
      from = from->connectFrom();
   }

   list<const DgRFBase*>::iterator it = rev.begin();
   for (; it != rev.end(); it++) stream << "  => " << (*it)->name() << "\n";

   return stream << " => " << name() << " }" << endl;  

} // void DgRFBase::traceToGround

////////////////////////////////////////////////////////////////////////////////
string
DgRFBase::toString (const DgLocBase& lb) const
{
   return lb.asString();

} // string DgRFBase::toString

////////////////////////////////////////////////////////////////////////////////
string
DgRFBase::toAddressString (const DgLocBase& lb) const
{
   return lb.asAddressString();

} // string DgRFBase::toAddressString

////////////////////////////////////////////////////////////////////////////////
string
DgRFBase::toString (const DgLocBase& lb, char delimiter) const
{
   return lb.asString(delimiter);

} // string DgRFBase::toString

////////////////////////////////////////////////////////////////////////////////
string
DgRFBase::toAddressString (const DgLocBase& lb, char delimiter) const
{
   return lb.asAddressString(delimiter);

} // string DgRFBase::toAddressString

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

