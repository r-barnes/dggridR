////////////////////////////////////////////////////////////////////////////////
//
// transform.C: perform transformations of addresses
//
// Created by Kevin Sahr, October 1, 2001
//
//    version 3.1b: November 11, 2001
//    version 4.3b: June 21, 2003
//    version 6.2b: October 25, 2014
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

using namespace std;

#include "dggrid.h"
#include "DgIDGG.h"
#include "DgBoundedIDGG.h"
#include "DgInputStream.h"
#include "DgOutputStream.h"
#include "DgCell.h"
#include "DgOutAIGenFile.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
TransformParam::TransformParam (DgParamList& plist)
      : MainParam(plist), inSeqNum (0), outSeqNum (false), inputDelimiter (' '),
        outputDelimiter (' '), nDensify (1)
{ 
      using namespace dgg;

      /////// fill state variables from the parameter list //////////

      // input file name

      getParamValue(plist, "input_file_name", inFileName, false);

      // input address type

      getParamValue(plist, "input_address_type", inAddType, false);

      // input delimiter

      string dummy;
      getParamValue(plist, "input_delimiter", dummy, false);
      if (dummy.length() != 3 || dummy.c_str()[0] != '"' ||
          dummy.c_str()[2] != '"')
      {
         ::report(
          "invalid input_delimiter; must be a single char in double quotes",
          DgBase::Fatal);
      }
      inputDelimiter = dummy.c_str()[1];

      // output file name

      getParamValue(plist, "output_file_name", outFileNameBase, false);

      // output address type

      getParamValue(plist, "output_address_type", outAddType, false);

      // output delimiter

      getParamValue(plist, "output_delimiter", dummy, false);
      if (dummy.length() != 3 || dummy.c_str()[0] != '"' ||
          dummy.c_str()[2] != '"')
      {
         ::report(
          "invalid output_delimiter; must be a single char in double quotes",
          DgBase::Fatal);
      }
      outputDelimiter = dummy.c_str()[1];

      getParamValue(plist, "densification", nDensify, false);

      if (inAddType == "GEO" && outAddType == "GEO")
      {
         ::report("TransformParam::TransformParam() At least one of the " +
                  string("address types must be non-GEO"), DgBase::Fatal);
      }

} // TransformParam::TransformParam

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TransformParam::dump (void)
{
   MainParam::dump();

   cout << "BEGIN TRANSFORM PARAMETER DUMP" << endl;
   
   cout << " outFileNameBase: " << outFileNameBase << endl;
   cout << " outFileName: " << outFileName << endl;
   cout << " outAddType: " << outAddType << endl;
   cout << " outSeqNum: " << outSeqNum << endl;
   cout << " outputDelimiter: " << outputDelimiter << endl;
   cout << " nDensify: " << nDensify << endl;

   cout << " inFileName: " << inFileName << endl;
   cout << " inAddType: " << inAddType << endl;
   cout << " inSeqNum: " << inSeqNum << endl;
   cout << " inputDelimiter: " << inputDelimiter << endl;

   cout << "END TRANSFORM PARAMETER DUMP" << endl;

} // void TransformParam::dump

