#ifndef _EXCEPTIONS_HPP_
#define _EXCEPTIONS_HPP_

#include<exception>

class EvaluatorException : std::exception { };
class ClassNameExistsException : std::exception { };
class ClassDoesNotExistException : std::exception { };
class PropertyDoesNotExistException : std::exception { };
class CannotEvaluateClassException : std::exception { };
class BadPropertyValueException : std::exception { };
class InstanceExistsException : std::exception { };
class FileDoesNotExistException : std::exception { };
class InstanceDoesNotExistException : std::exception { };

#endif
