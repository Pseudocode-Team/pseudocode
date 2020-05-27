#ifndef RUNTIME
#define RUNTIME
#include <map>
#include <iostream>
#include "scope.h"

struct ASTNode;

struct Runtime {
	PseudoValue* acc;
	Scope* currentScope = new Scope();
	std::map<std::string, ASTNode*> functionStack;
	void error(char* err) {
		std::cerr << "Unexpected error has occured:\n\t" << err << std::endl;
		std::exit(1);
	}
	void newScope() {
		currentScope = currentScope->enterScope();
	}
	void destroyScope() {
		currentScope = currentScope->exitScope();
	}
};
#endif
