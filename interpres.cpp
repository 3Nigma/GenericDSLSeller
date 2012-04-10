#include "interpres.hpp"

Interpres::Interpres(MetaAction *recv) 
  : mReceiver(recv) {
  
  // add create syntax parser
  insertChecker(std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType>{[](const std::string &instr, MetaAction *env)->bool {
	boost::regex c("create +class +(\\w+) +(?:.*\\b(?:inherits)\\b +(\\w+) +)?.*\\b(?:properties:) *(.*)\\b(?:and)\\b.*(?:with *)'(.*)'");
	boost::smatch captures;
	
	if(boost::regex_match(instr, captures, c, boost::match_extra)){
	  // (1) = ClassName, (2 - OPT) = ParentClass, (3) = Property list, (4) = Evaluation rule
	  GenericClass *newClass = new GenericClass(captures[1]);
	
	  std::string propList(captures[3]);
	  boost::regex propExpr("\\b(\\w+)\\b *(?:\\( *(?i)(numerical|string) *\\))");
	  boost::sregex_iterator m1(propList.begin(), propList.end(), propExpr);
	  boost::sregex_iterator m2;
	  std::for_each(m1, m2, [&newClass](const boost::smatch &m) -> bool{
	      // (1) = PropertyName, (2) = PropertyType {"numerical", "string"} case insensitive
	      std::string propName = m[1];
	      std::string propType = m[2];

	      std::transform(propType.begin(), propType.end(), propType.begin(), ::tolower);	      
	      if("numerical" == propType) {
		newClass->addProperty(new NumericalProperty(propName));
	      } else if("string" == propType) {
		newClass->addProperty(new StringProperty(propName));
	      } else {
		throw BadPropertyTypeException();
	      }

	      return true;
	    });
	  
	  newClass->setEvalRule(captures[4]);
	  
	  env->addClass(newClass);
	  if(captures.length(2) != 0)
	    env->appendClassParent(captures.str(1), captures.str(2));
	  
	  return true;
	}
	
	return false;
      }, InstrType::AddClass});

  // add instantion command parser
  insertChecker(std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType>{[](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("instantiate +(\\w+) +in +(\\w+) +setting +(.*)");
      boost::smatch captures;
      
      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = ClassName, (2) = InstanceName, (3) = Property list
	GenericInstance *newInst = env->makeInstance(captures[1], captures[2]);
       	std::string propList(captures[3]);
	boost::regex propExpr("\\b(\\w+)\\b *(?:\\( *(.+?) *\\))");
	boost::sregex_iterator m1(propList.begin(), propList.end(), propExpr);
	boost::sregex_iterator m2;

	std::for_each(m1, m2, [&newInst](const boost::smatch &m) -> bool{
	    // (1) = PropertyName, (2) = value
	    std::string propName = m[1];
	    std::string propValue = m[2];

	    if(propValue.length() == 0)
	      throw BadPropertyValueException();
	    newInst->modifyPropertyValue(propName, propValue);

	    return true;
	  });

	env->addInstance(newInst);
	
	return true;
      }

      return false;
      }, InstrType::AddInstance});

  // add evaluation object parser
  insertChecker(std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType>{[](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("evaluate +(\\w+)");
      boost::smatch captures;
      
      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = ObjectName
	
	env->evaluateObject(captures[1]);
        return true;
      }
      
      return false;
      }, InstrType::EvaluateInstance});

  // add parsing code to modify the structure of an existing class
  insertChecker(std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType>{[](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("update +class +\\b(\\w+)\\b +(erasing|adding|evaluating).*\\b(?:properties|property|in)\\b *: *(.+)");
      boost::smatch captures;
      
      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = ClassName, (2) = update type : "erasing" | "adding" | "evaluating", (3) = Property list | Evaluation rule
	
	GenericClass *gc = env->findClass(captures[1]);
	if(captures.str(2) == "evaluating") {
	  gc->setEvalRule(captures.str(3));
	} else {
	  std::string propList(captures[3]);
	  boost::regex propExpr("\\b(\\w+)\\b *(?:\\( *(?i)(numerical|string) *\\))?");
	  boost::sregex_iterator m1(propList.begin(), propList.end(), propExpr);
	  boost::sregex_iterator m2;
	  std::for_each(m1, m2, [&](const boost::smatch &m) -> bool{
	      // (1) = PropertyName, (2) = PropertyType {"numerical", "string"} case insensitive
	      std::string propName = m[1];
	      std::string propType = m[2];

	      std::transform(propType.begin(), propType.end(), propType.begin(), ::tolower);
	      if(captures.str(2) == "erasing") {
		gc->removeProperty(propName);
	      } else if(captures.str(2) == "adding") {
		if(propType == "numerical") {
		  gc->addProperty(new NumericalProperty(propName));
		} else if(propType == "string") {
		  gc->addProperty(new StringProperty(propName));
		} else {
		  throw BadPropertyTypeException();
		}
	      }
	      
	      return true;
	    });
	}
	env->updateClass(gc);
	
	return true;
      }

      return false;
      }, InstrType::ModifyClass});

  // parse modification of instance properties
  insertChecker(std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType>{[](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("update +instance +(\\w+).*\\b(?:value.?) *: *(.+)");
      boost::smatch captures;
      
      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = ObjectName, (2) = Property list
	std::string propList(captures[2]);
	boost::regex propExpr("\\b(\\w+)\\b *(?:\\( *(.+?) *\\))");
	boost::sregex_iterator m1(propList.begin(), propList.end(), propExpr);
	boost::sregex_iterator m2;
	GenericInstance *obj = env->findInstanceByName(captures.str(1));
	std::for_each(m1, m2, [&](const boost::smatch &m) -> bool{
	    // (1) = PropertyName, (2) = value
	    std::string propName = m[1];
	    std::string propValue = m[2];
	    
	    GenericProperty *objProp = obj->findProperty(propName);
	    objProp->setValue(propValue);
	    
	    return true;
	  });
	
        return true;
      }
      
      return false;
      }, InstrType::ModifyInstance});

  // parse listing of classes/objects
  insertChecker(std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType>{[](const std::string &instr, MetaAction *env)->bool {
      boost::regex c("list +(all|following) +(instance|class)e?s? *(?:\\: *(.+))?");
      boost::smatch captures;
      
      if(boost::regex_match(instr, captures, c, boost::match_extra)){
	// (1) = "all" | "following", (2) = "instance" | "class", (3) = Object/Classes list
	if(captures.str(1) == "all") {
	  if(captures.str(2) == "instance") {
	    env->listInstances({});
	  } else if(captures.str(2) == "class") {
	    env->listClasses({});
	  }
	} else {
	  std::string tokList(captures[3]);
	  boost::regex tokExpr("\\b(\\w+)\\b *");
	  boost::sregex_iterator m1(tokList.begin(), tokList.end(), tokExpr);
	  boost::sregex_iterator m2;
	  std::list<std::string> tokenList;
	  std::for_each(m1, m2, [&](const boost::smatch &m) -> bool{
	      // (1) = Token name
	      std::string tokenName = m[1];
	    
	      tokenList.push_back(tokenName);
	      
	      return true;
	    });
	
	  if(captures.str(2) == "instance") {
	    env->listInstances(tokenList);
	  } else if(captures.str(2) == "class") {
	    env->listClasses(tokenList);
	  }
	}
	return true;
      }
      return false;
      }, InstrType::List});
}

