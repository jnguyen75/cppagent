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
  a = new Agent("../include/test_config.xml");
  agentId = intToString(getCurrentTimeInSec());
}

void AgentTest::tearDown()
{
  delete a;
}

void AgentTest::testConstructor()
{
  CPPUNIT_ASSERT_THROW(new Agent("../include/badPath.xml"), int);
  CPPUNIT_ASSERT_NO_THROW(new Agent("../include/test_config.xml"));
}

void AgentTest::testBadPath()
{
  path = "/bad_path";
  bool response = a->on_request(path, result, queries, cookies, new_cookies, incoming_headers,
    response_headers, foreign_ip, local_ip, 123, 321, out);
  
  std::string pathError = getFile("../include/test_error.xml");
  fillErrorText(pathError, "The following path is invalid: " + path);
  fillAttribute(pathError, "errorCode", "UNSUPPORTED");
  fillAttribute(pathError, "instanceId", agentId);
  fillAttribute(pathError, "bufferSize", "131072");
  fillAttribute(pathError, "creationTime", getCurrentTime(GMT));
  
  CPPUNIT_ASSERT(response);
  CPPUNIT_ASSERT_EQUAL(pathError, result);
  
  path = "/bad/path/";
  response = a->on_request(path, result, queries, cookies, new_cookies, incoming_headers,
    response_headers, foreign_ip, local_ip, 123, 321, out);
  
  pathError = getFile("../include/test_error.xml");
  fillErrorText(pathError, "The following path is invalid: " + path);
  fillAttribute(pathError, "errorCode", "UNSUPPORTED");
  fillAttribute(pathError, "instanceId", agentId);
  fillAttribute(pathError, "bufferSize", "131072");
  fillAttribute(pathError, "creationTime", getCurrentTime(GMT));
  
  CPPUNIT_ASSERT(response);
  CPPUNIT_ASSERT_EQUAL(pathError, result);
  
  path = "/bad/path/blah";
  response = a->on_request(path, result, queries, cookies, new_cookies, incoming_headers,
    response_headers, foreign_ip, local_ip, 123, 321, out);
  
  pathError = getFile("../include/test_error.xml");
  fillErrorText(pathError, "The following path is invalid: " + path);
  fillAttribute(pathError, "errorCode", "UNSUPPORTED");
  fillAttribute(pathError, "instanceId", agentId);
  fillAttribute(pathError, "bufferSize", "131072");
  fillAttribute(pathError, "creationTime", getCurrentTime(GMT));
  
  CPPUNIT_ASSERT(response);
  CPPUNIT_ASSERT_EQUAL(pathError, result);
}

void AgentTest::testProbe()
{
  path = "/probe";
  bool response = a->on_request(path, result, queries, cookies, new_cookies, incoming_headers,
    response_headers, foreign_ip, local_ip, 123, 321, out);
  
  std::string pathError = getFile("../include/test_probe.xml");
  fillAttribute(pathError, "instanceId", agentId);
  fillAttribute(pathError, "bufferSize", "131072");
  fillAttribute(pathError, "creationTime", getCurrentTime(GMT));
  
  CPPUNIT_ASSERT(response);
  CPPUNIT_ASSERT_EQUAL(pathError, result);
  
  path = "/";
  response = a->on_request(path, result, queries, cookies, new_cookies, incoming_headers,
    response_headers, foreign_ip, local_ip, 123, 321, out);
  
  pathError = getFile("../include/test_probe.xml");
  fillAttribute(pathError, "instanceId", agentId);
  fillAttribute(pathError, "bufferSize", "131072");
  fillAttribute(pathError, "creationTime", getCurrentTime(GMT));
  
  CPPUNIT_ASSERT(response);
  CPPUNIT_ASSERT_EQUAL(pathError, result);
}



void AgentTest::testBadXPath()
{
  path = "/current";
  std::string key = "path";
  std::string value = "//////Linear";
  queries.add(key, value);
  
  bool response = a->on_request(path, result, queries, cookies, new_cookies, incoming_headers,
    response_headers, foreign_ip, local_ip, 123, 321, out);
  
  std::string pathError = getFile("../include/test_error.xml");
  fillErrorText(pathError, "The following path is invalid: " + path);
  fillAttribute(pathError, "errorCode", "UNSUPPORTED");
  fillAttribute(pathError, "instanceId", agentId);
  fillAttribute(pathError, "bufferSize", "131072");
  fillAttribute(pathError, "creationTime", getCurrentTime(GMT));
  
  CPPUNIT_ASSERT(response);
  CPPUNIT_ASSERT_EQUAL(pathError, result);
}

void AgentTest::testAddAdapter()
{
  
}

