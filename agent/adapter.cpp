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
  // Load .xml configuration
  mConfig = new XmlParser(configXml);
  
  // Load devices from configuration
  loadDevices();
  
  // Sequence number and sliding buffer for data
  mSequence = 1;
  mSlidingBuffer = new sliding_buffer_kernel_1<ComponentEvent *>();
  mSlidingBuffer->set_size(SLIDING_BUFFER_EXP);
  
  // Mutex used for synchronized access to sliding buffer and sequence number
  mSequenceLock = new dlib::mutex;
  
  // Will start threaded object: Adapter::thread()
  start();
}

Adapter::~Adapter()
{
  // Will stop threaded object gracefully Adapter::thread()
  stop();
  wait();
  
  delete mConfig;
  delete mSlidingBuffer;
  delete mSequenceLock;
}

std::vector<ComponentEvent *> Adapter::current(
    unsigned int * seq,
    unsigned int * firstSeq,
    std::string path
  )
{
  mSequenceLock->lock();
  
  *seq = mSequence;
  *firstSeq = (mSequence > mSlidingBuffer->size()) ? mSlidingBuffer->size() - mSequence : 1;
  
  std::vector<ComponentEvent *> results;
  
  for (unsigned int i=*firstSeq; i<mSequence; i++)
  {
    if ((*mSlidingBuffer)[i]->getFValue() != 0.0f)
    {
      results.push_back((*mSlidingBuffer)[i]);
    }
  }
  
  mSequenceLock->unlock();
  
  return results;
}

void Adapter::sample(unsigned int start, unsigned int count, std::string path)
{
  mSequenceLock->lock();
  unsigned int lastSequence = mSequence - 1;
  unsigned int firstSequence = (mSequence > mSlidingBuffer->size()) ? mSlidingBuffer->size() - mSequence : 1;
  mSequenceLock->unlock();
}

