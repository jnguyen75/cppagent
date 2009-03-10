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

ComponentEvent::ComponentEvent(const char * item, unsigned int sequence, std::string time, void * value)
{
  /*if (item == "Alarm") // TODO
  {
    mItem = new Alarm(item);
  }
  else*/ if (item == "line")
  {
    mItem = new IntEvent(item);
    int valueToSet = *((int *) value);
    (dynamic_cast<IntEvent *>(mItem))->setValue(valueToSet);
  }
  else if (item == "power")
  {
    mItem = new Power(item);
  }
  else if (item == "execution")
  {
    mItem = new Execution(item);
  }
  else if (item == "mode")
  {
    mItem = new ControllerMode(item);
  }
  else
  {
    mItem = new Event(item);
  }
  
  
  mSequence = sequence;
  mTime = time;
  
  convertValue();
}

ComponentEvent::~ComponentEvent()
{
  delete mItem;
}

template <class T>
T ComponentEvent::getValue()
{
  return (dynamic_cast<IntEvent *>(mItem))->getValue();
}

/* ComponentEvent protected methods */
void ComponentEvent::convertValue()
{
  if (false) // TODO if data type is Alarm
  {
  
  }
  else
  {
    
  }
}

/*int main ()
{
  int value = 352;
  ComponentEvent * test = new ComponentEvent("line", 1, "today", &value);
  
  std::cout << test->getValue<int>() << std::endl;

  return 0;
}*/
