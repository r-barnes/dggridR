////////////////////////////////////////////////////////////////////////////////
//
// binvals.C: perform binning of point values into grid
//
// Created by Kevin Sahr, October 1, 2001
//    version 3.1b: November 11, 2001
//    version 4.3b: June 21, 2003
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

using namespace std;

#include "dggrid.h"
#include "gpc.h"
#include "DgIDGG.h"
#include "DgInputStream.h"
#include "DgIDGGS.h"
#include "DgBoundedIDGGS.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
BinValsParam::BinValsParam (DgParamList& plist)
      : MainParam(plist), wholeEarth (true), outFile (0), outSeqNum (false),
        inputDelimiter (' '), outputDelimiter (' '), outputAllCells (true)
{ 
      /////// fill state variables from the parameter list //////////

      string dummy;
      getParamValue(plist, "bin_method", dummy, false); // ignore

      getParamValue(plist, "bin_coverage", dummy, false);
      if (dummy == "GLOBAL")
         wholeEarth = true;
      else
         wholeEarth = false;

      //// input file names

      string inFileStr;
      getParamValue(plist, "input_files", inFileStr, false);
  
      char* names = new char[inFileStr.length() + 1];
      inFileStr.copy(names, string::npos);
      names[inFileStr.length()] = 0;

      char* name = strtok(names, " ");
      while (name != NULL)
      {
         inputFiles.push_back(string(name));
         name = strtok(NULL, " ");
      }
      delete [] names;

      // input delimiter

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

      // cell_output_control

      getParamValue(plist, "cell_output_control", dummy, false);
      if (dummy == "OUTPUT_ALL") outputAllCells = true;
      else outputAllCells = false;

} // BinValsParam::BinValsParam

////////////////////////////////////////////////////////////////////////////////
BinValsParam::~BinValsParam() 
{
} // BinValsParam::~BinValsParam()

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void BinValsParam::dump (void)
{
   MainParam::dump();

   cout << "BEGIN BINVALS PARAMETER DUMP" << endl;
   
   cout << " wholeEarth: " << wholeEarth << endl;
   cout << " outFileNameBase: " << outFileNameBase << endl;
   cout << " outFileName: " << outFileName << endl;

   cout << " *outFile: ";
   if (outFile)
      cout << outFileName << endl;
   else
      cout << "null" << endl;

   cout << " inputFiles: " << endl;
   for (unsigned int i = 0; i < inputFiles.size(); i++)
      cout << "  " << i << " " << inputFiles[i] << endl;

   cout << " outAddType: " << outAddType << endl;
   cout << " outSeqNum: " << outSeqNum << endl;
   cout << " inputDelimiter: " << inputDelimiter << endl;
   cout << " outputDelimiter: " << outputDelimiter << endl;
   cout << " inFormatStr: " << inFormatStr << endl;
   cout << " outputAllCells: " << outputAllCells << endl;

   cout << "END BINVALS PARAMETER DUMP" << endl;

} // void BinValsParam::dump

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// values for a single cell

class Val {

   public:

      int nVals;
      double val;

};

