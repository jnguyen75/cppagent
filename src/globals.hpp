/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AMT nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
* BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
* RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
* (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
* WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
* LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. 

* LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
* PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
* OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
* CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
* WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
* THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
* SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

/***** CONSTANTS *****/

/* Port number to put server on */
const unsigned int SERVER_PORT = 8080;

/* Size of sliding buffer */
const unsigned int SLIDING_BUFFER_SIZE = 131072;

/* Size of buffer exponent: 2^SLIDING_BUFFER_EXP */
const unsigned int SLIDING_BUFFER_EXP = 17;

/* Message for when enumerations do not exist in an array/enumeration */
const int ENUM_MISS = -1;

/* Log file for logging events */
extern const char *gLogFile;

/* Time format */
enum TimeFormat
{
  HUM_READ,
  GMT,
  GMT_UV_SEC,
  LOCAL
};

/***** METHODS *****/

/* Convert an unsigned integer to string */
std::string intToString(int i);

/* Convert a float to string */
std::string floatToString(float f);

/* Convert a string to the same string with all upper case letters */
std::string toUpperCase(std::string& text);

/* Check if each char in a string is a positive integer */
bool isNonNegativeInteger(const std::string& s);

/* Get the current time formatted */
std::string getCurrentTime(TimeFormat format);

/* Get the current time in number of seconds as an integer */
unsigned int getCurrentTimeInSec();

void logEvent(const std::string& source, const std::string& message);

int getEnumeration(
  const std::string& name,
  const std::string *array,
  unsigned int size
);

#endif

