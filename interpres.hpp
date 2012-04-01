#ifndef _INTERPRES_HPP_
#define _INTERPRES_HPP_

#include <iostream>
#include <fstream>
#include <functional>
#include <list>
#include <boost/regex.hpp>

#include "metaaction.hpp"

class Interpres{
public:
  Interpres();
  ~Interpres();
  
  bool runFile(const std::string &file, MetaAction *doer);
  bool runInstruction(const std::string &instr, MetaAction *doer);
private:
  std::list<std::function<bool(const std::string &, MetaAction *)>> checkers;
};

#endif
