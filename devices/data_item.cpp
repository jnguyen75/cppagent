#include "data_item.hpp"

DataItem::DataItem(std::map<std::string, std::string> attributes)
{
  mId = atoi(attributes["id"].c_str());
  mName = attributes["name"];
  mType = attributes["type"];
  
  if (!attributes["subtype"].empty())
  {
    mSubType = attributes["subType"];
  }
  
  mCategory = attributes["category"];
  
  if (!attributes["nativeUnits"].empty())
  {
    mNativeUnits = attributes["nativeUnits"];
  }
  
  if (!attributes["units"].empty())
  {
    mUnits = attributes["units"];
  }
  
  if (!attributes["nativeScale"].empty())
  {
    mNativeScale = attributes["nativeScale"];
  }
  
  if (!attributes["significantDigits"].empty())
  {
    mSignificantDigits = attributes["significantDigits"];
  }
  
  if (!attributes["coordinateSystem"].empty())
  {
    mCoordinateSystem = attributes["coordinateSystem"];
  }
}

std::string DataItem::getName()
{
  return mName;
}

bool DataItem::hasName(std::string name)
{
  return mName == name || (!mSource.empty() && mSource == name);
}

std::string DataItem::getType()
{
  return mType;
}

std::string DataItem::getNativeUnits()
{
  return mNativeUnits;
}

void DataItem::addSource(std::string source)
{
  mSource = source;
}
