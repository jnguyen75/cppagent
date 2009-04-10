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

#ifndef XML_PRINTER_HPP
#define XML_PRINTER_HPP

#include <map>
#include <list>

#include <libxml++/libxml++.h>

#include "component_event.hpp"
#include "device.hpp"

class DataItem;

extern std::string intToString(unsigned int i);
extern std::string floatToString(float i);
extern std::string getCurrentTime();

namespace XmlPrinter
{
  /***** Helper Methods *****/
  /* Initiate all documents */
  xmlpp::Document * initXmlDoc(
    std::string rootName,
    std::string xmlnsM,
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    const unsigned int firstSeq = 0
  );
  
  /* Function to parse and write XML */
  std::string printNode(
    const xmlpp::Node* node, 
    const unsigned int indentation = 0
  );
  
  void printProbeHelper(xmlpp::Element * element, Component * component);
  
  /* Simple helper function to put indentations into the XML stream */
  std::string printIndentation(const unsigned int indentation);
  
  void addAttributes(
    xmlpp::Element * element,
    std::map<std::string, std::string> attributes
  );
  
  xmlpp::Element * searchParentsForId(
    std::list<xmlpp::Element *> elements,
    const unsigned int name
  );
  
  xmlpp::Element * getDeviceStream(
    xmlpp::Element * element,
    Device * device
  );
  
  /***** Main methods to call *****/
  std::string printError
  (
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    std::string errorCode,
    std::string errorText
  );
  
  std::string printProbe
  (
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    std::list<Device *> devices
  );
  
  std::string printCurrent
  (
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    const unsigned int firstSeq,
    std::list<DataItem *> dataItems
  );
  
  std::string printSample
  (
    const unsigned int instanceId,
    const unsigned int bufferSize,
    const unsigned int nextSeq,
    const unsigned int firstSeq,
    std::list<ComponentEvent *> results
  );
};

#endif

