#ifndef _SELLER_HPP_
#define _SELLER_HPP_

#include <iostream>
#include <list>
#include <string>
#include <boost/regex.hpp>
#include "genericclass.hpp"
#include "genericinstance.hpp"

class Seller{
public:
  Seller() = default;
  ~Seller();
  
  void addClass(GenericClass *gc);
  void appendClassParent(GenericClass *child, const std::string &parentName);
  GenericClass *findClass(const std::string &className);
  GenericInstance *makeInstance(const std::string &className);
  
  void run();

protected:
  std::list<GenericClass *> instanceClasses;
  std::list<GenericInstance *> instanceObjects;
};


#endif
