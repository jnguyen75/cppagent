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

/* DataItem public static constants */
const std::string DataItem::STypeUpper[NumTypes] = {
  "ACCELERATION",
  "ALARM",
  "ANGLE",
  "ANGULAR_ACCELERATION",
  "BLOCK",
  "CODE",
  "DIRECTION",
  "EXECUTION",
  "PATH_FEEDRATE",
  "AXIS_FEEDRATE",
  "LINE",
  "LOAD",
  "CONTROLLER_MODE",
  "OTHER",
  "POSITION",
  "POWER_STATUS",
  "PRESSURE",
  "PROGRAM",
  "SPINDLE_SPEED",
  "STATUS",
  "TEMPERATURE",
  "TICK",
  "TRANSFER",
  "VELOCITY",
  "ANGULAR_VELOCITY"
};

const std::string DataItem::STypeCamel[NumTypes] = {
  "Acceleration",
  "Alarm",
  "Angle",
  "AngularAcceleration",
  "Block",
  "Code",
  "Direction",
  "Execution",
  "PathFeedrate",
  "AxisFeedrate",
  "Line",
  "Load",
  "ControllerMode",
  "Other",
  "Position",
  "PowerStatus",
  "Pressure",
  "Program",
  "SpindleSpeed",
  "Status",
  "Temperature",
  "Tick",
  "Transfer",
  "Velocity",
  "AngularVelocity"
};


/* DataItem public methods */
DataItem::DataItem(std::map<std::string, std::string> attributes)
{
  mId = attributes["id"];
  mName = attributes["name"];
  mType = (EType) getEnumeration(attributes["type"], STypeUpper, NumTypes);
  
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
  
  mNativeScale = (!attributes["nativeScale"].empty()) ?
    atof(attributes["nativeScale"].c_str()) : 0.0f;
  
  mSignificantDigits = (!attributes["significantDigits"].empty()) ?
    atoi(attributes["significantDigits"].c_str()) : 0;
  
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

std::map<std::string, std::string> DataItem::getAttributes() const
{
  std::map<std::string, std::string> attributes;
  
  attributes["id"] = mId;
  attributes["name"] = mName;
  attributes["type"] = STypeUpper[mType];
  
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
  
  attributes["nativeScale"] = floatToString(mNativeScale);
  
  attributes["significantDigits"] = intToString(mSignificantDigits);
  
  if (!mCoordinateSystem.empty())
  {
    attributes["coordinateSystem"] = mCoordinateSystem;
  }
  
  return attributes;
}

std::string DataItem::getTypeString(bool uppercase) const
{
  return (uppercase) ? STypeUpper[mType] : STypeCamel[mType];
}

bool DataItem::hasName(const std::string name)
{
  return mName == name or (!mSource.empty() and mSource == name);
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

