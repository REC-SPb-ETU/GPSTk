//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  Copyright 2004-2019, The University of Texas at Austin
//
//==============================================================================

//==============================================================================
//
//  This software developed by Applied Research Laboratories at the University of
//  Texas at Austin, under contract to an agency or agencies within the U.S. 
//  Department of Defense. The U.S. Government retains all rights to use,
//  duplicate, distribute, disclose, or release this software. 
//
//  Pursuant to DoD Directive 523024 
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public 
//                            release, distribution is unlimited.
//
//==============================================================================

#include "Rinex3ObsBase.hpp"
#include "Rinex3ObsData.hpp"
#include "Rinex3ObsStream.hpp"
#include "Rinex3ObsHeader.hpp"
#include "Rinex3ObsFilterOperators.hpp"

#include "build_config.h"

#include "TestUtil.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace gpstk;

//============================================================
// Class decalarations
//============================================================

class Rinex3Obs_T
{
public:

      // constructor
   Rinex3Obs_T()
   {
      init();
   }

   void init( void );

   void toRinex3( void );

      // return values indicate number of failures, i.e., 0=PASS, !0=FAIL
   int headerExceptionTest( void );
   int hardCodeTest( void );
   int filterOperatorsTest( void );
   int dataExceptionsTest( void );
      /// round-trip test for RINEX 3, read, write, compare.
   int roundTripTest( void );

   void toConversionTest( void );
   int version2ToVersion3Test( void );
   int version3ToVersion2Test( void );

   int embeddedHeadersTest();

private:

   string dataFilePath;
   string tempFilePath;
   string file_sep;

   string dataRinexObsFile;
   string dataIncompleteHeader;
   string dataInvalidLineLength;
   string dataInvalidNumPRNWaveFact;
   string dataNotObs;
   string dataSystemGeosync;
   string dataSystemGlonass;
   string dataSystemMixed;
   string dataSystemTransit;
   string dataUnSupVersion ;
   string dataRinexContData;
   string dataHeaderTest;

   string dataBadEpochLine;
   string dataBadEpochFlag;
   string dataBadLineSize;
   string dataInvalidTimeFormat;

   string dataFilterTest1;
   string dataFilterTest2;
   string dataFilterTest3;
   string dataFilterTest4;

   string dataTestOutput;
   string dataTestOutput2;
   string dataTestOutput3;
   string dataTestOutput4;
   string dataTestOutputObsDump;
   string dataTestOutputDataException;
   string dataTestFilterOutput;

   string dataInputRinex3ObsFile;
   string dataInputRinex2ObsFile;
   string dataOutputRinex3ObsFile;
   string dataOutputRinex2ObsFile;

   string testMesg;
};

//============================================================
// Initialize Test Data Filenames
//============================================================

void Rinex3Obs_T :: init( void )
{

   cout << "Running tests for Rinex version 2.x" << endl;

   TestUtil test0;
   dataFilePath = gpstk::getPathData();
   tempFilePath = gpstk::getPathTestTemp();

      //----------------------------------------
      // Full file paths
      //----------------------------------------
   file_sep = getFileSep();

   dataRinexObsFile            = dataFilePath + file_sep +
      "test_input_rinex2_obs_RinexObsFile.06o";
   dataIncompleteHeader        = dataFilePath + file_sep +
      "test_input_rinex2_obs_IncompleteHeader.06o";
   dataInvalidLineLength       = dataFilePath + file_sep +
      "test_input_rinex2_obs_InvalidLineLength.06o";
   dataInvalidNumPRNWaveFact   = dataFilePath + file_sep +
      "test_input_rinex2_obs_InvalidNumPRNWaveFact.06o";
   dataNotObs                  = dataFilePath + file_sep +
      "test_input_rinex2_obs_NotObs.06o";
   dataSystemGeosync           = dataFilePath + file_sep +
      "test_input_rinex2_obs_SystemGeosync.06o";
   dataSystemGlonass           = dataFilePath + file_sep +
      "test_input_rinex2_obs_SystemGlonass.06o";
   dataSystemMixed             = dataFilePath + file_sep +
      "test_input_rinex2_obs_SystemMixed.06o";
   dataSystemTransit           = dataFilePath + file_sep +
      "test_input_rinex2_obs_SystemTransit.06o";
   dataUnSupVersion            = dataFilePath + file_sep +
      "test_input_rinex2_obs_UnSupVersion.06o";
   dataRinexContData           = dataFilePath + file_sep +
      "test_input_rinex2_obs_RinexContData.06o";   // not in v3 test
   dataHeaderTest              = dataFilePath + file_sep +
      "mixed211.05o";

   dataBadEpochLine            = dataFilePath + file_sep +
      "test_input_rinex2_obs_BadEpochLine.06o";
   dataBadEpochFlag            = dataFilePath + file_sep +
      "test_input_rinex2_obs_BadEpochFlag.06o";
   dataBadLineSize             = dataFilePath + file_sep +
      "test_input_rinex2_obs_BadLineSize.06o";
   dataInvalidTimeFormat       = dataFilePath + file_sep +
      "test_input_rinex2_obs_InvalidTimeFormat.06o";

   dataFilterTest1             = dataFilePath + file_sep +
      "test_input_rinex2_obs_FilterTest1.06o";
   dataFilterTest2             = dataFilePath + file_sep +
      "test_input_rinex2_obs_FilterTest2.06o";
   dataFilterTest3             = dataFilePath + file_sep +
      "test_input_rinex2_obs_FilterTest3.06o";
   dataFilterTest4             = dataFilePath + file_sep +
      "test_input_rinex2_obs_FilterTest4.06o";

   dataTestOutput              = tempFilePath + file_sep +
      "test_output_rinex2_obs_TestOutput.06o";
   dataTestOutput2             = tempFilePath + file_sep +
      "test_output_rinex2_obs_TestOutput2.06o";
   dataTestOutput3             = tempFilePath + file_sep +
      "test_output_rinex2_obs_TestOutput3.06o";
   dataTestOutput4             = tempFilePath + file_sep +
      "test_output_rinex2_obs_TestOutput4.06o";
   dataTestOutputObsDump       = tempFilePath + file_sep +
      "test_output_rinex2_obs_ObsDump.06o";
   dataTestOutputDataException = tempFilePath + file_sep +
      "test_output_rinex2_obs_DataExceptionOutput.06o";
   dataTestFilterOutput        = tempFilePath + file_sep +
      "test_output_rinex2_obs_FilterOutput.txt";

}

