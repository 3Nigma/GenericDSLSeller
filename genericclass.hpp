#ifndef _GENERICCLASS_HPP_
#define _GENERICCLASS_HPP_

#include <string>
#include <list>
#include <algorithm>

#include "exceptions.hpp"
#include "genericproperty.hpp"

class GenericClass{
public:
  GenericClass(const std::string &name);
  GenericClass(const GenericClass *gcCopy);
  virtual ~GenericClass();

  std::string getName();
  std::list<GenericProperty *> getProperties();
  std::list<GenericClass *> getParents();

  void addProperty(GenericProperty *gp);
  void modifyPropertyValue(const std::string &propName, double newVal);
  void addDirectParent(GenericClass *gc);
  void setEvalRule(std::string rule);
  virtual double evaluateRule();

protected:
  std::list<GenericProperty *> mProperties;
  std::string mName;
  std::string mEvalRule;
  std::list<GenericClass *> mParents;

  GenericProperty *findDeepProperty(const std::string &pName);
  std::string expandRule();
};

#endif
