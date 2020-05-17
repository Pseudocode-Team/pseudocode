#include <stdio.h>
#include <vector>
#include <iostream>

#include "pdc.h"
#include "runtime.cpp"
#include "astnode.cpp"

void constIntResolver(Runtime* r, ASTNode* self) {
	r->acc = self->value;
}

void constStringResolver(Runtime* r, ASTNode* self) {
	r->acc = self->value;
}

ASTNode* createConstInt(std::string rawValue) {
	PseudoValue* value = new PseudoValue(rawValue, Int);
	return new ASTNode{value, &constIntResolver, nullptr, nullptr};
}

ASTNode* createConstString(std::string rawValue) {
	PseudoValue* value = new PseudoValue(rawValue, String);
	return new ASTNode{value, &constStringResolver, nullptr, nullptr};
}

void sumIntResolver(Runtime* r, ASTNode* self) {
	self->left->resolve(r);
	int leftValue = std::stoi(r->acc->value);
	self->right->resolve(r);
	int rightValue = std::stoi(r->acc->value);
	r->acc = new PseudoValue{std::to_string(leftValue + rightValue), Int};
}

void sumStringResolver(Runtime* r, ASTNode* self) {
	self->left->resolve(r);
	std::string leftValue = r->acc->value;
	self->right->resolve(r);
	std::string rightValue = r->acc->value;
	r->acc = new PseudoValue{leftValue + rightValue, String};
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

ASTNode* createIntSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{nullptr, &sumIntResolver, a, b};
}

ASTNode* createStringSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{nullptr, &sumStringResolver, a, b};
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
		createIntSum(
			createIntSum(createConstInt("1"), createConstInt("15")),
			createConstInt("2")
		),
		createPrint(createConstInt("69")),
		createPrint(createStringSum(
			createConstString("Hello "),
			createConstString("World")
		)),
		createAssignment("a", createConstInt("6")),
		createPrint(createGetVariable("a")),
		createAssignment("a", createIntSum(createGetVariable("a"), createConstInt("1"))),
		createPrint(createGetVariable("a")),

	};
	for (auto instruction : program) {
		instruction->resolve(&R);
	}
	return 0;
}
