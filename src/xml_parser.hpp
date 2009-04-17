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

#ifndef XML_PARSER_HPP
#define XML_PARSER_HPP

#include <iostream>
#include <list>

#include "component.hpp"

#include "axes.hpp"
#include "controller.hpp"
#include "device.hpp"
#include "data_item.hpp"
#include "linear.hpp"
#include "power.hpp"
#include "spindle.hpp"
#include "thermostat.hpp"

// Include the LibXML Library
#include <libxml++/libxml++.h>

class XmlParser
{
protected:
  /* LibXML++ XML DOM Parser */
  xmlpp::DomParser * mParser;
  
  /* Arrays to keep track of all devices and dataItems */
  std::list<Device *> mDevices;
  std::list<DataItem *> mDataItems;

protected:
  std::map<std::string, std::string> getAttributes(const xmlpp::Element * element);
  
  /* Main method to process the nodes and return the objects */
  Component * handleComponent(xmlpp::Node * component, Component * parent = NULL);
  
  /* Helper to handle/return each component of the device */
  Component * loadComponent(xmlpp::Node * node, Component::EComponentSpecs spec);
  
  /* Load the data items */
  void loadDataItem(xmlpp::Node * dataItems, Component * component);
  
  /* Helper method to perform loading on children and set up relationships */
  void handleChildren(xmlpp::Node * components, Component * parent = NULL);
  
public:
  /* Constructor to set the open the correct file */
  XmlParser(std::string xmlPath);
  
  /* Virtual destructor */
  virtual ~XmlParser();
  
  /* Get list of devices and data items */
  std::list<Device *> getDevices() const;
  std::list<DataItem *> getDataItems() const;
  
  xmlpp::Node * getRootNode() const;
};

#endif