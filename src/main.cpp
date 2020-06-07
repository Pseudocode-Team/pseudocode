#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "pdc.h"
#include "runtime.h"
#include "astnode.h"
#include "lexer.h"

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		std::cout << "No arguments has been provided" << std::endl;
		return 1;
	}

	std::ifstream myfile;
	myfile.open(argv[1]);
	if (!myfile.is_open()) {
		std::cout << "Unable to open " << argv[1] << std::endl;
		return 1;
	}	

	Runtime R;
	Lexer lexer = Lexer(myfile);
	myfile.close();

	Instructions program = lexer.getProgram(); 

	for (auto instruction : program) {
		instruction->resolve(&R);
	}

	return 0;
}