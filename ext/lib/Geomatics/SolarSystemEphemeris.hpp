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

/// @file SolarSystemEphemeris.hpp
/// Implement JPL solar system ephemeris handling, including reading and writing of
/// ASCII and binary files and the computation of position and velocity of the sun,
/// moon and the 9 planets, as well as nutation and lunar libration and their rate.
/// JPL ephemeris files may be obtained from ftp://ssd.jpl.nasa.gov/pub/eph/planets.
/// Generally you should download the ASCII files and use tools based on this code
/// to convert to your own binary files; this avoids compiler- and platform-dependent
/// differences in the binary files.

//------------------------------------------------------------------------------------
#ifndef SOLAR_SYSTEM_EPHEMERIS_INCLUDE
#define SOLAR_SYSTEM_EPHEMERIS_INCLUDE

//------------------------------------------------------------------------------------
// includes
// system
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
// GPSTk
#include "Exception.hpp"
#include "TimeConstants.hpp"

namespace gpstk {

//------------------------------------------------------------------------------------
/// Class SolarSystemEphemeris encapsulates the information in the JPL ephemeris file,
/// header and data, reading and writing of both ASCII and binary files, as well as
/// the computation of position and velocity of the sun, moon and planets, plus
/// nutations and lunar librations and their rates.
/// The user of this class should not have to read or write new files except either
/// when the class is initially installed on a platform, or when a new ephemeris is
/// obtained from JPL. Then procedure is first to download ASCII files for the
/// desired ephemeris from the JPL ftp site at ftp://ssd.jpl.nasa.gov/pub/eph/planets.
/// This consists of an ASCII header file (e.g. header.403) plus one or more
/// ephemeris data files for the same ephemeris (e.g. ascp1975.403, ascp2000.403 and
/// ascp2025.403 - these files contain the complete "DE403" ephemeris covering years
/// 1975 to 2025). The user should then use a conversion program (such as
/// convertEphemeris in the gpstk) to read these files and write out a single binary
/// file for use in applications. Writing the binary file on the platform on which it
/// is going to be used avoids potential problems with platform dependencies.
/// The gpstk also includes a test program, testEphemeris, which will read a test
/// file (also at the JPL ftp site) and compute several states, comparing them with
/// JPL-generated 'truth' values; this will validate the generated binary file.
/// To make use of this class and the generated binary file, the programmer simply
/// instantiates a SolarSystemEphemeris object, calls initializeWithBinaryFile(file)
/// once, passing it the name of the binary file, then calling
/// RelativeInertialPositionVelocity() any number of times, passing it the time and
/// Planet of interest.
/// Time for this class is always Barycentric Dynamic Time (TDB), always as MJD.
class SolarSystemEphemeris {
public:
   /// These are indexes used by the caller of InertialPositionVelocity().
   enum Planet {
      // the following are relative to the solar system barycenter, except MOON
      idNone=0,                ///<  0 place holder
      idMercury,               ///<  1 Mercury
      idVenus,                 ///<  2 Venus
      idEarth,                 ///<  3 Earth
      idMars,                  ///<  4 Mars
      idJupiter,               ///<  5 Jupiter
      idSaturn,                ///<  6 Saturn
      idUranus,                ///<  7 Uranus
      idNeptune,               ///<  8 Neptune
      idPluto,                 ///<  9 Pluto
      idMoon,                  ///< 10 Moon (Geocentric coordinates)
      idSun,                   ///< 11 Sun
      idSolarSystemBarycenter, ///< 12 Solar system barycenter
      idEarthMoonBarycenter,   ///< 13 Earth-moon barycenter
      idNutations,             ///< 14 Nutations (psi, epsilon and their rates)
      idLibrations             ///< 15 Lunar Librations (3 euler angles)
   };

   /// Constructor. Set EphemerisNumber to -1 to indicate that nothing has been
   /// read yet.
   SolarSystemEphemeris(void) throw() : EphemerisNumber(-1) {};

   //------------------------------------------------------------------
   // reading and writing ASCII (JPL) files

   /// Read the header from a JPL ASCII planetary ephemeris file. Note that this
   /// routine clears the 'store' map and defines the 'constants' hash. It also
   /// sets EphemerisNumber to the constant "DENUM" if successful.
   /// @param filename the name of the ASCII header file.
   /// @throw if the file cannot be opened.
   /// @throw if the header ends prematurely or if it is not properly formatted.
   /// @throw if any stream error occurs.
   void readASCIIheader(std::string filename) throw(Exception);

