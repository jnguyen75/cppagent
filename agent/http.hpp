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

#ifndef HTTP_HPP
#define HTTP_HPP

// C++ libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

// External libraries
#include "dlib/server.h"
#include "adapter.hpp"

#include "xml_printer.hpp"

// Connect to http://localhost:{SERVER_PORT}/
#define SERVER_PORT 5000

// The basic calls
#define NUM_BASIC_CALLS 3
const std::string basicCalls[NUM_BASIC_CALLS] = { "current", "probe", "sample" };

using namespace dlib;

// Web server used from dlib example
class HTTP : public server::http_1a_c
{
protected:
  /* Methods to handle the 3 basic calls*/
  void handleCurrent();
  void handleProbe();
  void handleSample();
  
  /* Handle the device/path parameters for the xpath search */
  void handleDevicesAndPath(std::string device, std::string path = "");
  
  /* Determines if the URL path is one of the predetermined basic calls */
  bool isBasicCall(std::string call);
  
  /* Output an XML Error */
  void printError(std::string errorCode, std::string text);
  
  /* Class string stream to return XML on requests */
  std::ostringstream xmlStream;
  
  std::vector<Adapter *> mAdapters;
  
public:
  void on_request (
        const std::string& path,
        std::string& result,
        const map_type& queries,
        const map_type& cookies,
        queue_type& new_cookies,
        const map_type& incoming_headers,
        map_type& response_headers,
        const std::string& foreign_ip,
        const std::string& local_ip,
        unsigned short foreign_port,
        unsigned short local_port
    );
  
  void addAdapter(std::string server, unsigned int port, std::string configXmlPath);
};


#endif

