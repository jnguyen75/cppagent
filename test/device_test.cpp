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

#include "device_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(DeviceTest);

void DeviceTest::setUp()
{
  std::map<std::string, std::string> attributes1;
  attributes1["id"] = "3";
  attributes1["name"] = "DeviceTestA";
  attributes1["uuid"] = "UniversalUniqueIdA";
  attributes1["sampleRate"] = "100.11";
  attributes1["iso841Class"] = "12";
  
  a = new Device(attributes1);
  
  std::map<std::string, std::string> attributes2;
  attributes2["id"] = "5";
  attributes2["name"] = "DeviceTestB";
  attributes2["uuid"] = "UniversalUniqueIdB";
  attributes2["iso841Class"] = "24";
  
  b = new Device(attributes2);
}

void DeviceTest::tearDown()
{
  delete a;
  delete b;
}

void DeviceTest::testGetAttributes()
{
  std::map<std::string, std::string> attributes1 = a->getAttributes();
  CPPUNIT_ASSERT_EQUAL((std::string) "3", attributes1["id"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "DeviceTestA", attributes1["name"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "UniversalUniqueIdA", attributes1["uuid"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "100.11", attributes1["sampleRate"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "12", attributes1["iso841Class"]);
  
  std::map<std::string, std::string> attributes2 = b->getAttributes();
  CPPUNIT_ASSERT_EQUAL((std::string) "5", attributes2["id"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "DeviceTestB", attributes2["name"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "UniversalUniqueIdB", attributes2["uuid"]);
  CPPUNIT_ASSERT(attributes2["sampleRate"].empty());
  CPPUNIT_ASSERT_EQUAL((std::string) "24", attributes2["iso841Class"]);
}

void DeviceTest::testGetClass()
{
  CPPUNIT_ASSERT_EQUAL((std::string) "Device", a->getClass());
  CPPUNIT_ASSERT_EQUAL((std::string) "Device", b->getClass());
}

void DeviceTest::testDescription()
{
  std::map<std::string, std::string> getDescription, addDescription;
  
  getDescription = a->getDescription();
  CPPUNIT_ASSERT(getDescription.empty());
  
  addDescription["manufacturer"] = "MANU";
  addDescription["serialNumber"] = "SERIAL";
  a->addDescription(addDescription);
  getDescription = a->getDescription();
  
  CPPUNIT_ASSERT_EQUAL(getDescription.size(), (size_t) 2);
  CPPUNIT_ASSERT_EQUAL(getDescription["manufacturer"], (std::string) "MANU");
  CPPUNIT_ASSERT_EQUAL(getDescription["serialNumber"], (std::string) "SERIAL");
  CPPUNIT_ASSERT(getDescription["station"].empty());
  
  addDescription["station"] = "STAT";
  a->addDescription(addDescription);
  getDescription = a->getDescription();
  CPPUNIT_ASSERT_EQUAL(getDescription.size(), (size_t) 3);
  CPPUNIT_ASSERT_EQUAL(getDescription["manufacturer"], (std::string) "MANU");
  CPPUNIT_ASSERT_EQUAL(getDescription["serialNumber"], (std::string) "SERIAL");
  CPPUNIT_ASSERT_EQUAL(getDescription["station"], (std::string) "STAT");
}

void DeviceTest::testParents()
{
  CPPUNIT_ASSERT(a->getParent() == NULL);
  CPPUNIT_ASSERT(a->getDevice() == a);
}

void DeviceTest::testChildren()
{
  std::map<std::string, std::string> attributes1, attributes2;
  
  attributes1["id"] = "7";
  attributes1["name"] = "child1";
  attributes1["uuid"] = "UniversalUniqueIdA";
  
  Device * d1 = new Device(attributes1);
  
  attributes2["id"] = "9";
  attributes2["name"] = "child2";
  attributes2["uuid"] = "UniversalUniqueIdB";
  
  Device * d2 = new Device(attributes2);
  
  CPPUNIT_ASSERT(a->getChildren().empty());
  
  a->addChild(*d1);
  a->addChild(*d2);
  
  CPPUNIT_ASSERT_EQUAL((size_t) 2, a->getChildren().size());
  CPPUNIT_ASSERT_EQUAL(d1, dynamic_cast<Device *>(a->getChildren().front()));
  CPPUNIT_ASSERT_EQUAL(d2, dynamic_cast<Device *>(a->getChildren().back()));
}

void DeviceTest::testDataItems()
{
  std::map<std::string, std::string> attributes1, attributes2;
  
  CPPUNIT_ASSERT(a->getDataItems().empty());
  
  attributes1["id"] = "1";
  attributes1["name"] = "DataItem1";
  attributes1["type"] = "ACCELERATION";
  attributes1["category"] == "SAMPLE";
  
  DataItem * d1 = new DataItem(attributes1);
  
  attributes2["id"] = "2";
  attributes2["name"] = "DataItem2";
  attributes2["type"] = "ACCELERATION";
  attributes2["category"] == "SAMPLE";
    
  DataItem * d2 = new DataItem(attributes2);
  
  a->addDataItem(*d1);
  a->addDataItem(*d2);
  
  CPPUNIT_ASSERT_EQUAL((size_t) 2, a->getDataItems().size());
  CPPUNIT_ASSERT_EQUAL(d1, a->getDataItems().front());
  CPPUNIT_ASSERT_EQUAL(d2, a->getDataItems().back());
  
  delete d1;
  delete d2;
}

