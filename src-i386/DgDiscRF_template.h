////////////////////////////////////////////////////////////////////////////////
//
// DgDiscRF_template.h: DgDiscRF template class definition.
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setVertices (const DgLocation& loc, DgPolygon& vec) const
{
   vec.clearAddress(); 
   backFrame().convert(vec);
   
   if (loc.rf() == *this)
   {
      setAddVertices(*(this->getAddress(loc)), vec);
   }
   else
   {
      DgLocation tmpLoc(loc);
      this->convert(&tmpLoc);
      setAddVertices(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRF::setVertices

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setVertices (const DgLocation& loc, const DgRFBase& rf,
                                                     DgPolygon& vec) const
{
   setVertices(loc, vec);
   if (rf != backFrame()) backFrame().convert(vec);

} // const DgLocVector& DgDiscRF::setVertices

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setNeighbors (const DgLocation& loc, DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);
   
   if (loc.rf() == *this)
   {
      setAddNeighbors(*(this->getAddress(loc)), vec);
   }
   else
   {
      DgLocation tmpLoc(loc);
      this->convert(&tmpLoc);
      setAddNeighbors(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRF::setNeighbors

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setPoint (const DgLocation& loc, DgLocation& point) const
{
   if (loc.rf() == *this)
   {
      setAddPoint(*(this->getAddress(loc)), point);
   }
   else
   {
      DgLocation tmpLoc(loc);
      this->convert(&tmpLoc);
      setAddPoint(*(this->getAddress(tmpLoc)), point);
   }

} // void DgDiscRF::setPoint

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setPoint (const DgLocation& loc, const DgRFBase& rf,
                                                     DgLocation& point) const
{
   setPoint(loc, point);
   if (rf != backFrame()) backFrame().convert(&point);

} // void DgDiscRF::setPoint

////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRF<A, B, DB>::setAddPoint (const A& add, DgLocation& pt) const
{
   DgLocation* tmpLoc = backFrame().makeLocation(invQuantify(add));

   pt = *tmpLoc;

   delete tmpLoc;

} // void DgDiscRF<A, B, DB>::setPoint

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
