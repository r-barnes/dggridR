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
// DgParamList.cpp: DgParamList class implementation
//
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <algorithm>
#include <fstream>

#include "DgParamList.h"

////////////////////////////////////////////////////////////////////////////////
DgAssoc::~DgAssoc (void)
{
   // does nothing

} // DgAssoc::~DgAssoc

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
DgParamList::~DgParamList (void)
{
   clearList();

} // DgParamList::~DgParamList

////////////////////////////////////////////////////////////////////////////////
void
DgParamList::clearList (void)
{
   for (unsigned int i = 0; i < parameters.size(); i++)
   {
      delete parameters[i];
      parameters[i] = 0;
   }
   parameters.resize(0);

} // void DgParamList::clearList

////////////////////////////////////////////////////////////////////////////////
void
DgParamList::loadParams (const string& fileName)
{
   ifstream metaFile;
   metaFile.open(fileName.c_str(), ios::in);
   if (!metaFile.good())
   {
      report("DgParamList::loadParams() unable to open file " + fileName,
             DgBase::Fatal);
   }

   const int maxLine = 10000;
   char nextLine[maxLine];

   while (!metaFile.eof())
   {
      metaFile.getline(nextLine, maxLine);
        
      if (metaFile.eof()) break;

      if (strlen(nextLine) <= 1 || nextLine[0] == '#') continue;

      char* token = nextLine;
      while (isspace(*token)) token++;

      char* remainder = token;
      while (!isspace(*remainder)) remainder++;
      *remainder = '\0';

      remainder++;
      while (isspace(*remainder)) remainder++;

      setParam(token, remainder);
   }

   metaFile.close();

} // void DgParamList::loadParams

////////////////////////////////////////////////////////////////////////////////
void
DgParamList::setParam (const string& nameIn, const string& strValIn)
{
   if (toLower(strValIn) == string("invalid")) return;

   DgAssoc* existing = getParam(nameIn, false);
   if (!existing)
   {
      report(string("DgParamList::setParam() unknown parameter ") 
             + nameIn, DgBase::Fatal);
   }
   else // already exists
   {
      existing->setValStr(strValIn);
      existing->setIsDefault(false);
      existing->setIsUserSet(true);
      if (!existing->validate())
      {
         report(string("Invalid parameter data in parameter:\n") +
            existing->asString() + string("\n") +
            existing->validationErrMsg(), DgBase::Fatal);
      }
   }

} // void DgParamList::setParam

////////////////////////////////////////////////////////////////////////////////
void
DgParamList::setPresetParam (const string& nameIn, const string& strValIn)
{
   if (toLower(strValIn) == string("invalid")) return;

   DgAssoc* existing = getParam(nameIn, false);
   if (!existing)
   {
      report(string("DgParamList::setPresetParam() unknown parameter ") 
             + nameIn, DgBase::Fatal);
   }
   else // already exists
   {
      // only change value if it was not set explicitly by the user
      if (!existing->isUserSet())
      {
         existing->setValStr(strValIn);
         existing->setIsDefault(false);
         if (!existing->validate())
         {
            report(string("Invalid parameter data in parameter:\n") +
               existing->asString() + string("\n") +
               existing->validationErrMsg(), DgBase::Fatal);
         }
      }
   }

} // void DgParamList::setPresetParam

////////////////////////////////////////////////////////////////////////////////
void
DgParamList::insertParam (DgAssoc* param) // does not make a copy
{
   if (!param)
   {
      report("DgParamList::insertParam() null parameter", DgBase::Fatal);
   }

   DgAssoc* existing = getParam(param->name(), false);
   if (!existing)
   {
      parameters.push_back(param);
   }
   else // already exists
   {
      report(string("replacing parameter: ") + existing->asString()
        + string("\nwith parameter: ") + param->asString() +
        string("\n"), DgBase::Info);

      *existing = *param;
   }

} // void DgParamList::insertParam

////////////////////////////////////////////////////////////////////////////////
DgAssoc*
DgParamList::getParam (const string& nameIn, bool setToIsApplicable) const
{
   string lower = toLower(nameIn);

   for (unsigned int i = 0; i < parameters.size(); i++)
   {
      if (parameters[i]->name() == lower) 
      {
       if (setToIsApplicable)
          parameters[i]->setIsApplicable(true);
       return parameters[i];
      }
   }

   // if we're here we didn't find it
   return 0;

} // void DgParamList::getParam

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
