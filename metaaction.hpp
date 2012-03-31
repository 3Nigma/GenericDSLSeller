#ifndef _METAACTION_HPP_
#define _METAACTION_HPP_

#include <string>
#include "genericclass.hpp"
#include "genericinstance.hpp"

class MetaAction{
public:
  virtual void addClass(GenericClass *gc) = 0;
  virtual void appendClassParent(const std::string &childName, const std::string &parentName) = 0;
  virtual GenericClass *findClass(const std::string &className) = 0;
  virtual GenericInstance *makeInstance(const std::string &className) = 0;
};

#endif
