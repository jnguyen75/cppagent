#include "connector.hpp"

#include <iostream>
#include <sstream>
#include <string>

//using namespace std;

class TestConnector : public Connector
{
public:
  TestConnector(std::string server, unsigned int port)
  : Connector(server, port)
  {
  }
  
  void processLine(std::string line)
  {
    std::istringstream toParse(line);
    std::string token;
    
    getline(toParse, token, '|');
    std::string time = token;
    
    std::cout << "Time = " << time << std::endl;
    
    getline(toParse, token, '|');
    std::string type = token;
    
    if (type == "Alarm")
    {
      std::cout << "Alarm!" << std::endl;
    }
    else // Key -> Value Pairings
    {
      std::string token2;
      getline(toParse, token2, '|');
      
      std::cout << "Key = " << token << std::endl;
      std::cout << "Value = " << token2 << std::endl;
      
      // Will be bypassed by single "Time|Item|Value" event
      while (getline(toParse, token, '|') && getline(toParse, token2, '|'))
      {
        std::cout << "Key = " << token << std::endl;
        std::cout << "Value = " << token2 << std::endl;
      }
    }
  }
};



int main()
{
  TestConnector c("agent.mtconnect.org", 7878);
  c.connect();
}
