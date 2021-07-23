////////////////////////////////////////////////////////////////////////////////
//
// DgPhysicalRF_template.h: DgPhysicalRF template class definition.
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

template<class A, class C> void
DgPhysicalRF<A, C>::deleteContents (const DgLocation& loc, bool convert)
{
   DgLocation* pLoc = discRF().createLocation(loc, convert);
   const A* add = discRF().getAddress(*pLoc);

   if (!boundedRF().validAddress(*add))
   {
      report("DgPhysicalRF<A, C>::deleteContents() invalid address",
             DgBase::Fatal);
   }

   deleteAddContents(*add);

   delete pLoc;

} // void DgPhysicalRF<A, C>::deleteContents

////////////////////////////////////////////////////////////////////////////////
template<class A, class C> C*
DgPhysicalRF<A, C>::getContents (const DgLocation& loc, bool convert,
                                       bool allocate) const
{
   DgLocation* pLoc = discRF().createLocation(loc, convert);
   const A* add = discRF().getAddress(*pLoc);

   if (!boundedRF().validAddress(*add)) return (C*) 0;
/*
   {
      report("DgPhysicalRF<A, C>::getContents() invalid address",
             DgBase::Fatal);
   }
*/

   C* cont = getAddContents(*add, allocate);

   delete pLoc;

   return cont;

} // C* DgPhysicalRF<A, C>::getContents

////////////////////////////////////////////////////////////////////////////////
template<class A, class C> void
DgPhysicalRF<A, C>::replaceContents (const DgLocation& loc, C* cont, 
                                        bool convert)
{
   DgLocation* pLoc = discRF().createLocation(loc, convert);
   const A* add = discRF().getAddress(*pLoc);

   if (!boundedRF().validAddress(*add))
   {
      report("DgPhysicalRF<A, C>::replaceContents() invalid address",
             DgBase::Fatal);
   }

   replaceAddContents(*add, cont);

   delete pLoc;

} // void DgPhysicalRF<A, C>::setContents

////////////////////////////////////////////////////////////////////////////////
template<class A, class C> void
DgPhysicalRF<A, C>::setContents (const DgLocation& loc, const C& cont, 
                                    bool convert)
{
   DgLocation* pLoc = discRF().createLocation(loc, convert);
   const A* add = discRF().getAddress(*pLoc);

   if (!boundedRF().validAddress(*add))
   {
      report("DgPhysicalRF<A, C>::setContents() invalid address",
             DgBase::Fatal);
   }

   setAddContents(*add, cont);

   delete pLoc;

} // void DgPhysicalRF<A, C>::setContents

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
