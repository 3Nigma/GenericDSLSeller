#include <iostream>
#include <tuple>
#include <list>
#include <algorithm>
#include <lua5.1/lua.hpp>
#include <boost/regex.hpp>
#include <exception>

#include "genericproperty.hpp"

class EvaluatorException : std::exception { };
class ClassNameExistsException : std::exception { };
class ClassDoesNotExistException : std::exception { };
class PropertyDoesNotExistException : std::exception { };
class CannotEvaluateClassException : std::exception { };

class GenericClass{
public:
  GenericClass(const std::string &name) : mName(name){}
  GenericClass(const GenericClass *gcCopy) : mName(gcCopy->mName), mEvalRule(gcCopy->mEvalRule) {
    std::for_each(gcCopy->mProperties.begin(), gcCopy->mProperties.end(), [&](GenericProperty *p){
	this->mProperties.push_back(new GenericProperty(p));
      });
    std::for_each(gcCopy->mParents.begin(), gcCopy->mParents.end(), [&](GenericClass *p){
	this->mParents.push_back(new GenericClass(p));
      });
  }
  virtual ~GenericClass(){ 
    this->mProperties.clear();
    this->mParents.clear();
  }

  std::string getName(){return mName;}
  std::list<GenericProperty *> getProperties(){return mProperties;}
  std::list<GenericClass *> getParents(){return mParents;}
  void addProperty(GenericProperty *gp) {mProperties.push_back(gp);}
  void modifyPropertyValue(const std::string &propName, double newVal){
    bool propFound = false;
    
    GenericProperty *targetedProp = findDeepProperty(propName);
    if(nullptr != targetedProp){
      targetedProp->setValue(newVal);
      propFound = true;
    }
    
    if(!propFound) throw PropertyDoesNotExistException();
  }
  void addDirectParent(GenericClass *gc) {mParents.push_back(gc);}
  void setEvalRule(std::string rule) {mEvalRule = rule;}

  virtual double evaluateRule(){
    throw CannotEvaluateClassException();
  }

protected:
  std::list<GenericProperty *> mProperties;
  std::string mName;
  std::string mEvalRule;
  std::list<GenericClass *> mParents;

  GenericProperty *findDeepProperty(const std::string &pName){
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
  
  std::string expandRule(){
    std::string composedRule = mEvalRule;
    
    // expand parents
    if(mParents.size() != 0){
      std::for_each(mParents.begin(), mParents.end(), [&composedRule](GenericClass *gc){
	  std::string parentNameToken = std::string("<") + gc->getName() + std::string(">");
	  size_t start_pos = composedRule.find(parentNameToken);
	  if(start_pos != std::string::npos){
	    std::string parentRule = std::string("(") + gc->expandRule() + std::string(")");
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
};

class GenericInstance : public GenericClass {
public:
  GenericInstance(const GenericClass *gc) : GenericClass(gc) { }
  virtual double evaluateRule(){
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
};

class SellerApp{
public:
  SellerApp() = default;
  ~SellerApp(){
    this->instanceClasses.clear();
    this->instanceObjects.clear();
  }
  
  void addClass(GenericClass *gc){
    std::for_each(instanceClasses.begin(), instanceClasses.end(), [&](GenericClass *gcit){
	if(gcit->getName() == gc->getName())
	  throw ClassNameExistsException();
      });
    instanceClasses.push_back(gc);
  }

  void appendClassParent(GenericClass *child, const std::string &parentName){
    child->addDirectParent(findClass(parentName));
  }

  GenericClass *findClass(const std::string &className){
    std::list<GenericClass *>::iterator cls = 
      std::find_if(instanceClasses.begin(), instanceClasses.end(), [&](GenericClass *gcit){
	  return gcit->getName() == className;
	});
    
    if(cls == instanceClasses.end())
      throw ClassDoesNotExistException();
    
    return (*cls);
  }
  
  GenericInstance *makeInstance(const std::string &className){
    GenericInstance *newInstance = new GenericInstance(findClass(className));
    return newInstance;
  }
  
  void run(){
    GenericClass *objA = new GenericClass("objA");
    objA->addProperty(new GenericProperty({"property_A1", "1.5"}));
    objA->addProperty(new GenericProperty({"property_A2", "2.1"}));
    objA->setEvalRule("property_A1  * 4");
    addClass(objA);
  
    GenericInstance *objAS = makeInstance(objA->getName());
    objAS->modifyPropertyValue("property_A1", 0.9);
    std::cout << objAS->evaluateRule() << std::endl;

    GenericClass *objB = new GenericClass("objB");
    objB->addProperty(new GenericProperty({"property_B1", 0.6}));
    objB->addProperty(new GenericProperty({"property_B2", 1.1}));
    appendClassParent(objB, "objA");
    objB->setEvalRule("<objA> + property_B2 - property_B1");
    addClass(objB);

    GenericInstance *objBS = makeInstance(objB->getName());
    objBS->modifyPropertyValue("property_A1", 0.1);
    std::cout << objBS->evaluateRule() << std::endl;

    //std::cout << objB->evaluateRule() << std::endl;
    boost::regex e("\"http:\\\\/\\\\/localhostr.com\\\\/files\\\\/.+?\"");
  }

protected:
  std::list<GenericClass *> instanceClasses;
  std::list<GenericInstance *> instanceObjects;
};

int main(int argc, char *argv[]){
  SellerApp itseller;
  itseller.run();
  return 0;
}
