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

#include <list>
#include <map>
#include <string>

#include <iostream>
#include <sstream>

#include "data_item.hpp"

// Forward Declaration
class DataItem;

class Component
{
public:
  
  /* String enumeration for component parts and details */
  enum EComponentSpecs
  {
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
  
  
protected:
  /* Unique ID for each component */
  unsigned int mId;
  
  /* Name for itself */
  std::string mName;
  
  /* uuid */
  std::string mUuid;
  
  /* Pointer to the parent component */
  Component * mParent;
  
  
  /* Each component keeps track of it's children in a list */
  std::list<Component *> mChildren;
  
  /* Keep Track of all the data items associated with this component */
  std::list<DataItem *> mDataItems;
  
  /* Description of itself */
  std::string mManufacturer;
  std::string mSerialNumber;
  std::string mStation;

public:
  static std::string intToString(unsigned int i);
  
  static std::string floatToString(float i);

public:
  /* Take in mapping of attributes */
  Component(std::map<std::string, std::string> attributes);
  
  virtual std::string getClass() = 0;
  
  virtual std::map<std::string, std::string> getAttributes();
  
  std::map<std::string, std::string> getDescription();
  
  /* Get the component ID */
  unsigned int getId();
  
  /* Get the component name */
  std::string getName();
  
  /* Get the component's parent component */
  Component * getParent();
  
  /* Get the component's list of children */
  std::list<Component *> getChildren();
  
  /* Add a child to component's list of children */
  void addChild(Component * child);
  
  /* Get the component's list of data items */
  std::list<DataItem *> getDataItems();
  
  /* Add a data item to component's list of data items */
  void addDataItem(DataItem * dataItem);
  
  /* Set the component's parent */
  void setParent(Component * parent);
  
  /* Add a description specifications from an attribute map */
  void addDescription(std::map<std::string, std::string> attributes);
  
public:
  /* Get the enumeration corresponding to the string */
  static Component::EComponentSpecs getComponentEnum(std::string name);
  
  /* Error checking to check attributes if there is a name & ID keys */
  static bool hasNameAndId(std::map<std::string, std::string> attributes);
};

#endif


