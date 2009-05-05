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

#include "agent.hpp"
#include <time.h>
#include <iostream>

/* Agent public methods */
Agent::Agent(const std::string& configXmlPath)
{
  try
  {
    // Load the configuration for the Agent
    mConfig = new XmlParser(configXmlPath);
  }
  catch (std::exception & e)
  {
    logEvent("Agent::Agent",
      "Error loading xml configuration: " + configXmlPath);
    delete mConfig;
    throw e.what();
  }
  
  // Grab data from configuration
  mDevices = mConfig->getDevices();
  for (std::list<Device*>::iterator iter = mDevices.begin(); iter != mDevices.end(); iter++) 
  {
    mDeviceMap[(*iter)->getName()] = *iter;

    std::cout << "Loaded device: " << (*iter)->getName() << std::endl;
    
    std::map<std::string, DataItem*> items = (*iter)->getDeviceDataItems();
    for (std::map<std::string, DataItem*>::iterator item = items.begin(); item != items.end(); ++item)
    {
      DataItem *d = item->second;
      mDataItemMap[d->getId()] = d;
    }
  }
  
  // Unique id number for agent instance
  mInstanceId = getCurrentTimeInSec();
  
  // Sequence number and sliding buffer for data
  mSequence = 1;
  mSlidingBuffer = new sliding_buffer_kernel_1<ComponentEvent *>();
  mSlidingBuffer->set_size(SLIDING_BUFFER_EXP);
  for (int i = 0; i < mSlidingBuffer->size(); i++)
    (*mSlidingBuffer)[i] = 0;
  
  // Mutex used for synchronized access to sliding buffer and sequence number
  mSequenceLock = new dlib::mutex;
}

Agent::~Agent()
{
  delete mSlidingBuffer;
  delete mSequenceLock;
  delete mConfig;
}

bool Agent::on_request(
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
  )
{
  try 
  {
    // Parse the URL path looking for '/'
    std::string::size_type loc1 = path.find("/", 1);
    
    std::string::size_type end = (path[path.length()-1] == '/') ?
      path.length()-1 : std::string::npos;
    
    // If a '/' was found
    if (loc1 < end)
    {
      // Look for another '/'
      std::string::size_type loc2 = path.find("/", loc1+1);
      
      if (loc2 == end)
      {
        return handleCall(out, path, result, queries, 
          path.substr(loc1+1, loc2-loc1-1), path.substr(1, loc1-1));
      }
      else
      {
        // Path is too long
        result = printError("UNSUPPORTED",
          "The following path is invalid: " + path);
        return true;
      }
    }
    else
    {
      // Try to handle the call
      return handleCall(out, path, result, queries, path.substr(1, loc1-1));
    }
  }
  catch (std::exception & e)
  {
    logEvent("Agent", (std::string) e.what());
    printError("SERVER_EXCEPTION",(std::string) e.what()); 
  }
  
  return true;
}

void Agent::addAdapter(const std::string &device, const std::string& host, const unsigned int port)
{
  Adapter *adapter = new Adapter(device, host, port);
  adapter->setAgent(*this);
}

unsigned int Agent::addToBuffer(
  const std::string& device,
  const std::string& dataItemName,
  const std::string& value,
  std::string time
  )
{
  DataItem *dataItem = getDataItemByName(device, dataItemName);
  
  if (dataItem == NULL)
  {
    logEvent("Agent", "Could not find data item: " + dataItemName);
    return 0;
  }
  else
  {
    mSequenceLock->lock();
    
    if ((*mSlidingBuffer)[mSequence] != NULL)
    {
      delete (*mSlidingBuffer)[mSequence];
    }
    
    // If this function is being used as an API, add the current data in
    if (time.empty())
    {
      time = getCurrentTime(GMT_UV_SEC);
    }
    
    ComponentEvent *event =
      new ComponentEvent(*dataItem, mSequence, time, value);
    
    unsigned int seqNum = mSequence++;
    
    (*mSlidingBuffer)[seqNum] = event;
    dataItem->setLatestEvent(*event);
    
    mSequenceLock->unlock();
    return seqNum;
  }
}