Interpres::~Interpres() {
  mCheckers.clear();
  mInstrQueue.clear();
}

void Interpres::setReceiver(MetaAction *recv) {
  this->mReceiver = recv;
}

void Interpres::insertChecker(const std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType> &checker) {
  mCheckers.push_back(checker);
}

void Interpres::dumpInstructions(const std::string &file, std::list<InstrType> filteredInstrTypes) {
  std::ofstream dumpFile(file);

  if(!dumpFile.good())
    throw FileDoesNotExistException();

  dumpFile << "- File automatically generated. Be careful what you're touching here ..." << std::endl;
  for(std::tuple<std::string, InstrType> currentInstr : mInstrQueue) {
    if(std::find_if(filteredInstrTypes.begin(), filteredInstrTypes.end(), [&](InstrType &instrT) {
	  return instrT == std::get<1>(currentInstr);
	}) == filteredInstrTypes.end()) {
      dumpFile << std::get<0>(currentInstr) << std::endl;
    }
  }

  dumpFile.close();
}

bool Interpres::runFile(const std::string &file) {
  std::ifstream instrFile(file);
  std::string line;

  if(!instrFile.good())
    throw FileDoesNotExistException();

  while(!instrFile.eof()){
    std::getline(instrFile, line);
    if(line.length() != 0) {
      if(line[0] == '-')
	std::cout << "\033[0;37;46m" << line << "\033[0m" << std::endl;
      else
	std::cout << line << std::endl;
      if(line[0] != '-') {
	if(executeInstruction(line) == InstrType::Unrecognized){
	  std::cout << "Unable to parse line '" << line << "'" << std::endl;
	}
      } 
    }
  }
  instrFile.close();
  
  return true;
}

Interpres::InstrType Interpres::executeInstruction(const std::string &instr) {
  Interpres::InstrType procInstrType = Interpres::InstrType::Unrecognized;
  std::function<bool(const std::string &, MetaAction *)> parser;

  for(std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType> checker : mCheckers) {
    parser = std::get<0>(checker);
    if(parser(instr, mReceiver)){
      procInstrType = std::get<1>(checker);
      break;
    }
  }
  if(InstrType::Unrecognized != procInstrType)
    mInstrQueue.push_back(std::tuple<std::string, InstrType>{instr, procInstrType});

  return procInstrType;
}