////////////////////////////////////////////////////////////////////////////////
void binValsGlobal (BinValsParam& dp)
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

   // set-up the output reference frame

   dp.outSeqNum = false;
   const DgRFBase* pOutRF = NULL;
   if (dp.outAddType == "PROJTRI") pOutRF = &dgg.projTriRF();
   else if (dp.outAddType == "VERTEX2DD") pOutRF = &dgg.vertexRF();
   else if (dp.outAddType == "Q2DD") pOutRF = &dgg.q2ddRF();
   else if (dp.outAddType == "INTERLEAVE") pOutRF = &dgg.intRF();
   else if (dp.outAddType == "PLANE") pOutRF = &dgg.planeRF();
   else if (dp.outAddType == "Q2DI") pOutRF = &dgg;
   else if (dp.outAddType == "SEQNUM") 
   {
      dp.outSeqNum = true;
      pOutRF = &dgg;
   }
   else
   {
      ::report("binValsGlobal(): invalid output_address_type " + 
               dp.outAddType, DgBase::Fatal);
   }

   const DgRFBase& outRF = *pOutRF;

   // create an array to store the values

   Val* vals = new Val[dgg.bndRF().size()];
   for (unsigned long int i = 0; i < dgg.bndRF().size(); i++) 
   {
      vals[i].nVals = 0;
      vals[i].val = 0.0;
   }

   // now process the points in each input file

   cout << "binning values..." << endl;

   const int maxLine = 100;
   char buff[maxLine];
   double lon, lat, val;
   for (unsigned int fc = 0; fc < dp.inputFiles.size(); fc++)
   {
      DgInputStream inFile(dp.inputFiles[fc], "", DgBase::Fatal);

      while (1)
      {
         inFile.getline(buff, maxLine);
         if (inFile.eof()) break;

         int result;
         result = sscanf(buff, dp.inFormatStr.c_str(), &lon, &lat, &val);
         if (result != 3)
         {
            ::report("binValsGlobal(): invalid format in file " + 
                     dp.inputFiles[fc], DgBase::Fatal);
         }

         DgLocation* tloc = geoRF.makeLocation(DgGeoCoord(lon, lat, false));
         dgg.convert(tloc);
         long int sNum = dgg.bndRF().seqNum(*tloc);
         long int ndx = sNum - 1;
         delete tloc;

         vals[ndx].nVals++;
         vals[ndx].val += val;
      }

      inFile.close();
   }

   ///// calculate the averages /////

   for (unsigned long int i = 0; i < dgg.bndRF().size(); i++) 
   {
      if (vals[i].nVals > 0) vals[i].val /= vals[i].nVals;
   }

   ///// output the values /////

   for (unsigned long int i = 0; i < dgg.bndRF().size(); i++) 
   {
      if (!dp.outputAllCells && vals[i].nVals <= 0) continue;
      unsigned long int sNum = i + 1;
      if (dp.outSeqNum)
         *dp.outFile << sNum << dp.outputDelimiter << vals[i].val << endl;
      else
      {
         DgLocation* tloc = dgg.bndRF().locFromSeqNum(sNum);
         outRF.convert(tloc);
         *dp.outFile << tloc->asString(dp.outputDelimiter)
                        << dp.outputDelimiter << vals[i].val << endl;
         delete tloc;
      }
   }

   ///// clean-up /////

   delete [] vals;

} // void binValsGlobal

////////////////////////////////////////////////////////////////////////////////
class QuadVals {

   public:

      bool isUsed;
      DgIVec2D offset;     // offset of min (i, j)
      DgIVec2D upperRight; // (maxi, maxj) relative to offset
      int numI;
      int numJ;

      Val** vals;

};

