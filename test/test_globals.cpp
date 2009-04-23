#include <sstream>
#include <fstream>
#include <string>


std::string getFile(std::string fileLoc)
{
  std::ifstream ifs(fileLoc.c_str());
  std::stringstream stream;
  stream << ifs.rdbuf();
  return stream.str();
}

void fillErrorText(std::string& errorXml, const std::string& text)
{
  size_t pos = errorXml.find("</Error>");
  
  if (pos == std::string::npos)
  {
    return;
  }
  errorXml.insert(pos, text);
}

void fillAttribute(
    std::string& xmlString,
    const std::string& attribute,
    const std::string& value
  )
{
  size_t pos = xmlString.find(attribute + "=\"\"");
  if (pos == std::string::npos)
  {
    return;
  }
  else
  {
    pos += attribute.length() + 2;
  }
  xmlString.insert(pos, value);
}

