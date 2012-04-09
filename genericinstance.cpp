#include "genericinstance.hpp"

GenericInstance::GenericInstance(GenericClass *gc, const std::string &name)
  : GenericClass(gc), mInstanceName(name) {

  std::list<GenericClass *> clsParents = gc->getParents();
  std::for_each(clsParents.begin(), clsParents.end(), [&](GenericClass *p) {
      this->mInstanceParents.push_back(new GenericInstance(p, name));
    });
}

GenericInstance::~GenericInstance() {
  mInstanceParents.clear();
}

std::string GenericInstance::getName() {
  return mInstanceName;
}

std::string GenericInstance::getClassName() {
  return GenericClass::getName();
}

GenericProperty *GenericInstance::findProperty(const std::string &propName) {
  return findDeepProperty(propName);
}

void GenericInstance::propagateUpdatedClass(GenericClass *gc) {
  if(gc->getName() == this->getClassName()){
    // class modifications will be reflected in this instance also
    std::list<GenericProperty *> thisPropList = this->getProperties();
    std::list<GenericProperty *> gcPropList = gc->getProperties();
    if(gcPropList.size() < thisPropList.size()) {
      // erasing requested
      auto newListEnd = std::remove_if(this->getProperties().begin(), this->getProperties().end(), [&gcPropList](GenericProperty *gpit){
	  return std::find_if(gcPropList.begin(), gcPropList.end(), [&gpit](GenericProperty *gpits){
	      return (*gpit) == (*gpits);
	    }) == gcPropList.end();
	});
      this->getProperties().erase(newListEnd, this->getProperties().end());
    } else if(gcPropList.size() > thisPropList.size()){
      // addition requested
      for(GenericProperty *gpit : gcPropList){
	if(std::find_if(this->getProperties().begin(), this->getProperties().end(), [&](GenericProperty *gpits){
	      return (*gpit) == (*gpits);
	    }) == this->getProperties().end()){
	  this->getProperties().push_back(new GenericProperty(gpit));
	}
      }
    } else {
      // evaluation request
      this->setEvalRule(gc);
    }
    return;
  }

  // propagate the change to the parents
  for(GenericInstance *gp : this->mInstanceParents)
    gp->propagateUpdatedClass(gc);
}

double GenericInstance::evaluateRule() {
  lua_State *lstate = luaL_newstate();
  double evalResult = 0.0;
  
  if(nullptr == lstate) throw EvaluatorException();
  else{
    //std::cout << std::string("eres = ") + expandRule() << std::endl;
    if(luaL_dostring(lstate, (std::string("eres = ") + expandRule()).c_str()) != 0){
      lua_close(lstate);
      throw EvaluatorException();
    } else {
      lua_getglobal(lstate, "eres");
      if(lua_isnumber(lstate, -1) != 1){
	lua_close(lstate);
	throw EvaluatorException();
      }else{
	evalResult = lua_tonumber(lstate, -1);
      }
    }
  }
  
  lua_close(lstate);
  return evalResult;
}

std::string GenericInstance::expandRule() {
  std::string composedRule = mEvalRule;
  
  // expand parents
  if(mInstanceParents.size() != 0){
    std::for_each(mInstanceParents.begin(), mInstanceParents.end(), [&composedRule](GenericInstance *gi){
	std::string parentNameToken = std::string("<") + gi->getClassName() + std::string(">");
	size_t start_pos = composedRule.find(parentNameToken);
	if(start_pos != std::string::npos){
	  std::string parentRule = std::string("(") + gi->expandRule() + std::string(")");
	  composedRule.replace(start_pos, parentNameToken.length(), parentRule);
	}
      });
  }
  
  // expand properties
  std::for_each(mProperties.begin(), mProperties.end(), [&composedRule](GenericProperty *gp){
      size_t start_pos = composedRule.find(gp->getName());
      if(start_pos != std::string::npos){
	composedRule.replace(start_pos, gp->getName().length(), gp->getFormattedStringVal());
      }
    });
  
  return composedRule;
}

GenericProperty *GenericInstance::findDeepProperty(const std::string &pName) {
  // search in current property list
  std::list<GenericProperty *>::iterator pFoundPropIt = 
    std::find_if(mProperties.begin(), mProperties.end(), [&](GenericProperty *gpit){
	return gpit->getName() == pName;
      });
  
  GenericProperty *pFoundProp = (*pFoundPropIt);
  if(pFoundPropIt == mProperties.end()){
    // if not found, search in parents property list
    for (GenericInstance *it : mInstanceParents){
      pFoundProp = it->findDeepProperty(pName);
      if(nullptr != pFoundProp)
	break;
    }
  }
  
  return pFoundProp;
}

std::string GenericInstance::inspect() {
  std::string result = std::string(" >> '") + 
    this->getName() + 
    std::string("' instance of class '") +
    this->getClassName() +
    std::string("' with the following properties : \n");
  
  for(GenericProperty *gp : this->mProperties)
    result += std::string("+ ") + gp->getName() + std::string(" = ") + gp->getFormattedStringVal() + std::string("\n");
  result += std::string("Evaluates with : '") + this->getEvalRule() + std::string("' = ") + std::to_string(this->evaluateRule());
  result += std::string("\n");

  return result;
}
