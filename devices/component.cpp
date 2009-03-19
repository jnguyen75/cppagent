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
  // Error checking..?
  mId = atoi(attributes["id"].c_str());
  mName = attributes["name"];
  
  mParent = NULL;
}

unsigned int Component::getId()
{
  return mId;
}

std::string Component::getName()
{
  return mName;
}

Component * Component::getParent()
{
  return mParent;
}

std::string Component::getManufacturer()
{
  return mManufacturer;
}

std::string Component::getSerialNumber()
{
  return mSerialNumber;
}

std::string Component::getStation()
{
  return mStation;
}

std::list<Component *> Component::getChildren()
{
  return mChildren;
}

void Component::addChild(Component * child)
{
  mChildren.push_back(child);
}

void Component::setParent(Component * parent)
{
  mParent = parent;
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

/* Component public static methods */
Component::EComponentSpecs Component::getComponentEnum(std::string name)
{
  for (unsigned int i=0; i<Component::NumComponentSpecs; i++)
  {
    if (name == Component::SComponentSpecs[i])
    {
       return (Component::EComponentSpecs) i;
    }
  }
  
  // TODO: Error/exception
}

bool Component::hasNameAndId(std::map<std::string, std::string> attributes)
{
  return !attributes["name"].empty() && !attributes["id"].empty();
}
