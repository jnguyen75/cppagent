

#include "component.hpp"

class Device : public Component
{ 
protected:
  /* Protected member variables */
  float mSampleRate;
  
  unsigned int mIso841Class;
  
  std::string mUuid;
  
public:
  Device(std::map<std::string, std::string> attributes);
  
public:
  static bool hasDeviceSpecs(std::map<std::string, std::string> attributes);
};
