#include <map>
#include "pdc.h"

struct Runtime {
	PseudoValue* acc;
	Scope* currentScope = new Scope();
	void error(char* err) {
		std::cerr << "Unexpected error has occured:\n\t" << err << std::endl;
		std::exit(1);
	}
	void newScope() {
		this->currentScope = this->currentScope->enterScope();
	}
};
