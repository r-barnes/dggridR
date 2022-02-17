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
// DgSeriesConverter.cpp: DgSeriesConverter class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <vector>

using namespace std;

#include "DgBase.h"
#include "DgString.h"
#include "DgSeriesConverter.h"

////////////////////////////////////////////////////////////////////////////////
DgSeriesConverter::DgSeriesConverter 
      (const vector<const DgConverterBase*>& series, bool userGenerated)
   : DgConverterBase (series[0]->fromFrame(), 
                      series[series.size() - 1]->toFrame(), userGenerated)
{
   // verify the series does what it claims

   for (unsigned long i = 0; i < (series.size() - 1); i++)
   {
      if (series[i]->toFrame() != series[i+1]->fromFrame())
      {
         report(string("DgSeriesConverter::DgSeriesConverter() mismatch in "
           "toFrame/fromFrame: ") + series[i]->toFrame().name() + string("/")
           + series[i+1]->fromFrame().name(), DgBase::Fatal);
      }
   }

   series_.resize(series.size());
   for (unsigned long i = 0; i < series.size(); i++) 
    series_[i] = series[i];

} // DgSeriesConverter::DgSeriesConverter

////////////////////////////////////////////////////////////////////////////////
DgSeriesConverter::DgSeriesConverter (const DgRFBase& fromFrame, 
                                      const DgRFBase& toFrame,
                                      bool userGenerated)
   : DgConverterBase (fromFrame, toFrame, userGenerated)
{
   if (fromFrame.network() != toFrame.network())
   {
      report("DgSeriesConverter::DgSeriesConverter() network mismatch",
             DgBase::Fatal);
   }

   // if we're here we need to generate a passthrough converter:
   //    fromFrame => network ground => toFrame

   const DgRFNetwork& net = fromFrame.network();
   int i = 0;
   const DgRFBase* from = &fromFrame;
   series_.resize(0);
   while (true)
   {
      // can we get directly there from here?

      if (net.existsConverter(*from, toFrame)) 
      {
         series_.resize(series_.size() + 1);
         series_[i] =
           const_cast<DgConverterBase*>(net.getConverter(*from, toFrame));
         from = &toFrame;

         break;
      }

      // are we at the ground?

      if (from->id() == 0) break;

      // otherwise keep heading towards the ground

      if (!from->connectTo())
      {
         report("DgSeriesConverter::DgSeriesConverter() disconnect in "
                "series", DgBase::Fatal);
      }

      if (!net.existsConverter(*from, *(from->connectTo()))) 
      {
         report("DgSeriesConverter::DgSeriesConverter() bad connection",
                DgBase::Fatal);
      }

      series_.resize(series_.size() + 1);
      series_[i++] = const_cast<DgConverterBase*>(net.getConverter(*from, 
                                                  *(from->connectTo())));
      from = from->connectTo();
   }

   // are we done?

   if (from->id() == toFrame.id()) return;
   
   // otherwise we should be at the ground

   if (from->id() != 0)
   {
      report("DgSeriesConverter::DgSeriesConverter() ground not "
             "encountered", DgBase::Fatal);
   }

   // now build in reverse the series from the ground to the fromFrame

   list<const DgRFBase*> rev;
   const DgRFBase* ground = from;
   from = &toFrame;
   while (true)
   {
      rev.push_front(from);

      // are we there yet?

      if (from == ground) break;

      // can we get directly here from the ground?

      if (net.existsConverter(*ground, *from))
      {
         from = ground;
         rev.push_front(ground);
         break;
      }

      // otherwise keep trying to get back to the ground

      if (!from->connectFrom())
      {
         report("DgSeriesConverter::DgSeriesConverter() disconnect in "
                "series", DgBase::Fatal);
      }

      if (!net.existsConverter(*(from->connectFrom()), *from))
      {
         report("DgSeriesConverter::DgSeriesConverter() bad connection",
                DgBase::Fatal);
      }

      from = from->connectFrom();
   }

   // we should be back at ground

   if (from->id() != 0)
   {
      report("DgSeriesConverter::DgSeriesConverter() ground not "
             "encountered", DgBase::Fatal);
   }

   // now build the converter series from the ground to the toFrame

   list<const DgRFBase*>::iterator it = rev.begin();
   it++;
   const DgRFBase* to;
   for (; it != rev.end(); it++)
   {
      to = *it;
      series_.resize(series_.size() + 1);

      // can we get directly there from here?

      if (net.existsConverter(*from, toFrame)) 
      {
         series_[i] =
           const_cast<DgConverterBase*>(net.getConverter(*from, toFrame));

         break;
      }

      // otherwise add this conversion to the series and continue

      if (!net.existsConverter(*from, *to)) 
      {
         report("DgSeriesConverter::DgSeriesConverter() bad connection",
                DgBase::Fatal);
      }

      series_[i++] =
           const_cast<DgConverterBase*>(net.getConverter(*from, *to));

      from = to;
   }

} // DgSeriesConverter::DgSeriesConverter

////////////////////////////////////////////////////////////////////////////////
DgSeriesConverter::~DgSeriesConverter (void)
{
   // virtual destructor

} // DgSeriesConverter::~DgSeriesConverter

////////////////////////////////////////////////////////////////////////////////
DgAddressBase* 
DgSeriesConverter::createConvertedAddress (const DgAddressBase& addIn) const
{
   // keep track of nested series depth for formatting output
   static int seriesDepth = 0;
   seriesDepth++;

   if (isTraceOn())
      traceStream() << " -> " << std::string(seriesDepth, '*') << " <SERIES> " 
                    << fromFrame().name() << ": " << addIn << endl;

   DgAddressBase* pAdd0 = series_[0]->createConvertedAddress(addIn);
   if (isTraceOn()) 
      traceStream() << std::string(seriesDepth, '*') << "  --> " 
                    << fromFrame().name() << ": " << *pAdd0 << endl;

#if DGDEBUG
dgcout << "BEGIN series conversion" << endl;
#endif
   for (int i = 1; i < size(); i++)
   {
      DgAddressBase* pAdd1 = series_[i]->createConvertedAddress(*pAdd0);

      delete pAdd0;
      pAdd0 = pAdd1;
      if (isTraceOn()) 
         traceStream() << std::string(seriesDepth, '*') << "  --> " 
                       << fromFrame().name() << ": " << *pAdd0 << endl;
   }
#if DGDEBUG
dgcout << "END series conversion" << endl;
#endif

   seriesDepth--;

   return pAdd0;

} // DgAddressBase* DgSeriesConverter::createConvertedAddress

////////////////////////////////////////////////////////////////////////////////
const DgConverterBase&
DgSeriesConverter::converter (int ndx) const
{
   if (ndx < 0 || ndx >= size())
   {
      report("DgSeriesConverter::converter(" + dgg::util::to_string(ndx) + 
             ") index out of range", DgBase::Fatal);
      return *series_[0]; // will never actually get here
   }

   return *series_[ndx];

} // const DgConverterBase* DgSeriesConverter::converter

////////////////////////////////////////////////////////////////////////////////
ostream& operator<< (ostream& stream, const DgSeriesConverter& con)
{ 
   stream << "{\n";
   for (int i = 0; i < con.size(); i++) 
   {
      stream << "  " << con.converter(i) << "\n";
   }
   return stream << "}" << endl;

} // ostream& operator<<

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

