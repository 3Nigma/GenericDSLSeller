#ifndef _GENERICINSTANCE_HPP_
#define _GENERICINSTANCE_HPP_

#include <iostream>
#include <string>
#include <lua5.1/lua.hpp>

#include "exceptions.hpp"
#include "genericclass.hpp"

class GenericInstance : public GenericClass {
public:
  GenericInstance(GenericClass *gc, const std::string &name);
  ~GenericInstance();

  virtual double evaluateRule();
  void propagateUpdatedClass(GenericClass *gc);
  
  virtual std::string getName();
  std::string getClassName();
protected:
  std::string mInstanceName;
  std::list<GenericInstance *> mInstanceParents;

  std::string expandRule();
};

#endif
