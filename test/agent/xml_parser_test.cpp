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

#include "xml_parser_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(XmlParserTest);

void XmlParserTest::setUp()
{
  mParser = new XmlParser("../include/test.xml");
  
  a = new xmlpp::Element;
  a->set_attribute("attribute1", "value1");
  a->set_attribute("attribute2", "value2");
  
  b = new xmlpp::Element;
  a->set_attribute("attribute3", "value3");
  a->set_attribute("attribute4", "value4");
}

void XmlParserTest::tearDown()
{
  delete mParser;
  
  delete a;
  delete b;
}

void XmlParserTest::testConstructor()
{
  XmlParser * badParser, * goodParser;
  try
  {
    badParser = new XmlParser("../include/testt.xml");
    CPPUNIT_ASSERT_FAIL("");
  }
  catch (std::exception & e)
  {
  }
  
  try
  {
    goodParser = new XmlParser("../include/testt.xml");
  }
  catch (std::exception & e)
  {
    CPPUNIT_ASSERT_FAIL("");
  }
  
  delete badParser;
  delete goodParser;
}

void XmlParserTest::testGetAttributes()
{
  std::map<std::string, std::string> attributes1, attributes2;
  
  attributes1 = getAttributes(a);
  CPPUNIT_ASSERT_EQUAL(attributes1.size(), (size_t) 2);
  CPPUNIT_ASSERT_EQUAL(attributes1["attribute1"], (std::string) "value1");
  CPPUNIT_ASSERT_EQUAL(attributes1["attribute2"], (std::string) "value2");
  
  attributes2 = getAttributes(b);
  CPPUNIT_ASSERT_EQUAL(attributes2.size(), (size_t) 2);
  CPPUNIT_ASSERT_EQUAL(attributes2["attribute3"], (std::string) "value3");
  CPPUNIT_ASSERT_EQUAL(attributes2["attribute4"], (std::string) "value4");
}

void XmlParserTest::testGetters()
{
  std::list<Device *> devices = mParser->getDevices();
  std::list<DataItem *> dataItems = mParser->getDataItems();
  mParser->getRootNode();
}

