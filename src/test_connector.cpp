#include "connector.hpp"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class TestConnector : public Connector
{
public:
  TestConnector(std::string server, unsigned int port)
  : Connector(server, port)
  {
  }
  
  void processLine(std::string line)
  {
    istringstream toParse(line);
    string token;
    
    getline(toParse, token, '|');
    string time = token;
    
    cout << "Time = " << time << endl;
    
    getline(toParse, token, '|');
    string type = token;
    
    if (type == "Alarm")
    {
      cout << "Alarm!" << endl;
    }
    else // Key -> Value Pairings
    {
      string token2;
      getline(toParse, token2, '|');
      
      cout << "Key = " << token << endl;
      cout << "Value = " << token2 << endl;
      
      // Will be bypassed by single "Time|Item|Value" event
      while (getline(toParse, token, '|') && getline(toParse, token2, '|'))
      {
        cout << "Key = " << token << endl;
        cout << "Value = " << token2 << endl;
      }
    }
  }
};



int main()
{
  TestConnector c("agent.mtconnect.org", 7878);
  c.connect();
}
