// Dlib library
#include "../lib/dlib/all/source.cpp"

#include <string>
#include <sstream>

std::string intToString(unsigned int i)
{
  std::ostringstream stm;
  stm << i;
  return stm.str();
}

std::string floatToString(float f)
{
  std::ostringstream stm;
  stm << f;
  return stm.str();
}

std::string toUpperCase(std::string text)
{
  for (unsigned int i=0; i<text.length();++i)
  {
    text[i] = toupper(text[i]);
  }
  
  return text;
}

