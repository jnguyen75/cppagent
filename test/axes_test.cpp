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

#include "axes_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(AxesTest);

void AxesTest::setUp()
{
  std::map<std::string, std::string> attributes1;
  attributes1["id"] = "3";
  attributes1["name"] = "AxesTestA";
  attributes1["uuid"] = "UniversalUniqueIdA";
  attributes1["sampleRate"] = "100.11";
  a = new Axes(attributes1);
  
  std::map<std::string, std::string> attributes2;
  attributes2["id"] = "5";
  attributes2["name"] = "AxesTestB";
  b = new Axes(attributes2);
}

void AxesTest::tearDown()
{
  delete a;
  delete b;
}

void AxesTest::testGetClass()
{
  CPPUNIT_ASSERT_EQUAL((std::string) "Axes", a->getClass());
  CPPUNIT_ASSERT_EQUAL((std::string) "Axes", b->getClass());
}

void AxesTest::testGetAttributes()
{
  std::map<std::string, std::string> attributes1 = a->getAttributes();
  CPPUNIT_ASSERT_EQUAL(attributes1["id"], (std::string) "3");
  CPPUNIT_ASSERT_EQUAL(attributes1["name"], (std::string) "AxesTestA");
  CPPUNIT_ASSERT_EQUAL(attributes1["uuid"], (std::string) "UniversalUniqueIdA");
  CPPUNIT_ASSERT_EQUAL(attributes1["sampleRate"], (std::string) "100.11");
  
  std::map<std::string, std::string> attributes2 = b->getAttributes();
  CPPUNIT_ASSERT_EQUAL((std::string) "AxesTestB", attributes2["name"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "5", attributes2["id"]);
  CPPUNIT_ASSERT(attributes2["sampleRate"].empty());
}

void AxesTest::testGetters()
{
  CPPUNIT_ASSERT_EQUAL((unsigned) 3, a->getId());
  CPPUNIT_ASSERT_EQUAL((std::string) "AxesTestA", a->getName());
  CPPUNIT_ASSERT_EQUAL((std::string) "UniversalUniqueIdA", a->getUuid());
  
  CPPUNIT_ASSERT_EQUAL((unsigned) 5, b->getId());
  CPPUNIT_ASSERT_EQUAL((std::string) "AxesTestB", b->getName());
  CPPUNIT_ASSERT(b->getUuid().empty());
}

void AxesTest::testDescription()
{
  std::map<std::string, std::string> getDescription = a->getDescription();
  CPPUNIT_ASSERT(getDescription.empty());
  
  std::map<std::string, std::string> addDescription;
  addDescription["manufacturer"] = "MANU";
  addDescription["serialNumber"] = "SERIAL";
  a->addDescription(addDescription);
  getDescription = a->getDescription();
  
  CPPUNIT_ASSERT_EQUAL((size_t) 2, getDescription.size());
  CPPUNIT_ASSERT_EQUAL((std::string) "MANU", getDescription["manufacturer"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "SERIAL", getDescription["serialNumber"]);
  CPPUNIT_ASSERT(getDescription["station"].empty());
  
  addDescription.clear();
  getDescription.clear();
  
  addDescription["serialNumber"] = "SERIAL";
  addDescription["station"] = "STAT";
  b->addDescription(addDescription);
  getDescription = b->getDescription();
  
  CPPUNIT_ASSERT_EQUAL((size_t) 2, getDescription.size());
  CPPUNIT_ASSERT(getDescription["manufacturer"].empty());
  CPPUNIT_ASSERT_EQUAL((std::string) "SERIAL", getDescription["serialNumber"]);
  CPPUNIT_ASSERT_EQUAL((std::string) "STAT", getDescription["station"]);
}

void AxesTest::testParentsAndDevice()
{
  CPPUNIT_ASSERT(a->getParent() == NULL);
  
  std::map<std::string, std::string> attributes;
  attributes["id"] = "15";
  attributes["name"] = "DeviceTest";
  attributes["iso841Class"] = "24";
  
  Device * d = new Device(attributes);
  
  a->setParent(b);
  b->setParent(d);
  
  CPPUNIT_ASSERT_EQUAL(b, dynamic_cast<Axes *>(a->getParent()));
  CPPUNIT_ASSERT_EQUAL(d, dynamic_cast<Device *>(b->getParent()));
  
  CPPUNIT_ASSERT_EQUAL(d, a->getDevice());
  CPPUNIT_ASSERT_EQUAL(d, b->getDevice());
  
  delete d;
  a->setParent(NULL);
  b->setParent(NULL);
}

void AxesTest::testChildren()
{
  CPPUNIT_ASSERT(a->getChildren().empty());
  
  std::map<std::string, std::string> attributes1, attributes2;
  
  attributes1["id"] = "7";
  attributes1["name"] = "child1";
  attributes1["uuid"] = "UniversalUniqueIdA";
  Axes * child1 = new Axes(attributes1);
  
  attributes2["id"] = "9";
  attributes2["name"] = "child2";
  attributes2["uuid"] = "UniversalUniqueIdB";
  Axes * child2 = new Axes(attributes2);
  
  a->addChild(child1);
  a->addChild(child2);
  
  CPPUNIT_ASSERT_EQUAL((size_t) 2, a->getChildren().size());
  CPPUNIT_ASSERT_EQUAL(child1, dynamic_cast<Axes *>(a->getChildren().front()));
  CPPUNIT_ASSERT_EQUAL(child2, dynamic_cast<Axes *>(a->getChildren().back()));
  
  delete child1;
  delete child2;
  a->getChildren().clear();
}

void AxesTest::testDataItems()
{
  CPPUNIT_ASSERT(a->getDataItems().empty());
  
  std::map<std::string, std::string> attributes1, attributes2;
  
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
  
  a->addDataItem(d1);
  a->addDataItem(d2);
  
  CPPUNIT_ASSERT_EQUAL((size_t) 2, a->getDataItems().size());
  CPPUNIT_ASSERT_EQUAL(d1, a->getDataItems().front());
  CPPUNIT_ASSERT_EQUAL(d2, a->getDataItems().back());
  
  delete d1;
  delete d2;
  a->getDataItems().clear();
}

void AxesTest::testGetComponentEnum()
{
  CPPUNIT_ASSERT_EQUAL(Component::AXES, Component::getComponentEnum("Axes"));
  CPPUNIT_ASSERT_EQUAL(Component::THERMOSTAT,
    Component::getComponentEnum("Thermostat"));
  CPPUNIT_ASSERT_EQUAL(Component::COMPONENTS,
    Component::getComponentEnum("Components"));
  CPPUNIT_ASSERT_EQUAL(Component::TEXT,
    Component::getComponentEnum("text"));
  CPPUNIT_ASSERT_EQUAL((Component::EComponentSpecs) -1,
    Component::getComponentEnum("NON_EXISTANT"));
}