/* Agent protected methods */
bool Agent::handleCall(
    std::ostream& out,
    const std::string& path,
    std::string& result,
    const map_type& queries,
    const std::string& call,
    const std::string& device
  )
{
  std::string deviceName;
  if (!device.empty())
  {
    deviceName = device;
  }
  else if (queries.is_in_domain("device"))
  {
    deviceName = queries["device"];
  }
  
  if (call == "current")
  {
    const std::string path = queries.is_in_domain("path") ?
      queries["path"] : "";
    
    int freq = checkAndGetParam(result, queries, "frequency", NO_FREQ,
      FASTEST_FREQ, false, SLOWEST_FREQ);
    
    if (freq == PARAM_ERROR)
    {
      return true;
    }
    
    return handleStream(out, result, devicesAndPath(path, deviceName), true,
      freq);
  }
  else if (call == "probe" || call.empty())
  {
    result = handleProbe(deviceName);
    return true;
  }
  else if (call == "sample")
  {
    std::string path = queries.is_in_domain("path") ?
      queries["path"] : "";
    
    int count = checkAndGetParam(result, queries, "count", DEFAULT_COUNT,
      1, true);
    int freq = checkAndGetParam(result, queries, "frequency", NO_FREQ,
      FASTEST_FREQ, false, SLOWEST_FREQ);
    
    int start = checkAndGetParam(result, queries, "start", NO_START);
    
    if (start == NO_START) // If there was no data in queries
    {
      start = checkAndGetParam(result, queries, "from", 1);
    }
    
    if (freq == PARAM_ERROR || count == PARAM_ERROR || start == PARAM_ERROR)
    {
      return true;
    }
    
    return handleStream(out, result, devicesAndPath(path, deviceName), false,
			freq, start, count);
  }
  else if (hasDevice(call) && device.empty())
  {
    result = handleProbe(call);
    return true;
  }
  else
  {
    result = printError("UNSUPPORTED",
      "The following path is invalid: " + path);
    return true;
  }
}

std::string Agent::handleProbe(const std::string& name)
{
  std::list<Device *> mDeviceList;
  
  if (!name.empty())
  {
    Device * device = getDeviceByName(name);
    if (device == NULL)
    {
      return printError("NO_DEVICE",
        "Could not find the device '" + name + "'");
    }
    else
    {
      mDeviceList.push_back(device);
    }
  }
  else
  {
    mDeviceList = mDevices;
  }
  
  return XmlPrinter::printProbe(mInstanceId, SLIDING_BUFFER_SIZE, mSequence,
    mDeviceList);
}

bool Agent::handleStream(
    std::ostream& out,
    std::string& result,
    const std::string& path,
    bool current,
    unsigned int frequency,
    unsigned int start,
    unsigned int count
  )
{
  std::list<DataItem *> dataItems;
  try
  {
    dataItems = getDataItems(path);
  }
  catch (std::exception& e)
  {
    result = printError("INVALID_XPATH", e.what());
    logEvent("Agent::handleStream", e.what());
    return true;
  }
    
  if (dataItems.empty())
  {
    result = printError("INVALID_XPATH",
      "The path could not be parsed. Invalid syntax: " + path);
    return true;
  }
  
  // Check if there is a frequency to stream data or not
  if (frequency != (unsigned) NO_FREQ)
  {
    if (current)
    {
      streamData(out, dataItems, true, frequency);
    }
    else
    {
      streamData(out, dataItems, false, frequency, start, count);
    }
    return false;
  }
  else
  {
    result = (current) ?
      fetchCurrentData(dataItems) : fetchSampleData(dataItems, start, count);
    return true;
  }
}

void Agent::streamData(
    std::ostream& out,
    std::list<DataItem *>& dataItems,
    bool current,
    unsigned int frequency,
    unsigned int start,
    unsigned int count
  )
{
  // Create header
  out << "HTTP/1.1 200 OK" << std::endl;
  out << "Connection: close" << std::endl;
  out << "Date: " << getCurrentTime(HUM_READ) << std::endl;
  out << "Status: 200 OK" << std::endl;
  out << "Content-Disposition: inline" << std::endl;
  out << "X-Runtime: 144ms" << std::endl;
  out << "Content-Type: multipart/x-mixed-replace;";
  
  std::string boundary = md5(intToString(time(NULL)));
  out << "boundary=" << boundary << std::endl << std::endl;
  
  // Loop until the user closes the connection
  while (out.good())
  {
    out << "--" + boundary << std::endl;
    out << "Content-type: text/xml" << std::endl;
    
    std::string content = (current) ?
      fetchCurrentData(dataItems) : fetchSampleData(dataItems, start, count);
    logEvent("Start", intToString(start));
    logEvent("Count", intToString(count));
    
    start = (start + count < mSequence) ? (start + count) : mSequence - 1;
    
    out << "Content-length: " << content.length() << std::endl;
    
    out << std::endl << content;
    
    out.flush();
    dlib::sleep(frequency);
  }
}

std::string Agent::fetchCurrentData(std::list<DataItem *>& dataItems)
{
  mSequenceLock->lock();
  unsigned int firstSeq = (mSequence > SLIDING_BUFFER_SIZE) ?
    mSequence - SLIDING_BUFFER_SIZE : 1;
  
  std::string toReturn = XmlPrinter::printCurrent(mInstanceId,
    SLIDING_BUFFER_SIZE, mSequence, firstSeq, dataItems);
  
  mSequenceLock->unlock();
  return toReturn;
}