//============================================================
// Change input and output file names for Rinex v.3 types
//============================================================

void Rinex3Obs_T :: toRinex3(void)
{

   cout << "Running tests for Rinex version 3.x" << endl;

      //----------------------------------------
      // Full file paths
      //----------------------------------------

   dataRinexObsFile            = dataFilePath + file_sep +
      "test_input_rinex3_obs_RinexObsFile.15o";
   dataIncompleteHeader        = dataFilePath + file_sep +
      "test_input_rinex3_obs_IncompleteHeader.15o";
   dataInvalidLineLength       = dataFilePath + file_sep +
      "test_input_rinex3_obs_InvalidLineLength.15o";
   dataInvalidNumPRNWaveFact   = dataFilePath + file_sep +
      "test_input_rinex3_obs_InvalidNumPRNWaveFact.15o";
   dataNotObs                  = dataFilePath + file_sep +
      "test_input_rinex3_obs_NotObs.15o";
   dataSystemGeosync           = dataFilePath + file_sep +
      "test_input_rinex3_obs_SystemGeosync.15o";
   dataSystemGlonass           = dataFilePath + file_sep +
      "test_input_rinex3_obs_SystemGlonass.15o";
   dataSystemMixed             = dataFilePath + file_sep +
      "test_input_rinex3_obs_SystemMixed.15o";
   dataSystemTransit           = dataFilePath + file_sep +
      "test_input_rinex3_obs_SystemTransit.15o";
   dataUnSupVersion            = dataFilePath + file_sep +
      "test_input_rinex3_obs_UnSupVersion.15o";
   dataRinexContData           = dataFilePath + file_sep +
      "test_input_rinex3_obs_RinexContData.15o";

   dataBadEpochLine            = dataFilePath + file_sep +
      "test_input_rinex3_obs_BadEpochLine.15o";
   dataBadEpochFlag            = dataFilePath + file_sep +
      "test_input_rinex3_obs_BadEpochFlag.15o";
   dataBadLineSize             = dataFilePath + file_sep +
      "test_input_rinex3_obs_BadLineSize.15o";
   dataInvalidTimeFormat       = dataFilePath + file_sep +
      "test_input_rinex3_obs_InvalidTimeFormat.15o";

   dataFilterTest1             = dataFilePath + file_sep +
      "test_input_rinex3_obs_FilterTest1.15o";
   dataFilterTest2             = dataFilePath + file_sep +
      "test_input_rinex3_obs_FilterTest2.15o";
   dataFilterTest3             = dataFilePath + file_sep +
      "test_input_rinex3_obs_FilterTest3.15o";
   dataFilterTest4             = dataFilePath + file_sep +
      "test_input_rinex3_obs_FilterTest4.15o";

   dataTestOutput              = tempFilePath + file_sep +
      "test_output_rinex3_obs_TestOutput.15o";
   dataTestOutput2             = tempFilePath + file_sep +
      "test_output_rinex3_obs_TestOutput2.15o";
   dataTestOutput3             = tempFilePath + file_sep +
      "test_output_rinex3_obs_TestOutput3.15o";
   dataTestOutput4             = tempFilePath + file_sep +
      "test_output_rinex3_obs_TestOutput4.15o";
   dataTestOutputObsDump       = tempFilePath + file_sep +
      "test_output_rinex3_obs_ObsDump.15o";
   dataTestOutputDataException = tempFilePath + file_sep +
      "test_output_rinex3_obs_DataExceptionOutput.15o";
   dataTestFilterOutput        = tempFilePath + file_sep +
      "test_output_rinex3_obs_FilterOutput.txt";

}

