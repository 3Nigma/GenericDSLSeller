#include "seller.hpp"

Seller::~Seller() {
  this->instanceClasses.clear();
  this->instanceObjects.clear();
}
  
void Seller::addClass(GenericClass *gc) {
  std::for_each(instanceClasses.begin(), instanceClasses.end(), [&](GenericClass *gcit) {
      if(gcit->getName() == gc->getName())
	throw ClassNameExistsException();
    });
  instanceClasses.push_back(gc);
}

void Seller::appendClassParent(GenericClass *child, const std::string &parentName) {
  child->addDirectParent(findClass(parentName));
}

GenericClass *Seller::findClass(const std::string &className) {
  std::list<GenericClass *>::iterator cls = 
    std::find_if(instanceClasses.begin(), instanceClasses.end(), [&](GenericClass *gcit){
	return gcit->getName() == className;
      });
  
  if(cls == instanceClasses.end())
    throw ClassDoesNotExistException();
  
  return (*cls);
}
  
GenericInstance *Seller::makeInstance(const std::string &className) {
  GenericInstance *newInstance = new GenericInstance(findClass(className));
  return newInstance;
}
  
void Seller::run() {
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
