#include "interpres.hpp"

Interpres::Interpres() {
  // add create syntax parser
  checkers.push_back([](const std::string &instr, MetaAction *target)->bool {
      boost::regex e("create +class +(\\w+) +(?:.*\\b(?:inherits)\\b +(\\w+) +)?.*\\b(?:properties:) *(.*)\\b(?:and)\\b.*(?:with *)'(.*)'");
      boost::smatch what;

      if(boost::regex_match(instr, what, e, boost::match_extra)){
	unsigned int ix;
	for(ix = 1; ix < what.size(); ++ix){
	  std::cout << "      $" << ix << " = \"" << what[ix] << "\"" << std::endl;
	}

	return true;
      }

      return false;
    });
}

Interpres::~Interpres() {
  checkers.clear();
}

bool Interpres::runFile(const std::string &file, MetaAction *doer) {
  return true;
}

bool Interpres::runInstruction(const std::string &instr, MetaAction *doer) {
  (*checkers.begin())(instr, doer);
  return true;
}
