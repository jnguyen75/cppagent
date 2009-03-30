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

#ifndef AGENT_HPP
#define AGENT_HPP

// C++ libraries
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>

#include <ctime>

// External libraries
#include "dlib/md5.h"
#include "dlib/server.h"
#include "dlib/sliding_buffer.h"

#include "adapter.hpp"
#include "component_event.hpp"
#include "xml_printer.hpp"

using namespace dlib;

class Adapter;

// Web server used from dlib example
class Agent : public server::http_1a_c
{
public:
  static const unsigned int SERVER_PORT = 3000;
  static const unsigned int SLIDING_BUFFER_EXP = 17;
  static const unsigned int SLIDING_BUFFER_SIZE = 131072;
  
protected:
  /* Handle the device/path parameters for the xpath search */
  std::string devicesAndPath(std::string path = "", std::string device = "");
  
  /* Get list of data items in path */
  std::list<DataItem *> getDataItems(std::string path, xmlpp::Node * node = NULL);
  
  /* HTTP methods to handle the 3 basic calls*/
  void handleCall(std::ostream& out, const map_type& queries, std::string call, std::string device = "");
  void handleCurrent(std::ostream& out, std::string path, unsigned int frequency = 0);
  void handleProbe(std::string device);
  void handleSample(
    std::ostream& out,
    std::string path,
    unsigned int start,
    unsigned int count,
    unsigned int frequency
  );
  
  void streamData(
    std::ostream& out,
    std::list<DataItem *> dataItems,
    bool current,
    unsigned int frequency,
    unsigned int start = 0,
    unsigned int count = 0
  );
  
  void fetchCurrentData(std::list<DataItem *> dataItems);
  void fetchSampleData(std::list<DataItem *> dataItems, unsigned int start, unsigned int count);
  
  /* Find devices/data items by name */
  Device * getDeviceByName(std::string name);
  DataItem * getDataItemByName(std::string name);
  
  /* retrieve the sequence number and the first sequence number in buffer */
  void getSequenceNumbers(unsigned int * seq, unsigned int * firstSeq);
  
  /* Output an XML Error */
  void printError(std::string errorCode, std::string text);
  
protected:
  XmlParser * mConfig;
  
  /* For access to the sequence number and sliding buffer, use the mutex */
  dlib::mutex * mSequenceLock;
  
  /* Sequence number */
  unsigned int mSequence;
  
  /* The sliding/circular buffer to hold all of the events/sample data */
  dlib::sliding_buffer_kernel_1<ComponentEvent *> * mSlidingBuffer;
  
  /* Class string stream to return XML on requests */
  std::ostringstream mXmlStream;
  
  XmlPrinter * mXmlPrinter;
  
  /* Lists of data */
  std::list<Adapter *> mAdapters;
  std::list<Device *> mDevices;
  std::list<DataItem *> mDataItems;
  
public:
  Agent();
  
  virtual ~Agent();
  
  /* Overridden method that is called per web request */  
  bool on_request (
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
    unsigned short local_port,
    std::ostream& out
  );
  
  void addAdapter(std::string server, unsigned int port, std::string configXmlPath, unsigned int numAdapters = 1);
  
  /* Add ComponentEvent and specs to the SlidingBuffer */
  void addToBuffer(std::string time, std::string key, std::string value);
};


#endif