   /// Read one or more ASCII data files. Call only after having read the header,
   /// and call only with data files for the same ephemeris as the header (the JPL
   /// files are named 'header.NNN' and 'ascSYYYY.NNN' where NNN is the ephemeris
   /// number (appears inside the header file, but not inside the data files),
   /// S is either 'p' or 'n' as the year is positive or negative (AD or BC), and
   /// YYYY is the year of the first record in the file.
   /// @param filenames vector containting the names of the ASCII data files
   ///                  (downloaded from JPL), in any order.
   /// @return 0 ok, -1 if a stream error occurred.
   /// @throw if the header has not yet been read.
   /// @throw if any file could not be opened.
   /// @throw if any record in any file has a 'number of coefficients' that differs
   ///        from the header value.
   int readASCIIdata(std::vector<std::string>& filenames) throw(Exception);

   /// Read only one ASCII data file. Also see the documentation for the
   /// other version of this routine.
   /// @param filename name of an ASCII data files (downloaded from JPL)
   /// @return 0 ok, -1 if a stream error occurred, -4 header has not been read.
   /// @throw if the header has not yet been read.
   /// @throw if the file could not be opened.
   /// @throw if any record has a 'number of coefficients' that differs
   ///        from the header value.
   int readASCIIdata(std::string filename) throw(Exception);

   /// Write the header (ASCII) to an output stream
   /// @throw if any stream error occurs
   /// @return 0 success,
   ///        -4 header has not yet been read.
   int writeASCIIheader(std::ostream& os) throw(Exception);

   /// Write the stored data (ASCII) to an output stream
   /// NB. This routine does NOT clear the store - use clearStorage()
   /// @throw if any stream error occurs
   /// @return 0 success,
   ///        -4 header has not yet been read.
   int writeASCIIdata(std::ostream& os) throw(Exception);

   //------------------------------------------------------------------
   // reading and writing binary files

   /// Write the header and the stored data to a binary output file.
   /// NB. This routine does NOT clear the store - use clearStorage()
   /// @param filename  name of binary file to be read.
   /// @return 0 ok
   ///        -4 if data has not been read into the object
   /// @throw if any stream error occurs.
   int writeBinaryFile(std::string filename) throw(Exception);

   /// clear the store map containing all the data read by
   /// readASCIIdata() or readBinaryData(true).
   void clearStorage(void) throw() { store.clear(); }

   /// Read header and data from a binary file, storing ALL the data in store.
   /// For use with copying, merging or editing data files. Closes the stream before
   /// returning.
   /// @param filename  name of binary file to be read.
   /// @return 0 success,
   ///        -3 input stream is not open or not valid
   ///        -4 header has not yet been read.
   /// @throw if a gap in time is found between consecutive records.
   int readBinaryFile(std::string filename) throw(Exception);

   /// Open the given binary file, read the header and prepare for reading data
   /// records at random using seekToJD() and computing positions and velocities
   /// with InertialPositionVelocity(). Does not store the data.
   /// @param filename  name of binary file to be read.
   /// @return 0 success,
   ///        -3 input stream is not open or not valid
   ///        -4 header has not yet been read.
   /// @throw if a gap in time is found between consecutive records.
   int initializeWithBinaryFile(std::string filename) throw(Exception);

   //------------------------------------------------------------------
   // utilizing the ephemeris

   /// Compute inertial frame position and velocity of given 'target' body, relative
   /// to the 'center' body, at the given time. On successful return, PV contains
   /// position (in components 0-2) and velocity (components 3-5) (units: cf param km)
   /// for regular bodies; for nutations and librations the units are radians and
   /// radians/day; nutations (components 0-3 only) are longitude or psi (component 0)
   /// and obliquity or epsilon (component 1) and their rates (components 2,3);
   /// librations (components 0-5) are the 3 euler angles in radians and their
   /// rates in radians/day.
   /// @param  MJD   time (Modified Julian Date) of interest, in TDB system.
   /// @param target Body for which position and velocity are to be computed.
   /// @param center Body relative to which the results apply. However, center
   ///                  may == SolarSystemEphemeris::None, in which case the results
   ///                  are relative to the solar system barycenter.
   ///                  If target == Nutations or Librations, center is ignored.
   /// @param PV     Double array of length 6 containing output position and velocity
   ///                  components of target relative to center, in the order
   ///                  X,Y,Z,Vx,Vy,Vz. Units are determined by parameter km.
   ///                  If target == Nutations, PV contains 4 results, psi, eps,
   ///                  psi dot, eps dot in units radians and radians/day.
   ///                  If target == Librations, PV contains 3 euler angles in radians
   ///                  and their rates in radians/day.
   /// @param km     boolean: if true (default), units are km, km/day; else AU, AU/day
   ///                  (but not Nutations or Librations - see above).
   /// @throw if given time is before the first record in the file,
   /// the given time is after the last record, or in a gap between records,
   /// the input stream is not open or not valid, or EOF was found prematurely, or
   /// the ephemeris is not initialized; most likely the last two happen because
   /// initializeWithBinaryFile() has not been called, or reading failed.
   void RelativeInertialPositionVelocity(const double MJD,
                  Planet target, Planet center, double PV[6], bool kilometers = true)
      throw(Exception);

