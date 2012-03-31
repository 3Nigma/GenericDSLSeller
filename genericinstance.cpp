#include "genericinstance.hpp"

GenericInstance::GenericInstance(const GenericClass *gc)
  : GenericClass(gc) {
}

double GenericInstance::evaluateRule() {
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
