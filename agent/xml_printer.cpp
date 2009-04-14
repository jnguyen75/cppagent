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

#include "xml_printer.hpp"

/* XmlPrinter public methods */
std::string XmlPrinter::printError(
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    std::string errorCode,
    std::string errorText
  )
{
  xmlpp::Document * mErrorXml = initXmlDoc(
    "Error",
    instanceId,
    bufferSize,
    nextSeq
  );
  
  xmlpp::Element * error = mErrorXml->get_root_node()->add_child("Error");
  error->set_attribute("errorCode", errorCode);
  error->set_child_text(errorText);
  
  std::string toReturn = printNode(mErrorXml->get_root_node());
  
  delete mErrorXml;
  
  return toReturn;
}

std::string XmlPrinter::printProbe(
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    std::list<Device *> deviceList
  )
{
  xmlpp::Document * mProbeXml = initXmlDoc(
    "Devices",
    instanceId,
    bufferSize,
    nextSeq
  );
  
  xmlpp::Element * devices = mProbeXml->get_root_node()->add_child("Devices");
  
  std::list<Device *>::iterator d;
  for (d=deviceList.begin(); d!=deviceList.end(); d++ )
  {
    xmlpp::Element * device = devices->add_child("Device");
    printProbeHelper(device, *d);
  }
  
  std::string toReturn = printNode(mProbeXml->get_root_node());
  
  delete mProbeXml;
  
  return toReturn;
}

std::string XmlPrinter::printCurrent(
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    const unsigned int firstSeq,
    std::list<DataItem *> dataItems
  )
{
  xmlpp::Document * mSampleXml = initXmlDoc(
    "Streams",
    instanceId,
    bufferSize,
    nextSeq,
    firstSeq
  );
  
  xmlpp::Element * streams = mSampleXml->get_root_node()->add_child("Streams");
  
  std::list<xmlpp::Element *> elements;
  
  std::list<DataItem *>::iterator dataItem;
  for (dataItem=dataItems.begin(); dataItem!=dataItems.end(); dataItem++)
  {
    if ((*dataItem)->getLatestEvent() != NULL)
    {
      Component * component = (*dataItem)->getComponent();
      
      xmlpp::Element * deviceStream = getDeviceStream(streams,
        component->getDevice());
      
      xmlpp::Element * element = searchParentsForId(elements,
        component->getId());
      
      xmlpp::Element * child;
      
      if (element == NULL)
      {
        
        xmlpp::Element * componentStream =
          deviceStream->add_child("ComponentStream");
        
        componentStream->set_attribute("component", component->getClass());
        componentStream->set_attribute("name", component->getName());
        componentStream->set_attribute("componentId",
          intToString(component->getId()));
        
        bool sample = (*dataItem)->isSample();
        
        std::string dataName = (sample) ? "Samples" : "Events";
        
        xmlpp::Element * data = componentStream->add_child(dataName);
        
        elements.push_back(data);
        
        child = data->add_child((*dataItem)->getTypeString(false));
      }
      else
      {
        child = element->add_child((*dataItem)->getTypeString(false));
      }
      
      if ((*dataItem)->isSample())
      {
        child->add_child_text(floatToString((*dataItem)->getLatestEvent()->getFValue()));
      }
      else
      {
        child->add_child_text((*dataItem)->getLatestEvent()->getSValue());
      }
      
      addAttributes(child, (*dataItem)->getLatestEvent()->getAttributes());
    }
  }
  
  
  std::string toReturn = printNode(mSampleXml->get_root_node());
  
  delete mSampleXml;
  
  return toReturn;
}

