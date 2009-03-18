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

#ifndef COMPONENT_EVENT_HPP
#define COMPONENT_EVENT_HPP

#include "../devices/data_item.hpp"

#include <string>
#include <iostream>
#include <cmath>

/* Component Event */
class ComponentEvent
{
public:
  enum ESimpleUnits
  {
    INCH,
    FOOT,
    CENTIMETER,
    DECIMETER,
    METER,
    FAHRENHEIT,
    POUND,
    GRAM,
    RADIAN,
    MINUTE,
    HOUR,
    SECOND, 
    MILLIMETER, 
    LITER,
    DEGREE,
    KILOGRAM,
    NEWTON,
    CELSIUS,
    REVOLUTION,
    STATUS,
    PERCENT,
    NEWTON_MILLIMETER,
    HERTZ
  };
  
  static const unsigned int NumSimpleUnits = 23;
  static const std::string SSimpleUnits[];
  
protected:
  /* Holds the data item from the device */
  DataItem * mDataItem;
  
  /* Sequence number of the event */
  unsigned int mSequence;
  
  /* Timestamp of the event's occurence */
  std::string mTime;
  
  /* The value of the event */
  float mValue;

protected:
  /* Convert the value to the agent unit standards */
  void convertValue(std::string value);
  
  /* Convert a simple value from native units to MTConnect units */
  float convertSimple(std::string units, float v);
  
  //void
  
public:
  /* Initialize the ComponentEvent with the type of event, sequence number, time and value */
  ComponentEvent(DataItem * dataItem, unsigned int sequence, std::string time, std::string value);
  
  /* Virtual destructor */
  ~ComponentEvent();
  
  /* Get value, whatever the value may be*/
  template <class T>
  T getValue();
  
public:
  static ComponentEvent::ESimpleUnits getSimpleUnitsEnum(std::string name);
};

#endif