   /// Return the value of 1 AU (Astronomical Unit) in km. If the file header has not
   /// been read, return -1.0.
   /// @return the value of 1 AU in km;
   ///                return -1 if ephemeris has not been initialized.
   double AU(void) throw()
      { if(EphemerisNumber == -1) return -1.0; return constants["AU"]; }

   /// Return the ephemeris number.
   /// @return the 'DE' ephemeris number, e.g. 403,
   ///         or -1 if ephemeris has not been initialized.
   int EphNumber(void) const throw()
      { return EphemerisNumber; }

   /// @return the value of the contant with the given name. If the header
   /// has not been read, return -1. Return zero if the constant is not found.
   double getConstant(std::string name) throw() {
      if(EphemerisNumber == -1) return -1.0;
      if(constants.find(name) != constants.end()) return constants[name];
      return 0.0;
   }

   /// Return the Earth-to-Moon mass ratio
   double EarthToMoonMassRatio(void) throw()
      { return getConstant(std::string("EMRAT")); }

   /// Return the Sun-to-Earth mass ratio
   double SunToEarthMassRatio(void) throw() {
      double em=getConstant(std::string("EMRAT"));
      double gms=getConstant(std::string("GMS"));
      double gmb=getConstant(std::string("GMB"));
      return ( gms * ((1.0+em)/em) / gmb );
   }

   /// Return the MJD of start time of the data (system TDB)
   double startTimeMJD(void) const throw(Exception)
   {
      return (startJD - MJD_TO_JD);
   }

   /// Return the MJD of end time of the data (system TDB)
   double endTimeMJD(void) const throw(Exception)
   {
      return (endJD - MJD_TO_JD);
   }

   //------------------------------------------------------------------
   // private functions

private:
   /// Helper routine for binary writing.
   /// @throw if there is any stream error.
   void writeBinary(std::ofstream& strm, const char *ptr, size_t size)
      throw(Exception);

   /// Helper routine for binary reading.
   /// @throw if there is any error or if EOF is found.
   void readBinary(char *ptr, size_t size) throw(Exception);

   /// Read header from a binary file.
   /// @param filename  name of binary file, probably written by writeBinaryFile().
   /// @throw if read error or premature EOF if found.
   void readBinaryHeader(std::string filename) throw(Exception);

   /// Read data from a binary file, already opened by readBinaryHeader.
   /// Build the file position map, and store the first set of coefficients.
   /// If calling argument is true, save all the coefficient data in a map.
   /// @param save if true, save all the data in store, else clear the store.
   /// @return 0 success,
   ///        -3 input stream is not open or not valid
   ///        -4 header has not yet been read.
   /// @throw if a gap in time is found between consecutive records.
   int readBinaryData(bool save) throw(Exception);

   /// Read a single binary record (not a header record) at the current file
   /// position, into the given vector. For use by readBinaryData() and seekToJD().
   /// @param data_vector  vector<double> to hold coefficients.
   /// @return 0 success,
   ///        -2 EOF was reached
   ///        -3 input stream is not open or not valid
   int readBinaryRecord(std::vector<double>& data_vector) throw(Exception);

   /// Search the data records of the file opened by initializeWithBinaryFile() and
   /// read the one whose time limits include the given time. May be called only
   /// after initializeWithBinaryFile().
   /// @param JD the time (Julian Date) of interest
   /// @return 0 success, or
   ///        -1 given time is before the first record in the file,
   ///        -2 given time is after the last record, or in a gap between records,
   ///        -3 input stream is not open or not valid, or EOF was found prematurely,
   ///        -4 ephemeris (binary file) is not initialized
   /// -3 or -4 => initializeWithBinaryFile() has not been called, or reading failed.
   int seekToJD(double JD) throw(Exception);

   //------------------------------------------------------------------
   // define here for use in next function
   /// These are indexes used in the actual computation, and correspond to indexes
   /// in the ephemeris file; for example computation for the SUN is done using
   /// c_offset[SUN], c_ncoeff[SUN] and c_nsets[SUN].
   enum computeID {
      // the following are relative to the solar system barycenter, except MOON
      NONE=-1,        ///< -1 Place holder
      MERCURY,        ///<  0 Mercury
      VENUS,          ///<  1 Venus
      EMBARY,         ///<  2 Earth-Moon barycenter
      MARS,           ///<  3 Mars
      JUPITER,        ///<  4 Jupiter
      SATURN,         ///<  5 Saturn
      URANUS,         ///<  6 Uranus
      NEPTUNE,        ///<  7 Neptune
      PLUTO,          ///<  8 Pluto
      MOON,           ///<  9 Moon (Geocentric coordinates)
      SUN,            ///< 10 Sun
      NUTATIONS,      ///< 11 Nutations (psi, epsilon and their rates)
      LIBRATIONS      ///< 12 Lunar Librations (3 euler angles)
   };

