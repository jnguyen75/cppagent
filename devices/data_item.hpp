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
#include <iostream>

#include "component.hpp"

class Component;

class DataItem
{
public:
  enum Category
  {
    SAMPLE,
    EVENT
  };
  
  struct dataItemCmp
  {
    bool operator()( DataItem * d1, DataItem * d2 ) const {
      return d1->getId() == d2->getId();
    }
  };
  
  
protected: // TODO: CREATE ENUMERATIONS FOR EVERYTHING
  /* Unique ID for each component */
  unsigned int mId;
  
  /* Name for itself */
  std::string mName;
  
  /* Type of data item */
  std::string mType;
  
  /* Subtype of data item */
  std::string mSubType;
  
  /* Category of data item */
  Category mCategory;
  
  /* Native units of data item */
  std::string mNativeUnits;
  
  /* Units of data item */
  std::string mUnits;
  
  /* Native scale of data item */
  float mNativeScale;
  
  /* Sig figs of data item */
  unsigned int mSignificantDigits;
  
  /* Coordinate system of data item */
  std::string mCoordinateSystem;
  
  /* Extra source information of data item */
  std::string mSource;
  
  /* Component that data item is associated with */  
  Component * mComponent;
  
public:
  /* Construct a data item with appropriate attributes mapping */
  DataItem(std::map<std::string, std::string> attributes);
  
  std::map<std::string, std::string> getAttributes();
  
  /* Get ID of data item */
  unsigned int getId();
  
  /* Get name of data item */
  std::string getName();
  
  std::string getSource();
  
  /* Returns if data item has this name (or source name) */
  bool hasName(std::string name);
  
  /* Get type of data item */
  std::string getType();
  std::string getSubType();
  
  Category getCategory();
  
  bool isSample();
  
  /* Get native units of data item */
  std::string getNativeUnits();
  
  /* Get native scale of data item */
  float getNativeScale();
  
  /* Add a source (extra information) to data item */
  void addSource(std::string source);
  
  /* Set/get component that data item is associated with */
  void setComponent(Component * component);
  Component * getComponent();
};

#endif