//=============================================================
// Change input and output file names for Rinex Conversion test
//=============================================================

void Rinex3Obs_T :: toConversionTest( void )
{
   dataInputRinex3ObsFile  = dataFilePath + file_sep +
      "test_input_rinex3_obs_RinexObsFile.15o";
   dataInputRinex2ObsFile  = dataFilePath + file_sep +
      "test_input_rinex2_obs_RinexObsFile.06o";

   dataOutputRinex3ObsFile = tempFilePath + file_sep +
      "test_output_rinex3_obs_Rinex2to3Output.06o";
   dataOutputRinex2ObsFile = tempFilePath + file_sep +
      "test_output_rinex2_obs_Rinex3to2Output.15o";
}

//============================================================
// Test Method Definitions
//============================================================

   /* What the hell are we doing here?
    *
    * 1) Doing permissive reads of error-ridden RINEX OBS headers,
    *    i.e. allowing the headers to be read into memory despite errors.
    *    This is done by leaving the default behavior of streams that no
    *    exceptions are thrown on error conditions.
    *
    * 2) Doing strict writes of same error-ridden headers and verifying
    *    that exceptions are being thrown as expected.
    */
int Rinex3Obs_T :: headerExceptionTest( void )
{
   TUDEF( "Rinex3ObsStream", "headerExceptionTest" );

   string msg_test_desc  = "Rinex3ObsStream, headerExceptionTest";
   string msg_false_pass = ", threw the wrong number of exceptions.";
   string msg_fail       = ", threw an unexpected exception.";

   try
   {

      gpstk::Rinex3ObsStream rinex3ObsFile( dataRinexObsFile );
      gpstk::Rinex3ObsStream ih( dataIncompleteHeader );
      gpstk::Rinex3ObsStream il( dataInvalidLineLength );
      gpstk::Rinex3ObsStream inpwf( dataInvalidNumPRNWaveFact );
      gpstk::Rinex3ObsStream no( dataNotObs );
      gpstk::Rinex3ObsStream ss( dataSystemGeosync );
      gpstk::Rinex3ObsStream sr( dataSystemGlonass );
      gpstk::Rinex3ObsStream sm( dataSystemMixed );
      gpstk::Rinex3ObsStream st( dataSystemTransit );
      gpstk::Rinex3ObsStream unsupv( dataUnSupVersion );
      gpstk::Rinex3ObsStream contdata( dataRinexContData );

      gpstk::Rinex3ObsStream out( dataTestOutput, ios::out );
      gpstk::Rinex3ObsStream out2( dataTestOutput3, ios::out );
      gpstk::Rinex3ObsStream dump( dataTestOutputObsDump, ios::out );

      gpstk::Rinex3ObsHeader rinex3ObsHeader;
      gpstk::Rinex3ObsHeader ihh;
      gpstk::Rinex3ObsHeader ilh;
      gpstk::Rinex3ObsHeader inpwfh;
      gpstk::Rinex3ObsHeader noh;
      gpstk::Rinex3ObsHeader ssh;
      gpstk::Rinex3ObsHeader srh;
      gpstk::Rinex3ObsHeader smh;
      gpstk::Rinex3ObsHeader sth;
      gpstk::Rinex3ObsHeader unsupvh;
      gpstk::Rinex3ObsHeader contdatah;

      gpstk::Rinex3ObsData rinex3ObsData;
      gpstk::Rinex3ObsData contdatad;

         // read in some good headers and some crap ones
      rinex3ObsFile >> rinex3ObsHeader;
      ih >> ihh;
      il >> ilh;
      inpwf >> inpwfh;
      no >> noh;
      ss >> ssh;
      sr >> srh;
      sm >> smh;
      unsupv >> unsupvh;
      contdata >> contdatah; // not in v3 test

      out.exceptions( fstream::failbit );
         // write good and bad headers, checking for exceptions
      try
      {
         out << rinex3ObsHeader;
         TUPASS("exception");
      }
      catch (...)
      {
         TUFAIL("Exception while writing valid RINEX OBS header");
      }
      out.clear();
      try
      {
         out << ihh;
         TUFAIL("No Exception while writing invalid RINEX OBS header");
      }
      catch (...)
      {
         TUPASS("exception");
      }
      out.clear();
      try
      {
         out << ilh;
         TUFAIL("No Exception while writing invalid RINEX OBS header");
      }
      catch (...)
      {
         TUPASS("exception");
      }
      out.clear();
      try
      {
         out << inpwfh;
         TUFAIL("No Exception while writing invalid RINEX OBS header");
      }
      catch (...)
      {
         TUPASS("exception");
      }
      out.clear();
      try
      {
         out << noh;
         TUFAIL("No Exception while writing invalid RINEX OBS header");
      }
      catch (...)
      {
         TUPASS("exception");
      }
      out.clear();
      try
      {
         out << ssh;
         TUPASS("exception");
      }
      catch (...)
      {
         TUFAIL("Exception while writing valid RINEX OBS header");
      }
      out.clear();
      try
      {
         out << srh;
         TUPASS("exception");
      }
      catch (...)
      {
         TUFAIL("Exception while writing valid RINEX OBS header");
      }
      out.clear();
      try
      {
         out << smh;
         TUPASS("exception");
      }
      catch (...)
      {
         TUFAIL("Exception while writing valid RINEX OBS header");
      }
      out.clear();
      try
      {
         out << unsupvh;
         TUFAIL("No Exception while writing invalid RINEX OBS header");
      }
      catch (...)
      {
         TUPASS("exception");
      }
      out.clear();
      try
      {
         out2 << contdatah;  // not in v3 test
         TUPASS("exception");
      }
      catch (...)
      {
         TUFAIL("Exception while writing valid RINEX OBS header");
      }
      out.clear();
      rinex3ObsFile >> rinex3ObsData;
      rinex3ObsData.dump( dump );

      while( rinex3ObsFile >> rinex3ObsData )
      {
         out << rinex3ObsData; // Outputting v.3 data instead of v.2
      }

      while( contdata >> contdatad )  // not in v3 test
      {
         out2 << contdatad;
      }
      TUPASS(msg_test_desc);
   }
   catch(gpstk::Exception e)
   {
      TUFAIL( msg_test_desc + msg_fail + e.what() );
   }
   catch(...)
   {
      TUFAIL( msg_test_desc + msg_fail );
   }

   return( testFramework.countFails() );
}