std::string XmlPrinter::printSample(
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    const unsigned int firstSeq,
    std::list<ComponentEvent *> results
  )
{
  xmlpp::Document * mSampleXml = initXmlDoc(
    "Streams",
    instanceId,
    bufferSize,
    nextSeq,
    firstSeq
  );
    
  xmlpp::Element * streams = mSampleXml->get_root_node()->add_child("Streams");
  
  std::list<xmlpp::Element *> elements;
  
  std::list<ComponentEvent *>::iterator result;
  for (result=results.begin(); result!=results.end(); result++)
  {
    Component * component = (*result)->getDataItem()->getComponent();
    
    xmlpp::Element * element = searchParentsForId(elements, component->getId());
    xmlpp::Element * child;
    
    if (element == NULL)
    {
      
      xmlpp::Element * deviceStream = getDeviceStream(streams,
        component->getDevice());
      
      xmlpp::Element * componentStream = deviceStream->add_child("ComponentStream");
      
      componentStream->set_attribute("component", component->getClass());
      componentStream->set_attribute("name", component->getName());
      componentStream->set_attribute("componentId", intToString(component->getId()));
      
      bool sample = (*result)->getDataItem()->isSample();
      
      std::string dataName = (sample) ? "Samples" : "Events";
      
      xmlpp::Element * data = componentStream->add_child(dataName);
      
      elements.push_back(data);
      
      child = data->add_child((*result)->getDataItem()->getTypeString(false));
    }
    else
    {
      child = element->add_child((*result)->getDataItem()->getTypeString(false));
    }
    
    if ((*result)->getDataItem()->isSample())
    {
      child->add_child_text(floatToString((*result)->getFValue()));
    }
    else
    {
      child->add_child_text((*result)->getSValue());
    }
    
    addAttributes(child, (*result)->getAttributes());
  }
  
  std::string toReturn = printNode(mSampleXml->get_root_node());
  
  delete mSampleXml;
  
  return toReturn;
}

/* XmlPrinter Protected Methods */
xmlpp::Document * XmlPrinter::initXmlDoc(
    const std::string xmlType,
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    const unsigned int firstSeq
  )
{
  xmlpp::Document * doc = new xmlpp::Document;
  
  std::string rootName = "MTConnect" + xmlType;
  std::string xmlns = "urn:mtconnect.com:MTConnect" + xmlType + ":1.0";
  std::string xsi = "urn:mtconnect.com:MTConnect" + xmlType +
    ":1.0 /schemas/MTConnect" + xmlType + ".xsd";
  
  // Root
  xmlpp::Element * root = doc->create_root_node(rootName);
  root->set_attribute("xmlns:m", xmlns);
  root->set_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  root->set_attribute("xmlns", xmlns);
  root->set_attribute("xsi:schemaLocation", xsi);
  
  // Header
  xmlpp::Element * header = doc->get_root_node()->add_child("Header");
  header->set_attribute("creationTime", getCurrentTime());
  header->set_attribute("sender", "localhost");
  header->set_attribute("instanceId", intToString(instanceId));
  header->set_attribute("bufferSize", intToString(bufferSize));
  header->set_attribute("version", "1.0");
  header->set_attribute("nextSequence", intToString(nextSeq));
  
  if (firstSeq > 0)
  {
    header->set_attribute("firstSequence", intToString(firstSeq));
    //header->set_attribute("lastSequence", intToString(nextSeq - 1));
  }
  
  return doc;
}

std::string XmlPrinter::printNode(
    const xmlpp::Node* node,
    const unsigned int indentation
  )
{
  // Constant node data determined for each node
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
  
  xmlpp::Node::NodeList children = node->get_children();
  bool hasChildren = (children.size() > 0);
  
  std::string toReturn;

  // Ignore empty whitespace
  if (nodeText and nodeText->is_white_space())
  {
    return "";
  }
  
  Glib::ustring nodename = node->get_name();
  
  // Element node: i.e. "<element"
  if (!nodeText and !nodename.empty())
  {
    toReturn += printIndentation(indentation) + "<" + nodename;
  }
  
  if (nodeText)
  {
    toReturn += nodeText->get_content();
  }
  else if (nodeElement)
  {
    // Print attributes for the element: i.e. attribute1="value1"
    const xmlpp::Element::AttributeList& attributes =
      nodeElement->get_attributes();
    
    xmlpp::Element::AttributeList::const_iterator attribute;
    for (attribute=attributes.begin(); attribute!=attributes.end(); attribute++)
    {
      toReturn += " " + (*attribute)->get_name() +
        "=\"" + (*attribute)->get_value() + "\"";
    }
    
    toReturn += (hasChildren) ? ">" : " />";
    
    if (!nodeElement->has_child_text())
    {
      toReturn += "\r\n";
    }
  }
  
  // If node does NOT have content, then it may have children
  if (!dynamic_cast<const xmlpp::ContentNode*>(node))
  {
    xmlpp::Node::NodeList::iterator child;
    for (child=children.begin(); child!= children.end(); child++)
    {
      toReturn += printNode(*child, indentation + 2);
    }
  }
  
  // Close off xml tag, i.e. </tag>
  if (!nodeText and !nodename.empty() and hasChildren)// or indentation == 0)
  {
    if (!nodeElement->has_child_text())
    {
      toReturn += printIndentation(indentation);
    }
    toReturn += "</" + nodename + ">\r\n";
  }
  
  return toReturn;
}

