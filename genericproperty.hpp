#ifndef _GENERICPROPERTY_HPP_
#define _GENERICPROPERTY_HPP_

#include<string>
#include<tuple>

class GenericProperty{
public:
  GenericProperty(const GenericProperty *gpc);
  GenericProperty(std::string name, double value);
  GenericProperty(std::string name, std::string value);

  bool operator==(GenericProperty &rhs);
  std::tuple<std::string, double> get();
  void set(std::tuple<std::string, double> &rhs);

  double getValue();
  void setValue(double newVal); 
  std::string getName();

protected:
  std::tuple<std::string, double> mProperty;
};

#endif
