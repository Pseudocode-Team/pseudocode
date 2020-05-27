#include <stdio.h>
#include <vector>
#include <iostream>

#include "pdc.h"
#include "runtime.h"
#include "astnode.h"
#include "bool.h"

#define EMPTY_ARGS Instructions{}

void constResolver(Runtime* r, ASTNode* self) {
	r->acc = self->value;
}

ASTNode* createConst(std::string rawValue, PseudoType dataType) {
	PseudoValue* value = new PseudoValue(rawValue, dataType);
	return new ASTNode{value, &constResolver, EMPTY_ARGS};
}

ASTNode* createConstInt(std::string rawValue) {
	return createConst(rawValue, Int);
}

ASTNode* createConstFloat(std::string rawValue) {
	return createConst(rawValue, Float);
}

ASTNode* createConstString(std::string rawValue) {
	return createConst(rawValue, String);
}

ASTNode* createConstBool(std::string rawValue) {
	return createConst(rawValue, Bool);
}

void sumResolver(Runtime* r, ASTNode* self) {
	self->args[0]->resolve(r);
	PseudoValue* leftValue = r->acc;
	self->args[1]->resolve(r);
	PseudoValue* rightValue = r->acc;
	if (leftValue->type == rightValue->type) {
		if (isNumeric(leftValue->type) && isNumeric(rightValue->type)) {
			r->acc = new PseudoValue(
					std::to_string(
						std::stof(leftValue->value) +
						std::stof(rightValue->value)
					),
					leftValue->type
				);
		} else if (leftValue->type == String) {
			r->acc = new PseudoValue(
					leftValue->value + rightValue->value,
					String
				);
		}
	} else if (isNumeric(leftValue->type) && isNumeric(rightValue->type)) {
		r->acc = new PseudoValue(
				std::to_string(
					std::stof(leftValue->value) +
					std::stof(rightValue->value)
				),
				Float
			);
	} else {
		char* err;
		sprintf(err, "Cannot add %s to %s", PSEUDO_TYPES[leftValue->type], PSEUDO_TYPES[rightValue->type]);
		r->error(err);
	}
}

void printResolver(Runtime* r, ASTNode* self) {
	self->args[0]->resolve(r);
	std::string printingValue = r->acc->value;
	std::cout << printingValue << std::endl;
}

void assignmentResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value->value;
	self->args[0]->resolve(r);
	r->mem[varName] = r->acc;
}

void variableResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value->value;
	r->acc = r->mem[varName];
}

ASTNode* createSum(ASTNode* a, ASTNode* b) {
	Instructions args = { a, b };
	return new ASTNode{nullptr, &sumResolver, args};
}

ASTNode* createPrint(ASTNode* arg) {
	Instructions args = { arg };
	return new ASTNode{nullptr, &printResolver, args};
}

ASTNode* createAssignment(std::string rawVarName, ASTNode* value) {
	PseudoValue* varName = new PseudoValue( rawVarName, VarName );
	Instructions args = { value };
	return new ASTNode{varName, &assignmentResolver, args};
}

ASTNode* createGetVariable(std::string rawVarName) {
	PseudoValue* varName = new PseudoValue( rawVarName, VarName );
	return new ASTNode{varName, &variableResolver, EMPTY_ARGS};
}

int main() {
	Runtime R;
	Instructions program = {
		createPrint(createSum(
			createSum(createConstInt("1"), createConstInt("15")),
			createConstFloat("2.1")
		)),
		createPrint(createConstInt("69")),
		createAssignment("a", createConstInt("6")),
		createPrint(createGetVariable("a")),
		createAssignment("a", createSum(createGetVariable("a"), createConstInt("1"))),
		createPrint(createGetVariable("a")),
		createPrint(createComparison(EQUAL, createConstString("10"), createConstInt("10"))),
	};
	for (auto instruction : program) {
		instruction->resolve(&R);
	}
	return 0;
}
