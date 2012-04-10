**What is it about?**

Using the following articles covering *Dynamic Object Creation/Management* :

*  [Dirk Riehle, Michel Tilman, Ralph Johnson: The Dynamic Object Model, PLOP 2000](http://hillside.net/plop/plop2k/proceedings/Riehle/Riehle.pdf)
*  [Joseph Yoder, Ralph Johnson: The Adaptive Object-Model Architectural Style, WICSA 2002](http://www.adaptiveobjectmodel.com/WICSA3/ArchitectureOfAOMsWICSA3.pdf)

build a generic seller that can handle, upon request, the creation and ordering of various client-specific items with desired properties and own evaluation rule.

**How is it done?**

*  Build entirely in C++ with the aid of [Lua](http://www.lua.org/) for the evaluation environment
*  [Boost::Regex](http://www.boost.org/doc/libs/1_49_0/libs/regex/doc/html/index.html) was also used in the project since C++11 hasn't got any mature compilers yet
*  Supports the creating of hierarchical meta class structures with complex, composite evaluation rules
*  Upon resolving the dependencies (liblua5.1 and boostregex), the project can be built provided that a sane C++11 compiler exists (g++4.6 or above) with a simple *make*
*  The following UML diagram depicts the relationship amoung the classes
![GenericDSLSeller UML Diagram](http://i42.tinypic.com/11ru1dh.png)

**How can I interact with the system?**

This is done via a console. Upon startup, the application tries to open and evaluate a pre-loading environment through 'instruct.ld'. At the end, all sane commands (that impacted the system only e.g. no listings) are dumped to 'instruct.ld'.
The complete DSL syntax looks as follows :

*  Creating new stuff : 
   *   create class ^newClassName^ [inherits ^className^ ]with the following properties: ^prop1^[(numerical|string)][, ...[, ^propN^[(numerical|string)]]] and evaluates with 'result = ^evaluationRule^'
*  Ordering existing items : 
   *   instantiate ^existingClassName^ in ^instanceName^ setting ^prop1^(^startValue1^][, ...[, ^propN^(^startValueN^)]]
*  Custom tweak an item type : 
   *   update class ^existingClassName^ ^'erasing'|'adding'^ the following properties : ^prop1^[(numerical|string)][, ...[, ^propN^[(numerical|string)]]]
   *   update class ^existingClassName^ evaluating in : result = ^newEvaluationRule^
*  Ask for the price of an ordered item : 
   *   evaluate ^instanceName^
*  Fine-tweak a property of an ordered item : 
   *   update instance ^instanceName^ setting property values : ^prop1^[(^newValue1^)][, ...[, ^propN^[(^newValueN^)]]]
*  List ordered/available items : 
   *   list all ^'instances'|'classes'^
   *   list following ^'instances'|'classes'^ : ^token1^[, ...[, ^tokenN^]]

**Wouldn't a screen of the actual program running be nice ?**

Indeed, and here it is :

![App actual run screen](http://i39.tinypic.com/2urqckl.png)

*Cheers*
