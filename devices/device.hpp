#include "component.hpp"

class Device : public Component {
protected:
  /*unsigned int mId;*/
  
public:
  Device(unsigned int id);
  
  int getId() { return mId; }
};
