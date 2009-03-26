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
HTTP::HTTP()
{
  mXmlPrinter = new XmlPrinter(&mXmlStream);
}

HTTP::~HTTP()
{
  delete mXmlPrinter;
}

/* Overridden method that is called per web request */
void HTTP::on_request(
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
    mXmlStream.str("");
    
    // Find the first position of '/'
    std::string::size_type loc1 = path.find( "/", 1 );
    
    // Set the ending position of the string, which also handles if end of string is a '/'
    std::string::size_type end = (path[path.length()-1] == '/') ? path.length()-1 : std::string::npos;
    
    // If a '/' was found
    if (loc1 < end)
    {
      // Look for another '/', should not find one unless it is the end of the path
      std::string::size_type loc2 = path.find( "/", loc1+1);
      
      if (loc2 == end)
      {
        //handleDevicesAndPath(path.substr(1, loc1-1), path.substr(loc+1, loc2-loc-1));
      }
      else
      {
        std::cout << "Routing Error1" << std::endl;
        printError("ROUTING_ERROR", "The path '" + path + "' does not exist.");
      }
    }
    else
    {
      
      std::string call = path.substr(1, loc1-1);
      
      if (call == "current")
      {
        handleCurrent(queries);
      }
      else if (call == "probe")
      {
        handleProbe(queries);
      }
      else if (call == "sample")
      {
        handleSample(queries);
      }
      else
      {
        std::cout << "Routing Error2" << std::endl;
        printError("ROUTING_ERROR", "The path '" + path + "' does not exist.");
      }
    }
    
    // Output the XML stream into the browser
    result = mXmlStream.str();
  }
  catch (std::exception & e)
  {
    std::cout << "Server Exception: " << e.what() << std::endl;
  }
}

void HTTP::addAdapter(std::string server, unsigned int port, std::string configXmlPath)
{
  mAdapters.push_back(new Adapter(server, port, configXmlPath));
}

/* HTTP protected methods */
std::list<DataItem *> HTTP::devicesAndPath(const map_type& queries)
{
  std::string dataPath = "";
  
  if (queries.is_in_domain("device"))
  {
    std::string deviceName = queries["device"];
    
    // Set the prefix
    std::string prefix = "//Devices/Device[@name=\"" + deviceName + "\"]";
    
    if (queries.is_in_domain("path"))
    {
      std::istringstream toParse(queries["path"]);
      std::string token;
      
      // Prefix each part of the path:
      // i.e. "path1|path2" => "{prefix}path1|{prefix}path2"
      while (std::getline(toParse, token, '|'))
      {
        dataPath += prefix + token + "|";
      }
      
      // Get rid of last extraneous '|'
      dataPath.erase(dataPath.length()-1);
    }
    else
    {
      dataPath = prefix;
    }
    
    // TODO: ADAPTER
    std::list<Adapter *>::iterator first = mAdapters.begin();
    Device * device = (*first)->getDeviceByName(deviceName);
    //mDeviceList.push_back(device);
  }
  else
  {
    
    dataPath = (queries.is_in_domain("path")) ? queries["path"] : "//Devices/Device";
    // TODO: ADAPTER
    std::list<Adapter *>::iterator adapter;
    for (adapter=mAdapters.begin(); adapter!=mAdapters.end(); adapter++)
    {
      std::list<Device *> adapterDevices = (*adapter)->getDevices();
      //mDeviceList.merge(adapterDevices);
    }
  }
  
  // TODO: Clear_Devices..?
  // TODO: ADAPTER
  std::list<Adapter *>::iterator first = mAdapters.begin();
  return (*first)->getDataItems(dataPath);
}

Device * HTTP::findDeviceByName(std::string name)
{
  for (unsigned int i=0; i<mAdapters.size(); i++)
  {
    // TODO: ADAPTER
    std::list<Adapter *>::iterator first = mAdapters.begin();
    std::list<Device *> devices = (*first)->getDevices();
    
    std::list<Device *>::iterator device;
    for (device=devices.begin(); device!=devices.end(); device++)
    {
      if ((*device)->getName() == name)
      {
        return *device;
      }
    }
  }
  
  return NULL;
}

void HTTP::clearDevices()
{
  
}

