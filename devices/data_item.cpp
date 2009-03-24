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

#include "data_item.hpp"

DataItem::DataItem(std::map<std::string, std::string> attributes)
{
  mId = atoi(attributes["id"].c_str());
  mName = attributes["name"];
  mType = attributes["type"];
  
  if (!attributes["subType"].empty())
  {
    mSubType = attributes["subType"];
  }
  
  mCategory = (attributes["category"] == "SAMPLE") ? SAMPLE : EVENT;
  
  if (!attributes["nativeUnits"].empty())
  {
    mNativeUnits = attributes["nativeUnits"];
  }
  
  if (!attributes["units"].empty())
  {
    mUnits = attributes["units"];
  }
  
  mNativeScale = (!attributes["nativeScale"].empty()) ? atof(attributes["nativeScale"].c_str()) : 0.0f;
  
  mSignificantDigits = (!attributes["significantDigits"].empty()) ? atoi(attributes["significantDigits"].c_str()) : 0;
  
  if (!attributes["coordinateSystem"].empty())
  {
    mCoordinateSystem = attributes["coordinateSystem"];
  }
  
  mComponent = NULL;
  mLatestEvent = NULL;
}

std::map<std::string, std::string> DataItem::getAttributes()
{
  std::map<std::string, std::string> attributes;
  
  attributes["id"] = Component::intToString(mId);
  attributes["name"] = mName;
  attributes["type"] = mType;
  
  if (!mSubType.empty())
  {
    attributes["subType"] = mSubType;
  }
  
  attributes["category"] = (mCategory == SAMPLE) ? "SAMPLE" : "EVENT";
  
  if (!mNativeUnits.empty())
  {
    attributes["nativeUnits"] = mNativeUnits;
  }
  
  if (!mUnits.empty())
  {
    attributes["units"] = mUnits;
  }
  
  attributes["nativeScale"] = Component::floatToString(mNativeScale);
  
  attributes["significantDigits"] = Component::intToString(mSignificantDigits);
  
  if (!mCoordinateSystem.empty())
  {
    attributes["coordinateSystem"] = mCoordinateSystem;
  }
  
  return attributes;
}


unsigned int DataItem::getId()
{
  return mId;
}

std::string DataItem::getName()
{
  return mName;
}

std::string DataItem::getSource()
{
  return mSource;
}

bool DataItem::hasName(std::string name)
{
  return mName == name || (!mSource.empty() && mSource == name);
}

std::string DataItem::getType()
{
  return mType;
}

std::string DataItem::getSubType()
{
  return mSubType;
}

DataItem::Category DataItem::getCategory()
{
  return mCategory;
}

bool DataItem::isSample()
{
  return mCategory == SAMPLE;
}

std::string DataItem::getNativeUnits()
{
  return mNativeUnits;
}

float DataItem::getNativeScale()
{
  return mNativeScale;
}

void DataItem::addSource(std::string source)
{
  mSource = source;
}

void DataItem::setComponent(Component * component)
{
  mComponent = component;
}

Component * DataItem::getComponent()
{
  return mComponent;
}

void DataItem::setLatestEvent(ComponentEvent * event)
{
  mLatestEvent = event;
}

ComponentEvent * DataItem::getLatestEvent()
{
  return mLatestEvent;
}
