#include "seller.hpp"

Seller::Seller(const std::string &ldFile) {
  // apply a instruction file to the app instance
  std::cout << " Genetic system preloading ..." << std::endl;
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

void Seller::printHelpMenu() {
  std::cout << "Alright, here's what I understand :" << std::endl
	    << " * Creating new stuff : " << std::endl
	    << "create class <newClassName> [inherits <className> ]with the following properties: <prop1>[(<defValue1>)][, ...[, <propN>[(<defValueN>)]]] and evaluates with '<evaluationRule>'" << std::endl
	    << " * Ordering existing items : " << std::endl
	    << "instantiate <existingClassName> in <instanceName> setting <prop1>(<defValue1>][, ...[, <propN>(<defValueN>)]]" << std::endl
	    << " * Custom tweak an item type : " << std::endl
	    << "update class <existingClassName> <'erasing'|'adding'> the following properties : <prop1>[(<defValue1>)][, ...[, <propN>[(<defValueN>)]]]" << std::endl
	    << "update class <existingClassName> evaluating in : <newEvaluationRule>" << std::endl
	    << " * Ask for the price of an ordered item : " << std::endl
	    << "evaluate <instanceName>" << std::endl
	    << " * Fine-tweak a property of an ordered item : " << std::endl
	    << "update instance <instanceName> setting property values : <prop1>[(<defValue1>)][, ...[, <propN>[(<defValueN>)]]]" << std::endl
	    << " * List ordered/available items : " << std::endl
	    << "list all <'instances'|'classes'>" << std::endl
	    << "list following <'instances'|'classes'> : <token1>[, ...[, <tokenN>]]" << std::endl
	    << "And that's about it ... hope that helps! :-)" << std::endl;
}

void Seller::run() {
  std::string command;
  
  std::cout << "Welcome to my Store!" << std::endl;
  std::cout << "I'm a generic seller, please tell me what I can do for you ..." << std::endl;
  std::cout << "(If you don't know how to say something, you can always shout for 'help')" << std::endl << std::endl;
  
  command = "help";
  do{
    if(command == "help"){
      printHelpMenu();
    } else if(!transi.runInstruction(command, this)){
      std::cout << "I didn't quite get that, can you try again please ? ..." << std::endl;
    }
    
    std::cout << " > ";
    std::getline(std::cin, command);
  }while(command != "exit");
  
  std::cout << "Goodbye! Come again ..." << std::endl;
}
