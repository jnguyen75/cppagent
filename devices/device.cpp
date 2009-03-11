#include "device.hpp"

/* Component public static variables */

/* Component public methods */
Device::Device(unsigned int id)
: Component(id)
{

}

/* Component protected methods */

int main()
{
  Device d(4);
  
  //std::cout << d.getId();
  
  return 0;
}
