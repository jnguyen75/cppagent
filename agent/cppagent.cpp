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

#include "../src/agent.hpp"
#include "fcntl.h"
#include "sys/stat.h"


void terminateServerThread(Agent *server)
{
  std::cout << "Press enter at anytime to terminate web server" << std::endl;
  std::cin.get();
 
  // Shut down server by unblocking Agent::start()
  server->clear();
}

void addToBufferThread(Agent *server)
{
  while (true)
  {
    std::string dataItem, value;
    
    std::cout << std::endl << "Data item: ";
    std::cin >> dataItem;
    
    std::cout << "Value: ";
    std::cin >> value;
    
    bool added = server->addToBuffer(dataItem, value);
    std::cout << "Success: " << std::boolalpha << added << std::endl;
  }
}

#ifndef WIN32
void signal_handler(int sig)
{
  switch(sig) {
  case SIGHUP:
    std::cout << "hangup signal catched" << std::endl;
    break;
    
  case SIGTERM:
    std::cout << "terminate signal catched" << std::endl;
    exit(0);
    break;
  }
}
#endif

void daemonize()
{
#ifndef WIN32
  int i,lfp;
  char str[10];
  if(getppid()==1) return; /* already a daemon */
  
  i=fork();
  if (i<0) exit(1); /* fork error */
  if (i>0)
  {
    std::cout << "Parent process now exiting, child process started" << std::endl;
    exit(0); /* parent exits */
  }
  
  
  /* child (daemon) continues */
  setsid(); /* obtain a new process group */

  // Close stdin
  close(0);
  open("/dev/null", O_RDONLY);

  // Redirect stdout and stderr to another file.
  close(1);
  close(2);
  umask(027); /* set newly created file permissions */
  i = open("agent.log", O_WRONLY | O_CREAT, 0640);
  dup(i); /* handle standart I/O */
  
  // chdir(RUNNING_DIR); /* change running directory */

  // Create the pid file.
  lfp = open("agent.pid", O_RDWR|O_CREAT, 0640);
  if (lfp<0) exit(1); /* can not open */

  // Lock the pid file.
  if (lockf(lfp, F_TLOCK, 0)<0) exit(0); /* can not lock */
  
  /* first instance continues */
  sprintf(str,"%d\n", getpid());
  write(lfp, str, strlen(str)); /* record pid to lockfile */
  
  signal(SIGCHLD,SIG_IGN); /* ignore child */
  signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
  signal(SIGTTOU,SIG_IGN);
  signal(SIGTTIN,SIG_IGN);
  
  signal(SIGHUP,signal_handler); /* catch hangup signal */
  signal(SIGTERM,signal_handler); /* catch kill signal */
#endif
}


int main()
{
  //daemonize();
  
  try
  {
    //Agent * agent = new Agent("../include/128.32.164.245.xml");
    //agent->addAdapter("128.32.164.245", 7878);
    
    Agent agent ("../include/agent.mtconnect.org.xml");
    agent.addAdapter("agent.mtconnect.org", 7878);
    
    // ***** DEBUGGING TOOLS *****
    
    // Create a thread that will listen for the user to end this program
    //thread_function t(terminateServerThread, &agent);
    
    // Use the addToBuffer API to allow user input for data
    thread_function t(addToBufferThread, &agent);
    
    agent.set_listening_port(SERVER_PORT);
    agent.start();
  }
  catch (std::exception & e)
  {
    logEvent("Cppagent::Main", e.what());
    std::cerr << "Agent failed to load: " << e.what() << std::endl;
    return -1;
  }
  
  return 0;
}

