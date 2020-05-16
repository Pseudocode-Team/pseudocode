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

ASTNode* createConstInt(PseudoValue value) {
	return new ASTNode{value, &constIntResolver, nullptr, nullptr};
}

ASTNode* createConstString(PseudoValue value) {
	return new ASTNode{value, &constStringResolver, nullptr, nullptr};
}

void sumIntResolver(Runtime* r, ASTNode* self) {
	self->left->resolve(r);
	PseudoValue leftValue = r->acc;
	self->right->resolve(r);
	PseudoValue rightValue = r->acc;
	r->acc = std::to_string(std::stoi(leftValue)+std::stoi(rightValue));
}

void sumStringResolver(Runtime* r, ASTNode* self) {
	self->left->resolve(r);
	PseudoValue leftValue = r->acc;
	self->right->resolve(r);
	PseudoValue rightValue = r->acc;
	r->acc = leftValue + rightValue;
}

void printResolver(Runtime* r, ASTNode* self) {
	self->left->resolve(r);
	PseudoValue printingValue = r->acc;
	std::cout << printingValue << std::endl;
}

void assignmentResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value;
	self->left->resolve(r);
	PseudoValue value = r->acc;
	r->mem[varName] = {value, "number"};
}

void variableResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value;
	r->acc = r->mem[varName].value;
}

ASTNode* createIntSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{"", &sumIntResolver, a, b};
}

ASTNode* createStringSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{"", &sumStringResolver, a, b};
}

ASTNode* createPrint(ASTNode* arg) {
	return new ASTNode{"", &printResolver, arg, nullptr};
}

ASTNode* createAssignment(std::string varName, ASTNode* value) {
	return new ASTNode{varName, &assignmentResolver, value, nullptr};
}

ASTNode* createGetVariable(std::string varName) {
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
