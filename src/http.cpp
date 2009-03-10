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

#include "http.hpp"

/* HTTP public methods */

/* Overridden method that is called per web request */
void HTTP::on_request (
      const std::string& path,
      std::string& result,
      const map_type& queries,
      const map_type& cookies,
      queue_type& new_cookies,
      const map_type& incoming_headers,
      map_type& response_headers,
      const std::string& foreign_ip,
      const std::string& local_ip,
      unsigned short foreign_port,
      unsigned short local_port
    )
{
  try 
  {
    // Reset the XML output stream for each request
    xmlStream.str("");
    
    // Find the first position of '/'
    std::string::size_type loc1 = path.find( "/", 1 );
    
    // Set the ending position of the string, which also handles if end of string is a '/'
    std::string::size_type end = (path[path.length()-1] == '/') ? path.length()-1 : std::string::npos;
    
    // If a '/' was found
    if (loc1 < end)
    {
      // Look for another '/', should not find one unless it is the end of the path
      std::string::size_type loc2 = path.find( "/", loc1+1 );
      
      if (loc2 == end)
      {
        //handleDevicesAndPath(path.substr(1, loc1-1), path.substr(loc+1, loc2-loc-1));
      }
      else
      {
        printError("Routing Error", "The path '" + path + "' does not exist.");
      }
    }
    else
    {
      // Look in queries for device & path parameters
      std::string qDevice = (queries.is_in_domain("device")) ? queries["device"] : "";
      std::string qPath = (queries.is_in_domain("path")) ? queries["path"] : "";
      //handleDevicesAndPath(qDevice, qPath);
      
      std::string call = path.substr(1, loc1-1);
      
      if (call == "current")
      {
        handleCurrent();
      }
      else
      {
      
      }
    }
    
    // Output the XML stream into the browser
    result = xmlStream.str();
  }
  catch (std::exception & e)
  {
    std::cout << "Server Exception: " << e.what() << std::endl;
  }
}

void HTTP::handleDevicesAndPath(std::string device, std::string path)
{
  std::string dataPath = "";
  std::string deviceList = "";
  
  if (!device.empty()) // If there is a device
  {
    // Set the prefix
    std::string prefix = "//Devices/Device[@name=\"" + device + "\"]";
    if (!path.empty())
    {
      std::istringstream toParse(path);
      std::string token;
      
      while (std::getline(toParse, token, '|'))
      {
        dataPath += token + "|";
      }
      
      dataPath.erase(dataPath.length()-1);
    }
    else
    {
      dataPath = prefix;
    }
  }
  else
  {
    dataPath = (!path.empty()) ? path : "//Devices/Device";
  }
  
  // TODO: Clear_Devices
  // TODO: @@adapter.data_items(dataPath)
}

void HTTP::handleCurrent()
{
  try
  {
    XmlPrinter xmlPrinter("../include/current.xml", &xmlStream);
    xmlPrinter.printNode(xmlPrinter.getRootNode(), 0);
  }
  catch (std::exception & e)
  {
    std::cout << "XML Exception: " << e.what() << std::endl;
  }
}

bool HTTP::isBasicCall(std::string call)
{
  for (int i=0; i<NUM_BASIC_CALLS; i++)
  {
    if (call == basicCalls[i])
    {
      return true;
    }
  }
  return false;
}

void HTTP::printError(std::string errorCode, std::string text)
{
  // Error template
  xmlStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
  xmlStream << "<m:MTConnectStreams xsi:schemaLocation=\"urn:mtconnect.com:MTConnectError:0.9 /schemas/MTConnectError.xsd\">" << std::endl;
  xmlStream << "  <Header creationTime=\"2009-03-02T22:09:34+00:00\" sender=\"localhost\" instanceId=\"1235613755\" bufferSize=\"100000\" version=\"0.9\" nextSequence=\"\"/>" << std::endl;
  xmlStream << "  <Error errorCode=\"" << errorCode << "\">" << text << "</Error>" << std::endl;
  xmlStream << "</m:MTConnectStreams>" << std::endl;
}

/***** Procedural code ******/

// Create an instance of our web server
HTTP webServer;

void serverExitThread()
{
  // Wait for user to close server
  std::cout << "Press enter to end this program" << std::endl;
  std::cin.get();

  // Shut down server by unblocking HTTP::start()
  webServer.clear();
}

int main()
{
  try
  {
    // Create a thread that will listen for the user to end this program
    thread_function t(serverExitThread);
    
    // Start web server listen on specified port
    webServer.set_listening_port(SERVER_PORT);
    webServer.start();
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << std::endl;
  }
}
