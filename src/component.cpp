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

using namespace std;

/* Component static constants */
const string Component::SComponentSpecs[NumComponentSpecs] = {
  // Component parts
  "Device",
  // Component details
  "Components",
  "DataItem",
  "DataItems",
  "Description",
  "Source",
  "text"
};

/* Component public methods */
Component::Component(const string& cls, std::map<string, string> attributes)
{
  mId = attributes["id"];
  mName = attributes["name"];
  
  mUuid = attributes["uuid"];
  mSampleRate = (float) (attributes["sampleRate"].empty()) ?
    0.0f : atof(attributes["sampleRate"].c_str());
  
  mParent = NULL;
  mClass = cls;
}

Component::~Component()
{
}

std::map<string, string> Component::getAttributes() const
{
  std::map<string, string> attributes;
  
  attributes["id"] = mId;
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

void Component::addDescription(std::map<string, string> attributes)
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

std::map<string, string> Component::getDescription() const
{
  std::map<string, string> description;
  
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

Device * Component::getDevice() const
{
  if (getClass() == "Device")
  {
    return (Device *) (this);
  }
  else if (mParent != NULL)
  {
    return mParent->getDevice();
  }
  else // Should never get here
  {
    return NULL;
  }
}

