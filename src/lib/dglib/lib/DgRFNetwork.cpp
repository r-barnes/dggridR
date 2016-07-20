////////////////////////////////////////////////////////////////////////////////
//
// DgRFNetwork.cpp: DgRFNetwork class implementation
//
// Version 6.1 - Kevin Sahr, 5/23/13
//
////////////////////////////////////////////////////////////////////////////////

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "DgBase.h"
#include "DgRFBase.h"
#include "DgLocation.h"
#include "DgSeriesConverter.h"

////////////////////////////////////////////////////////////////////////////////
DgRFNetwork::~DgRFNetwork (void)
{
   for (uint64_t i = 0; i < size(); i++)
   {
      for (uint64_t j = 0; j < size(); j++) 
      {
         if (matrix_[i][j] && !(matrix_[i][j]->userGenerated()))
         {
            delete matrix_[i][j];
            matrix_[i][j] = 0;
         }
      }
      matrix_[i].clear();
   }

/* 
   for (int i = 0; i < frames_.size(); i++)
   {
      delete frames_[i];
      frames_[i] = 0;
   }
*/

} // DgRFNetwork::~DgRFNetwork

////////////////////////////////////////////////////////////////////////////////
bool
DgRFNetwork::existsConverter (const DgRFBase& fromFrame,
                                    const DgRFBase& toFrame) const
{
   return matrix_[fromFrame.id()][toFrame.id()];

} // bool DgRFNetwork::existsConverter

////////////////////////////////////////////////////////////////////////////////
const DgConverterBase* 
DgRFNetwork::getConverter (const DgLocation& fromLoc, 
                                 const DgRFBase& toFrame) const
{
   return getConverter(fromLoc.rf(), toFrame);

} // const DgConverter* DgRFNetwork::getConverter

////////////////////////////////////////////////////////////////////////////////
const DgConverterBase* 
DgRFNetwork::getConverter (const DgRFBase& fromFrame, 
                                 const DgRFBase& toFrame) const
{
   // check for network match

   if (fromFrame.network() != *this && toFrame.network() != *this)
   {
      report("DgRFNetwork::getConverter() network mismatch",
             DgBase::Fatal);
      return 0;
   }

   // check for existing converter

   if (!matrix_[fromFrame.id()][toFrame.id()]) 
   {
      // attempt to generate a passthrough converter
      
      if (!fromFrame.connectTo() || !toFrame.connectFrom())
      {
         report("DgRFNetwork::getConverter() frames not connected: " +
                 fromFrame.name() + string(" -> ") + toFrame.name(),
                DgBase::Fatal);
         return 0;
      }
      
      const_cast<DgRFNetwork*>(this)->matrix_
                           [fromFrame.id()][toFrame.id()] = 
                                new DgSeriesConverter(fromFrame, toFrame);
   }

   return matrix_[fromFrame.id()][toFrame.id()];

} // const DgConverter* DgRFNetwork::getConverter

////////////////////////////////////////////////////////////////////////////////
void
DgRFNetwork::update (void)
{
   for (uint64_t i = 0; i < size(); i++)
   {
      for (uint64_t j = 0; j < size(); j++)
      {
         if (i != j && matrix_[i][j] && !matrix_[i][j]->userGenerated())
         {
            delete matrix_[i][j];
            matrix_[i][j] = 0;
            if (frames_[i]->connectTo() && frames_[j]->connectFrom())
            {
               matrix_[i][j] = new DgSeriesConverter(*frames_[i], *frames_[j]);
            }
         }
      }
   }
} // void DgRFNetwork::update

////////////////////////////////////////////////////////////////////////////////
void
DgRFNetwork::reserve (const size_t& capacity)
{
   frames_.reserve(capacity);
   matrix_.reserve(capacity);

   for (unsigned long i = 0; i < matrix_.size(); i++) 
    matrix_[i].reserve(capacity);

} // void DgRFNetwork::reserve

////////////////////////////////////////////////////////////////////////////////
int
DgRFNetwork::generateId (DgRFBase* frame)
{
   uint64_t newSize = size() + 1;

   frames_.resize(newSize, 0);
   matrix_.resize(newSize);

   for (uint64_t i = 0; i < newSize; i++) 
    matrix_[i].resize(newSize, 0);
   
   frames_[nextId_] = frame;
   
   matrix_[nextId_][nextId_] = new DgIdentityConverter(*frame);
   
   if (nextId_ == 0) 
    frame->connectTo_ = frame->connectFrom_ = frame; // ground

   if (nextId_ == (int) capacity()) 
    reserve(capacity() + chunkSize());
   
   nextId_++;
   
   return (nextId_ - 1);
} 

////////////////////////////////////////////////////////////////////////////////
