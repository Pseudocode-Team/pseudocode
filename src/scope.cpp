#include "scope.h"

bool Scope::localVariableDefined(std::string varName) {
	return !(variables.find(varName) == variables.end());
}


bool Scope::variableDefined(std::string varName) {
	if (localVariableDefined(varName)) {
		return true;
	}
	if (parent != nullptr) {
		return parent->variableDefined(varName);
	}
	return false;
}

PseudoValue* Scope::getVar(std::string varName) {
	if (localVariableDefined(varName)) {
		return variables[varName];
	}
	// Get variable from upper scope
	if (parent != nullptr) {
		return parent->getVar(varName);
	}
	return NIL;
}

void Scope::setVar(std::string varName, PseudoValue* value) {
	if (localVariableDefined(varName)) {
		variables[varName] = value;
		return;
	}
	// Check if variable defined in upper scope
	if (parent != nullptr && parent->variableDefined(varName)) {
		parent->setVar(varName, value);
	}
	variables[varName] = value;
}

Scope* Scope::enterScope() {
	return new Scope(this);
}

Scope* Scope::exitScope() {
	Scope* p = parent;
	delete this;
	return p;
}
