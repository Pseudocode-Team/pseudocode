#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "pdc.h"
#include "runtime.h"
#include "astnode.h"
#include "block.h"
#include "bool.h"
#include "loop.h"
#include "functions.h"
#include "variables.h"
#include "sum.h"
#include "condition.h"
#include "print.h"
#include "utils.h"

class SyntaxError {};

static SyntaxError kSyntaxError = SyntaxError();

ASTNode * getStringNode(int &index, std::vector<std::string> &line) {
	char firstSignToEndSequence = line[index][0];
	bool canEnd = false;
	std::string result = "";
	while(index < line.size()) {
		int j = 0;
		while(j < line[index].size()) {
			if (line[index][j] != firstSignToEndSequence) {
				result.push_back(line[index][j]);
			} else if (canEnd) {
				if (++j < line[index].size()) {
					throw kSyntaxError;
				} else if (index + 1 < line.size() && line[index + 1][0] != '+') {
					throw kSyntaxError;
				} else if (line[index + 1][0] == '+') {
					index += 2;
					return createSum(createConstString(result), getStringNode(index, line));
				}
				return createConstString(result);
			} else {
				canEnd = true;
			}
			j++;
		}
		index++;
	}
	return createConstString(result);
}

ASTNode * getIntOrFloatNode(int &index, std::vector<std::string> &line) {
	bool isInt = true;
	std::string result = "";
	int j = 0;
	while(j < line[index].size()) {
		if (line[index][j] == '.') {
			if (isInt) isInt = false;
			result.push_back(line[index][j]);
		} else if (line[index][j] >= '0' && line[index][j] <= '9') {
			result.push_back(line[index][j]);
		} else {
			throw kSyntaxError;
		}
		j++;
	}
	if (index + 1 >= line.size()) {
		return isInt ? createConstInt(result) : createConstFloat(result);
	} else if (line[index + 1] == "+") {
		index += 2;
		return createSum(isInt ? createConstInt(result) : createConstFloat(result), getIntOrFloatNode(index, line));
	} else {
		throw kSyntaxError;
	}
}

ASTNode * getVariable(int &index, std::vector<std::string> &line);

ASTNode * getValue(int &index, std::vector<std::string> &line) {
	if (line[index][0] == '"' || line[index][0] == "'"[0]) {
		return getStringNode(index, line);
	} else if (line[index][0] >= '0' && line[index][0] <= '9') {
		return getIntOrFloatNode(index, line);
	} else {
		return getVariable(index, line);
	}	
}

ASTNode * getVariable(int &index, std::vector<std::string> &line) {
	ASTNode * node = createGetVariable(line[index]);
	if (index + 1 >= line.size()) {
		return node;
	} else if (line[index + 1] == "+") {
		index += 1;
		return createSum(node, getValue(index, line));
	} else {
		throw kSyntaxError;
	}
}

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
	Instructions program =  {}; 

	std::string line;
	while (std::getline(myfile, line)) {
		std::cout << "New line: " <<  line << std::endl;
	 	std::vector<std::string> splitted;
		split<std::vector<std::string>>(line, splitted);

 		int i = 0;
		while(i < splitted.size()) {
			// Comments implemented only for testing purposes
			if (splitted[i] == "#")
				break;

			if (splitted[i] == "write") {
				if (++i >= splitted.size()) {
					throw kSyntaxError;
				} 

				ASTNode * node = getValue(i, splitted);
				program.push_back(createPrint(node));
			} else if (i + 1 < splitted.size() && splitted[i + 1] == ":="){
				ASTNode * node;
				std::string varName = splitted[i];
				i += 2;
				if (i >= splitted.size()) {
					throw kSyntaxError;
				} else {
					node = getValue(i, splitted);
				}

				program.push_back(createAssignment(varName, node));
			}
			i++;
		}
	}

	for (auto instruction : program) {
		instruction->resolve(&R);
	}

	myfile.close();
	return 0;
}