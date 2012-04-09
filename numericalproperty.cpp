#include "numericalproperty.hpp"

NumericalProperty::NumericalProperty(GenericProperty &rhs) {
  this->mProperty = rhs.get();
}

NumericalProperty::NumericalProperty(const std::string &name, const std::string &value) 
  : GenericProperty(name, value) {
}

std::string NumericalProperty::getFormattedStringVal() {
  return std::get<1>(this->mProperty);
}

void NumericalProperty::dumpToLua(lua_State *l) {
  lua_Number val = stod(std::get<1>(mProperty));
  
  lua_pushnumber(l, val);
  lua_setglobal(l, this->getName().c_str());
}

void NumericalProperty::setValue(const std::string &newVal) throw(BadPropertyValueException) {
  // test to see if the conversion can be done
  std::stod(newVal);
  // assign the respective value
  std::get<1>(mProperty) = newVal;
} 

GenericProperty * NumericalProperty::clone() {
  return new NumericalProperty(*this);
}
