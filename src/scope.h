#include "pdc.h"
#include <map>

class Scope {
	std::map <std::string, PseudoValue*> variables;
	Scope* parent;

	bool localVariableDefined(std::string varName);

	public:
	Scope()=default;
	Scope(Scope* _parent): parent(_parent) {}

	bool variableDefined(std::string varName);

	PseudoValue* getVar(std::string varName);
	void setVar(std::string varName, PseudoValue* value);

	Scope* enterScope();
	Scope* exitScope();
};
