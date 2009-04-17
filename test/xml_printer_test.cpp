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

#include "xml_printer_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(XmlPrinterTest);

void XmlPrinterTest::setUp()
{
}

void XmlPrinterTest::tearDown()
{
}

void XmlPrinterTest::testInitXmlDoc()
{
  // Devices
  xmlpp::Document * initXml1 = XmlPrinter::initXmlDoc("Devices", 12345, 100000, 10);
  
  // Root
  xmlpp::Element * root1 = initXml1->get_root_node();
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "urn:mtconnect.com:MTConnectDevices:1.0",
    root1->get_attribute_value("xmlns:m"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "http://www.w3.org/2001/XMLSchema-instance",
    root1->get_attribute_value("xmlns:xsi"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "urn:mtconnect.com:MTConnectDevices:1.0",
    root1->get_attribute_value("xmlns"));
  CPPUNIT_ASSERT_EQUAL(
    (Glib::ustring) "urn:mtconnect.com:MTConnectDevices:1.0"
      + " /schemas/MTConnectDevices.xsd",
    root1->get_attribute_value("xsi:schemaLocation"));
  
  // Header
  xmlpp::Node::NodeList rootChildren1 = root1->get_children("Header");
  CPPUNIT_ASSERT_EQUAL((size_t) 1, rootChildren1.size());
  
  xmlpp::Element * header1 = dynamic_cast<xmlpp::Element *>(rootChildren1.front());
  CPPUNIT_ASSERT(header1);
  
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "localhost",
    header1->get_attribute_value("sender"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "12345",
    header1->get_attribute_value("instanceId"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "100000",
    header1->get_attribute_value("bufferSize"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "1.0",
    header1->get_attribute_value("version"));
  
  CPPUNIT_ASSERT(header1->get_attribute_value("nextSequence").empty());
  CPPUNIT_ASSERT(header1->get_attribute_value("firstSequence").empty());
  CPPUNIT_ASSERT(header1->get_attribute_value("lastSequence").empty());
  
  // Streams
  xmlpp::Document * initXml2 = XmlPrinter::initXmlDoc("Streams", 54321, 100000, 10, 1);
  
  // Root
  xmlpp::Element * root2 = initXml2->get_root_node();
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "urn:mtconnect.com:MTConnectStreams:1.0",
    root2->get_attribute_value("xmlns:m"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "http://www.w3.org/2001/XMLSchema-instance",
    root2->get_attribute_value("xmlns:xsi"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "urn:mtconnect.com:MTConnectStreams:1.0",
    root2->get_attribute_value("xmlns"));
  CPPUNIT_ASSERT_EQUAL(
    (Glib::ustring) "urn:mtconnect.com:MTConnectStreams:1.0"
      + " /schemas/MTConnectStreams.xsd",
    root2->get_attribute_value("xsi:schemaLocation"));
  
  // Header
  xmlpp::Node::NodeList rootChildren2 = root2->get_children("Header");
  CPPUNIT_ASSERT_EQUAL((size_t) 1, rootChildren2.size());
  
  xmlpp::Element * header2 = dynamic_cast<xmlpp::Element *>(rootChildren2.front());
  CPPUNIT_ASSERT(header2);
  
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "localhost",
    header2->get_attribute_value("sender"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "54321",
    header2->get_attribute_value("instanceId"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "100000",
    header2->get_attribute_value("bufferSize"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "1.0",
    header2->get_attribute_value("version"));
  
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "10",
    header2->get_attribute_value("nextSequence"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "1",
    header2->get_attribute_value("firstSequence"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "9",
    header2->get_attribute_value("lastSequence"));
}

void XmlPrinterTest::testPrintNode()
{
  xmlpp::Document * doc = new xmlpp::Document;
  
  xmlpp::Element * root = doc->create_root_node("Root");
  xmlpp::Element * child1 = root->add_child("Child1");
  xmlpp::Element * child2 = root->add_child("Child2");
  xmlpp::Element * grandChild = child2->add_child("GrandChild");
  
  child1->set_attribute("abc", "def");
  child1->set_attribute("ABC", "DEF");
  grandChild->set_child_text("TEXT!!!");
  
  std::string expected;
  
  expected += "<Root>\r\n";
  expected += "  <Child1 abc=\"def\" ABC=\"DEF\" />\r\n";
  expected += "  <Child2>\r\n";
  expected += "    <GrandChild>TEXT!!!</GrandChild>\r\n";
  expected += "  </Child2>\r\n";
  expected += "</Root>\r\n";
  
  CPPUNIT_ASSERT_EQUAL(expected, XmlPrinter::printNode(root));
}

void XmlPrinterTest::testPrintIndentation()
{
  CPPUNIT_ASSERT(XmlPrinter::printIndentation(0).empty());
  CPPUNIT_ASSERT_EQUAL((std::string) "  ",
    XmlPrinter::printIndentation(2));
  CPPUNIT_ASSERT_EQUAL((std::string) "    ",
    XmlPrinter::printIndentation(4));
  CPPUNIT_ASSERT_EQUAL((std::string) "      ",
    XmlPrinter::printIndentation(6));
  CPPUNIT_ASSERT_EQUAL((std::string) "        ",
    XmlPrinter::printIndentation(8));
  CPPUNIT_ASSERT_EQUAL((std::string) "                    ",
    XmlPrinter::printIndentation(20));
}

void XmlPrinterTest::testAddAttributes()
{
  xmlpp::Document * doc = new xmlpp::Document;
  
  xmlpp::Element * element = doc->create_root_node("Root");
  
  CPPUNIT_ASSERT(element->get_attribute_value("a").empty());
  CPPUNIT_ASSERT(element->get_attribute_value("key1").empty());
  CPPUNIT_ASSERT(element->get_attribute_value("key2").empty());
  
  std::map<std::string, std::string> attributes;
  attributes["a"] = "A";
  attributes["key1"] = "value1";
  attributes["key2"] = "value2";
  
  XmlPrinter::addAttributes(element, attributes);
  
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "A", element->get_attribute_value("a"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "value1",
    element->get_attribute_value("key1"));
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "value2",
    element->get_attribute_value("key2"));
  
  delete doc;
}

void XmlPrinterTest::testSearchParentForId()
{
  xmlpp::Document * doc = new xmlpp::Document;
  xmlpp::Element * root = doc->create_root_node("Root");
  
  xmlpp::Element * child1 = root->add_child("child1");
  xmlpp::Element * child2 = child1->add_child("child2");
  xmlpp::Element * child3 = child2->add_child("child3");
  xmlpp::Element * child4 = child3->add_child("child4");
  
  root->set_attribute("componentId", "1");
  child1->set_attribute("componentId", "2");
  child2->set_attribute("componentId", "3");
  child3->set_attribute("componentId", "4");
  child4->set_attribute("componentId", "5");
  
  std::list<xmlpp::Element *> elements;
  elements.push_back(child1);
  elements.push_back(child2);
  elements.push_back(child3);
  elements.push_back(child4);
  
  CPPUNIT_ASSERT_EQUAL(child1, XmlPrinter::searchParentsForId(elements, 1));
  CPPUNIT_ASSERT_EQUAL(child2, XmlPrinter::searchParentsForId(elements, 2));
  CPPUNIT_ASSERT_EQUAL(child3, XmlPrinter::searchParentsForId(elements, 3));
  CPPUNIT_ASSERT_EQUAL(child4, XmlPrinter::searchParentsForId(elements, 4));
  
  delete doc;
}

void XmlPrinterTest::testGetDeviceStream()
{
  xmlpp::Document * doc = new xmlpp::Document;
  xmlpp::Element * root = doc->create_root_node("Root");
  
  xmlpp::Element * deviceStream1 = root->add_child("DeviceStream");
  xmlpp::Element * deviceStream2 = root->add_child("DeviceStream");
  
  deviceStream1->set_attribute("name", "Device1");
  deviceStream2->set_attribute("name", "Device2");
  
  std::map<std::string, std::string> attributes1;
  attributes1["id"] = "3";
  attributes1["name"] = "Device1";
  attributes1["uuid"] = "UniversalUniqueIdA";
  attributes1["sampleRate"] = "100.11";
  attributes1["iso841Class"] = "12";
  Device * device1 = new Device(attributes1);
  
  std::map<std::string, std::string> attributes2;
  attributes2["id"] = "5";
  attributes2["name"] = "Device2";
  attributes2["uuid"] = "UniversalUniqueIdA";
  attributes2["sampleRate"] = "100.11";
  attributes2["iso841Class"] = "12";
  Device * device2 = new Device(attributes2);
  
  
  CPPUNIT_ASSERT_EQUAL((size_t) 2, root->get_children("DeviceStream").size());
  
  CPPUNIT_ASSERT_EQUAL(deviceStream1,
    XmlPrinter::getDeviceStream(root, device1));
  CPPUNIT_ASSERT_EQUAL(deviceStream2,
    XmlPrinter::getDeviceStream(root, device2));
  
  std::map<std::string, std::string> attributes3;
  attributes3["id"] = "5";
  attributes3["name"] = "Device3";
  attributes3["uuid"] = "UniversalUniqueIdA";
  attributes3["sampleRate"] = "100.11";
  attributes3["iso841Class"] = "12";
  Device * device3 = new Device(attributes3);
  
  xmlpp::Element * deviceStream3 = XmlPrinter::getDeviceStream(root, device3);
  
  CPPUNIT_ASSERT_EQUAL((size_t) 3, root->get_children("DeviceStream").size());
  CPPUNIT_ASSERT_EQUAL((Glib::ustring) "Device3",
    deviceStream3->get_attribute_value("name"));
}

void XmlPrinterTest::testPrintError()
{
  std::string expected;
  
  expected += "<MTConnectError xmlns:m=\"urn:mtconnect.com:MTConnectError:1.0";
  expected += "\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
  expected += "xmlns=\"urn:mtconnect.com:MTConnectError:1.0\" ";
  expected += "xsi:schemaLocation=\"urn:mtconnect.com:MTConnectError:1.0 ";
  expected += "/schemas/MTConnectError.xsd\">\r\n";
  expected += "  <Header creationTime=\"" + getCurrentTime() + "\" sender=\"";
  expected += "localhost\" instanceId=\"123\" bufferSize=\"9999\" ";
  expected += "version=\"1.0\" />\r\n";
  expected += "  <Error errorCode=\"ERROR_CODEE\">ERROR TEXT!</Error>\r\n";
  expected += "</MTConnectError>\r\n";
  
  CPPUNIT_ASSERT_EQUAL(expected,
    XmlPrinter::printError(123, 9999, 1, "ERROR_CODEE", "ERROR TEXT!"));
}