//------------------------------------------------------------
// This test checks to make sure that the output
// from a read in rinex3ObsFile matches the input.
//------------------------------------------------------------
int Rinex3Obs_T :: hardCodeTest( void )
{

   bool files_equal = false;
   double CompareVersion;
   string CompareFileProgram;
   string CompareFileAgency;
   string CompareDate;

      // Previous comments indicated that these Rinex methods
      // are not expected to match in the top two lines of the file
   int num_lines_skip = 2;

   TestUtil test2( "Rinex3ObsStream", "hardCodeTest", __FILE__, __LINE__ );

   string msg_test_desc   =
      "Rinex3ObsStream, read write test, comparing input file and output file";
   string msg_fail_equal  = ", input and output do not match.";
   string msg_fail_except = ", threw an unexpected exception.";

   try
   {
      gpstk::Rinex3ObsStream rinex3ObsFile( dataRinexObsFile );
      gpstk::Rinex3ObsStream out( dataTestOutput2, ios::out );
      gpstk::Rinex3ObsStream dump( dataTestOutputObsDump, ios::out );
      gpstk::Rinex3ObsHeader rinex3ObsHeader;
      gpstk::Rinex3ObsData rinex3ObsData;

      rinex3ObsFile >> rinex3ObsHeader;
      out << rinex3ObsHeader;

      while( rinex3ObsFile >> rinex3ObsData )
      {
         out << rinex3ObsData;
            // cout<<out.header.version<<endl; stream has header info passed to it
      }

      if (rinex3ObsHeader.version == 2.1)
      {
         CompareVersion = 2.10;
         CompareFileProgram = (string)"row";
         CompareFileAgency = (string)"Dataflow Processing";
         CompareDate = (string)"04/11/2006 23:59:18";
      }

      else if (rinex3ObsHeader.version == 3.02)
      {
         CompareVersion = 3.02;
         CompareFileProgram = (string)"cnvtToRINEX 2.25.0";
         CompareFileAgency = (string)"convertToRINEX OPR";
         CompareDate = (string)"23-Jan-15 22:34 UTC";
      }

      test2.assert( rinex3ObsHeader.version == CompareVersion,
                    "RinexObs Header version comparison",      __LINE__ );
      test2.assert( rinex3ObsHeader.fileProgram == CompareFileProgram,
                    "RinexObs Header file program comparison", __LINE__ );
      test2.assert( rinex3ObsHeader.fileAgency == CompareFileAgency,
                    "RinexObs Header file agency comparison",  __LINE__ );
      test2.assert( rinex3ObsHeader.date == CompareDate,
                    "RinexObs Header date comparison",         __LINE__ );

      rinex3ObsData.dump( dump );
      rinex3ObsHeader.dump( dump );

      test2.assert_files_equal(__LINE__, dataRinexObsFile, dataTestOutput2,
                               msg_test_desc + msg_fail_equal,
                               num_lines_skip, false, true );
   }
   catch(...)
   {
      test2.assert( false, msg_test_desc + msg_fail_except, __LINE__ );
   }

   return( test2.countFails() );
}