   /// Compute inertial position and velocity of given body at given time, relative
   /// to the solar system barycenter, using the current coefficient array.
   /// NB caller MUST call seekToJD(time) BEFORE calling this.
   /// On successful return, PV[0-2] contains the three position components, in km,
   /// and PV[3-5] the velocity components in km/day (for regular bodies), relative
   /// to the solar system barycenter, except for the moon, which is relative to
   /// Earth. For nutations and librations the units are radians and radians/day;
   /// nutations (components 0-3 only) are longitude and obliquity, and librations
   /// are the three euler angles.
   /// @param  MJD    time (Modified Julian Date) of interest (system TDB).
   /// @param  which  computeID of the body of interest.
   /// @param  PV     double(6) array containing the inertial position and velocity
   ///                 relative to the solar system barycenter.
   void InertialPositionVelocity(const double MJD, computeID which, double PV[6])
      throw(Exception);

   //------------------------------------------------------------------
   // member data

   // input stream, for use by readBinary...()
   std::ifstream istrm;  ///< input stream for binary files

   // header information

   // protected so it can be used by class SolarSystem
   /// -1 if the header has not been filled; also, for binary file input, 0 if
   /// the file position map has not yet been filled; otherwise it equals the
   /// number JPL assigns the ephemeris, e.g. 403, 405, which is identical to
   /// constants["DENUM"].
   int EphemerisNumber;
   
   /// The number of coefficients in a single record. This will determine the
   /// binary record size
   int Ncoeff;

   int Nconst;           ///< number of constants in the header (see map constants)
   std::string label[3]; ///< lines under group 1010
   double startJD;       ///< JD of the start of the first record in the file
   double endJD;         ///< JD of the end of the last record in the file
   double interval;      ///< number of days covered by each block of coeff.s
   int c_offset[13];     ///< starting index in the coefficients array for each planet
   int c_ncoeff[13];     ///< number of coefficients per component for each planet
   int c_nsets[13];      ///< number of sets of coefficients for each planet

   /// Hash of labels and values of constants read from the header.
   /// This is taken directly from the JPL documentation:
   /// <pre>
   /// The following is a partial list of constants found on the ephemeris file:
   /// DENUM           Planetary ephemeris number.
   /// LENUM           Lunar ephemeris number.
   /// TDATEF, TDATEB  Dates of the Forward and Backward Integrations
   /// CLIGHT          Speed of light (km/s).
   /// AU              Number of kilometers per astronomical unit.
   /// EMRAT           Earth-Moon mass ratio.
   /// GMi             GM for ith planet [au**3/day**2].
   /// GMB             GM for the Earth-Moon Barycenter [au**3/day**2].
   /// GMS             Sun (= k**2) [au**3/day**2].
   /// X1, ..., ZD9    Initial conditions for the numerical integration,
   ///                   given at "JDEPOC", with respect to "CENTER".
   /// JDEPOC          Epoch (JED) of initial conditions, normally JED 2440400.5.
   /// CENTER          Reference center for the initial conditions.
   ///                   (Sun: 11,  Solar System Barycenter: 12)
   /// RADi            Radius of ith planet [km].
   /// MA0001...MA0324 GM's of asteroid number 0001 ... 0234 [au**3/day**2].
   /// PHASE           The phase angle of the moon's rotation.
   /// LOVENO          The Love Number, k2, for the moon.
   /// PHI, THT, PSI   Euler angles of the orientation of the lunar mantle.
   /// OMEGAX, ...     Rotational velocities of the lunar mantle.
   /// PHIC,THTC,PSIC  Euler angles of the orientation of the lunar core.
   /// OMGCX, ...      Rotational velocities of the lunar core.
   /// </pre>
   std::map<std::string,double> constants;

   /// Hash of start times (JD) and coefficient arrays.
   /// This object is not intended to store the entire dataset, except temporarily
   /// for the purpose of reading/writing files, NOT for ephemeris computation.
   std::map<double, std::vector<double> > store;

   /// Hash of start times (JD) and file positions. This object is filled by
   /// readBinaryData(), which is called by initializeWithBinaryFile(), and is
   /// used by seekToJD() to read records in random order.
   std::map<double, long> fileposMap;

   /// One complete data record (Ncoeff doubles) consisting of times and coefficients.
   /// seekToJD() stores the current record here, and InertialPositionVelocity()
   /// uses it.
   std::vector<double> coefficients;

}; // end class SolarSystemEphemeris

}  // end namespace gpstk

#endif // SOLAR_SYSTEM_EPHEMERIS_INCLUDE
// nothing below this
