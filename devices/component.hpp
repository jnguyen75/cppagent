//#include <iostream>
#include <list>
#include <string>

class Component {
protected:
  /* Unique ID for each component */
  unsigned int mId;
  
  /* Name for itself */
  std::string mName;
  
  /* Keep track of host/port for each */
  std::string mHost;
  unsigned int mPort;
  
  /* Each component keeps track of it's children in a list */
  std::list<Component> mChildren;

public:
  /* */
  Component(unsigned int id);

};
