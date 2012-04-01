#include "genericproperty.hpp"

GenericProperty::GenericProperty(const GenericProperty *gpc) 
  : mProperty(gpc->mProperty) {
}

GenericProperty::GenericProperty(const std::string &name, double value) 
  : mProperty(name, value) {
}

GenericProperty::GenericProperty(const std::string &name, const std::string &value) 
  : mProperty(name, std::stof(value)) {
}

bool GenericProperty::operator==(const GenericProperty &rhs) const{
  return this->getName() == rhs.getName();
}

std::tuple<std::string, double> GenericProperty::get() {
  return mProperty;
}

double GenericProperty::getValue() {
  return std::get<1>(mProperty);
}

void GenericProperty::setValue(double newVal) {
  std::get<1>(mProperty) = newVal;
} 

std::string GenericProperty::getName() const {
  return std::get<0>(mProperty);
}

void GenericProperty::set(std::tuple<std::string, double> &rhs) {
  this->mProperty = rhs;
}
