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
Adapter::Adapter(std::string server, unsigned int port, std::string configXml)
: Connector(server, port)
{
  // Set Adapter ID
  unsigned int mId = time(NULL);
  
  // Load .xml configuration
  mConfig = new XmlParser(configXml);
  
  // Load devices from configuration
  mDevices = mConfig->getDevices();
  mDataItems = mConfig->getDataItems();
  
  // Will start threaded object: Adapter::thread()
  start();
}

Adapter::~Adapter()
{
  // Will stop threaded object gracefully Adapter::thread()
  stop();
  wait();
  
  delete mConfig;
}

unsigned int Adapter::getId()
{
  return mId;
}

void Adapter::current(
    unsigned int * seq,
    unsigned int * firstSeq,
    std::string path
  )
{
  mSequenceLock->lock();
  
  *seq = mSequence;
  *firstSeq = (mSequence > mSlidingBuffer->size()) ? mSlidingBuffer->size() - mSequence : 1;
  
  mSequenceLock->unlock();
}

std::list<ComponentEvent *> Adapter::sample(
    unsigned int * seq,
    unsigned int * firstSeq,
    unsigned int start,
    unsigned int count,
    std::list<DataItem *> dataItems
  )
{
  mSequenceLock->lock();
  
  *seq = mSequence;
  *firstSeq = (mSequence > mSlidingBuffer->size()) ? mSlidingBuffer->size() - mSequence : 1;
  
  std::list<ComponentEvent *> results;
  
  // START SHOULD BE BETWEEN 0 AND SEQUENCE NUMBER
  start = (start <= *firstSeq) ? *firstSeq + 1  : start;
  unsigned int end = (count + start >= mSequence) ? mSequence-1 : count+start;
  
  for (unsigned int i = start; i<end; i++)
  {
    // Filter out according to if it exists in the list
    unsigned int dataId = (*mSlidingBuffer)[i]->getDataItem()->getId();
    if (hasDataItem(dataItems, dataId))
    {
      results.push_back((*mSlidingBuffer)[i]);
    }
    else
    {
      // TODO: increment counter?
    }
  }
  
  mSequenceLock->unlock();
  
  return results;
}

Device * Adapter::getDeviceByName(std::string name)
{
  std::list<Device *>::iterator device;
  for (device=mDevices.begin(); device!=mDevices.end(); device++)
  {
    if ((*device)->getName() == name)
    {
      return *device;
    }
  }
  return NULL;
}

std::list<Device *> Adapter::getDevices()
{
  return mDevices;
}

std::list<DataItem *> Adapter::getDataItems()
{
  return mDataItems;
}

std::list<DataItem *> Adapter::getDataItems(std::string path, xmlpp::Node * node)
{
  std::list<DataItem *> items;
  
  node = (node == NULL) ? mConfig->getRootNode() : node;
  
  xmlpp::NodeSet elements = node->find(path);
  
  for (unsigned int i=0; i<elements.size(); i++)
  {
    if (const xmlpp::Element * nodeElement = dynamic_cast<const xmlpp::Element*>(elements[i]))
    {
      std::string nodename = nodeElement->get_name();
      if (nodename == "DataItem")
      {
        unsigned int id = atoi(nodeElement->get_attribute_value("id").c_str());
        DataItem * item = getDataItemById(id);
        if (item != NULL)
        {
          items.push_back(item);
        }
        else
        {
          std::cerr << "DATA ITEM NOT FOUND\n";
        }
      }
      else if (nodename == "Components" or nodename == "DataItems")
      {
        std::list<DataItem *> toMerge = getDataItems("*", elements[i]);
        items.merge(toMerge);
      }
      else // Hopefully "Component"
      {
        std::list<DataItem *> toMerge = getDataItems("Components/*|DataItems/*", elements[i]);
        items.merge(toMerge);
      }
    }
  }
  
  return items;
}

void Adapter::processData(std::string line)
{
  std::istringstream toParse(line);
  std::string key;
  
  getline(toParse, key, '|');
  std::string time = key;
  
  //std::cout << "Time = " << time << std::endl;
  
  getline(toParse, key, '|');
  std::string type = key;
  
  std::string value;
  getline(toParse, value, '|');
  
  if (type == "Alarm")
  {
    std::cout << "Alarm!" << std::endl;
    addToBuffer(time, key, value);
  }
  else // Key -> Value Pairings
  {
    addToBuffer(time, key, value);
    
    // Will be bypassed by single "Time|Item|Value" event
    while (getline(toParse, key, '|') and getline(toParse, value, '|'))
    {
      addToBuffer(time, key, value);
    }
  }
}

/* Adapter private methods */
void Adapter::thread()
{
  std::cout << "Starting adapter thread to read data" << std::endl;
  connect();
}

/* Adapter protected methods */
DataItem * Adapter::getDataItemByName(std::string name)
{
  std::list<DataItem *>::iterator dataItem;
  for (dataItem = mDataItems.begin(); dataItem!=mDataItems.end(); dataItem++)
  {
    if ((*dataItem)->hasName(name))
    {
      return (*dataItem);
    }
  }
  return NULL;
}

DataItem * Adapter::getDataItemById(unsigned int id)
{
  std::list<DataItem *>::iterator dataItem;
  for (dataItem = mDataItems.begin(); dataItem!=mDataItems.end(); dataItem++)
  {
    if ((*dataItem)->getId() == id)
    {
      return (*dataItem);
    }
  }
  return NULL;
}

bool Adapter::hasDataItem(std::list<DataItem *> dataItems, unsigned int id)
{
  std::list<DataItem *>::iterator dataItem;
  for (dataItem = dataItems.begin(); dataItem!=dataItems.end(); dataItem++)
  {
    if ((*dataItem)->getId() == id)
    {
      return true;
    }
  }
  return false;
}
