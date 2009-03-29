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

#include "component.hpp"

/* Component static constants */
const std::string Component::SComponentSpecs[NumComponentSpecs] = {
  // Component parts
  "Axes",
  "Controller",
  "Device",
  "Linear",
  "Power",
  "Spindle",
  // Component details
  "Components",
  "DataItem",
  "DataItems",
  "Description",
  "Source",
  "text"
};

/* Component public methods */
Component::Component(std::map<std::string, std::string> attributes)
{
  // TODO: Error checking..?
  mId = atoi(attributes["id"].c_str());
  mName = attributes["name"];
  
  mUuid = attributes["uuid"];
  mSampleRate = (attributes["sampleRate"].empty()) ? 0.0f : atof(attributes["sampleRate"].c_str());
  
  mParent = NULL;
}

std::map<std::string, std::string> Component::getAttributes()
{
  std::map<std::string, std::string> attributes;
  
  attributes["id"] = intToString(mId);
  attributes["name"] = mName;
  
  if (mSampleRate != 0.0f)
  {
    attributes["sampleRate"] = floatToString(mSampleRate);
  }
  
  if (!mUuid.empty())
  {
    attributes["uuid"] = mUuid;
  }
  
  return attributes;
}

void Component::addDescription(std::map<std::string, std::string> attributes)
{
  if (!attributes["manufacturer"].empty())
  {
    mManufacturer = attributes["manufacturer"];
  }
  
  if (!attributes["serialNumber"].empty())
  {
    mSerialNumber = attributes["serialNumber"];
  }
  
  if (!attributes["station"].empty())
  {
    mStation = attributes["station"];
  }
}

std::map<std::string, std::string> Component::getDescription()
{
  std::map<std::string, std::string> description;
  
  if (!mManufacturer.empty())
  {
    description["manufacturer"] = mManufacturer;
  }
  
  if (!mSerialNumber.empty())
  {
    description["serialNumber"] = mSerialNumber;
  }
  
  if (!mStation.empty())
  {
    description["station"] = mStation;
  }
  
  return description;
}

unsigned int Component::getId()
{
  return mId;
}

std::string Component::getName()
{
  return mName;
}

void Component::setParent(Component * parent)
{
  mParent = parent;
}

Component * Component::getParent()
{
  return mParent;
}

void Component::addChild(Component * child)
{
  mChildren.push_back(child);
}

std::list<Component *> Component::getChildren()
{
  return mChildren;
}

void Component::addDataItem(DataItem * dataItem)
{
  mDataItems.push_back(dataItem);
}

std::list<DataItem *> Component::getDataItems()
{
  return mDataItems;
}  

/* Component public static methods */
std::string Component::intToString(unsigned int i)
{
  std::ostringstream stm;
  stm << i;
  return stm.str();
}

std::string Component::floatToString(float f)
{
  std::ostringstream stm;
  stm << f;
  return stm.str();
}


Component::EComponentSpecs Component::getComponentEnum(std::string name)
{
  for (unsigned int i=0; i<Component::NumComponentSpecs; i++)
  {
    if (name == Component::SComponentSpecs[i])
    {
      return (Component::EComponentSpecs) i;
    }
  }
  
  return (Component::EComponentSpecs) -1;
}

bool Component::hasNameAndId(std::map<std::string, std::string> attributes)
{
  return !attributes["name"].empty() and !attributes["id"].empty();
}
