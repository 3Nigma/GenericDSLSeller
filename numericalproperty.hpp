#ifndef _NUMERICALPROPERTY_HPP_
#define _NUMERICALPROPERTY_HPP_

#include <string>
#include <tuple>
#include <lua5.1/lua.hpp>

#include "exceptions.hpp"
#include "genericproperty.hpp"

class NumericalProperty : public GenericProperty {
public:
  NumericalProperty(GenericProperty &rhs);
  NumericalProperty(const std::string &name, const std::string &value = "0.0");

  virtual std::string getFormattedStringVal();
  virtual void dumpToLua(lua_State *l);
  virtual void setValue(const std::string &newVal) throw(BadPropertyValueException);
  virtual GenericProperty *clone();
};

#endif
