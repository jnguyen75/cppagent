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

#include "component_event_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(ComponentEventTest);

void ComponentEventTest::setUp()
{
  std::map<std::string, std::string> attributes1, attributes2;
  
  attributes1["id"] = "1";
  attributes1["name"] = "DataItemTest1";
  attributes1["type"] = "ACCELERATION";
  attributes1["category"] = "EVENT";
  d1 = new DataItem(attributes1);
  
  attributes2["id"] = "3";
  attributes2["name"] = "DataItemTest2";
  attributes2["type"] = "POSITION";
  attributes2["nativeUnits"] = "MILLIMETER";
  attributes2["subType"] = "ACTUAL";
  attributes2["category"] = "SAMPLE";
  d2 = new DataItem(attributes2);
  
  a = new ComponentEvent(d1, 2, "NOW", "ON");
  b = new ComponentEvent(d2, 4, "LATER", "1.1231");
}

void ComponentEventTest::tearDown()
{
  delete a;
  delete b;
  delete d1;
  delete d2;
}

void ComponentEventTest::testGetAttributes()
{
  std::map<std::string, std::string> attributes1, attributes2;
  
  attributes1 = a->getAttributes();
  CPPUNIT_ASSERT_EQUAL((std::string) "1", attributes1["dataItemId"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "NOW", attributes1["timestamp"]);
  CPPUNIT_ASSERT(attributes1["subType"].empty());
  CPPUNIT_ASSERT_EQUAL((std::string) "DataItemTest1", attributes1["name"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "2", attributes1["sequence"]);
  
  attributes2 = b->getAttributes();
  CPPUNIT_ASSERT_EQUAL((std::string) "3", attributes2["dataItemId"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "LATER", attributes2["timestamp"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "ACTUAL", attributes2["subType"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "DataItemTest2", attributes2["name"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "4", attributes2["sequence"]);
}

void ComponentEventTest::testGetDataItem()
{
  CPPUNIT_ASSERT_EQUAL(d1, a->getDataItem());
  CPPUNIT_ASSERT_EQUAL(d2, b->getDataItem());
}

void ComponentEventTest::testGetValue()
{
  CPPUNIT_ASSERT_EQUAL((std::string) "ON", a->getSValue());
  CPPUNIT_ASSERT_EQUAL(1.1231f, b->getFValue());
}

void ComponentEventTest::testGetSimpleUnits()
{
  CPPUNIT_ASSERT_EQUAL(ComponentEvent::INCH,
    ComponentEvent::getSimpleUnitsEnum("INCH"));
  CPPUNIT_ASSERT_EQUAL(ComponentEvent::RADIAN,
    ComponentEvent::getSimpleUnitsEnum("RADIAN"));
  CPPUNIT_ASSERT_EQUAL(ComponentEvent::HERTZ,
    ComponentEvent::getSimpleUnitsEnum("HERTZ"));
  
  CPPUNIT_ASSERT_EQUAL((ComponentEvent::ESimpleUnits) -1,
    ComponentEvent::getSimpleUnitsEnum("NON_EXISTANT"));
}

