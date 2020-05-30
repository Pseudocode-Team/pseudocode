#ifndef RUNTIME
#define RUNTIME
#include <map>
#include <iostream>

struct Runtime {
	PseudoValue* acc;
	std::map<std::string, PseudoValue*> mem;
	void error(char* err) {
		std::cerr << "Unexpected error has occured:\n\t" << err << std::endl;
		std::exit(1);
	}
};
#endif
