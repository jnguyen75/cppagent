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

#include "xml_parser.hpp"

/* XmlParser public methods */
XmlParser::XmlParser(std::string xmlPath)
{
  try
  {
    mParser = new xmlpp::DomParser;
    
    // Set to false now because XML does not contain DTD
    mParser->set_validate(false);
    
    // We want the text to be resolved/unescaped automatically.
    mParser->set_substitute_entities();
    
    // Parse the input file
    mParser->parse_file(xmlPath);
  }
  catch (std::exception & e)
  {
    std::cout << "XML Exception: " << e.what() << std::endl;
  }
  
  xmlpp::NodeSet devices = mParser->get_document()->get_root_node()->find("//MTConnectDevices/Devices/*");
  
  for (unsigned int i=0; i<devices.size(); i++)
  {
    // MAKE SURE FIRST ITEM IS DEVICE
    mDevices.push_back(static_cast<Device *>(handleComponent(devices[i])));
  }
}

XmlParser::~XmlParser()
{
  delete mParser;
}

std::list<Device *> XmlParser::getDevices()
{
  return mDevices;
}

std::list<DataItem *> XmlParser::getDataItems()
{
  return mDataItems;
}

xmlpp::Node * XmlParser::getRootNode()
{
  return mParser->get_document()->get_root_node();
}

/* XmlParser protected methods */
std::map<std::string, std::string> XmlParser::getAttributes(const xmlpp::Node * node)
{
  std::map<std::string, std::string> mapToReturn;
  
  // Only element nodes have attributes
  if (const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
  {
    // Load all the attributes into the map to return
    const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
    
    xmlpp::Element::AttributeList::const_iterator attribute;
    for (attribute=attributes.begin(); attribute!=attributes.end(); attribute++)
    {
      mapToReturn[(*attribute)->get_name()] = (*attribute)->get_value();
    }
  }
  else
  {
    // Error handling..?
    std::cout << "ERROR! xml_parser.cpp" << std::endl; 
  }
  return mapToReturn;
}

Component * XmlParser::handleComponent(xmlpp::Node * component, Component * parent)
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
        toReturn->addDescription(getAttributes(*child));
      }
      else
      {
        handleComponent(*child, toReturn);
      }
    }
  }
  
  return toReturn;
}

Component * XmlParser::loadComponent(xmlpp::Node * component, Component::EComponentSpecs spec)
{
  std::map<std::string, std::string> attributes = getAttributes(component);
  
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

void XmlParser::loadDataItem(xmlpp::Node * dataItem, Component * parent)
{
  // TODO: Error check attributes
  DataItem * d = new DataItem(getAttributes(dataItem));
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

void XmlParser::handleChildren(xmlpp::Node * components, Component * parent)
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
