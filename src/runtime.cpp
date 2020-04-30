#include "pdc.h"

struct Runtime {
	PseudoValue acc;
	void error(char* err) {
		std::cerr << "Unexpected error has occured:\n" << err << std::endl;
		std::exit(1);
	}
};

