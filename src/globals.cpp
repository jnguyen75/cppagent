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

// Dlib library
#include "../lib/dlib/all/source.cpp"

#include <ctime>
#include <string>
#include <sstream>

/* Convert an unsigned integer to string */
std::string intToString(unsigned int i)
{
  std::ostringstream stm;
  stm << i;
  return stm.str();
}

/* Convert a float to string */
std::string floatToString(float f)
{
  std::ostringstream stm;
  stm << f;
  return stm.str();
}

/* Convert a string to the same string with all upper case letters */
std::string toUpperCase(std::string text)
{
  for (unsigned int i=0; i<text.length();++i)
  {
    text[i] = toupper(text[i]);
  }
  
  return text;
}

/* Check if each char in a string is a positive integer */
bool isNonNegativeInteger(const std::string& s)
{
  for (unsigned int i=0; i<s.length(); i++)
  {
    if (!std::isdigit(s[i]))
    {
      return false;
    }
  }
  
  return true;
}

/* Get the current time formatted */
std::string getCurrentTime(bool formatted)
{
  char timeBuffer [30];
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = gmtime ( &rawtime );
  
  if (formatted)
  {
    strftime (timeBuffer, 50, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
  }
  else
  {
    strftime (timeBuffer, 50, "%Y-%m-%dT%H:%M:%S+00:00", timeinfo);
  }
  
  return std::string(timeBuffer);
}

/* Get the current time in number of seconds as an integer */
unsigned int getCurrentTimeInSec()
{
  return time(NULL);
}

