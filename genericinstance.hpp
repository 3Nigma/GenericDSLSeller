#ifndef _GENERICINSTANCE_HPP_
#define _GENERICINSTANCE_HPP_

#include <iostream>
#include <string>
#include <lua5.1/lua.hpp>

#include "exceptions.hpp"
#include "genericclass.hpp"

class GenericInstance : public GenericClass {
public:
  GenericInstance(const GenericClass *gc);

  virtual double evaluateRule();
};

#endif
