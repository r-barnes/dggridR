////////////////////////////////////////////////////////////////////////////////
//
// DgIDGGS43H.cpp: DgIDGGS43H class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "DgContCartRF.h"
#include "DgDiscRF.h"
#include "DgHexC1Grid2D.h"
#include "DgHexC2Grid2D.h"
#include "DgIDGGS43H.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgIDGGS43H::DgIDGGS43H (const DgIDGGS43H& rf) 
  : DgIDGGS (rf)
{
   report("DgIDGGS43H::operator=() not implemented yet", DgBase::Fatal);

} // DgIDGGS43H::DgIDGGS43H

////////////////////////////////////////////////////////////////////////////////
DgIDGGS43H::~DgIDGGS43H (void)
{
   for (unsigned int i = 0; i < grids().size(); i++) 
    delete (*grids_)[i]; 

   delete grids_;

} // DgIDGGS43H::~DgIDGGS43H

////////////////////////////////////////////////////////////////////////////////
DgIDGGS43H&
DgIDGGS43H::operator= (const DgIDGGS43H& rf)
{
   report("DgIDGGS43H::operator=() not implemented", DgBase::Fatal);

   return *this;

} // DgIDGGS43H& DgIDGGS43H::operator=

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS43H::setAddParents (const DgResAdd<DgQ2DICoord>& add, 
                             DgLocVector& vec) const
{
   report("DgIDGGS43H::setAddParents()  not implemented yet", DgBase::Fatal);

} // void DgIDGGS43H::setAddParents

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS43H::setAddInteriorChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   report("DgIDGGS43H::setAddInteriorChildren() not implemented yet", 
          DgBase::Fatal);

} // void DgIDGGS43H::setAddInteriorChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS43H::setAddBoundaryChildren (const DgResAdd<DgQ2DICoord>& add, 
                                        DgLocVector& vec) const
{
   report("DgIDGGS43H::setAddBoundaryChildren() not implemented yet", 
          DgBase::Fatal);

} // void DgIDGGS43H::setAddBoundaryChildren

////////////////////////////////////////////////////////////////////////////////
void 
DgIDGGS43H::setAddAllChildren (const DgResAdd<DgQ2DICoord>& add, 
                                   DgLocVector& vec) const
{
   report("DgIDGGS43H::setAddAllChildren() not implemented yet", DgBase::Fatal);

} // void DgIDGGS43H::setAddAllChildren

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
