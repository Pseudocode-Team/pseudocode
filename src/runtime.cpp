#include <map>
#include "pdc.h"

struct Variable {
	PseudoValue value;
	std::string type;
};

struct Runtime {
	PseudoValue acc;
	std::map<std::string, Variable> mem;
	void error(char* err) {
		std::cerr << "Unexpected error has occured:\n" << err << std::endl;
		std::exit(1);
	}
};
