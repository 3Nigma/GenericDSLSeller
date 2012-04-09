#include "stringproperty.hpp"

StringProperty::StringProperty(GenericProperty &rhs) {
  this->mProperty = rhs.get();
}

StringProperty::StringProperty(const std::string &name, const std::string &value) 
  : GenericProperty(name, value) {
}

std::string StringProperty::getFormattedStringVal() {
  return std::string("\"") + std::get<1>(this->mProperty) + std::string("\"");
}

void StringProperty::dumpToLua(lua_State *l) {  
  lua_pushstring(l, std::get<1>(mProperty).c_str());
  lua_setglobal(l, this->getName().c_str());
}

void StringProperty::setValue(const std::string &newVal) throw(BadPropertyValueException) {
  std::get<1>(mProperty) = newVal;
} 

GenericProperty * StringProperty::clone() {
  return new StringProperty(*this);
}
