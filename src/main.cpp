#include <stdio.h>
#include <vector>
#include <iostream>

#include "pdc.h"
#include "runtime.cpp"
#include "astnode.cpp"

void constResolver(Runtime* r, ASTNode* self) {
	r->acc = self->value;
}

ASTNode* createConst(std::string rawValue, PseudoType dataType) {
	PseudoValue* value = new PseudoValue(rawValue, dataType);
	return new ASTNode{value, &constResolver, nullptr, nullptr};
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

void sumResolver(Runtime* r, ASTNode* self) {
	self->left->resolve(r);
	PseudoValue* leftValue = r->acc;
	self->right->resolve(r);
	PseudoValue* rightValue = r->acc;
	if (leftValue->type == rightValue->type) {
		if (leftValue->type == Int) {
			r->acc = new PseudoValue(
						std::to_string(
							std::stoi(leftValue->value) +
							std::stoi(rightValue->value)
						),
						Int
					);
		} else if (leftValue->type == Float) {
			r->acc = new PseudoValue(
						std::to_string(
							std::stof(leftValue->value) +
							std::stof(rightValue->value)
						),
						Float
					);
		} else if (leftValue->type == String) {
			r->acc = new PseudoValue(
						leftValue->value + rightValue->value,
						String
					);
		}
	}
	else if ((leftValue->type == Int && rightValue->type == Float)
		|| (leftValue->type == Float && rightValue->type == Int)) {
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
	self->left->resolve(r);
	std::string printingValue = r->acc->value;
	std::cout << printingValue << std::endl;
}

void assignmentResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value->value;
	self->left->resolve(r);
	r->mem[varName] = r->acc;
}

void variableResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value->value;
	r->acc = r->mem[varName];
}

ASTNode* createSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{nullptr, &sumResolver, a, b};
}

ASTNode* createPrint(ASTNode* arg) {
	return new ASTNode{nullptr, &printResolver, arg, nullptr};
}

ASTNode* createAssignment(std::string rawVarName, ASTNode* value) {
	PseudoValue* varName = new PseudoValue( rawVarName, VarName );
	return new ASTNode{varName, &assignmentResolver, value, nullptr};
}

ASTNode* createGetVariable(std::string rawVarName) {
	PseudoValue* varName = new PseudoValue( rawVarName, VarName );
	return new ASTNode{varName, &variableResolver, nullptr, nullptr};
}

int main() {
	Runtime R;
	Program program = {
		createPrint(createSum(
			createSum(createConstInt("1"), createConstInt("15")),
			createConstFloat("2.1")
		)),
		createPrint(createConstInt("69")),
		createAssignment("a", createConstInt("6")),
		createPrint(createGetVariable("a")),
		createAssignment("a", createSum(createGetVariable("a"), createConstInt("1"))),
		createPrint(createGetVariable("a")),
		createPrint(createSum(
			createConstString("Hello "),
			createConstInt("1")
		)),
	};
	for (auto instruction : program) {
		instruction->resolve(&R);
	}
	return 0;
}