void HTTP::fetchData(std::list<DataItem *> dataItems, bool current, unsigned int start, unsigned int count)
{
  //names
  //sender
  unsigned int seq, firstSeq;
  if (current)
  {
    std::list<Adapter *>::iterator first = mAdapters.begin();
    //std::list<DataItem *> dataItems = (*first)->getDataItems();
    
    (*first)->current(&seq, &firstSeq, "");
    
    mXmlPrinter->printCurrent(1, Adapter::SLIDING_BUFFER_SIZE, seq, firstSeq, dataItems);
  }
  else
  {
    std::list<Adapter *>::iterator first = mAdapters.begin();
    std::list<Device *> devices = (*first)->getDevices();
    
    std::list<ComponentEvent *> results = (*first)->sample(&seq, &firstSeq, start, count, "");
    mXmlPrinter->printSample(1, Adapter::SLIDING_BUFFER_SIZE, seq, firstSeq, results);
  }
}

void HTTP::handleCurrent(const map_type& queries)
{
  unsigned int seq, firstSeq;
  
  // TODO: ADAPTER
  std::list<Adapter *>::iterator first = mAdapters.begin();
  (*first)->current(&seq, &firstSeq);
  
  std::list<DataItem *> dataItems = devicesAndPath(queries);
  
  //mXmlPrinter->printSample(1, Adapter::SlidingBufferSize, seq, firstSeq, mAdapters[0]->getDevices());
  if (queries.is_in_domain("frequency"))
  {
    //stream
  }
  else
  {
    std::string path = "";
    fetchData(dataItems, true);
  }
}

void HTTP::handleProbe(const map_type& queries)
{
  if (queries.is_in_domain("device"))
  {
    // Find queries["device"]
    Device * device = findDeviceByName(queries["device"]);
    if (device == NULL)
    {
      printError("NO_DEVICE", "Could not find the device '" + queries["device"] + "'");
      return;
    }
    else
    {
      mDeviceList.push_back(device);
    }
  }
  else
  {
    // TODO: GET ALL DEVICES
    std::list<Adapter *>::iterator first = mAdapters.begin();
    mDeviceList = (*first)->getDevices();
  }
  
  unsigned int seq, firstSeq;
  
  std::list<Adapter *>::iterator first = mAdapters.begin();
  (*first)->current(&seq, &firstSeq);
  mXmlPrinter->printProbe(1, Adapter::SLIDING_BUFFER_SIZE, seq, mDeviceList);
  
  mDeviceList.clear();
}

void HTTP::handleSample(const map_type& queries)
{
  unsigned int count = (queries.is_in_domain("count")) ? atoi(queries["count"].c_str()) : 100;
  std::list<DataItem *> dataItems = devicesAndPath(queries);
  
  if (queries.is_in_domain("frequency"))
  {
    // TODO
  }
  else
  {
    unsigned int start;
    if (queries.is_in_domain("start"))
    {
      start = atoi(queries["start"].c_str());
    }
    else if (queries.is_in_domain("from"))
    {
      start = atoi(queries["from"].c_str());
    }
    else
    {
      start = 0;
    }
    
    fetchData(dataItems, false, start, count);
  }
}

void HTTP::printError(std::string errorCode, std::string text)
{
  mXmlPrinter->printError(1, Adapter::SLIDING_BUFFER_SIZE, 2, errorCode, text);
}

/***** Procedural Code *****/
void terminateServerThread(HTTP * server)
{
  // FIXME: Wait for user to close server
  std::cout << "Press enter at anytime to terminate web server" << std::endl;
  std::cin.get();

  // Shut down server by unblocking HTTP::start()
  server->clear();
  delete server;
}

int main()
{
  try
  {
    HTTP * webServer = new HTTP();
    
    // create a thread that will listen for the user to end this program
    thread_function t(terminateServerThread, webServer);
    
    webServer->addAdapter("agent.mtconnect.org", 7878, "../include/config_no_namespace.xml");

    // make it listen on port 5000
    webServer->set_listening_port(5000);
    webServer->start();
  }
  catch (std::exception & e)
  {
    std::cout << e.what() << std::endl;
  }
  
  return 0;
}
