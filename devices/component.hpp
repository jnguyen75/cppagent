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

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

//#include <iostream>
#include <list>
#include <map>
#include <string>

#include <iostream>

class Component
{
public:
  
  enum EComponentSpecs {
    // Component parts
    AXES,
    CONTROLLER,
    DEVICE,
    LINEAR,
    POWER,
    SPINDLE,
    // Component details
    COMPONENTS,
    DATA_ITEM,
    DATA_ITEMS,
    DESCRIPTION,
    SOURCE,
    TEXT
  };
  
  static const unsigned int NumComponentSpecs = 12;
  static const std::string SComponentSpecs[];
  
  enum EDescriptions {
    MANUFACTURER,
    SERIAL_NUMBER
  };
  
  static const unsigned int NumDescriptions = 2;
  static const std::string SDescriptions[];
  
protected:
  /* Unique ID for each component */
  unsigned int mId;
  
  /* Name for itself */
  std::string mName;
  
  /* Keep track of host/port for each */
  std::string mHost;
  unsigned int mPort;
  
  /* The ID of the parent */
  unsigned int mParentId;
  
  /* Each component keeps track of it's children in a list */
  std::list<Component *> mChildren;
  
  /* Description of itself */
  std::string mManufacturer;
  std::string mSerialNumber;
  std::string mStation;

public:
  Component(std::map<std::string, std::string> attributes);
  
  unsigned int getId();
  
  unsigned int getParentId();
  
  std::string getName();
  
  std::string getManufacturer();
  
  std::list<Component *> getChildren();
  
  void addChild(Component * child);
  
  void setParentId(unsigned int id);
  
  void addDescription(std::map<std::string, std::string> attributes);
  
  Component * findById(unsigned int id);
  
public:
  static Component::EComponentSpecs getComponentEnum(std::string name);
  
  static bool hasNameAndId(std::map<std::string, std::string> attributes);
};

#endif
