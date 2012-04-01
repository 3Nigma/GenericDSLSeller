#ifndef _GENERICPROPERTY_HPP_
#define _GENERICPROPERTY_HPP_

#include <string>
#include <tuple>

class GenericProperty{
public:
  GenericProperty(const GenericProperty *gpc);
  GenericProperty(const std::string &name, double value = 0.0);
  GenericProperty(const std::string &name, const std::string &value = "0.0");

  bool operator==(const GenericProperty &rhs) const;
  std::tuple<std::string, double> get();
  void set(std::tuple<std::string, double> &rhs);

  double getValue();
  void setValue(double newVal); 
  void setValue(const std::string &newVal);

  std::string getName() const;

protected:
  std::tuple<std::string, double> mProperty;
};

#endif
