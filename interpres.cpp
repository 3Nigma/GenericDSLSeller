#include "interpres.hpp"

Interpres::Interpres() {
  // add create syntax parser
  checkers.push_back([](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("create +class +(\\w+) +(?:.*\\b(?:inherits)\\b +(\\w+) +)?.*\\b(?:properties:) *(.*)\\b(?:and)\\b.*(?:with *)'(.*)'");
      boost::smatch captures;

      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = ClassName, (2 - OPT) = ParentClass, (3) = Property list, (4) = Evaluation rule
	GenericClass *newClass = new GenericClass(captures[1]);
	
	std::string propList(captures[3]);
	boost::regex propExpr("\\b(\\w+)\\b *(?:\\( *([0-9\\.]+) *\\))?");
	boost::sregex_iterator m1(propList.begin(), propList.end(), propExpr);
	boost::sregex_iterator m2;
	std::for_each(m1, m2, [&newClass](const boost::smatch &m) -> bool{
	    // (1) = PropertyName, (2 - OPT) = Initial value
	    std::string propName = m[1];
	    std::string propValue = m[2];
	    if(propValue.length() == 0)
	      propValue = "0.0";
	    newClass->addProperty(new GenericProperty(propName, propValue));
	    return true;
	  });
	
	newClass->setEvalRule(captures[4]);

	env->addClass(newClass);
	if(captures.length(2) != 0)
	  env->appendClassParent(captures.str(1), captures.str(2));
	
	return true;
      }

      return false;
    });

  // add instantion command parser
  checkers.push_back([](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("instantiate +(\\w+) +in +(\\w+) +setting +(.*)");
      boost::smatch captures;
      
      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = ClassName, (2) = InstanceName, (3) = Property list
	GenericInstance *newInst = env->makeInstance(captures[1], captures[2]);
	
	std::string propList(captures[3]);
	boost::regex propExpr("\\b(\\w+)\\b *(?:\\( *([0-9\\.]+) *\\))?");
	boost::sregex_iterator m1(propList.begin(), propList.end(), propExpr);
	boost::sregex_iterator m2;
	std::for_each(m1, m2, [&newInst](const boost::smatch &m) -> bool{
	    // (1) = PropertyName, (2) = value
	    std::string propName = m[1];
	    std::string propValue = m[2];
	    if(propValue.length() == 0)
	      throw BadPropertyValueException();
	    newInst->modifyPropertyValue(propName, std::stof(propValue));
	    return true;
	  });

	env->addInstance(newInst);
	
	return true;
      }

      return false;
    });

  // add evaluation object parser
  checkers.push_back([](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("evaluate +(\\w+)");
      boost::smatch captures;
      
      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = ObjectName
	
	env->evaluateObject(captures[1]);
        return true;
      }
      
      return false;
    });

  // add parsing code to modify the structure of an existing class
  checkers.push_back([](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("update +class +\\b(\\w+)\\b +(erasing|adding|evaluating).*\\b(?:properties|property|in)\\b *: *(.+)");
      boost::smatch captures;
      
      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = ClassName, (2) = update type : "erasing" | "adding" | "evaluating", (3) = Property list | Evaluation rule
	
	GenericClass *gc = env->findClass(captures[1]);
	if(captures.str(2) == "evaluating") {
	  gc->setEvalRule(captures.str(3));
	} else {
	  if(captures.str(2) == "erasing") {
	    
	  } else { // "adding"
	    
	  }
	}
	env->updateClass(gc);
	
	return true;
      }

      return false;
    });
}

Interpres::~Interpres() {
  checkers.clear();
}

bool Interpres::runFile(const std::string &file, MetaAction *doer) {
  std::ifstream instrFile(file);
  std::string line;

  if(!instrFile.good())
    throw FileDoesNotExistException();

  while(!instrFile.eof()){
    std::getline(instrFile, line);
    if(line.length() != 0 && line[0] != '-')
      runInstruction(line, doer);
  }
  instrFile.close();

  return true;
}

bool Interpres::runInstruction(const std::string &instr, MetaAction *doer) {
  for(std::function<bool(const std::string &, MetaAction *)> parser : checkers)
    if(parser(instr, doer))
      break;
  
  return true;
}