////////////////////////////////////////////////////////////////////////////////
void doTransform (TransformParam& dp)
{
   ////// create the reference frames ////////

   DgRFNetwork net0;
   DgGeoSphRF geoRF(net0, dp.datum, dp.earthRadius);
   DgIDGG dgg(geoRF, dp.vert0, dp.azimuthDegs, dp.aperture, dp.actualRes,
              "DDG", dp.gridTopo, dp.projType, dp.isMixed43, dp.numAp4,
              dp.isSuperfund, dp.sfRes, dp.precision);

   cout << "Res " << dgg.outputRes() << " " << dgg.gridStats() << endl;

   // set-up to convert to degrees
   DgGeoSphDegRF deg(geoRF, geoRF.name() + "Deg");

   // set-up the input reference frame

   dp.inSeqNum = false;
   const DgRFBase* pInRF = NULL;
   if (dp.inAddType == "GEO") pInRF = &geoRF;
   else if (dp.inAddType == "PROJTRI") pInRF = &dgg.projTriRF();
   else if (dp.inAddType == "VERTEX2DD") pInRF = &dgg.vertexRF();
   else if (dp.inAddType == "Q2DD") pInRF = &dgg.q2ddRF();
   //else if (dp.inAddType == "INTERLEAVE") pInRF = &dgg.intRF();
   //else if (dp.inAddType == "PLANE") pInRF = &dgg.planeRF();
   else if (dp.inAddType == "Q2DI") pInRF = &dgg;
   else if (dp.inAddType == "SEQNUM") 
   {
      dp.inSeqNum = true;
      pInRF = &dgg;
   }
   const DgRFBase& inRF = *pInRF;

   // set-up the output reference frame

   bool outAIG = (dp.outAddType == "AIGEN");
   dp.outSeqNum = false;
   const DgRFBase* pOutRF = NULL;
   if (dp.outAddType == "GEO") pOutRF = &geoRF;
   else if (dp.outAddType == "PROJTRI") pOutRF = &dgg.projTriRF();
   else if (dp.outAddType == "VERTEX2DD") pOutRF = &dgg.vertexRF();
   else if (dp.outAddType == "Q2DD") pOutRF = &dgg.q2ddRF();
   else if (dp.outAddType == "INTERLEAVE") pOutRF = &dgg.intRF();
   else if (dp.outAddType == "PLANE") pOutRF = &dgg.planeRF();
   else if (dp.outAddType == "Q2DI") pOutRF = &dgg;
   else if (outAIG) pOutRF = &dgg;
   else if (dp.outAddType == "SEQNUM") 
   {
      dp.outSeqNum = true;
      pOutRF = &dgg;
   }
   const DgRFBase& outRF = *pOutRF;

   // set the precision

   const_cast<DgRFBase&>(outRF).setPrecision(dp.precision);

   // now process the addresses in the input file

   const int maxLine = 1000;
   char buff[maxLine];
   const char* remainder;

   cout << "transforming values..." << endl;

   DgInputStream inFile(dp.inFileName, "", DgBase::Fatal);
   ofstream* pOutFile;
   if (outAIG)
      pOutFile = new DgOutAIGenFile(geoRF, dp.outFileName);
   else
      pOutFile = new DgOutputStream(dp.outFileName, "", DgBase::Fatal);

   ofstream& outFile = *pOutFile;

   char delimStr[2];
   delimStr[0] = dp.inputDelimiter;
   delimStr[1] = '\0';

   while (1)
   {
      // get the next line

      inFile.getline(buff, maxLine);
      if (inFile.eof()) break;

      // parse the address

      DgLocation* loc = NULL; 
      if (dp.inSeqNum)
      {
         char* snStr;
         snStr = strtok(buff, delimStr);
         unsigned long int sNum;
         if (sscanf(snStr, "%ld", &sNum) != 1)
         {
            ::report("doTransform(): invalid SEQNUM " + string(snStr), 
                     DgBase::Fatal);
         }

         loc = static_cast<const DgIDGG&>(inRF).bndRF().locFromSeqNum(sNum);
         remainder = &(buff[strlen(snStr) + 1]);
      }
      else
      {    
         loc = new DgLocation(inRF);
         remainder = loc->fromString(buff, dp.inputDelimiter);
      }

      // convert the address
      outRF.convert(loc);

      // output the converted line

      if (outAIG)
      {
         const DgIDGG& idgg = static_cast<const DgIDGG&>(outRF);
         unsigned long int sn = idgg.bndRF().seqNum(*loc);
         DgPolygon verts(idgg);
         idgg.setVertices(*loc, verts, dp.nDensify);
         DgCell cell(geoRF, dgg::util::to_string(sn), *loc, new DgPolygon(verts));
         static_cast<DgOutAIGenFile&>(outFile) << cell;
      }
      else
      {
         if (dp.outSeqNum)
         {
            outFile << static_cast<const DgIDGG&>(outRF).bndRF().seqNum(*loc);
         }
         else
         {
            outFile << loc->asString(dp.outputDelimiter);
         }

         while (remainder && isspace(*remainder)) remainder++;
         if (remainder && strlen(remainder) > 0) 
            outFile << dp.outputDelimiter << remainder << endl;
         else 
            outFile << endl;
      }

      delete loc;

   }

   inFile.close();
   outFile.close();
   delete pOutFile;

} // void doTransform

////////////////////////////////////////////////////////////////////////////////
void doTransforms (TransformParam& dp, DgGridPList& plist)
{
   for (dp.curGrid = 1; dp.curGrid <= dp.numGrids; dp.curGrid++)
   {
      // first get the grid placement

      dp.outFileName = dp.outFileNameBase;
      dp.metaOutFileName = dp.metaOutFileNameBase; 

      orientGrid(dp, plist);

      if (dp.numGrids > 1) 
      {
         string suffix = string(".") + dgg::util::to_string(dp.curGrid, 4);
         dp.metaOutFileName = dp.metaOutFileName + suffix;
         dp.outFileName = dp.outFileName + suffix;
      }
      
      if (dp.numGrids > 1 || dp.placeRandom) 
      {
         ofstream metaOutFile;
         metaOutFile.open(dp.metaOutFileName.c_str());
         metaOutFile.setf(ios::fixed, ios::floatfield);
         metaOutFile.precision(12);
         metaOutFile << plist;
         metaOutFile.close();
      }

      // now do it

      if (dp.curGrid == dp.numGrids) dp.lastGrid = true;

      doTransform(dp);

      cout << endl;
   }

   cout << "** transformation complete **" << endl;

} // void doBinVals

////////////////////////////////////////////////////////////////////////////////
