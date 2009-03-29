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

#include "adapter.hpp"

/* Adapter public methods */
Adapter::Adapter(std::string server, unsigned int port, XmlParser * configXml)
: Connector(server, port)
{
  // Set Adapter ID
  mId = time(NULL);
  
  // Will start threaded object: Adapter::thread()
  start();
}

Adapter::~Adapter()
{
  // Will stop threaded object gracefully Adapter::thread()
  stop();
  wait();
}

unsigned int Adapter::getId()
{
  return mId;
}

void Adapter::setAgent(Agent * agent)
{
  mAgent = agent;
}

void Adapter::processData(std::string line)
{
  std::istringstream toParse(line);
  std::string key;
  
  getline(toParse, key, '|');
  std::string time = key;
  
  getline(toParse, key, '|');
  std::string type = key;
  
  std::string value;
  getline(toParse, value, '|');
  
  if (type == "Alarm")
  {
    std::cout << "Alarm!" << std::endl;
    mAgent->addToBuffer(time, key, value);
  }
  else // Key -> Value Pairings
  {
    mAgent->addToBuffer(time, key, value);
    
    // Will be bypassed by single "Time|Item|Value" event
    while (getline(toParse, key, '|') and getline(toParse, value, '|'))
    {
      mAgent->addToBuffer(time, key, value);
    }
  }
}

/* Adapter private methods */
void Adapter::thread()
{
  std::cout << "Starting adapter thread to read data" << std::endl;
  connect();
}
