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

std::string Adapter::getName()
{
  return mName;
}
