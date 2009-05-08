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

using namespace std;

/* DataItem public methods */
DataItem::DataItem(std::map<string, string> attributes) 
  : mHasNativeScale(false), mHasSignificantDigits(false)
{
  mId = attributes["id"];
  mName = attributes["name"];
  mType = attributes["type"];
  mCamelType = getCamelType(mType);
  
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

  if (!attributes["nativeScale"].empty())
  {
    mNativeScale = atof(attributes["nativeScale"].c_str());
    mHasNativeScale = true;
  }

  if (!attributes["significantDigits"].empty())
  {
    mSignificantDigits = atoi(attributes["significantDigits"].c_str());
    mHasSignificantDigits = true;
  }
  
  if (!attributes["coordinateSystem"].empty())
  {
    mCoordinateSystem = attributes["coordinateSystem"];
  }
  
  mComponent = NULL;
  mLatestEvent = NULL;
  
  mLatestEventLock = new dlib::mutex;
}

DataItem::~DataItem()
{
  if (mLatestEvent != NULL)
  {
    delete mLatestEvent;
  }
}

std::map<string, string> DataItem::getAttributes() const
{
  std::map<string, string> attributes;
  
  attributes["id"] = mId;
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

  if (mHasNativeScale)
  {
    attributes["nativeScale"] = floatToString(mNativeScale);
  }

  if (mHasSignificantDigits)
  {
    attributes["significantDigits"] = intToString(mSignificantDigits);
  }
  
  if (!mCoordinateSystem.empty())
  {
    attributes["coordinateSystem"] = mCoordinateSystem;
  }
  
  return attributes;
}

string DataItem::getTypeString(bool uppercase) const
{
  return (uppercase) ? mType : mCamelType;
}

bool DataItem::hasName(const string& name)
{
  return mName == name || (!mSource.empty() && mSource == name);
}

void DataItem::setLatestEvent(ComponentEvent& event)
{
  mLatestEventLock->lock();
  
  if (mLatestEvent != NULL)
  {
    delete mLatestEvent;
  }
  mLatestEvent = new ComponentEvent(event);
  
  mLatestEventLock->unlock();
}

ComponentEvent * DataItem::getLatestEvent() const
{
  mLatestEventLock->lock();
  ComponentEvent * toReturn = mLatestEvent;
  mLatestEventLock->unlock();
  
  return toReturn;
}

string DataItem::getCamelType(const string& aType)
{
  if (aType.empty())
  {
    return "";
  }
  
  string camel = aType;
  string::iterator second = camel.begin();
  second++;
  transform(second, camel.end(), second, ::tolower);

  string::iterator word = find(second, camel.end(), '_');
  while (word != camel.end())
  {
    camel.erase(word);
    camel.replace(word, word + 1, 1, ::toupper(*word));
    word = find(word, camel.end(), '_');
  }

  return camel;
}

