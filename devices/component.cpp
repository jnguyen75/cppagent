#include "component.hpp"

const std::string Component::SComponentSpecs[NumComponentSpecs] = {
  // Component parts
  "Axes",
  "Controller",
  "Device",
  "Linear",
  "Power",
  "Spindle",
  // Component details
  "Components",
  "DataItem",
  "DataItems",
  "Description",
  "Source",
  "text"
};

const std::string Component::SDescriptions[NumDescriptions] = {
  "manufacturer",
  "serialNumber"
};

/* Component public methods */
Component::Component(std::map<std::string, std::string> attributes)
{
  // Error checking..?
  mId = atoi(attributes["id"].c_str());
  mName = attributes["name"];
}

unsigned int Component::getId()
{
  return mId;
}

unsigned int Component::getParentId()
{
  return mParentId;
}

std::string Component::getName()
{
  return mName;
}

std::string Component::getManufacturer()
{
  return mManufacturer;
}

std::list<Component *> Component::getChildren()
{
  return mChildren;
}

void Component::addChild(Component * child)
{
  mChildren.push_back(child);
}

void Component::setParentId(unsigned int id)
{
  mParentId = id;
}

void Component::addDescription(std::map<std::string, std::string> attributes)
{
  if (!attributes["manufacturer"].empty())
  {
    mManufacturer = attributes["manufacturer"];
  }
  
  if (!attributes["serialNumber"].empty())
  {
    mSerialNumber = attributes["serialNumber"];
  }
  
  if (!attributes["station"].empty())
  {
    mStation = attributes["station"];
  }
  
}

Component * Component::findById(unsigned int id)
{
  std::cout << "Mine: " << mId << std::endl;
  std::cout << "Looking for: " << id << std::endl;
  if (getId() == id)
  {
    return this;
  }
  else
  {
    std::list<Component *> children = getChildren();
    
    for (std::list<Component *>::iterator it = children.begin(); it != children.end(); ++it)
    {
      Component * toFind = findById(id);
      if (toFind != NULL)
      {
        return toFind;
      }
    }
    return NULL;
  }
}

/* Component public static methods */
Component::EComponentSpecs Component::getComponentEnum(std::string name)
{
  for (unsigned int i=0; i<Component::NumComponentSpecs; i++)
  {
    if (name == Component::SComponentSpecs[i])
    {
       return (Component::EComponentSpecs) i;
    }
  }
  
  // TODO: Error/exception
}

bool Component::hasNameAndId(std::map<std::string, std::string> attributes)
{
  return !attributes["name"].empty() && !attributes["id"].empty();
}
