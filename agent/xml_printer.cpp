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
XmlPrinter::XmlPrinter(std::ostringstream * xmlStream)
{
  mXmlStream = xmlStream;
  try
  {
    initErrorXml();
    initProbeXml();
    initSampleXml();
  }
  catch (std::exception & e)
  {
    std::cout << "XML Exception: " << e.what() << std::endl;
  }
}

XmlPrinter::~XmlPrinter()
{
  delete mErrorXml;
  delete mProbeXml;
  delete mSampleXml;
}


void XmlPrinter::printNode(const xmlpp::Node* node, unsigned int indentation)
{
  // Constant node data determined for each node
  const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);
  const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node);
  
  bool hasChildren = (node->get_children().size() > 0);

  // Ignore empty whitespace
  if (nodeText and nodeText->is_white_space())
  {
    return;
  }
  
  Glib::ustring nodename = node->get_name();
  
  // Node name tag: i.e. "<tag"
  // Leave ">" out in case there are other attributes
  if (!nodeText and !nodeComment and !nodename.empty())
  {
    printIndentation(indentation);
    *mXmlStream << "<" << nodename;
  }
  
  // Right now, nothing is being done for conditions: if nodeContent/nodeComment
  if (nodeText)
  {
    printIndentation(indentation);
    *mXmlStream << nodeText->get_content() << std::endl;
  }
  else if (nodeElement)
  {
    // Print attributes for the element
    // i.e. ...attribute1="value1" attribute2="value2"...
    const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
    for (xmlpp::Element::AttributeList::const_iterator iter=attributes.begin();
          iter != attributes.end();
          ++iter
        )
    {
      const xmlpp::Attribute* attribute = *iter;
      *mXmlStream << " " << attribute->get_name() << "=\"" << attribute->get_value() << "\"";
    }
    
    //std::string endTag = (nodeElement->has_child_text() or indentation == 0) ? ">" : " />";
    
    std::string endTag = (hasChildren) ? ">" : " />";
    
    // TODO: TAKE CARE OF <TAG>\nTEXT\n</TAG>
    *mXmlStream << endTag << std::endl;
  }
  
  // If node does NOT have content, then it may have children, so perform print on children
  if (!nodeContent)
  {
    // Recurse through child nodes
    xmlpp::Node::NodeList list = node->get_children();
    for (xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
    {
      //if (!dynamic_cast<const xmlpp::TextNode*>(*iter))
      //{
        printNode(*iter, indentation + 2);
      //}
    }
  }
  
  // Close off xml tag, i.e. </tag>
  if (!nodeText and !nodeComment and !nodename.empty() and hasChildren)// or indentation == 0)
  {
    printIndentation(indentation);
    *mXmlStream << "</" << nodename << ">" << std::endl;
  }
}

void XmlPrinter::printError(
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    std::string errorCode,
    std::string errorText
  )
{  
  xmlpp::Element * header = addHeader(mErrorXml, adapterId, bufferSize, nextSeq);
  
  xmlpp::Element * error = mErrorXml->get_root_node()->add_child("Error");
  error->set_attribute("errorCode", errorCode);
  error->set_child_text(errorText);
  
  printNode(mErrorXml->get_root_node());
  
  mErrorXml->get_root_node()->remove_child(header);
  mErrorXml->get_root_node()->remove_child(error);
}

void XmlPrinter::printProbe(
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    std::list<Device *> deviceList
  )
{  
  xmlpp::Element * header = addHeader(mProbeXml, adapterId, bufferSize, nextSeq);
  
  xmlpp::Element * devices = mProbeXml->get_root_node()->add_child("Devices");
  
  std::list<Device *>::iterator d;
  for (d=deviceList.begin(); d!=deviceList.end(); d++ )
  {
    xmlpp::Element * device = devices->add_child("Device");
    printProbeHelper(device, *d);
  }
  
  printNode(mProbeXml->get_root_node());
  
  mProbeXml->get_root_node()->remove_child(header);
  mProbeXml->get_root_node()->remove_child(devices);
}

