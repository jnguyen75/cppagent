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

#include "agent_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(AgentTest);

void AgentTest::setUp()
{
  a = new Agent("../include/test.xml");
  agentId = intToString(getCurrentTimeInSec());
}

void AgentTest::tearDown()
{
  delete a;
}

void AgentTest::testConstructor()
{
  CPPUNIT_ASSERT_THROW(new Agent("../include/badPath.xml"), int);
  CPPUNIT_ASSERT_NO_THROW(new Agent("../include/test.xml"));
}

void AgentTest::testRequest()
{
  path = "/bad_path";
  
  bool response = a->on_request(path, result, queries, cookies, new_cookies, incoming_headers,
    response_headers, foreign_ip, local_ip, 123, 321, out);
  
  std::string expected;
  expected += "<MTConnectError xmlns:m=\"urn:mtconnect.com:MTConnectError:1.0\"";
  expected += " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"";
  expected += " xmlns=\"urn:mtconnect.com:MTConnectError:1.0\" xsi:";
  expected += "schemaLocation=\"urn:mtconnect.com:MTConnectError:1.0 /schemas/";
  expected += "MTConnectError.xsd\">\r\n";
  expected += "  <Header creationTime=\"" + getCurrentTime(false) + "\" sender=";
  expected += "\"localhost\" instanceId=\"" + agentId + "\" bufferSize=\"131072\" ";
  expected += "version=\"1.0\" />\r\n";
  expected += "  <Error errorCode=\"UNSUPPORTED\">The request was not one of ";
  expected += "the specified requests: bad_path</Error>\r\n";
  expected += "</MTConnectError>\r\n";
  
  CPPUNIT_ASSERT(response);
  CPPUNIT_ASSERT_EQUAL(expected, result);
}

void AgentTest::testAddAdapter()
{
  
}

