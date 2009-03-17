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
  //mSequenceStart = 1;
  //mSequenceNumber = 1;
  mSlidingBuffer = new sliding_buffer_kernel_1<ComponentEvent *>();
}

Adapter::~Adapter()
{
  delete mConfig;
  delete mSlidingBuffer;
}

Component * Adapter::searchDevicesForId(unsigned int id)
{
  for (int i=0; i<mDevices.size(); i++)
  {
    // For each device, perform a recursive call on itself and children
    Component * toFind = mDevices[i]->findById(id);
    if (toFind != NULL)
    {
      return toFind;
    }
  }
  
  return NULL;
}

void Adapter::processData(std::string line)
{
  std::istringstream toParse(line);
  std::string key;
  
  getline(toParse, key, '|');
  std::string time = key;
  
  std::cout << "Time = " << time << std::endl;
  
  getline(toParse, key, '|');
  std::string type = key;
  
  std::string value;
  getline(toParse, value, '|');
  
  if (type == "Alarm")
  {
    std::cout << "Alarm!" << std::endl;
    addToBuffer(new ComponentEvent(getDataItemByName(key), mSequence++, time, value));
  }
  else // Key -> Value Pairings
  {
    //std::string value;
    //getline(toParse, value, '|');
    
    addToBuffer(new ComponentEvent(getDataItemByName(key), mSequence++, time, value));
    
    // Will be bypassed by single "Time|Item|Value" event
    while (getline(toParse, key, '|') && getline(toParse, value, '|'))
    {
      addToBuffer(new ComponentEvent(getDataItemByName(key), mSequence++, time, value));
    }
  }
}

void Adapter::printComponents()
{
  std::cout << "** Component **" << std::endl;
  
  for (int i=0; i<mDevices.size(); i++)
  {
    printNodesAndChildren(mDevices[i]);
  }
  
  std::cout << std::endl;
}

void Adapter::printNodesAndChildren(Component * component)
{
  std::cout << "#" << " - " << component->getId() << ": " << component->getName();
  std::cout << " (Belongs to #" << component->getParentId() << ")" << std::endl;
//  std::cout << "Manufacturer: " << component->getManufacturer() << std::endl;
  
  std::list<Component *> children = component->getChildren();
  
  for (std::list<Component *>::iterator child=children.begin(); child!=children.end(); ++child)
  {
    printNodesAndChildren(*child);
  }
}

void Adapter::printDataItems()
{
  std::cout << "** Data Items **" << std::endl;
  
  for (int i=0; i<mDataItems.size(); i++)
  {
    std::cout << "#" << mDataItems[i]->mId << ": " << mDataItems[i]->mName << std::endl;
  }
  
  std::cout << std::endl;
}


/* Component protected methods */
void Adapter::loadDevices()
{
  xmlpp::NodeSet devices = mConfig->getRootNode()->find("//MTConnectDevices/Devices/*");
  
  for (int i=0; i<devices.size(); i++)
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
      handleChildren(component, parent);
      break;
    case Component::DATA_ITEMS:
      loadDataItems(component);
      break;
    case Component::TEXT:
      break;
    default:
      std::cout << "ERROR: parsing XML" << std::endl;
      std::cout << "Received: " << Component::getComponentEnum(component->get_name()) << std::endl;
  }
  
  // Construct
  if (toReturn != NULL && parent != NULL)
  {
    parent->addChild(toReturn);
    toReturn->setParentId(parent->getId());
  }
  
  
  // Check if there are children
  if (toReturn != NULL && !dynamic_cast<const xmlpp::ContentNode*>(component))
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
  
  if (!attributes.empty() && Component::hasNameAndId(attributes))
  {
    switch (spec)
    {
      case Component::AXES:
      {
        Axes * a = new Axes(attributes);
        mAxes.push_back(a);
        return a;
      }
      case Component::CONTROLLER:
      {
        Controller * c = new Controller(attributes);
        mControllers.push_back(c);
        return c;
      }
      case Component::DEVICE:
      {
        Device * d = new Device(attributes);
        mDevices.push_back(d);
        return d;
      }
      case Component::LINEAR:
      {
        Linear * l = new Linear(attributes);
        mLinears.push_back(l);
        return l;
      }
      case Component::POWER:
      {
        Power * p = new Power(attributes);
        mPowers.push_back(p);
        return p;
      }
      case Component::SPINDLE:
      {
        Spindle * s = new Spindle(attributes);
        mSpindles.push_back(s);
        return s;
      }
    }
  }
  
  std::cout << "ERERERR" << std::endl;
  return NULL;
}

void Adapter::loadDataItems(xmlpp::Node * dataItems)
{
  xmlpp::Node::NodeList children = dataItems->get_children();
  for (xmlpp::Node::NodeList::iterator child=children.begin(); child!=children.end(); ++child)
  {
    if ((*child)->get_name() == "DataItem")
    {
      // TODO: Error check attributes
      std::map<std::string, std::string> attributes = mConfig->getAttributes(*child);
      mDataItems.push_back(new DataItem(attributes));
    }
  }
}

DataItem * Adapter::getDataItemByName(std::string name)
{
  for (int i=0; i<mDataItems.size(); i++)
  {
    if (mDataItems[i]->getName() == name)
    {
      return mDataItems[i];
    }
  }
  
  return NULL;
}

void Adapter::handleChildren(xmlpp::Node * components, Component * parent)
{
  xmlpp::Node::NodeList children = components->get_children();
  for (xmlpp::Node::NodeList::iterator child=children.begin(); child!=children.end(); ++child)
  {
    handleComponent(*child, parent);
  }
}

/***** Procedural Code *****/
int main ()
{
  Adapter * adapter = new Adapter("agent.mtconnect.org", 7878, "../include/config_no_namespace.xml");
  
 // adapter->printComponents();
  //adapter->printDataItems();
  adapter->connect();
  
  return 0;
}