void XmlPrinter::printProbeHelper(xmlpp::Element * element, Component * component)
{
  addAttributes(element, component->getAttributes());
    
  std::map<std::string, std::string> desc = component->getDescription();
  
  if (desc.size() > 0)
  {
    xmlpp::Element * description = element->add_child("Description");
    addAttributes(description, desc);
  }
  
  std::list<DataItem *> datum = component->getDataItems();
  if (datum.size() > 0)
  {
    xmlpp::Element * dataItems = element->add_child("DataItems");
    for (std::list<DataItem *>::iterator data=datum.begin();
          data!=datum.end();
          data++
        )
    {
      xmlpp::Element * dataItem = dataItems->add_child("DataItem");
      addAttributes(dataItem, (*data)->getAttributes());
      std::string source = (*data)->getSource();
      if (!source.empty())
      {
        xmlpp::Element * src = dataItem->add_child("Source");
        src->add_child_text(source);
      }
    }
  }

  std::list<Component *> children = component->getChildren();
  
  if (children.size() > 0)
  {
    xmlpp::Element * components = element->add_child("Components");
    for (std::list<Component *>::iterator child=children.begin();
          child!=children.end();
          child++
        )
    {
      xmlpp::Element * component = components->add_child((*child)->getClass());
      printProbeHelper(component, *child);
    }
  }
}

std::string XmlPrinter::printIndentation(unsigned int indentation)
{
  std::string indents;
  for(unsigned int i = 0; i < indentation; ++i)
  {
    indents += " ";
  }
  return indents;
}

void XmlPrinter::addAttributes(
    xmlpp::Element * element,
    std::map<std::string, std::string> attributes
  )
{
  std::map<std::string, std::string>::iterator attribute;
  for (attribute=attributes.begin(); attribute!=attributes.end(); attribute++)
  {
    element->set_attribute(attribute->first, attribute->second);
  }
}

xmlpp::Element * XmlPrinter::getDeviceStream(
    xmlpp::Element * element,
    Device * device
  )
{
  xmlpp::Node::NodeList children = element->get_children();
  xmlpp::Node::NodeList::iterator child;
  for (child=children.begin(); child!=children.end(); child++)
  {
    xmlpp::Element * nodeElement;
    nodeElement = dynamic_cast<xmlpp::Element *>(*child);
    
    if (nodeElement and
      nodeElement->get_attribute_value("name") == device->getName())
    {
      return nodeElement;
    }
  }
  
  // No element for device found, create a new one
  xmlpp::Element * deviceStream = element->add_child("DeviceStream");
  deviceStream->set_attribute("name", device->getName());
  deviceStream->set_attribute("uuid", device->getUuid());
  return deviceStream;
}

xmlpp::Element * XmlPrinter::searchParentsForId(
    std::list<xmlpp::Element *> elements,
    const unsigned int componentId
  )
{
  std::list<xmlpp::Element *>::iterator element;
  for (element=elements.begin(); element!=elements.end(); element++)
  {
    if ((*element)->get_parent()->get_attribute_value("componentId") ==
      intToString(componentId))
    {
      return *element;
    }
  }
  
  return NULL;
}