//------------------------------------------------------------
// This test throws many GPSTK exceptions within the
// Rinex3ObsData, including BadEpochLine and BadEpochFlag
//------------------------------------------------------------
int Rinex3Obs_T :: dataExceptionsTest( void )
{

   TestUtil test3( "Rinex3ObsStream", "dataExceptionsTest", __FILE__, __LINE__ );

   string msg_test_desc   =
      "Rinex3ObsStream, test various gpstk exception throws, including BadEpochLine and BadEpochFlag";
   string msg_fail_throw  =
      ", not all gpstk exceptions were thrown as expected.";
   string msg_fail_except = ", threw an unexpected exception.";

   try
   {
      gpstk::Rinex3ObsStream BadEpochLine( dataBadEpochLine );
      gpstk::Rinex3ObsStream BadEpochFlag( dataBadEpochFlag );
      gpstk::Rinex3ObsStream BadLineSize( dataBadLineSize );
      gpstk::Rinex3ObsStream InvalidTimeFormat( dataInvalidTimeFormat );
      gpstk::Rinex3ObsStream out( dataTestOutputDataException, ios::out );
      gpstk::Rinex3ObsData BadEpochLined;
      gpstk::Rinex3ObsData BadEpochFlagd;
      gpstk::Rinex3ObsData BadLineSized;
      gpstk::Rinex3ObsData InvalidTimeFormatd;

      while( BadEpochLine >> BadEpochLined )
      {
         out << BadEpochLined;
      }
      while( BadEpochFlag >> BadEpochFlagd )
      {
         out << BadEpochFlagd;
      }
      while( BadLineSize >> BadLineSized )
      {
         out << BadLineSized;
      }
      while( InvalidTimeFormat >> InvalidTimeFormatd )
      {
         out << InvalidTimeFormatd;
      }
      test3.assert( true, msg_test_desc + msg_fail_throw, __LINE__ );
   }
   catch(...)
   {
      test3.assert( false, msg_test_desc + msg_fail_except, __LINE__ );
   }

   return( test3.countFails() );

}

//------------------------------------------------------------
// This is the test for several of the members within Rinex3ObsFilterOperators
// including merge, LessThanSimple, EqualsSimple
//------------------------------------------------------------
int Rinex3Obs_T :: filterOperatorsTest( void )
{
   TUDEF( "Rinex3Obs", "filterOperatorsTest");
   try
   {
      fstream out( dataTestFilterOutput.c_str(), ios::out );
   
      gpstk::Rinex3ObsStream s1(dataFilterTest1);
      gpstk::Rinex3ObsHeader h1;
      gpstk::Rinex3ObsData d1;
      s1 >> h1;
      while( s1 >> d1)
         d1.dump(out);
   
      out << "Reading dataFilterTest2:" << endl;
      gpstk::Rinex3ObsStream s2(dataFilterTest2);   
      gpstk::Rinex3ObsHeader h2;
      gpstk::Rinex3ObsData d2;
      s2 >> h2;
      while( s2 >> d2)
         d2.dump(out);

      gpstk::Rinex3ObsDataOperatorEqualsSimple EqualsSimple;
      testFramework.changeSourceMethod("Rinex3ObsDataOperatorEqualsSimple");
      testFramework.assert( EqualsSimple( d1, d1 ), "", __LINE__);
      
      gpstk::Rinex3ObsDataOperatorLessThanSimple LessThanSimple;
      testFramework.changeSourceMethod("Rinex3ObsDataOperatorLessThanSimple");
      testFramework.assert( !LessThanSimple( d1, d1 ), "", __LINE__ );

      gpstk::Rinex3ObsHeaderTouchHeaderMerge merged;
      merged( h1 );
      merged( h2 );
      out << "Merged Header:" << endl;
      merged.theHeader.dump(out);
      gpstk::Rinex3ObsDataOperatorLessThanFull LessThanFull(merged.theHeader.mapObsTypes);
      testFramework.changeSourceMethod("Rinex3ObsDataOperatorLessThanFull");
      testFramework.assert( !LessThanFull( d1, h1, d1, h1, 5 ) , "",  __LINE__ );
      testFramework.assert( !LessThanFull( d1, h1, d2, h2, 5 ) , "", __LINE__ );
   }
   catch (gpstk::Exception& e)
   {
      cout << e << endl;
      testFramework.assert( false , "caught exception", __LINE__ );
   }
   return testFramework.countFails();     
}


//------------------------------------------------------------
// Tests if a input Rinex 3 file can be output as a version 2 file
//------------------------------------------------------------

