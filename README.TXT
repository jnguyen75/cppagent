
* Overview

The MTConnect adapter is toolkit is a framework for quickly achieving MTConnect 
compliance. The framework is organized as follows. 

src               - Common framework library
src/adapter       - Abstract adabter
src/client        - Client of the adapter
src/device_datum  - Classes that manage the individual pieces of data retrieved from 
                    a machine tool.
src/server        - Socket server
src/string_buffer - A auto-extending text buffer that can be time-stamped

agent             - The agent library
agent/agent       - Main agent executable
agent/component_event - Wrapper class to help keep track of all the components and data
                    items
agent/connector   - Used to connect and read lines of data from a socket
agent/http        - HTTP web server used to return xml data based on path and queries
agent/xml_printer - XML wrapper class used to parse and print XML (using libxml++)

fake              - Minimal adapter with only power. Makefile for UNIX
fake/fake.cpp     - main for the application
fake/fake_adapter - The adapter

fanuc             - FANUC adapter (minimal for now, needs work). Include MSDev SLN
fanuc/FanucAdapter - main for the application
fanuc/fanuc_adapter - The adapter for the fanuc controller

lib               - Future location for the framework library
lib/dlib          - Dlib-17.15.  Used for Sockets, HTTP, etc.
lib/libxml++      - Powerful XML Parser/generator

include           - Future location for the framework include files
include/current.xml - Location of xml for agent to parse
include/config.xml - Configuration file that has the agent specifications

emc               - A complete implementation of the emc2 linux CNC simulator adapter
emc/emc.cpp       - main for the application
emc/emc_adapter   - The adapter

* Details

  The framework provides all the socket communications to multiple clients. The 
  abstract adapter provide everything except for the connectivity to the 
  propriatary API.
  
* To Do
  
  Complete FANUC
  Port to .NET
  Start OPC
  Start Okuma
  
