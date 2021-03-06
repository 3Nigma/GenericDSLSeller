#ifndef _METAACTION_HPP_
#define _METAACTION_HPP_

#include <string>
#include "genericclass.hpp"
#include "genericinstance.hpp"

class MetaAction{
public:
  virtual void addClass(GenericClass *gc) = 0;
  virtual void addInstance(GenericInstance *gi) = 0;
  virtual void appendClassParent(const std::string &childName, const std::string &parentName) = 0;
  virtual GenericClass *findClass(const std::string &className) = 0;
  virtual GenericInstance *makeInstance(const std::string &className, const std::string &instanceName) = 0;
  virtual void evaluateObject(const std::string &instanceName) = 0;
  virtual GenericInstance *findInstanceByName(const std::string &instName) = 0;
  virtual void updateClass(GenericClass *gc) = 0;
  virtual void listClasses(std::list<std::string> clsNames) = 0;
  virtual void listInstances(std::list<std::string> instNames) = 0;
};

#endif
