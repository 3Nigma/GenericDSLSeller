#ifndef _GENERICPROPERTY_HPP_
#define _GENERICPROPERTY_HPP_

#include <string>
#include <tuple>
#include <lua5.1/lua.hpp>

#include "exceptions.hpp"

class GenericProperty{
public:
  GenericProperty() = default;
  GenericProperty(const GenericProperty *gpc);
  GenericProperty(const std::string &name, const std::string &value);

  bool operator==(const GenericProperty &rhs) const;
  std::tuple<std::string, std::string> get();
  void set(std::tuple<std::string, std::string> &rhs);

  virtual std::string getFormattedStringVal() = 0; 
  virtual void dumpToLua(lua_State *l) = 0;
  virtual void setValue(const std::string &newVal) throw (BadPropertyValueException) = 0;
  virtual GenericProperty *clone() = 0;

  std::string getName() const;

protected:
  std::tuple<std::string, std::string> mProperty;
};

#endif
