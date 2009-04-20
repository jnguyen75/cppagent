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

void ComponentEventTest::testConvertValue()
{
  std::map<std::string, std::string> attributes;
  attributes["id"] = "1";
  attributes["name"] = "DataItemTest1";
  attributes["type"] = "ACCELERATION";
  attributes["category"] = "SAMPLE";
  
  attributes["nativeUnits"] = "REVOLUTION/MINUTE";
  DataItem data1 (attributes);
  ComponentEvent event1 (&data1, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f, event1.getFValue());
  CPPUNIT_ASSERT(event1.getSValue().empty());
  
  attributes["nativeUnits"] = "REVOLUTION/SECOND";
  DataItem data2 (attributes);
  ComponentEvent event2 (&data2, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 60.0f, event2.getFValue());
  CPPUNIT_ASSERT(event2.getSValue().empty());
  
  attributes["nativeUnits"] = "GRAM/INCH";
  DataItem data3 (attributes);
  ComponentEvent event3 (&data3, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL((2.0f / 1000.0f) / 25.4f, event3.getFValue());
  CPPUNIT_ASSERT(event3.getSValue().empty());
  
  attributes["nativeUnits"] = "MILLIMETER/MINUTE^3";
  DataItem data4 (attributes);
  ComponentEvent event4 (&data4, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL((2.0f) / (60.0f * 60.0f * 60.0f), event4.getFValue());
  CPPUNIT_ASSERT(event4.getSValue().empty());
  
  attributes["nativeUnits"] = "MILLIMETER/MINUTE^3";
  attributes["nativeScale"] = "0.5";
  DataItem data5 (attributes);
  ComponentEvent event5 (&data5, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL((2.0f) / (60.0f * 60.0f * 60.0f * 0.5f),
    event5.getFValue());
  CPPUNIT_ASSERT(event5.getSValue().empty());
}

void ComponentEventTest::testConvertSimpleUnits()
{
  std::map<std::string, std::string> attributes;
  attributes["id"] = "1";
  attributes["name"] = "DataItemTest1";
  attributes["type"] = "ACCELERATION";
  attributes["category"] = "SAMPLE";
  
  attributes["nativeUnits"] = "INCH";
  DataItem data1 (attributes);
  ComponentEvent event1 (&data1, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 25.4f, event1.getFValue());
  CPPUNIT_ASSERT(event1.getSValue().empty());
  
  attributes["nativeUnits"] = "FOOT";
  DataItem data2 (attributes);
  ComponentEvent event2 (&data2, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 304.8f, event2.getFValue());
  CPPUNIT_ASSERT(event2.getSValue().empty());
  
  attributes["nativeUnits"] = "CENTIMETER";
  DataItem data3 (attributes);
  ComponentEvent event3 (&data3, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 10.0f, event3.getFValue());
  CPPUNIT_ASSERT(event3.getSValue().empty());
  
  attributes["nativeUnits"] = "DECIMETER";
  DataItem data4 (attributes);
  ComponentEvent event4 (&data4, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 100.0f, event4.getFValue());
  CPPUNIT_ASSERT(event4.getSValue().empty());
  
  attributes["nativeUnits"] = "METER";
  DataItem data5 (attributes);
  ComponentEvent event5 (&data5, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 1000.0f, event5.getFValue());
  CPPUNIT_ASSERT(event5.getSValue().empty());
  
  attributes["nativeUnits"] = "FAHRENHEIT";
  DataItem data6 (attributes);
  ComponentEvent event6 (&data6, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL((2.0f - 32.0f) * (5.0f / 9.0f), event6.getFValue());
  CPPUNIT_ASSERT(event6.getSValue().empty());
  
  attributes["nativeUnits"] = "POUND";
  DataItem data7 (attributes);
  ComponentEvent event7 (&data7, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 0.45359237f, event7.getFValue());
  CPPUNIT_ASSERT(event7.getSValue().empty());
  
  attributes["nativeUnits"] = "GRAM";
  DataItem data8 (attributes);
  ComponentEvent event8 (&data8, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f / 1000.0f, event8.getFValue());
  CPPUNIT_ASSERT(event8.getSValue().empty());
  
  attributes["nativeUnits"] = "RADIAN";
  DataItem data9 (attributes);
  ComponentEvent event9 (&data9, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 57.2957795f, event9.getFValue());
  CPPUNIT_ASSERT(event9.getSValue().empty());
  
  attributes["nativeUnits"] = "MINUTE";
  DataItem data10 (attributes);
  ComponentEvent event10 (&data10, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 60.0f, event10.getFValue());
  CPPUNIT_ASSERT(event10.getSValue().empty());
  
  attributes["nativeUnits"] = "HOUR";
  DataItem data11 (attributes);
  ComponentEvent event11 (&data11, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f * 3600.0f, event11.getFValue());
  CPPUNIT_ASSERT(event11.getSValue().empty());
  
  attributes["nativeUnits"] = "MILLIMETER";
  DataItem data12 (attributes);
  ComponentEvent event12 (&data12, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f, event12.getFValue());
  CPPUNIT_ASSERT(event12.getSValue().empty());
  
  attributes["nativeUnits"] = "PERCENT";
  DataItem data13 (attributes);
  ComponentEvent event13 (&data13, 123, "NOW", "2.0");
  CPPUNIT_ASSERT_EQUAL(2.0f, event13.getFValue());
  CPPUNIT_ASSERT(event13.getSValue().empty());
}

