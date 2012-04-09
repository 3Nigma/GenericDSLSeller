#include "genericproperty.hpp"

GenericProperty::GenericProperty(const GenericProperty *gpc) 
  : mProperty(gpc->mProperty) {
}

GenericProperty::GenericProperty(const std::string &name, const std::string &value) 
  : mProperty(name, value) {
}

bool GenericProperty::operator==(const GenericProperty &rhs) const{
  return this->getName() == rhs.getName();
}

std::tuple<std::string, std::string> GenericProperty::get() {
  return mProperty;
}

std::string GenericProperty::getFormattedStringVal() {
  return std::get<1>(mProperty);
}

void GenericProperty::setValue(const std::string &newVal) {
  std::get<1>(mProperty) = newVal;
} 

std::string GenericProperty::getName() const {
  return std::get<0>(mProperty);
}

void GenericProperty::set(std::tuple<std::string, std::string> &rhs) {
  this->mProperty = rhs;
}
