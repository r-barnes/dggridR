////////////////////////////////////////////////////////////////////////////////
//
// DgBoundedRF.hpp: DgBoundedRF template class definition.
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cstdint>

template<class A, class B, class DB>
DgBoundedRF<A, B, DB>::DgBoundedRF (const DgDiscRF<A, B, DB>& rfIn, 
      const A& firstAddIn, const A& lastAddIn, const A& endAddIn, bool zBasedIn)
   : DgBoundedRFBase<B, DB> (rfIn, *rfIn.makeLocation(firstAddIn), 
     *rfIn.makeLocation(lastAddIn), *rfIn.makeLocation(endAddIn), zBasedIn),
     discRF_(rfIn),
     firstAdd_(firstAddIn), lastAdd_(lastAddIn), endAdd_(endAddIn)
{

} // DgBoundedRF<A, B, DB>::DgBoundedRF

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> bool
DgBoundedRF<A, B, DB>::validLocation (const DgLocation& loc, bool convert) const
{   
   return validAddress(*discRF().getAddress(loc));

} // bool DgBoundedRF<A, B, DB>::validLocation

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> DgLocation&
DgBoundedRF<A, B, DB>::incrementLocation (DgLocation& loc, bool convert) const
{
   const A* add = discRF().getAddress(loc);
   
   incrementAddress(const_cast<A&>(*add)); // cheat for speed
   
   return loc;

} // DgLocation& DgBoundedRF<A, B, DB>::incrementLocation
                         
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> DgLocation&
DgBoundedRF<A, B, DB>::decrementLocation (DgLocation& loc, bool convert) const
{
   const A* add = discRF().getAddress(loc);
   
   decrementAddress(const_cast<A&>(*add)); // cheat for speed
   
   return loc;

} // DgLocation& DgBoundedRF<A, B, DB>::decrementLocation
                                                  
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> std::uint64_t
DgBoundedRF<A, B, DB>::seqNum (const DgLocation& loc, bool convert) const
{
   const A* add = discRF().getAddress(loc);
   
   return seqNumAddress(*add);

} // std::uint64_t DgBoundedRF<A, B, DB>::seqNum
                                                  
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> DgLocation*
DgBoundedRF<A, B, DB>::locFromSeqNum (std::uint64_t sNum) const
{
   A add = addFromSeqNum(sNum);

   return discRF().makeLocation(add);

} // DgLocation* DgBoundedRF<A, B, DB>::locFromSeqNum
                                                  
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

