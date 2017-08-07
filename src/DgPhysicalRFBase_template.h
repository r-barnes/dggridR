////////////////////////////////////////////////////////////////////////////////
//
// DgPhysicalRFBase_template.h: DgPhysicalRFBase template class definition.
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

template<class C>
DgPhysicalRFBase<C>::~DgPhysicalRFBase (void)
{
   // virtual destructor

} // DgPhysicalRFBase<C>::~DgPhysicalRFBase<C>

////////////////////////////////////////////////////////////////////////////////
template<class C> void
DgPhysicalRFBase<C>::initAllCells (void)
{
   for (DgLocation loc = boundedRFBase().first(); loc != boundedRFBase().end();
           boundedRFBase().incrementLocation(loc))
   {
      C* c = getContents(loc, false, true);
      if (c) initCell(c, loc);
   }

} // void DgPhysicalRFBase<C>::initAllCells

////////////////////////////////////////////////////////////////////////////////
template<class C> ostream& operator<< (ostream& stream,
                                       const DgPhysicalRFBase<C>& rfIn)
{ 
   return stream << "Phys-" << rfIn.boundedRFBase(); 

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

