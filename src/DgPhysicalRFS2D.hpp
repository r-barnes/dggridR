////////////////////////////////////////////////////////////////////////////////
//
// DgPhysicalRFS2D.hpp: DgPhysicalRFS2D template class definition.
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

template<class C>
DgPhysicalRFS2D<C>::DgPhysicalRFS2D (const DgBoundedRFS2D& rfIn, bool allocate)
   : DgPhysicalRF<DgResAdd<DgIVec2D>, C> (rfIn), boundedRFS2D_ (rfIn) 
{
   int nRes = rfIn.grids().size();

   grids_ = new vector<DgPhysicalRF2D<C>*>(nRes);

   for (int i = 0; i < nRes; i++)
   {
      (*grids_)[i] = new DgPhysicalRF2D<C>(*rfIn.grids()[i], allocate);
   }

   initAllCells();

} // DgPhysicalRFS2D<C>::DgPhysicalRFS2D

////////////////////////////////////////////////////////////////////////////////
template<class C> void
DgPhysicalRFS2D<C>::replaceAddContents (const DgResAdd<DgIVec2D>& add, C* cont)
{
   if (!boundedRFS2D().validAddress(add))
   {
      report("DgPhysicalRFS2D<C>::replaceAddContents() invalid address", 
      DgBase::Fatal);
   }

   (*grids_)[add.res()]->replaceAddContents(add.address(), cont);

} // void DgPhysicalRFS2D<C>::replaceAddContents

////////////////////////////////////////////////////////////////////////////////
template<class C> void
DgPhysicalRFS2D<C>::setAddContents (const DgResAdd<DgIVec2D>& add, 
                                    const C& cont)
{
   if (!boundedRFS2D().validAddress(add))
   {
      report("DgPhysicalRFS2D<C>::setAddContents() invalid address", 
      DgBase::Fatal);
   }

   (*grids_)[add.res()]->setAddContents(add.address(), cont);

} // void DgPhysicalRFS2D<C>::setAddContents

////////////////////////////////////////////////////////////////////////////////
template<class C> C*
DgPhysicalRFS2D<C>::getAddContents (const DgResAdd<DgIVec2D>& add, bool allocate) const
{
   if (!boundedRFS2D().validAddress(add))
   {
      report("DgPhysicalRFS2D<C>::getAddContents() invalid address", 
      DgBase::Fatal);
   }

   return (*grids_)[add.res()]->getAddContents(add.address(), allocate);

} // C* DgPhysicalRFS2D<C>::getAddContents

////////////////////////////////////////////////////////////////////////////////
template<class C> void
DgPhysicalRFS2D<C>::deleteAddContents (const DgResAdd<DgIVec2D>& add)
{
   if (!boundedRFS2D().validAddress(add))
   {
      report("DgPhysicalRFS2D<C>::deleteAddContents() invalid address",
             DgBase::Fatal);
   }

   (*grids_)[add.res()]->deleteAddContents(add.address());

} // void DgPhysicalRFS2D<C>::deleteAddContents

////////////////////////////////////////////////////////////////////////////////