std::vector<Device *> Adapter::getDevices()
{
  return mDevices;
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

void Adapter::printComponents()
{
  std::cout << "** Component **" << std::endl;
  
  for (unsigned int i=0; i<mDevices.size(); i++)
  {
    std::cout << i << std::endl;
    printNodesAndChildren(mDevices[i]);
  }
  
  std::cout << std::endl;
}

void Adapter::printNodesAndChildren(Component * component)
{
  std::cout << "#" << component->getId() << ": " << component->getName();
  if (component->getParent() != NULL)
  {
    std::cout << " (Belongs to #" << component->getParent()->getId() << ")" << std::endl;
  }
  
  std::list<Component *> children = component->getChildren();
  
  for (std::list<Component *>::iterator child=children.begin(); child!=children.end(); ++child)
  {
    printNodesAndChildren(*child);
  }
}

void Adapter::printDataItems()
{
  std::cout << "** Data Items **" << std::endl;
  
  for (unsigned int i=0; i<mDataItems.size(); i++)
  {
    std::cout << "#" << mDataItems[i]->getId() << ": " << mDataItems[i]->getName();
    std::cout << " (" << mDataItems[i]->getSource() << ")" << std::endl;
  }
  
  std::cout << std::endl;
}

/* Adapter private methods */
void Adapter::thread()
{
  std::cout << "Starting adapter thread to read data" << std::endl;
  connect();
}

/* Adapter protected methods */
void Adapter::loadDevices()
{
  xmlpp::NodeSet devices = mConfig->getRootNode()->find("//MTConnectDevices/Devices/*");
  
  for (unsigned int i=0; i<devices.size(); i++)
  {
    // MAKE SURE FIRST ITEM IS DEVICE
    mDevices.push_back(static_cast<Device *>(handleComponent(devices[i])));
  }
}

Component * Adapter::handleComponent(xmlpp::Node * component, Component * parent)
{
  Component * toReturn = NULL;
  Component::EComponentSpecs spec = Component::getComponentEnum(component->get_name());
  switch (spec)
  {
    case Component::AXES:
    case Component::CONTROLLER:
    case Component::DEVICE:
    case Component::LINEAR:
    case Component::POWER:
    case Component::SPINDLE:
      toReturn = loadComponent(component, spec);
      break;
    case Component::COMPONENTS:
    case Component::DATA_ITEMS:
      handleChildren(component, parent);
      break;
    case Component::DATA_ITEM:
      loadDataItem(component, parent);
      break;
    case Component::TEXT:
      break;
    default:
      std::cout << "ERROR: parsing XML" << std::endl;
      std::cout << "Received: " << Component::getComponentEnum(component->get_name()) << std::endl;
  }
  
  // Construct
  if (toReturn != NULL and parent != NULL)
  {
    parent->addChild(toReturn);
    toReturn->setParent(parent);
  }
  
  
  // Check if there are children
  if (toReturn != NULL and !dynamic_cast<const xmlpp::ContentNode*>(component))
  {
    xmlpp::Node::NodeList children = component->get_children();
    for (xmlpp::Node::NodeList::iterator child=children.begin(); child!=children.end(); ++child)
    {
      std::string childName = (*child)->get_name();
      //std::cout << childName << "\n";
      if (childName == "Description")
      {
        std::map<std::string, std::string> attributes = mConfig->getAttributes(*child);
        toReturn->addDescription(attributes);
      }
      else
      {
        handleComponent(*child, toReturn);
      }
    }
  }
  
  return toReturn;
}

Component * Adapter::loadComponent(xmlpp::Node * component, Component::EComponentSpecs spec)
{
  std::map<std::string, std::string> attributes = mConfig->getAttributes(component);
  
  // TODO: ERROR CHECKING
  if (!attributes.empty() and Component::hasNameAndId(attributes))
  {
    switch (spec)
    {
      case Component::AXES:
        return new Axes(attributes);
      case Component::CONTROLLER:
        return new Controller(attributes);
      case Component::DEVICE:
        return new Device(attributes);
      case Component::LINEAR:
        return new Linear(attributes);
      case Component::POWER:
        return new Power(attributes);
      case Component::SPINDLE:
        return new Spindle(attributes);
      // TODO: other cases/error
    }
  }
  
  std::cout << "ERERERR" << std::endl;
  return NULL;
}

void Adapter::loadDataItem(xmlpp::Node * dataItem, Component * parent)
{
  // TODO: Error check attributes
  std::map<std::string, std::string> attributes = mConfig->getAttributes(dataItem);
  DataItem * d = new DataItem(attributes);
  d->setComponent(parent);
  
  // Check children for "source"
  if (!dynamic_cast<const xmlpp::ContentNode*>(dataItem))
  {
    xmlpp::Node::NodeList children = dataItem->get_children();
    for (xmlpp::Node::NodeList::iterator child=children.begin(); child!=children.end(); ++child)
    {
      std::string childName = (*child)->get_name();
      //std::cout << childName << std::endl;
      if (childName == "Source")
      {
        xmlpp::Node::NodeList grandChildren = (*child)->get_children();
        const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(grandChildren.front());
        if (nodeText)
        {
          //std::cout << "Adding: " << nodeText->get_content() << "\n";
          d->addSource(nodeText->get_content());
        }
      }
    }
  }
  
  parent->addDataItem(d);
  mDataItems.push_back(d);
}

DataItem & Adapter::getDataItemByName(std::string name) throw (std::string)
{
  for (unsigned int i=0; i<mDataItems.size(); i++)
  {
    if (mDataItems[i]->hasName(name))
    {
      return *mDataItems[i];
    }
  }
  throw (std::string) "DataItem '" + name + "' was not found";
}

void Adapter::handleChildren(xmlpp::Node * components, Component * parent)
{
  if (!dynamic_cast<const xmlpp::ContentNode*>(components))
  {
    xmlpp::Node::NodeList children = components->get_children();
    for (xmlpp::Node::NodeList::iterator child=children.begin(); child!=children.end(); ++child)
    {
      handleComponent(*child, parent);
    }
  }
}

void Adapter::addToBuffer(std::string time, std::string key, std::string value)
{
  try
  {
    DataItem d = getDataItemByName(key);
    
    mSequenceLock->lock();
    (*mSlidingBuffer)[mSequence] = new ComponentEvent(&d, mSequence, time, value);
    mSequence++;
    mSequenceLock->unlock();
    
    //d->addComponentEvent();
  }
  catch (std::string msg)
  {
    std::cerr << "Adapter.cpp: " << msg << std::endl;
  }
}


