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

#define TIME_BUFFER_SIZE 80

#define MTCONNECT_XML_VERS "1.0"

#include <iostream>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <list>

#include "component_event.hpp"
#include "device.hpp"
#include "data_item.hpp"

// Include the LibXML Library
#include <libxml++/libxml++.h>

class XmlPrinter
{ 
protected:
  /* Pointer to the stream to write to XML */
  std::ostringstream * mXmlStream;
  
  xmlpp::Document * mErrorXml;
  
  xmlpp::Document * mSampleXml;
  
  xmlpp::Document * mProbeXml;
  
protected:
  void initErrorXml();
  
  void initProbeXml();
  
  void initSampleXml();
  
  xmlpp::Element * addRoot(xmlpp::Document * doc, std::string rootName, std::string xmlnsM);
  
  xmlpp::Element * addHeader(
    xmlpp::Document * doc,
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    unsigned int firstSeq = 0
  );
  
  void printProbeHelper(xmlpp::Element * element, Component * component);
  
  /* Simple helper function to put indentations into the XML stream */
  void printIndentation(unsigned int indentation);
  
  void getCurrentTime(char buffer[]);
  
  void addAttributes(xmlpp::Element * element, std::map<std::string, std::string> attributes);
  
  xmlpp::Element * searchListForElement(std::list<xmlpp::Element *> elements, unsigned int name);
  
  std::string intToString(unsigned int i);
  
  std::string floatToString(float i);
  
public:
  /* Constructor to set the pointer to the correct stringstream */
  XmlPrinter(std::ostringstream * xmlStream);
  
  /* Virtual destructor */
  virtual ~XmlPrinter();
  
  /* Function to parse and write XML */
  void printNode(const xmlpp::Node* node, unsigned int indentation = 0);
  
  void printError
  (
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    std::string errorCode,
    std::string errorText
  );
  
  void printProbe
  (
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    std::vector<Device *> devices
  );
  
  void printSample
  (
    unsigned int adapterId,
    unsigned int bufferSize,
    unsigned int nextSeq,
    unsigned int firstSeq,
    std::list<ComponentEvent *> results
  );
};

#endif
