#include "genericclass.hpp"

GenericClass::GenericClass(const std::string &name) 
  : mName(name) {
}

GenericClass::GenericClass(const GenericClass *gcCopy)
  : mName(gcCopy->mName), mEvalRule(gcCopy->mEvalRule) {
  std::for_each(gcCopy->mProperties.begin(), gcCopy->mProperties.end(), [&](GenericProperty *p) {
      this->mProperties.push_back(new GenericProperty(p));
    });
  //std::for_each(gcCopy->mParents.begin(), gcCopy->mParents.end(), [&](GenericClass *p) {
  //    this->mParents.push_back(new GenericClass(p));
  //  });
}

GenericClass::~GenericClass() { 
  this->mProperties.clear();
  this->mParents.clear();
}

std::string GenericClass::getName() {
  return mName;
}

std::list<GenericProperty *> &GenericClass::getProperties() {
  return mProperties;
}

std::list<GenericClass *> &GenericClass::getParents() {
  return mParents;
}

void GenericClass::addProperty(GenericProperty *gp) {
  mProperties.push_back(gp);
}

void GenericClass::modifyPropertyValue(const std::string &propName, double newVal) {
  bool propFound = false;
    
  GenericProperty *targetedProp = findDeepProperty(propName);
  if(nullptr != targetedProp){
    targetedProp->setValue(newVal);
    propFound = true;
  }
  
  if(!propFound) throw PropertyDoesNotExistException();
}

void GenericClass::addDirectParent(GenericClass *gc) {
  mParents.push_back(gc);
}

void GenericClass::setEvalRule(std::string rule) {
  this->mEvalRule = rule;
}

void GenericClass::setEvalRule(GenericClass *gc) {
  this->mEvalRule = gc->mEvalRule;
}

double GenericClass::evaluateRule() {
  throw CannotEvaluateClassException();
}

GenericProperty *GenericClass::findDeepProperty(const std::string &pName) {
  // search in current property list
  std::list<GenericProperty *>::iterator pFoundPropIt = 
    std::find_if(mProperties.begin(), mProperties.end(), [&](GenericProperty *gpit){
	return gpit->getName() == pName;
      });
  
  GenericProperty *pFoundProp = (*pFoundPropIt);
  if(pFoundPropIt == mProperties.end()){
    // if not found, search in parents property list
    for (GenericClass *it : mParents){
      pFoundProp = it->findDeepProperty(pName);
      if(nullptr != pFoundProp)
	break;
    }
  }
  
  return pFoundProp;
}
