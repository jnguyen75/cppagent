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

#ifndef ADAPTER_HPP
#define ADAPTER_HPP

#include <iostream>

#include <map>
#include <list>
#include <cmath>

#include "xml_parser.hpp"

#include "dlib/sockets.h"
#include "dlib/sliding_buffer.h"
#include "dlib/threads.h"

#include "connector.hpp"
#include "component_event.hpp"

#include "axes.hpp"
#include "controller.hpp"
#include "component.hpp"
#include "device.hpp"
#include "data_item.hpp"
#include "linear.hpp"
#include "power.hpp"
#include "spindle.hpp"

using namespace dlib;

class Adapter : public Connector, public threaded_object
{
public:
  static const unsigned int SLIDING_BUFFER_EXP = 17;
  static const unsigned int SLIDING_BUFFER_SIZE = 131072;

protected:
  /* Arrays to keep track of all devices and dataItems */
  std::list<Device *> mDevices;
  std::list<DataItem *> mDataItems;
  
  /* For access to the sequence number and sliding buffer, use the mutex */
  dlib::mutex * mSequenceLock;
  
  /* Sequence number */
  unsigned int mSequence;
  
  /* The sliding/circular buffer to hold all of the events/sample data */
  dlib::sliding_buffer_kernel_1<ComponentEvent *> * mSlidingBuffer;
  
private:
  /* Inherited and is run as part of the threaded_object */
  void thread();
  
protected:
  /* Initial method called to call the //Devices/  path and begin loading */
  void loadDevices(std::string configXml);
  
  DataItem * getDataItemByName(std::string name) throw (std::string);
  
  /* Add ComponentEvent and specs to the SlidingBuffer */
  void addToBuffer(std::string time, std::string key, std::string value);
  
public:
  /* Load the adapter with the .xml file */
  Adapter(std::string server, unsigned int port, std::string configXml);
  
  /* Destructor */
  virtual ~Adapter();
  
  std::list<ComponentEvent *> current(
    unsigned int * seq,
    unsigned int * firstSeq,
    std::string path = ""
  );
  
  std::list<ComponentEvent *> sample(
    unsigned int * seq,
    unsigned int * firstSeq,
    unsigned int start,
    unsigned int count,
    std::string path = ""
  );
  
  std::list<Device *> getDevices();
  
  /* Inherited method for incoming data from the server */
  void processData(std::string line);
};

#endif
