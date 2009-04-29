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

#include "component_event.hpp"

/* ComponentEvent public static constants */
const std::string ComponentEvent::SSimpleUnits[NumSimpleUnits] = {
  "INCH",
  "FOOT",
  "CENTIMETER",
  "DECIMETER",
  "METER",
  "FAHRENHEIT",
  "POUND",
  "GRAM",
  "RADIAN",
  "MINUTE",
  "HOUR",
  "SECOND",
  "MILLIMETER",
  "LITER",
  "DEGREE",
  "KILOGRAM",
  "NEWTON",
  "CELSIUS",
  "REVOLUTION",
  "STATUS",
  "PERCENT",
  "NEWTON_MILLIMETER",
  "HERTZ"
};

/* ComponentEvent public methods */
ComponentEvent::ComponentEvent(DataItem * dataItem, unsigned int sequence, std::string time, std::string value)
{
  mDataItem = dataItem;
  mSequence = sequence;
  mTime = time;
  
  fValue = 0.0f;
  convertValue(value);
}

ComponentEvent::ComponentEvent(ComponentEvent& ce)
{
  std::map<std::string, std::string> attributes = ce.getAttributes();
  
  mDataItem = ce.getDataItem();
  
  mTime = attributes["timestamp"];
  mSequence = atoi(attributes["sequence"].c_str());
  
  mAlarmData = attributes["code"] + "|" + attributes["nativeCode"] + "|" +
    attributes["severity"] + "|" + attributes["state"];
  
  fValue = ce.getFValue();
  sValue = ce.getSValue();
}

ComponentEvent::~ComponentEvent()
{
}

std::map<std::string, std::string> ComponentEvent::getAttributes()
{
  std::map<std::string, std::string> attributes;
  
  attributes["dataItemId"] = intToString(mDataItem->getId());
  attributes["timestamp"] = mTime;
  attributes["subType"] = mDataItem->getSubType();
  attributes["name"] = mDataItem->getName();
  attributes["sequence"] = intToString(mSequence);
  
  if (getDataItem()->getType() == DataItem::ALARM)
  {
    std::istringstream toParse(mAlarmData);
    std::string token;
    
    getline(toParse, token, '|');
    attributes["code"] = token;
  
    getline(toParse, token, '|');
    attributes["nativeCode"] = token;
    
    getline(toParse, token, '|');
    attributes["severity"] = token;
    
    getline(toParse, token, '|');
    attributes["state"] = token;
  }
  
  return attributes;
}

DataItem * ComponentEvent::getDataItem() const
{
  return mDataItem;
}

float ComponentEvent::getFValue() const
{
  return fValue;
}

std::string ComponentEvent::getSValue() const
{
  return sValue;
}

/* ComponentEvent protected methods */
void ComponentEvent::convertValue(std::string value)
{
  // Check if the type is an alarm or if it doesn't have units
  if (mDataItem->getType() == DataItem::ALARM)
  {
    logEvent("AlarmData", value);
    std::string::size_type lastPipe = value.rfind('|');
    
    mAlarmData = value.substr(0, lastPipe);
    sValue = value.substr(lastPipe+1);
    return;
  }
  else if (mDataItem->getNativeUnits().empty())
  {
    sValue = value;
    return;
  }
  
  std::string units = mDataItem->getNativeUnits();
  std::string::size_type slashLoc = units.find('/');
  
  if (slashLoc == std::string::npos)
  {
    fValue = convertSimple(units, atof(value.c_str()));
  }
  else if (units == "REVOLUTION/MINUTE")
  {
    fValue = atof(value.c_str());
  }
  else
  {
    std::string numerator = units.substr(0, slashLoc);
    std::string denominator = units.substr(slashLoc+1);
    
    std::string::size_type carotLoc = denominator.find('^');
    
    if (numerator == "REVOLUTION" && denominator == "SECOND")
    {
      fValue = atof(value.c_str()) * 60.0f;
    }
    else if (carotLoc == std::string::npos)
    {
      fValue = convertSimple(numerator, atof(value.c_str())) / convertSimple(denominator, 1.0);
    }
    else
    {
      std::string unit = denominator.substr(0, carotLoc);
      std::string power = denominator.substr(carotLoc+1);
      
      double div = pow((double) convertSimple(unit, 1.0f),
        (double) atof(power.c_str()));
      fValue = convertSimple(numerator, atof(value.c_str())) / div;
    }
  }
  
  if (mDataItem->getNativeScale() != 0.0f)
  {
    fValue /= mDataItem->getNativeScale();
  }
}

float ComponentEvent::convertSimple(std::string units, float v)
{
  switch(getSimpleUnitsEnum(units))
  {
    case INCH:
      return v * 25.4f;
    case FOOT:
      return v * 304.8f;
    case CENTIMETER:
      return v * 10.0f;
    case DECIMETER:
      return v * 100.0f;
    case METER:
      return v * 1000.0f;
    case FAHRENHEIT:
      return (v - 32.0f) * (5.0f / 9.0f);
    case POUND:
      return v * 0.45359237f;
    case GRAM:
      return v / 1000.0f;
    case RADIAN:
      return v * 57.2957795f;
    case MINUTE:
      return v * 60.0f;
    case HOUR:
      return v * 3600.0f;
    
    case SECOND:
    case MILLIMETER:
    case LITER:
    case DEGREE:
    case KILOGRAM:
    case NEWTON:
    case CELSIUS:
    case REVOLUTION:
    case STATUS:
    case PERCENT:
    case NEWTON_MILLIMETER:
    case HERTZ:
    default:
      // or return value..?
      return v;
  }
}

/* ComponentEvent public static methods */
ComponentEvent::ESimpleUnits ComponentEvent::getSimpleUnitsEnum(std::string name)
{
  for (unsigned int i=0; i<ComponentEvent::NumSimpleUnits; i++)
  {
    if (name == ComponentEvent::SSimpleUnits[i])
    {
       return (ComponentEvent::ESimpleUnits) i;
    }
  }
  
  return (ComponentEvent::ESimpleUnits) -1;
}