int Rinex3Obs_T :: version3ToVersion2Test( void )
{
   TUDEF("Rinex3Obs", "version3ToVersion2Test");

   gpstk::Rinex3ObsStream inputStream(dataInputRinex3ObsFile.c_str());
   gpstk::Rinex3ObsStream outputStream(dataOutputRinex2ObsFile.c_str(), ios::out);
   gpstk::Rinex3ObsHeader ObsHeader;
   gpstk::Rinex3ObsData ObsData;

   inputStream >> ObsHeader;

   ObsHeader.prepareVer2Write();

   outputStream << ObsHeader;
   while(inputStream >> ObsData)
   {
      outputStream << ObsData;
   }

   testMesg = "No automatic comparison implemented. If " +
      dataOutputRinex2ObsFile + " is not the proper conversion of " +
      dataInputRinex3ObsFile + "test has failed. Currently prepareVer2Write() " +
      "function is BROKEN!";

   testFramework.assert(false, testMesg, __LINE__);

   return testFramework.countFails();
}

//------------------------------------------------------------
// Tests if a input Rinex 2 file can be output as a version 3 file
//------------------------------------------------------------

int Rinex3Obs_T :: version2ToVersion3Test( void )
{
   TestUtil testFramework("Rinex3Obs", "version2ToVersion3Test", __FILE__, __LINE__ );

   gpstk::Rinex3ObsStream inputStream(dataInputRinex2ObsFile.c_str());
   gpstk::Rinex3ObsStream outputStream(dataOutputRinex3ObsFile.c_str(),
                                       ios::out);
   gpstk::Rinex3ObsHeader ObsHeader;
   gpstk::Rinex3ObsData ObsData;

   inputStream >> ObsHeader;

   ObsHeader.version =
      3.02; //No prepareVersion3Write function, only way to change version number

   outputStream << ObsHeader;
   while(inputStream >> ObsData)
   {
      outputStream << ObsData;
   }

   testMesg = "No automatic comparison implemented. If " +
      dataOutputRinex2ObsFile + " is not the proper conversion of " +
      dataInputRinex3ObsFile + "test has failed. Currently only conversion " +
      "from v.2.11 to v.3.02 is to change version number by hand. This "
      + "doesn't produce a valid header, so this functionality is MISSING!";

   testFramework.assert(false, testMesg, __LINE__);

   return testFramework.countFails();
}

int Rinex3Obs_T::roundTripTest( void )
{
   TUDEF("Rinex3Obs", "roundTripTest");

   try
   {
      gpstk::Rinex3ObsStream infile( dataRinexObsFile );
      gpstk::Rinex3ObsStream outfile( dataTestOutput4, ios::out );
      gpstk::Rinex3ObsHeader roh;
      gpstk::Rinex3ObsData rod;

      infile >> roh;
      roh.preserveDate = true;
      roh.preserveVerType = true;
      outfile << roh;
      while (infile >> rod)
      {
         outfile << rod;
      }
      infile.close();
      outfile.close();
      string failMsg = "input and output do not match: " +
         dataRinexObsFile + " " + dataTestOutput4;
      testFramework.assert_files_equal(
         __LINE__, dataRinexObsFile, dataTestOutput4,
         failMsg, 0, false, true );
   }
   catch (...)
   {
      TUFAIL("exception thrown during processing");
   }

   return testFramework.countFails();
}


