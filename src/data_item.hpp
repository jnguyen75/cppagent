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

#ifndef DATA_ITEM_HPP
#define DATA_ITEM_HPP

#include <map>

#include "dlib/threads.h"

#include "component.hpp"
#include "component_event.hpp"
#include "globals.hpp"

class ComponentEvent;

class DataItem
{
public:
  /* Enumeration for data item category */
  enum ECategory
  {
    SAMPLE,
    EVENT
  };
  
public:
  /* Construct a data item with appropriate attributes mapping */
  DataItem(std::map<std::string, std::string> attributes);
  
  /* Destructor */
  ~DataItem();
  
  /* Get a map of all the attributes of this data item */
  std::map<std::string, std::string> getAttributes() const;
  
  /* Getter methods for data item specs */
  std::string getId() const { return mId; }
  std::string getName() const { return mName; }
  std::string getSource() const { return mSource; }
  std::string getType() const { return mType; }
  std::string getTypeString(bool uppercase) const;
  std::string getSubType() const { return mSubType; }
  std::string getNativeUnits() const { return mNativeUnits; }
  float getNativeScale() const { return mNativeScale; }
  
  /* Returns if data item has this attribute */
  bool hasName(const std::string& name);
  bool hasNativeScale() const { return mHasNativeScale; }
  
  /* Add a source (extra information) to data item */
  void addSource(const std::string& source) { mSource = source; }
  
  /* Returns true if data item is a sample */
  bool isSample() const { return mCategory == SAMPLE; }
  
  /* Set/get component that data item is associated with */
  void setComponent(Component& component) { mComponent = &component; }
  Component * getComponent() const { return mComponent; }
  
  /* Set/get latest component event the data item is associated with */
  void setLatestEvent(ComponentEvent& event);
  ComponentEvent * getLatestEvent() const;

  /* Get the name for the adapter feed */
  std::string getSourceOrName() { return mSource.empty() ? mName : mSource; }
  
  /* Transform a name to camel casing */
  static std::string getCamelType(const std::string& aType);
  
protected:
  /* Unique ID for each component */
  std::string mId;
  
  /* Name for itself */
  std::string mName;
  
  /* Type of data item */
  std::string mType;
  std::string mCamelType;
  
  /* Subtype of data item */
  std::string mSubType;
  
  /* Category of data item */
  ECategory mCategory;
  
  /* Native units of data item */
  std::string mNativeUnits;
  
  /* Units of data item */
  std::string mUnits;
  
  /* Native scale of data item */
  float mNativeScale;
  bool mHasNativeScale;
  
  /* Sig figs of data item */
  unsigned int mSignificantDigits;
  bool mHasSignificantDigits;
  
  /* Coordinate system of data item */
  std::string mCoordinateSystem;
  
  /* Extra source information of data item */
  std::string mSource;
  
  /* Component that data item is associated with */  
  Component * mComponent;
  
  /* Pointer to the latest component event that occured for this data item */
  ComponentEvent * mLatestEvent;
  
  /* Lock to update and retrieve the latest event */
  dlib::mutex * mLatestEventLock;
};

#endif

