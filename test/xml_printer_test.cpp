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
  config = new XmlParser("../samples/test_config.xml");
  devices = config->getDevices();
}

void XmlPrinterTest::tearDown()
{
  delete config;
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
  
  expected += "<Root>\n";
  expected += "  <Child1 abc=\"def\" ABC=\"DEF\" />\n";
  expected += "  <Child2>\n";
  expected += "    <GrandChild>TEXT!!!</GrandChild>\n";
  expected += "  </Child2>\n";
  expected += "</Root>\n";
  
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
  
  CPPUNIT_ASSERT_EQUAL(child1, XmlPrinter::searchParentsForId(elements, "1"));
  CPPUNIT_ASSERT_EQUAL(child2, XmlPrinter::searchParentsForId(elements, "2"));
  CPPUNIT_ASSERT_EQUAL(child3, XmlPrinter::searchParentsForId(elements, "3"));
  CPPUNIT_ASSERT_EQUAL(child4, XmlPrinter::searchParentsForId(elements, "4"));
  
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
  expected += "/schemas/MTConnectError.xsd\">\n";
  expected += "  <Header creationTime=\"" + getCurrentTime(GMT) + "\" sender=\"";
  expected += "localhost\" instanceId=\"123\" bufferSize=\"9999\" ";
  expected += "version=\"1.0\" />\n";
  expected += "  <Error errorCode=\"ERROR_CODE\">ERROR TEXT!</Error>\n";
  expected += "</MTConnectError>\n";
  
  CPPUNIT_ASSERT_EQUAL(expected,
    XmlPrinter::printError(123, 9999, 1, "ERROR_CODE", "ERROR TEXT!"));
}

void XmlPrinterTest::testPrintProbe()
{
  std::ifstream probeXml("../include/test_probe.xml");
  std::stringstream probeStream;
  probeStream << probeXml.rdbuf();
  std::string probeString = probeStream.str();
  
  fillAttribute(probeString, "instanceId", "123");
  fillAttribute(probeString, "bufferSize", "9999");
  fillAttribute(probeString, "creationTime", getCurrentTime(GMT));
  
  CPPUNIT_ASSERT_EQUAL(probeString,
    XmlPrinter::printProbe(123, 9999, 1, devices));
}

void XmlPrinterTest::testPrintCurrent()
{  
  std::ifstream currentXml("../include/test_current.xml");
  std::stringstream currentStream;
  currentStream << currentXml.rdbuf();
  std::string currentString = currentStream.str();
  
  DataItem * d;
  
  d = getDataItemById("6");
  std::string time("TIME");
  ComponentEvent e6 (*d, 10254804, time, (std::string) "0");
  d->setLatestEvent(e6);
  
  d = getDataItemById("25");
  ComponentEvent e25 (*d, 15, time, (std::string)  "100");
  d->setLatestEvent(e25);
  
  d = getDataItemById("12");
  ComponentEvent e12 (*d, 10254803, time, (std::string) "0");
  d->setLatestEvent(e12);
  
  d = getDataItemById("13");
  ComponentEvent e13 (*d, 16, time, (std::string) "100");
  d->setLatestEvent(e13);
  
  d = getDataItemById("14");
  ComponentEvent e14 (*d, 10254797, time, (std::string) "0.00199");
  d->setLatestEvent(e14);
  
  d = getDataItemById("15");
  ComponentEvent e15 (*d, 10254800, time, (std::string) "0.00199");
  d->setLatestEvent(e15);
  
  d = getDataItemById("16");
  ComponentEvent e16 (*d, 10254798, time, (std::string) "0.0002");
  d->setLatestEvent(e16);
  
  d = getDataItemById("17");
  ComponentEvent e17 (*d, 10254801, time, (std::string) "0.0002");
  d->setLatestEvent(e17);
  
  d = getDataItemById("18");
  ComponentEvent e18 (*d, 10254799, time, (std::string) "1");
  d->setLatestEvent(e18);
  
  d = getDataItemById("19");
  ComponentEvent e19 (*d, 10254802, time, (std::string) "1");
  d->setLatestEvent(e19);
  
  d = getDataItemById("20");
  ComponentEvent e20 (*d, 10254789, time, (std::string) "x-0.132010 y-0.158143");
  d->setLatestEvent(e20);
  
  d = getDataItemById("21");
  ComponentEvent e21 (*d, 13, time, (std::string) "AUTOMATIC");
  d->setLatestEvent(e21);
  
  d = getDataItemById("22");
  ComponentEvent e22 (*d, 10254796, time, (std::string) "0");
  d->setLatestEvent(e22);
  
  d = getDataItemById("23");
  ComponentEvent e23 (*d, 12, time, (std::string) "/home/mtconnect/simulator/spiral.ngc");
  d->setLatestEvent(e23);
  
  d = getDataItemById("24");
  ComponentEvent e24 (*d, 10254795, time, (std::string)  "READY");
  d->setLatestEvent(e24);
  
  d = getDataItemById("1");
  ComponentEvent e1 (*d, 1, time, (std::string) "ON");
  d->setLatestEvent(e1);
  
  fillAttribute(currentString, "instanceId", "123");
  fillAttribute(currentString, "bufferSize", "9999");
  fillAttribute(currentString, "creationTime", getCurrentTime(GMT));

  std::list<DataItem*> dataItems;
  std::map<std::string, DataItem*> dataItemsMap = devices.front()->getDeviceDataItems();  
  std::map<std::string, DataItem*>::iterator dataItem;
  for (dataItem = dataItemsMap.begin(); dataItem!=dataItemsMap.end(); dataItem++)
  {
    dataItems.push_back(dataItem->second);
  }
  
  CPPUNIT_ASSERT_EQUAL(currentString,
    XmlPrinter::printCurrent(123, 9999, 10254805, 10123733, dataItems));
}

