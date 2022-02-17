#ifndef DGGRIDR
#define DGGRIDR
#endif
/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

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
// DgRFNetwork.cpp: DgRFNetwork class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include "DgBase.h"
#include "DgRFBase.h"
#include "DgLocation.h"
#include "DgSeriesConverter.h"

////////////////////////////////////////////////////////////////////////////////
DgRFNetwork::~DgRFNetwork (void)
{
   for (unsigned long long int i = 0; i < size(); i++) {
      for (unsigned long long int j = 0; j < size(); j++) {
         //if (matrix_[i][j] && !(matrix_[i][j]->userGenerated())) {
         if (matrix_[i][j]) {
            delete matrix_[i][j];
            matrix_[i][j] = 0;
         }
      }
      matrix_[i].clear();
   }

   for (auto &frame : frames_) {
      delete frame;
      frame = nullptr;
   }

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
   for (unsigned long long int i = 0; i < size(); i++)
   {
      for (unsigned long long int j = 0; j < size(); j++)
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

   for(auto &row : matrix_){
      row.reserve(capacity);
   }

} // void DgRFNetwork::reserve

////////////////////////////////////////////////////////////////////////////////
int
DgRFNetwork::generateId (DgRFBase* frame)
{
   frames_.push_back(frame);
   matrix_.resize(frames_.size());

   for (auto& row : matrix_){
      row.resize(frames_.size(), 0);
   }

   matrix_[nextId_][nextId_] = new DgIdentityConverter(*frame);

   if (nextId_ == 0)
    frame->connectTo_ = frame->connectFrom_ = frame; // ground

   if (nextId_ == (int) capacity())
    reserve(capacity() + chunkSize());

   nextId_++;

   return (nextId_ - 1);
}

////////////////////////////////////////////////////////////////////////////////
