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

ASTNode* createIntSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{"", &sumIntResolver, a, b};
}

ASTNode* createStringSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{"", &sumStringResolver, a, b};
}

int main() {
	Runtime R;
	Program program = {
		createIntSum(
			createIntSum(createConstInt("1"), createConstInt("15")),
			createConstInt("2")
		),
		createConstInt("69"),
		createStringSum(
			createConstString("Hello "),
			createConstString("World")
		),

	};
	for (auto instruction : program) {
		instruction->resolve(&R);
		std::cout << R.acc << std::endl;
	}
	return 0;
}
