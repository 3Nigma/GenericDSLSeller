#include "genericinstance.hpp"

GenericInstance::GenericInstance(GenericClass *gc, const std::string &name)
  : GenericClass(gc), mInstanceName(name) {

  std::list<GenericClass *> clsParents = gc->getParents();
  std::for_each(clsParents.begin(), clsParents.end(), [&](GenericClass *p) {
      this->mInstanceParents.push_back(new GenericInstance(p, name));
    });
}

std::string GenericInstance::getName() {
  return mInstanceName;
}

std::string GenericInstance::getClassName() {
  return GenericClass::getName();
}

void GenericInstance::propagateUpdatedClass(GenericClass *gc) {
  if(gc->getName() == this->getClassName()){
    // class modifications will be reflected in this instance also
    if(gc->getProperties().size() < this->getProperties().size()) {
      // erasing requested
    } else if(gc->getProperties().size() > this->getProperties().size()){
      // addition requested
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
    std::cout << std::string("eres = ") + expandRule() << std::endl;
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
	composedRule.replace(start_pos, gp->getName().length(), std::to_string(gp->getValue()));
      }
    });
  
  return composedRule;
}
