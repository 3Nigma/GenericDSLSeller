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
  
GenericInstance *Seller::makeInstance(const std::string &className, const std::string &instanceName) {
  GenericInstance *newInstance = new GenericInstance(findClass(className), instanceName);
  return newInstance;
}
  
void Seller::run() {
  
}