std::string Agent::fetchSampleData(
    std::list<DataItem *>& dataItems,
    unsigned int start,
    unsigned int count
  )
{
  std::list<ComponentEvent *> results;
  
  mSequenceLock->lock();

  unsigned int seq = mSequence;
  unsigned int firstSeq = (mSequence > SLIDING_BUFFER_SIZE) ?
    mSequence - SLIDING_BUFFER_SIZE : 1;
  
  // START SHOULD BE BETWEEN 0 AND SEQUENCE NUMBER
  start = (start <= firstSeq) ? firstSeq  : start;
  unsigned int end = (count + start >= mSequence) ? mSequence : count+start;
  
  for (unsigned int i = start; i < end; i++)
  {
    // Filter out according to if it exists in the list
    const std::string dataName = (*mSlidingBuffer)[i]->getDataItem()->getName();
    if (hasDataItem(dataItems, dataName))
    {
      results.push_back((*mSlidingBuffer)[i]);
    }
    else if (end < mSequence)
    {
      // Increase the end number if you are allowed to
      end++;
    }
  }
  
  mSequenceLock->unlock();
  
  return XmlPrinter::printSample(mInstanceId, SLIDING_BUFFER_SIZE, seq, 
    firstSeq, results);
}

std::string Agent::printError(
    const std::string& errorCode,
    const std::string& text
  )
{
  return XmlPrinter::printError(mInstanceId, SLIDING_BUFFER_SIZE, mSequence,
    errorCode, text);
}

std::string Agent::devicesAndPath(
    const std::string& path,
    const std::string& device
  )
{
  std::string dataPath = "";
  
  if (!device.empty())
  {
    std::string prefix = "//Devices/Device[@name=\"" + device + "\"]";
    
    if (!path.empty())
    {
      std::istringstream toParse(path);
      std::string token;
      
      // Prefix path (i.e. "path1|path2" => "{prefix}path1|{prefix}path2")
      while (getline(toParse, token, '|'))
      {
        dataPath += prefix + token + "|";
      }
      
      dataPath.erase(dataPath.length()-1);
    }
    else
    {
      dataPath = prefix;
    }
  }
  else
  {
    dataPath = (!path.empty()) ? path : "//Devices/Device";
  }
  
  return dataPath;
}

std::list<DataItem *> Agent::getDataItems(
    const std::string& path,
    xmlpp::Node * node
  )
{
  std::list<DataItem *> items;
  
  node = (node == NULL) ? mConfig->getRootNode() : node;
  
  xmlpp::NodeSet elements;
  
  elements = node->find(path);
  
  for (unsigned int i=0; i<elements.size(); i++)
  {
    const xmlpp::Element *nodeElement =
      dynamic_cast<const xmlpp::Element *>(elements[i]);
    
    if (nodeElement)
    {
      const std::string nodename = nodeElement->get_name();
      if (nodeElement->get_name() == "DataItem")
      {
        DataItem *item =
          getDataItemById(nodeElement->get_attribute_value("id"));
        
        if (item != NULL)
        {
          items.push_back(item);
        }
        else
        {
          logEvent("Agent", "Data item not found: " + nodename);
        }
      }
      else if (nodename == "Components" || nodename == "DataItems")
      {
        // Recursive call
        std::list<DataItem *> toMerge = getDataItems("*", elements[i]);
        items.merge(toMerge);
      }
      else // Hopefully "Component"
      {
        std::list<DataItem *> toMerge;
        toMerge = getDataItems("Components/*|DataItems/*", elements[i]);
        items.merge(toMerge);
      }
    }
  }
  
  return items;
}

int Agent::checkAndGetParam(
    std::string& result,
    const map_type& queries,
    const std::string& param,
    const int defaultValue,
    const int minValue,
    bool minError,
    const int maxValue
  )
{
  if (!queries.is_in_domain(param))
  {
    return defaultValue;
  }
  
  if (queries[param].empty())
  {
    result = printError("QUERY_ERROR",
      "'" + param + "' cannot be empty.");
    return PARAM_ERROR;
  }
  
  if (!isNonNegativeInteger(queries[param]))
  {
    result = printError("QUERY_ERROR",
      "'" + param + "' must be a positive integer.");
    return PARAM_ERROR;
  }
  
  long int value = strtol(queries[param].c_str(), NULL, 10);
  
  if (minValue != NO_VALUE && value < minValue)
  {
    if (minError)
    {
      result = printError("QUERY_ERROR",
        "'" + param + "' must be greater than " + intToString(minValue) + ".");
      return PARAM_ERROR;
    }
    return minValue;
  }
  
  if (maxValue != NO_VALUE && value > maxValue)
  {
    result = printError("QUERY_ERROR",
      "'" + param + "' must be less than "
      + intToString(maxValue) + ".");
    return PARAM_ERROR;
  }
  
  return value;
}

DataItem * Agent::getDataItemByName(const std::string& device, const std::string& name)
{
  Device *dev = mDeviceMap[device];
  if (dev) {
    return dev->getDeviceDataItem(name);
  }
  
  return NULL;
}

bool Agent::hasDataItem(
    std::list<DataItem *>& dataItems,
    const std::string& name
  )
{
  std::list<DataItem *>::iterator dataItem;
  for (dataItem=dataItems.begin(); dataItem!=dataItems.end(); dataItem++)
  {
    if ((*dataItem)->hasName(name))
    {
      return true;
    }
  }
  return false;
}

bool Agent::hasDevice(const std::string& name)
{
  Device *dev = mDeviceMap[name];
  return dev != 0;
}