////////////////////////////////////////////////////////////////////////////////
void binValsPartial (BinValsParam& dp)
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

   // set-up the output reference frame

   dp.outSeqNum = false;
   const DgRFBase* pOutRF = NULL;
   if (dp.outAddType == "PROJTRI") pOutRF = &dgg.projTriRF();
   else if (dp.outAddType == "VERTEX2DD") pOutRF = &dgg.vertexRF();
   else if (dp.outAddType == "Q2DD") pOutRF = &dgg.q2ddRF();
   else if (dp.outAddType == "INTERLEAVE") pOutRF = &dgg.intRF();
   else if (dp.outAddType == "PLANE") pOutRF = &dgg.planeRF();
   else if (dp.outAddType == "Q2DI") pOutRF = &dgg;
   else if (dp.outAddType == "SEQNUM") 
   {
      dp.outSeqNum = true;
      pOutRF = &dgg;
   }
   else
   {
      ::report("binValsPartial(): invalid output_address_type " + 
               dp.outAddType, DgBase::Fatal);
   }

   const DgRFBase& outRF = *pOutRF;

   // create a place to store the values by quad

   QuadVals qvals[12];
   for (int q = 0; q < 12; q++)
   {
      qvals[q].isUsed = false;
      qvals[q].offset = DgIVec2D(dgg.maxI() + 1, dgg.maxJ() + 1);
      qvals[q].upperRight = DgIVec2D(-1, -1);
      qvals[q].numI = 0;
      qvals[q].numJ = 0;
      qvals[q].vals = 0;
   }

   // now make a first pass through the input files and determine what
   // cells are represented

   cout << "determing quad bounds..." << endl;

   const int maxLine = 100;
   char buff[maxLine];
   double lon, lat, val;
   for (unsigned int fc = 0; fc < dp.inputFiles.size(); fc++)
   {
      DgInputStream inFile(dp.inputFiles[fc], "", DgBase::Fatal);

      while (1)
      {
         inFile.getline(buff, maxLine);
         if (inFile.eof()) break;

         int result;
         result = sscanf(buff, dp.inFormatStr.c_str(), &lon, &lat, &val);
         if (result != 3)
         {
            ::report("binValsPartial(): invalid format in file " + 
                      dp.inputFiles[fc], DgBase::Fatal);
         }

         DgLocation* tloc = geoRF.makeLocation(DgGeoCoord(lon, lat, false));
         dgg.convert(tloc);

         int q = dgg.getAddress(*tloc)->quadNum();
         const DgIVec2D& coord = dgg.getAddress(*tloc)->coord();
         QuadVals& qv = qvals[q];

         qv.isUsed = true;
         if (coord.i() < qv.offset.i()) qv.offset.setI(coord.i());
         if (coord.i() > qv.upperRight.i()) qv.upperRight.setI(coord.i());
         if (coord.j() < qv.offset.j()) qv.offset.setJ(coord.j());
         if (coord.j() > qv.upperRight.j()) qv.upperRight.setJ(coord.j());

         delete tloc;
      }

      inFile.close();
   }

   // now initialize the vals storage in the quads which are used

   for (int q = 0; q < 12; q++)
   {
      QuadVals& qv = qvals[q];
      if (!qv.isUsed) continue;
      
      qv.upperRight = qv.upperRight - qv.offset; // make relative

      qv.numI = qv.upperRight.i() + 1;
      qv.numJ = qv.upperRight.j() + 1;
      qv.vals = new Val*[qv.numI];
      for (int i = 0; i < qv.numI; i++)
      {
         qv.vals[i] = new Val[qv.numJ];
         
         for (int j = 0; j < qv.numJ; j++)
         {
            qv.vals[i][j].nVals = 0;
            qv.vals[i][j].val = 0.0;
         }
      }
   }

   // now process the points in each input file

   cout << "binning values..." << endl;

   for (unsigned int fc = 0; fc < dp.inputFiles.size(); fc++)
   {
      DgInputStream inFile(dp.inputFiles[fc], "", DgBase::Fatal);

      while (1)
      {
         inFile.getline(buff, maxLine);
         if (inFile.eof()) break;

         int result;
         result = sscanf(buff, dp.inFormatStr.c_str(), &lon, &lat, &val);
         if (result != 3)
         {
            ::report("binValsPartial(): invalid format in file " + 
                      dp.inputFiles[fc], DgBase::Fatal);
         }

         DgLocation* tloc = geoRF.makeLocation(DgGeoCoord(lon, lat, false));
         dgg.convert(tloc);

         int q = dgg.getAddress(*tloc)->quadNum();
         QuadVals& qv = qvals[q];
         DgIVec2D coord = dgg.getAddress(*tloc)->coord() - qv.offset;
         delete tloc;

         qv.vals[coord.i()][coord.j()].nVals++;
         qv.vals[coord.i()][coord.j()].val += val;
      }

      inFile.close();
   }

   ///// calculate the averages /////

   for (int q = 0; q < 12; q++)
   {
      QuadVals& qv = qvals[q];
      if (!qv.isUsed) continue;

      for (int i = 0; i < qv.numI; i++)
      {
         for (int j = 0; j < qv.numJ; j++)
         {
            if (qv.vals[i][j].nVals > 0) 
               qv.vals[i][j].val /= qv.vals[i][j].nVals;
         }
      }
   }

   ///// output the values /////

   if (dp.outputAllCells)
   {
      for (unsigned long int i = 0; i < dgg.bndRF().size(); i++) 
      {
         unsigned long int sNum = i + 1;
         DgLocation* tloc = dgg.bndRF().locFromSeqNum(sNum);
         
         double val = 0.0;

         // check to see if there is a value for this cell

         int q = dgg.getAddress(*tloc)->quadNum();
         QuadVals& qv = qvals[q];
         if (qv.isUsed)
         {
            DgIVec2D coord = dgg.getAddress(*tloc)->coord() - qv.offset;
            if (coord.i() >= 0 && coord.j() >= 0 &&
                coord.i() <= qv.upperRight.i() && 
                coord.j() <= qv.upperRight.j()) 
              val = qv.vals[coord.i()][coord.j()].val;
         }
            
         // output the value

         if (dp.outSeqNum)
            *dp.outFile << sNum << dp.outputDelimiter << val << endl;
         else
         {
            outRF.convert(tloc);
            *dp.outFile << tloc->asString(dp.outputDelimiter)
                        << dp.outputDelimiter << val << endl;
         }

         delete tloc;
      }
   }
   else
   {
      for (int q = 0; q < 12; q++)
      {
         QuadVals& qv = qvals[q];
         if (!qv.isUsed) continue;

         for (int i = 0; i < qv.numI; i++)
         {
            for (int j = 0; j < qv.numJ; j++)
            {
               double val = qv.vals[i][j].val;
               if (val == 0.0) continue;

               DgIVec2D coord(qv.offset.i() + i, qv.offset.j() + j);
               
               DgLocation* tloc = dgg.makeLocation(DgQ2DICoord(q, coord));

               if (dp.outSeqNum)
               {
                  unsigned long int sNum = dgg.bndRF().seqNum(*tloc);
                  *dp.outFile << sNum << dp.outputDelimiter << val << endl;
               }
               else
               {
                  outRF.convert(tloc);
                  *dp.outFile << tloc->asString(dp.outputDelimiter)
                              << dp.outputDelimiter << val << endl;
               }

               delete tloc;
            }
         }
      }
   }
   
   ///// clean-up /////

   for (int q = 0; q < 12; q++)
   {
      QuadVals& qv = qvals[q];
      if (!qv.isUsed) continue;
      
      for (int i = 0; i < qv.numI; i++)
      {
         delete [] qv.vals[i];
      }

      delete [] qv.vals;
   }

} // void binValsPartial

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// values for a single cell

class ValStat {

   public:

      ValStat (void)
       : nVals (0), val(0.0), min(DBL_MAX), max(DBL_MIN), leaf(false),
         covered(true) { }

      int nVals;
      double val;
      double min;
      double max;
      bool leaf; // a "leaf" node
      bool covered; // a "covered" node
};

////////////////////////////////////////////////////////////////////////////////
void doBinVals (BinValsParam& dp, DgGridPList& plist)
{
   char tmpStr[100];
   sprintf(tmpStr, "%%lf%c%%lf%c%%lf", dp.inputDelimiter, dp.inputDelimiter);
   dp.inFormatStr = tmpStr;

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
      
      /////// open the output file as applicable //////

      dp.outFile = new ofstream();
      dp.outFile->open(dp.outFileName.c_str());
      dp.outFile->setf(ios::fixed, ios::floatfield);
      dp.outFile->precision(dp.precision);

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

      if (dp.wholeEarth) binValsGlobal(dp);
      else binValsPartial(dp);

      cout << "\n** binning complete **" << endl;

      // close the output files

      dp.outFile->close();
      delete dp.outFile;
   }

} // void doBinVals

////////////////////////////////////////////////////////////////////////////////
