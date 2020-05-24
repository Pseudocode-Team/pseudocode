#ifndef RUNTIME
#define RUNTIME
#include <map>
#include <iostream>

struct ASTNode;

struct Runtime {
	PseudoValue* acc;
	std::map<std::string, PseudoValue*> mem;
	std::map<std::string, ASTNode*> functionStack;
	void error(char* err) {
		std::cerr << "Unexpected error has occured:\n\t" << err << std::endl;
		std::exit(1);
	}
};
#endif