void XmlPrinter::printCurrent(
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    unsigned int firstSeq,
    std::list<DataItem *> dataItems
  )
{
  xmlpp::Element * header = addHeader(mSampleXml, adapterId, bufferSize, nextSeq, firstSeq);
    
  xmlpp::Element * streams = mSampleXml->get_root_node()->add_child("Streams");
  
  xmlpp::Element * deviceStream = streams->add_child("DeviceStream");
  
  std::list<xmlpp::Element *> elements;
  
  std::list<DataItem *>::iterator dataItem;
  for (dataItem=dataItems.begin(); dataItem!=dataItems.end(); dataItem++)
  {
    if ((*dataItem)->getLatestEvent() != NULL)
    {
      xmlpp::Element * element = searchListForElement(elements, (*dataItem)->getComponent()->getId());
      xmlpp::Element * child;
      
      if (element == NULL)
      {
        Component * component = (*dataItem)->getComponent();
        
        xmlpp::Element * componentStream = deviceStream->add_child("ComponentStream");
        
        componentStream->set_attribute("component", component->getClass());
        componentStream->set_attribute("name", component->getName());
        componentStream->set_attribute("componentId", Component::intToString(component->getId()));
        
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
        child->add_child_text(Component::floatToString((*dataItem)->getLatestEvent()->getFValue()));
      }
      else
      {
        child->add_child_text((*dataItem)->getLatestEvent()->getSValue());
      }
      
      addAttributes(child, (*dataItem)->getLatestEvent()->getAttributes());
    }
  }
  
  
  printNode(mSampleXml->get_root_node());
  
  // This will recursively remove and get rid of its children  
  mSampleXml->get_root_node()->remove_child(streams);
  mSampleXml->get_root_node()->remove_child(header);
}
void XmlPrinter::printSample(
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    unsigned int firstSeq,
    std::list<ComponentEvent *> results
  )
{
  
  xmlpp::Element * header = addHeader(mSampleXml, adapterId, bufferSize, nextSeq, firstSeq);
    
  xmlpp::Element * streams = mSampleXml->get_root_node()->add_child("Streams");
  
  xmlpp::Element * deviceStream = streams->add_child("DeviceStream");
  
  std::list<xmlpp::Element *> elements;
  
  std::list<ComponentEvent *>::iterator result;
  for (result=results.begin(); result!=results.end(); result++)
  {
    xmlpp::Element * element = searchListForElement(elements, (*result)->getDataItem()->getComponent()->getId());
    xmlpp::Element * child;
    
    if (element == NULL)
    {
      Component * component = (*result)->getDataItem()->getComponent();
      
      xmlpp::Element * componentStream = deviceStream->add_child("ComponentStream");
      
      componentStream->set_attribute("component", component->getClass());
      componentStream->set_attribute("name", component->getName());
      componentStream->set_attribute("componentId", Component::intToString(component->getId()));
      
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
      child->add_child_text(Component::floatToString((*result)->getFValue()));
    }
    else
    {
      child->add_child_text((*result)->getSValue());
    }
    
    addAttributes(child, (*result)->getAttributes());
  }
  
  printNode(mSampleXml->get_root_node());
  
  // This will recursively remove and get rid of its children  
  mSampleXml->get_root_node()->remove_child(streams);
  mSampleXml->get_root_node()->remove_child(header);
}

/* XmlPrinter Protected Methods */
void XmlPrinter::initErrorXml()
{
  mErrorXml = new xmlpp::Document;
  addRoot(mErrorXml, "MTConnectStreams", "urn:mtconnect.com:MTConnectError:0.9");
}

void XmlPrinter::initProbeXml()
{
  mProbeXml = new xmlpp::Document;
  addRoot(mProbeXml, "MTConnectDevices", "urn:mtconnect.com:MTConnectDevices:0.9");
}

void XmlPrinter::initSampleXml()
{
  mSampleXml = new xmlpp::Document;
  addRoot(mSampleXml, "MTConnectStreams", "urn:mtconnect.com:MTConnectStreams:0.9");
}

xmlpp::Element * XmlPrinter::addRoot(xmlpp::Document * doc, std::string rootName, std::string xmlnsM)
{
  xmlpp::Element * root = doc->create_root_node(rootName);
  
  root->set_attribute("xmlns:m", xmlnsM);
  root->set_attribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  root->set_attribute("xmlns", "urn:mtconnect.com:MTConnectError:1.0");
  root->set_attribute("xsi:schemaLocation", "urn:mtconnect.com:MTConnectError:1.0 /schemas/MTConnectError.xsd");
  
  return root;
}

xmlpp::Element * XmlPrinter::addHeader(
    xmlpp::Document * doc,
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    unsigned int firstSeq
  )
{
  char timeBuffer [TIME_BUFFER_SIZE];
  getCurrentTime(timeBuffer);
  
  xmlpp::Element * header = doc->get_root_node()->add_child("Header");
  header->set_attribute("creationTime", timeBuffer);
  header->set_attribute("sender", "localhost");
  header->set_attribute("instanceId", intToString(adapterId));
  header->set_attribute("bufferSize", intToString(bufferSize));
  header->set_attribute("version", MTCONNECT_XML_VERS);
  header->set_attribute("nextSequence", intToString(nextSeq));
  
  if (firstSeq > 0)
  {
    header->set_attribute("firstSequence", intToString(firstSeq));
    header->set_attribute("lastSequence", intToString(nextSeq - 1));
  }
  
  return header;
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

void XmlPrinter::printIndentation(unsigned int indentation)
{
  for(unsigned int i = 0; i < indentation; ++i)
  {
    *mXmlStream << " ";
  }
}

void XmlPrinter::getCurrentTime(char buffer[])
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = gmtime ( &rawtime );

  strftime (buffer, TIME_BUFFER_SIZE, "%Y-%m-%dT%H:%M:%S+00:00", timeinfo);
}

void XmlPrinter::addAttributes(xmlpp::Element * element, std::map<std::string, std::string> attributes)
{
  for (std::map<std::string, std::string>::iterator attribute=attributes.begin();
        attribute!=attributes.end();
        attribute++
      )
  {
    element->set_attribute(attribute->first, attribute->second);
  }
}

xmlpp::Element * XmlPrinter::searchListForElement(std::list<xmlpp::Element *> elements, unsigned int componentId)
{
  for
    (
      std::list<xmlpp::Element *>::iterator element=elements.begin();
      element != elements.end();
      element++
    )
  {
    if ((*element)->get_parent()->get_attribute_value("componentId") == Component::intToString(componentId))
    {
      return *element;
    }
  }
  
  return NULL;
}

std::string XmlPrinter::intToString(unsigned int i)
{
  std::ostringstream stm;
  stm << i;
  return stm.str();
}

std::string XmlPrinter::floatToString(float f)
{
  std::ostringstream stm;
  stm << f;
  return stm.str();
}

