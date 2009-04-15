
#include "ustring.h"

namespace Glib {
ustring::ustring(std::string &str) : std::string(str) 
{ }

}