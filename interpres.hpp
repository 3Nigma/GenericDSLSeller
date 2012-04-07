#ifndef _INTERPRES_HPP_
#define _INTERPRES_HPP_

#include <iostream>
#include <fstream>
#include <functional>
#include <list>
#include <tuple>
#include <boost/regex.hpp>

#include "metaaction.hpp"

class Interpres{
public:
 enum InstrType {
    AddClass,
    AddInstance,
    EvaluateInstance,
    ModifyClass,
    ModifyInstance,
    List,
    Unrecognized
  };
  Interpres(MetaAction *recv);
  ~Interpres();

  void setReceiver(MetaAction *recv);
  void insertChecker(const std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType> &checker);
  void dumpInstructions(const std::string &file, std::list<InstrType> filteredInstrTypes);
 
  bool runFile(const std::string &file);
  Interpres::InstrType executeInstruction(const std::string &instr);

private:
  std::list<std::tuple<std::function<bool(const std::string &, MetaAction *)>, InstrType>> mCheckers;
  std::list<std::tuple<std::string, InstrType>> mInstrQueue;
  MetaAction *mReceiver;
};

#endif
