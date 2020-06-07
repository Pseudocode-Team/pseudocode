#pragma once
#include <vector>
#include <string>
#include <istream>
#include "pdc.h"
#include "astnode.h"

class SyntaxError {};

class Lexer {
	int index;
	std::vector<std::string> line;
	Instructions program =  {};

public:
	Lexer(std::istream &stream);

	Instructions getProgram();

private: 
	ASTNode * getStringNode();

	ASTNode * getIntOrFloatNode();

	ASTNode * getValue();

	ASTNode * getVariable();
};