void XmlPrinterTest::testPrintSample()
{
  std::ifstream sampleXml("../include/test_sample.xml");
  std::stringstream sampleStream;
  sampleStream << sampleXml.rdbuf();
  std::string sampleString = sampleStream.str();
  
  std::list<ComponentEvent *> results;
  DataItem * d;
  
  d = getDataItemById("17");
  std::string time("TIME");
  ComponentEvent e1(*d, 10843512, time, (std::string) "0.553472");
  results.push_back(&e1);
  
  d = getDataItemById("16");
  ComponentEvent e2 (*d, 10843514, time,  (std::string) "0.551123");
  results.push_back(&e2);
  
  d = getDataItemById("17");
  ComponentEvent e3 (*d, 10843516, time,  (std::string) "0.556826");
  results.push_back(&e3);
  
  d = getDataItemById("16");
  ComponentEvent e4 (*d, 10843518, "TIME",  (std::string) "0.55582");
  results.push_back(&e4);
  
  d = getDataItemById("17");
  ComponentEvent e5 (*d, 10843520, "TIME",  (std::string) "0.560181");
  results.push_back(&e5);
  
  d = getDataItemById("14");
  ComponentEvent e6 (*d, 10843513, "TIME",  (std::string) "-0.900624");
  results.push_back(&e6);
  
  d = getDataItemById("15");
  ComponentEvent e7 (*d, 10843515, "TIME",  (std::string) "-0.89692");
  results.push_back(&e7);
  
  d = getDataItemById("14");
  ComponentEvent e8 (*d, 10843517, "TIME",  (std::string) "-0.897574");
  results.push_back(&e8);
  
  d = getDataItemById("15");
  ComponentEvent e9 (*d, 10843519, "TIME",  (std::string) "-0.894742");
  results.push_back(&e9);
  
  d = getDataItemById("14");
  ComponentEvent e10 (*d, 10843521, time,  (std::string) "-0.895613");
  results.push_back(&e10);
  
  d = getDataItemById("22");
  ComponentEvent e11 (*d, 11351720, time,  (std::string) "229");
  results.push_back(&e11);
  
  d = getDataItemById("20");
  ComponentEvent e12 (*d, 11351726, time,  (std::string) "x-1.149250 y1.048981");
  results.push_back(&e12);
  
  fillAttribute(sampleString, "instanceId", "123");
  fillAttribute(sampleString, "bufferSize", "9999");
  fillAttribute(sampleString, "creationTime", getCurrentTime(GMT));
  
  CPPUNIT_ASSERT_EQUAL(sampleString,
    XmlPrinter::printSample(123, 9999, 10974584, 10843512, results));
}

DataItem * XmlPrinterTest::getDataItemById(const char *id)
{
  std::map<std::string, DataItem*> dataItems = devices.front()->getDeviceDataItems();  
  std::map<std::string, DataItem*>::iterator dataItem;
  for (dataItem = dataItems.begin(); dataItem!=dataItems.end(); dataItem++)
  {
    if ((dataItem->second)->getId() == id)
    {
      return (dataItem->second);
    }
  }
  return NULL;
}

