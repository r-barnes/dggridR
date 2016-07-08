////////////////////////////////////////////////////////////////////////////////
//
// DgDiscRFS.hpp: DgDiscRFS template class definition.
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include "DgRF.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setAddNeighbors (const DgResAdd<A>& add, 
                                            DgLocVector& vec) const
{
   grids()[add.res()]->convert(vec);
   grids()[add.res()]->setAddNeighbors(add.address(), vec);
   this->convert(vec);

} // void DgDiscRFS<A, B, DB>::setAddNeighbors

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setParents (int res, const DgLocation& loc, 
                                                DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);

   if (res > 0 && res < nRes())
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddParents(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setParents

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setInteriorChildren (int res, const DgLocation& loc, 
                                   DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);

   if (res >= 0 && res < (nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddInteriorChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setInteriorChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setBoundaryChildren (int res, const DgLocation& loc, 
                                   DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec); 
   if (res >= 0 && res < (nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddBoundaryChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> void
DgDiscRFS<A, B, DB>::setAllChildren (int res, const DgLocation& loc, 
                              DgLocVector& vec) const
{
   vec.clearAddress(); 
   this->convert(vec);

   if (res >= 0 && res < (nRes() - 1))
   {
      DgLocation tmpLoc(loc);
      grids()[res]->convert(&tmpLoc);
      this->convert(&tmpLoc);
      setAddAllChildren(*(this->getAddress(tmpLoc)), vec);
   }

} // void DgDiscRFS::setAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class A, class B, class DB> const char*
DgDiscRFS<A, B, DB>::str2add (DgResAdd<A>* add, const char* str,
                              char delimiter) const
{
   if (!add) add = new DgResAdd<A>();

   char delimStr[2];
   delimStr[0] = delimiter;
   delimStr[1] = '\0';

   char* tmpStr = new char[strlen(str) + 1];
   strcpy(tmpStr, str);

   char* tok;

   // get the resolution

   tok = strtok(tmpStr, delimStr);
   int res;
   if (sscanf(tok, "%d", &res) != 1)
   {
      ::report("DgDiscRFS<A, B, DB>::str2add() invalid res string " +
               string(tok), DgBase::Fatal);
   }

   // now get the address

   const char* tmp = &(str[strlen(tok) + 1]);
   DgLocation tloc(*grids()[res]);
   tmp = tloc.fromString(tmp, delimiter);
   const A& subAdd = *grids()[res]->getAddress(tloc);
   *add = DgResAdd<A>(subAdd, res);

   return tmp;

} // const char* DgDiscRFS::str2add

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