int Rinex3Obs_T ::
embeddedHeadersTest()
{
   TUDEF("Rinex3ObsData", "operator<<");
   try
   {
      cerr << "opening " << dataHeaderTest << endl;
      gpstk::Rinex3ObsStream ros(dataHeaderTest, std::ios::in);
      gpstk::Rinex3ObsData rod;
      gpstk::TimeSystem ts = gpstk::TimeSystem::GPS;
      TUASSERTE(bool, true, ros.good());
      ros >> ros.header;
      TUASSERTE(bool, true, ros.good());
         // make sure we read all of the header info, nothing more, nothing less
      TUASSERTE(unsigned long,
                gpstk::Rinex3ObsHeader::validVersion |
                gpstk::Rinex3ObsHeader::validComment |
                gpstk::Rinex3ObsHeader::validRunBy |
                gpstk::Rinex3ObsHeader::validMarkerName |
                gpstk::Rinex3ObsHeader::validMarkerNumber |
                gpstk::Rinex3ObsHeader::validObserver |
                gpstk::Rinex3ObsHeader::validReceiver |
                gpstk::Rinex3ObsHeader::validAntennaType |
                gpstk::Rinex3ObsHeader::validAntennaPosition |
                gpstk::Rinex3ObsHeader::validAntennaDeltaHEN |
                gpstk::Rinex3ObsHeader::validWaveFact |
                gpstk::Rinex3ObsHeader::validReceiverOffset |
                gpstk::Rinex3ObsHeader::validNumObs |
                gpstk::Rinex3ObsHeader::validInterval |
                gpstk::Rinex3ObsHeader::validFirstTime,
                ros.header.valid);
         // Go through each record in the source file (there aren't
         // many) and verify that the contents are reasonable,
         // i.e. comments associated with header record epoch flags
         // and correct times and so on.
         /*
 05  3 24 13 10 36.0000000  0  4G12G09G06E11                         -.123456789
  23629347.915            .300 8         -.353    23629364.158
  20891534.648           -.120 9         -.358    20891541.292
  20607600.189           -.430 9          .394    20607605.848
                          .324 8                                          .178 7
         */
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 0, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,10,36,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 4, rod.numSVs);
      TUASSERTE(size_t, 4, rod.obs.size());
      TUASSERTE(unsigned long, 0, rod.auxHeader.valid);

/*
 05  3 24 13 10 50.0000000  4  4
     1     2     2   G 9   G12                              WAVELENGTH FACT L1/2
  *** WAVELENGTH FACTOR CHANGED FOR 2 SATELLITES ***        COMMENT
      NOW 8 SATELLITES HAVE WL FACT 1 AND 2!                COMMENT
                                                            COMMENT
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 4, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,10,50,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 4, rod.numSVs);
      TUASSERTE(size_t, 0, rod.obs.size());
      TUASSERTE(unsigned long,
                gpstk::Rinex3ObsHeader::validWaveFact |
                gpstk::Rinex3ObsHeader::validComment,
                rod.auxHeader.valid);
      TUASSERTE(size_t, 3, rod.auxHeader.commentList.size());

/*
 05  3 24 13 10 54.0000000  0  6G12G09G06R21R22E11                   -.123456789
  23619095.450      -53875.632 8    -41981.375    23619112.008
  20886075.667      -28688.027 9    -22354.535    20886082.101
  20611072.689       18247.789 9     14219.770    20611078.410
  21345678.576       12345.567 5
  22123456.789       23456.789 5
                     65432.123 5                                     48861.586 7
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 0, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,10,54,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 6, rod.numSVs);
      TUASSERTE(size_t, 6, rod.obs.size());
      TUASSERTE(unsigned long, 0, rod.auxHeader.valid);

/*
 05  3 24 13 11  0.0000000  2  1
            *** FROM NOW ON KINEMATIC DATA! ***             COMMENT
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 2, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,11,0,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 1, rod.numSVs);
      TUASSERTE(size_t, 0, rod.obs.size());
      TUASSERTE(unsigned long,
                gpstk::Rinex3ObsHeader::validComment,
                rod.auxHeader.valid);
      TUASSERTE(size_t, 1, rod.auxHeader.commentList.size());

/*
 05  3 24 13 11 48.0000000  0  4G16G12G09G06                         -.123456789
  21110991.756       16119.980 7     12560.510    21110998.441
  23588424.398     -215050.557 6   -167571.734    23588439.570
  20869878.790     -113803.187 8    -88677.926    20869884.938
  20621643.727       73797.462 7     57505.177    20621649.276
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 0, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,11,48,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 4, rod.numSVs);
      TUASSERTE(size_t, 4, rod.obs.size());
      TUASSERTE(unsigned long, 0, rod.auxHeader.valid);

/*
                            3  4
A 9080                                                      MARKER NAME
9080.1.34                                                   MARKER NUMBER
         .9030         .0000         .0000                  ANTENNA: DELTA H/E/N
          --> THIS IS THE START OF A NEW SITE <--           COMMENT
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 3, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,11,48,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 4, rod.numSVs);
      TUASSERTE(size_t, 0, rod.obs.size());
      TUASSERTE(unsigned long,
                gpstk::Rinex3ObsHeader::validMarkerName |
                gpstk::Rinex3ObsHeader::validMarkerNumber |
                gpstk::Rinex3ObsHeader::validAntennaDeltaHEN |
                gpstk::Rinex3ObsHeader::validComment,
                rod.auxHeader.valid);
      TUASSERTE(size_t, 1, rod.auxHeader.commentList.size());

/*
 05  3 24 13 12  6.0000000  0  4G16G12G06G09                         -.123456987
  21112589.384       24515.877 6     19102.763 3  21112596.187
  23578228.338     -268624.234 7   -209317.284 4  23578244.398
  20625218.088       92581.207 7     72141.846 4  20625223.795
  20864539.693     -141858.836 8   -110539.435 5  20864545.943
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 0, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,12,6,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 4, rod.numSVs);
      TUASSERTE(size_t, 4, rod.obs.size());
      TUASSERTE(unsigned long, 0, rod.auxHeader.valid);

/*
 05  3 24 13 13  1.2345678  5  0
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 5, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,13,1.2345678,
                                 ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 0, rod.numSVs);
      TUASSERTE(size_t, 0, rod.obs.size());
      TUASSERTE(unsigned long, 0, rod.auxHeader.valid);

/*
                            4  1
        (AN EVENT FLAG WITH SIGNIFICANT EPOCH)              COMMENT
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 4, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,13,1.2345678,
                                 ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 1, rod.numSVs);
      TUASSERTE(size_t, 0, rod.obs.size());
      TUASSERTE(unsigned long,
                gpstk::Rinex3ObsHeader::validComment,
                rod.auxHeader.valid);
      TUASSERTE(size_t, 1, rod.auxHeader.commentList.size());

/*
 05  3 24 13 14 12.0000000  0  4G16G12G09G06                         -.123456012
  21124965.133       89551.30216     69779.62654  21124972.2754
  23507272.372     -212616.150 7   -165674.789 5  23507288.421
  20828010.354     -333820.093 6   -260119.395 5  20828017.129
  20650944.902      227775.130 7    177487.651 4  20650950.363
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 0, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,14,12,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 4, rod.numSVs);
      TUASSERTE(size_t, 4, rod.obs.size());
      TUASSERTE(unsigned long, 0, rod.auxHeader.valid);

/*
                            4  1
           *** ANTISPOOFING ON G 16 AND LOST LOCK           COMMENT
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 4, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,14,12,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 1, rod.numSVs);
      TUASSERTE(size_t, 0, rod.obs.size());
      TUASSERTE(unsigned long,
                gpstk::Rinex3ObsHeader::validComment,
                rod.auxHeader.valid);
      TUASSERTE(size_t, 1, rod.auxHeader.commentList.size());

/*
 05  3 24 13 14 12.0000000  6  2G16G09
                 123456789.0      -9876543.5
                         0.0            -0.5
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 6, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,14,12,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 2, rod.numSVs);
      TUASSERTE(size_t, 2, rod.obs.size());
      TUASSERTE(unsigned long, 0, rod.auxHeader.valid);

/*
                            4  2
           ---> CYCLE SLIPS THAT HAVE BEEN APPLIED TO       COMMENT
                THE OBSERVATIONS                            COMMENT
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 4, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,14,12,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 2, rod.numSVs);
      TUASSERTE(size_t, 0, rod.obs.size());
      TUASSERTE(unsigned long,
                gpstk::Rinex3ObsHeader::validComment,
                rod.auxHeader.valid);
      TUASSERTE(size_t, 2, rod.auxHeader.commentList.size());

/*
 05  3 24 13 14 48.0000000  0  4G16G12G09G06                         -.123456234
  21128884.159      110143.144 7     85825.18545  21128890.7764
  23487131.045     -318463.297 7   -248152.72824  23487146.149
  20817844.743     -387242.571 6   -301747.22925  20817851.322
  20658519.895      267583.67817    208507.26234  20658525.869
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 0, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,14,48,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 4, rod.numSVs);
      TUASSERTE(size_t, 4, rod.obs.size());
      TUASSERTE(unsigned long, 0, rod.auxHeader.valid);

/*
                            4  3
         ***   SATELLITE G 9   THIS EPOCH ON WLFACT 1 (L2)  COMMENT
         *** G 6 LOST LOCK AND THIS EPOCH ON WLFACT 2 (L2)  COMMENT
                (OPPOSITE TO PREVIOUS SETTINGS)             COMMENT
*/
      ros >> rod;
      TUASSERTE(bool, true, ros.good());
      TUASSERTE(short, 4, rod.epochFlag);
      TUASSERTE(gpstk::CommonTime,
                gpstk::CivilTime(2005,3,24,13,14,48,ts).convertToCommonTime(),
                rod.time);
      TUASSERTE(short, 3, rod.numSVs);
      TUASSERTE(size_t, 0, rod.obs.size());
      TUASSERTE(unsigned long,
                gpstk::Rinex3ObsHeader::validComment,
                rod.auxHeader.valid);
      TUASSERTE(size_t, 3, rod.auxHeader.commentList.size());
   }
   catch (...)
   {
      TUFAIL("unexpected exception");
   }
   TURETURN();
}

int main()
{
   int errorTotal = 0;
   Rinex3Obs_T testClass;

   errorTotal += testClass.headerExceptionTest();
   errorTotal += testClass.hardCodeTest();
   errorTotal += testClass.dataExceptionsTest();
   errorTotal += testClass.filterOperatorsTest();
   errorTotal += testClass.roundTripTest();
   errorTotal += testClass.embeddedHeadersTest();

      //Change to test v.3
   testClass.toRinex3();

   errorTotal += testClass.headerExceptionTest();
   errorTotal += testClass.hardCodeTest();
   errorTotal += testClass.dataExceptionsTest();
   errorTotal += testClass.filterOperatorsTest();

   testClass.toConversionTest();
   errorTotal += testClass.roundTripTest();

   cout << "Total Failures for " << __FILE__ << ": " << errorTotal << endl;

   return( errorTotal );
}
