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

#include "data_item_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(DataItemTest);

void DataItemTest::setUp()
{
  std::map<std::string, std::string> attributes1, attributes2;
  
  attributes1["id"] = "1";
  attributes1["name"] = "DataItemTest1";
  attributes1["type"] = "ACCELERATION";
  attributes1["category"] = "SAMPLE";
  a = new DataItem(attributes1);
  
  attributes2["id"] = "3";
  attributes2["name"] = "DataItemTest2";
  attributes2["type"] = "ACCELERATION";
  attributes2["subType"] = "ACTUAL";
  attributes2["category"] = "EVENT";
  attributes2["nativeUnits"] = "REVOLUTION/MINUTE";
  attributes2["units"] = "REVOLUTION/MINUTE";
  attributes2["nativeScale"] = "1.0";
  attributes2["significantDigits"] = "1";
  attributes2["coordinateSystem"] = "testCoordinateSystem";
  b = new DataItem(attributes2);
}

void DataItemTest::tearDown()
{
  delete a;
  delete b;
}

void DataItemTest::testGetters()
{
  CPPUNIT_ASSERT_EQUAL((std::string) "1", a->getId());
  CPPUNIT_ASSERT_EQUAL((std::string) "DataItemTest1", a->getName());
  CPPUNIT_ASSERT_EQUAL(DataItem::ACCELERATION, a->getType());
  CPPUNIT_ASSERT_EQUAL((std::string) "ACCELERATION", a->getTypeString(true));
  CPPUNIT_ASSERT_EQUAL((std::string) "Acceleration", a->getTypeString(false));
  CPPUNIT_ASSERT(a->getSubType().empty());
  CPPUNIT_ASSERT(a->getNativeUnits().empty());
  CPPUNIT_ASSERT_EQUAL(0.0f, a->getNativeScale());
  
  CPPUNIT_ASSERT_EQUAL((std::string) "3", b->getId());
  CPPUNIT_ASSERT_EQUAL((std::string) "DataItemTest2", b->getName());
  CPPUNIT_ASSERT_EQUAL(DataItem::ACCELERATION, b->getType());
  CPPUNIT_ASSERT_EQUAL((std::string) "ACCELERATION", b->getTypeString(true));
  CPPUNIT_ASSERT_EQUAL((std::string) "Acceleration", b->getTypeString(false));
  CPPUNIT_ASSERT_EQUAL((std::string) "ACTUAL", b->getSubType());
  CPPUNIT_ASSERT_EQUAL((std::string) "REVOLUTION/MINUTE", b->getNativeUnits());
  CPPUNIT_ASSERT_EQUAL(1.0f, b->getNativeScale());
}

void DataItemTest::testGetAttributes()
{
  std::map<std::string, std::string> attributes1 = a->getAttributes();
  CPPUNIT_ASSERT_EQUAL((std::string) "1", attributes1["id"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "DataItemTest1", attributes1["name"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "ACCELERATION", attributes1["type"]);
  CPPUNIT_ASSERT(attributes1["subType"].empty());
  CPPUNIT_ASSERT(attributes1["nativeUnits"].empty());
  CPPUNIT_ASSERT(attributes1["getNativeScale"].empty());
  CPPUNIT_ASSERT(attributes1["coordinateSystem"].empty());
  
  std::map<std::string, std::string> attributes2 = b->getAttributes();
  CPPUNIT_ASSERT_EQUAL((std::string) "3", attributes2["id"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "DataItemTest2", attributes2["name"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "ACCELERATION", attributes2["type"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "ACTUAL", attributes2["subType"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "REVOLUTION/MINUTE",
    attributes2["nativeUnits"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "1", attributes2["nativeScale"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "testCoordinateSystem",
    attributes2["coordinateSystem"]);
}

void DataItemTest::testHasNameAndSource()
{
  CPPUNIT_ASSERT(a->hasName("DataItemTest1"));
  CPPUNIT_ASSERT(b->hasName("DataItemTest2"));
  
  CPPUNIT_ASSERT(a->getSource().empty());
  CPPUNIT_ASSERT(b->getSource().empty());
  
  CPPUNIT_ASSERT(!b->hasName("DataItemTest2Source"));
  b->addSource("DataItemTest2Source");
  CPPUNIT_ASSERT(b->hasName("DataItemTest2Source"));
  CPPUNIT_ASSERT_EQUAL((std::string) "DataItemTest2Source", b->getSource());
}

void DataItemTest::testIsSample()
{
  CPPUNIT_ASSERT(a->isSample());
  CPPUNIT_ASSERT(!b->isSample());
}

void DataItemTest::testComponent()
{
  std::map<std::string, std::string> attributes1;
  attributes1["id"] = "3";
  attributes1["name"] = "AxesTestA";
  attributes1["uuid"] = "UniversalUniqueIdA";
  attributes1["sampleRate"] = "100.11";
  Component * axes = new Component("Axes", attributes1);
  a->setComponent(*axes);
  
  CPPUNIT_ASSERT_EQUAL(axes, a->getComponent());
  delete axes;
}

void DataItemTest::testComponentEvent()
{
  CPPUNIT_ASSERT(a->getLatestEvent() == NULL);
  
  ComponentEvent event (*a, 1, (std::string) "10:30am Today", (std::string) "1.34");
  
  a->setLatestEvent(event);
  
  std::map<std::string, std::string> attributes1 = event.getAttributes();
  std::map<std::string, std::string> attributes2 =
    a->getLatestEvent()->getAttributes();
  
  CPPUNIT_ASSERT_EQUAL(attributes1["sequence"], attributes2["sequence"]);
}

void DataItemTest::testGetTypeEnum()
{
  CPPUNIT_ASSERT_EQUAL(DataItem::ACCELERATION,
    DataItem::getTypeEnum("ACCELERATION"));
  CPPUNIT_ASSERT_EQUAL(DataItem::POWER_STATUS,
    DataItem::getTypeEnum("POWER_STATUS"));
  CPPUNIT_ASSERT_EQUAL(DataItem::TRANSFER,
    DataItem::getTypeEnum("TRANSFER"));
  CPPUNIT_ASSERT_EQUAL(DataItem::ANGULAR_VELOCITY,
    DataItem::getTypeEnum("ANGULAR_VELOCITY"));
  
  CPPUNIT_ASSERT_EQUAL((DataItem::EType) -1,
    DataItem::getTypeEnum("NON_EXISTANT"));
}

