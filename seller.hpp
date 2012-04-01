#ifndef _SELLER_HPP_
#define _SELLER_HPP_

#include <iostream>
#include <list>
#include <string>
#include "metaaction.hpp"
#include "genericclass.hpp"
#include "genericinstance.hpp"
#include "interpres.hpp"

class Seller : public MetaAction {
public:
  Seller(const std::string &ldFile = "instruct.ld");
  ~Seller();
  
  virtual void addClass(GenericClass *gc);
  virtual void addInstance(GenericInstance *gi);
  virtual void appendClassParent(const std::string &childName, const std::string &parentName);
  virtual GenericClass *findClass(const std::string &className);
  virtual GenericInstance *makeInstance(const std::string &className, const std::string &instanceName);
  virtual void evaluateObject(const std::string &instanceName);
  virtual GenericInstance *findInstanceByName(const std::string &instName);
  virtual void updateClass(GenericClass *gc);

  void run();

protected:
  Interpres transi;
  std::list<GenericClass *> instanceClasses;
  std::list<GenericInstance *> instanceObjects;
};


#endif
