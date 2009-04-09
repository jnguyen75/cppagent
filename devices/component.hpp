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

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>

extern std::string intToString(unsigned int i);
extern std::string floatToString(float f);

class DataItem;
class Device;

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
    THERMOSTAT,
    // Component details
    COMPONENTS,
    DATA_ITEM,
    DATA_ITEMS,
    DESCRIPTION,
    SOURCE,
    TEXT
  };
  
  static const unsigned int NumComponentSpecs = 13;
  static const std::string SComponentSpecs[];
  
protected:
  /* Component specs */
  /* Unique ID for each component */
  unsigned int mId;
  
  /* Name for itself */
  std::string mName;
  
  /* Universal unique identifier */
  std::string mUuid;
  
  /* If receiving data, a sample rate is needed */
  float mSampleRate;
  
  /* Description of itself */
  std::string mManufacturer;
  std::string mSerialNumber;
  std::string mStation;
  
  /* Component relationships */
  /* Pointer to the parent component */
  Component * mParent;
  
  /* Each component keeps track of it's children in a list */
  std::list<Component *> mChildren;
  
  /* Keep Track of all the data items associated with this component */
  std::list<DataItem *> mDataItems;

public:
  /* Default constructor, used for testing */
  Component();
  
  /* Take in mapping of attributes */
  Component(std::map<std::string, std::string> attributes);
  
  /* Return what part of the component it is */
  virtual const std::string getClass() const = 0;
  
  /* Return a map of attributes of all the component specs */
  virtual std::map<std::string, std::string> getAttributes() const;
  
  /* Add/get description specifications using an attribute map */
  void addDescription(std::map<std::string, std::string> attributes);
  std::map<std::string, std::string> getDescription() const;
  
  /* Getter methods for the component ID/Name */
  unsigned int getId() const;
  std::string getName() const;
  std::string getUuid() const;
  
  /* Get the device that any component is associated with */
  Device * getDevice() const;
  
  /* Set/Get the component's parent component */
  void setParent(Component * parent);
  Component * getParent() const;
  
  /* Add to/get the component's list of children */
  void addChild(Component * child);
  std::list<Component *> getChildren() const;
  
  /* Add to/get the component's list of data items */
  void addDataItem(DataItem * dataItem);
  std::list<DataItem *> getDataItems() const;
  
public:
  /* Get the enumeration corresponding to the string */
  static Component::EComponentSpecs getComponentEnum(const std::string name);
};

#endif

