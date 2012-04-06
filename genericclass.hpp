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

  virtual std::string getName();
  std::list<GenericProperty *> &getProperties();
  std::list<GenericClass *> &getParents();
  std::string getEvalRule();

  void setEvalRule(GenericClass *gc);
  void setEvalRule(std::string rule);
  
  void addProperty(GenericProperty *gp);
  void removeProperty(const std::string &propName);
  void modifyPropertyValue(const std::string &propName, double newVal);
  void addDirectParent(GenericClass *gc);
  virtual std::string inspect();
  virtual double evaluateRule();
  
protected:
  std::list<GenericProperty *> mProperties;
  std::string mName;
  std::string mEvalRule;
  std::list<GenericClass *> mParents;

  virtual GenericProperty *findDeepProperty(const std::string &pName);
};

#endif
