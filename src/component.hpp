/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this std::list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this std::list of conditions and the following disclaimer in the
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

#include <sstream>
#include <string>
#include <list>
#include <map>

#include "globals.hpp"

class DataItem;
class Device;

class Component
{
public:
  /* std::string enumeration for component parts and details */
  enum EComponentSpecs
  {
    // Component parts
    DEVICE,
    // Component details
    COMPONENTS,
    DATA_ITEM,
    DATA_ITEMS,
    DESCRIPTION,
    SOURCE,
    TEXT
  };
  
  static const unsigned int NumComponentSpecs = 7;
  static const std::string SComponentSpecs[];
  
public:
  /* Take in a class name & mapping of attributes */
  Component(
    const std::string& cls,
    std::map<std::string, std::string> attributes
  );
  
  /* Virtual destructor */
  virtual ~Component();
  
  /* Return a map of attributes of all the component specs */
  virtual std::map<std::string, std::string> getAttributes() const;
  
  /* Return what part of the component it is */
  const std::string& getClass() const { return mClass; }
  
  /* Getter methods for the component ID/Name */
  std::string getId() const { return mId; }
  std::string getName() const { return mName; }
  std::string getUuid() const { return mUuid; }
  
  /* Add/get description specifications using an attribute map */
  void addDescription(std::map<std::string, std::string> attributes);
  std::map<std::string, std::string> getDescription() const;
  
  /* Get the device that any component is associated with */
  Device * getDevice() const;
  
  /* Set/Get the component's parent component */
  void setParent(Component& parent) { mParent = &parent; }
  Component * getParent() const { return mParent; }
  
  /* Add to/get the component's std::list of children */
  void addChild(Component& child) { mChildren.push_back(&child); }
  std::list<Component *> getChildren() { return mChildren; }
  
  /* Add to/get the component's std::list of data items */
  void addDataItem(DataItem& dataItem) { mDataItems.push_back(&dataItem); }
  std::list<DataItem *> getDataItems() const { return mDataItems; }
  
protected:
  /* Unique ID for each component */
  std::string mId;
  
  /* Name for itself */
  std::string mName;

  /* The class */
  std::string mClass;
  
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
  
  /* Each component keeps track of it's children in a std::list */
  std::list<Component *> mChildren;
  
  /* Keep Track of all the data items associated with this component */
  std::list<DataItem *> mDataItems;
};

#endif

