#ifndef _STRINGPROPERTY_HPP_
#define _STRINGPROPERTY_HPP_

#include <string>
#include <tuple>
#include <lua5.1/lua.hpp>

#include "exceptions.hpp"
#include "genericproperty.hpp"

class StringProperty : public GenericProperty {
public:
  StringProperty(GenericProperty &rhs);
  StringProperty(const std::string &name, const std::string &value = "");

  virtual std::string getFormattedStringVal();
  virtual void dumpToLua(lua_State *l);
  virtual void setValue(const std::string &newVal) throw(BadPropertyValueException);
  virtual GenericProperty *clone();
};

#endif
