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

#include "globals_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(GlobalsTest);

void GlobalsTest::setUp()
{
}

void GlobalsTest::tearDown()
{
}

void GlobalsTest::testIntToString()
{
  CPPUNIT_ASSERT_EQUAL((std::string) "1234", intToString(1234));
  CPPUNIT_ASSERT_EQUAL((std::string) "0", intToString(0));
  CPPUNIT_ASSERT_EQUAL((std::string) "123456789", intToString(123456789));
  CPPUNIT_ASSERT_EQUAL((std::string) "1", intToString(1));
}

void GlobalsTest::testFloatToString()
{
  CPPUNIT_ASSERT_EQUAL((std::string) "1.234", floatToString(1.234));
  CPPUNIT_ASSERT_EQUAL((std::string) "0", floatToString(0.0));
  CPPUNIT_ASSERT_EQUAL((std::string) "0.123456", floatToString(.123456));
  CPPUNIT_ASSERT_EQUAL((std::string) "1", floatToString(1.0));
}

void GlobalsTest::testToUpperCase()
{
  CPPUNIT_ASSERT_EQUAL((std::string) "ABCDEF", toUpperCase("abcDef"));
  CPPUNIT_ASSERT_EQUAL((std::string) "A|B|CC|DDD", toUpperCase("a|b|CC|ddd"));
  CPPUNIT_ASSERT_EQUAL((std::string) "QWERTY.ASDF|", toUpperCase("qwerty.asdf|"));
}

void GlobalsTest::testIsNonNegativeInteger()
{
  CPPUNIT_ASSERT(isNonNegativeInteger("12345"));
  CPPUNIT_ASSERT(isNonNegativeInteger("123456789012345678901234567890"));
  CPPUNIT_ASSERT(isNonNegativeInteger("0"));
  CPPUNIT_ASSERT(!isNonNegativeInteger("-12345"));
  CPPUNIT_ASSERT(!isNonNegativeInteger("123456789012345678901234567890a"));
  CPPUNIT_ASSERT(!isNonNegativeInteger("123.45"));
}

void GlobalsTest::testTime()
{
  std::string time1 = getCurrentTime(GMT);
  std::string time2 = getCurrentTime(GMT);
  CPPUNIT_ASSERT_EQUAL(time1, time2);
  
  sleep(1);
  std::string time3 = getCurrentTime(GMT);
  CPPUNIT_ASSERT(time1 != time3);
  
  std::string time4 = getCurrentTime(GMT);
  std::string time5 = getCurrentTime(GMT);
  CPPUNIT_ASSERT_EQUAL(time4, time5);
  
  sleep(1);
  std::string time6 = getCurrentTime(GMT);
  CPPUNIT_ASSERT(time4 != time6);
  
  unsigned int time7 = getCurrentTimeInSec();
  unsigned int time8 = getCurrentTimeInSec();
  CPPUNIT_ASSERT_EQUAL(time7, time8);
  
  sleep(2);
  unsigned int time9 = getCurrentTimeInSec();
  CPPUNIT_ASSERT(time7 < time9);
}

