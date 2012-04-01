#include "seller.hpp"

Seller::Seller(const std::string &ldFile) {
  // apply a instruction file to the app instance
  transi.runFile(ldFile, this);
}

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

void Seller::addInstance(GenericInstance *gi) {
  for(GenericInstance *git : instanceObjects){
    if(git->getName() == gi->getName())
      throw InstanceExistsException();
  }
  instanceObjects.push_back(gi);
}

void Seller::appendClassParent(const std::string &childName, const std::string &parentName) {
  GenericClass *child = findClass(childName);

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

GenericInstance *Seller::findInstanceByName(const std::string &instName) {
  std::list<GenericInstance *>::iterator obj = 
    std::find_if(instanceObjects.begin(), instanceObjects.end(), [&](GenericInstance *goit){
	return goit->getName() == instName;
      });
  
  if(obj == instanceObjects.end())
    throw InstanceDoesNotExistException();
  
  return (*obj);
}  

GenericInstance *Seller::makeInstance(const std::string &className, const std::string &instanceName) {
  GenericInstance *newInstance = new GenericInstance(findClass(className), instanceName);
  return newInstance;
}
  
void Seller::evaluateObject(const std::string &instanceName) {
  GenericInstance *gi = findInstanceByName(instanceName);
  std::cout << gi->getClassName() << " (" << gi->getName() << ") has evaluated to " << gi->evaluateRule() << std::endl;
}

void Seller::updateClass(GenericClass *gc) {
  for(GenericInstance *gi : instanceObjects) {
    gi->propagateUpdatedClass(gc);
  }
}

void Seller::listClasses(std::list<std::string> clsNames) {
  std::cout << "***** The following classes are defined in the system : *****" << std::endl;
  for(GenericClass *gc: instanceClasses){
    if(clsNames.size() == 0 ||
       std::find_if(clsNames.begin(), clsNames.end(), [&](std::string &cName){
	   return cName == gc->getName();
	 }) != clsNames.end())
      std::cout << gc->inspect() << std::endl;
  }
  std::cout << "***** This is the end of class inspection *****" << std::endl;
}

void Seller::listInstances(std::list<std::string> instNames) {
  std::cout << "***** The following instances are defined in the system : *****" << std::endl;
  for(GenericInstance *go: instanceObjects){
    if(instNames.size() == 0 ||
       std::find_if(instNames.begin(), instNames.end(), [&](std::string &oName){
	   return oName == go->getName();
	 }) != instNames.end())
      std::cout << go->inspect() << std::endl;
  }
  std::cout << "***** This is the end of instance inspection *****" << std::endl;
}

void Seller::run() {
  
}
