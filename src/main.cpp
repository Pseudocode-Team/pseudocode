#include <stdio.h>
#include <vector>
#include <iostream>

#include "pdc.h"
#include "runtime.cpp"
#include "astnode.cpp"

void constIntResolver(Runtime* r, ASTNode* self) {
	r->acc = self->value;
}

ASTNode* createConstInt(PseudoValue value) {
	return new ASTNode{value, &constIntResolver, nullptr, nullptr};
}

void sumIntResolver(Runtime* r, ASTNode* self) {
	self->left->resolve(r);
	PseudoValue leftValue = r->acc;
	self->right->resolve(r);
	PseudoValue rightValue = r->acc;
	r->acc = leftValue + rightValue;
}

ASTNode* createSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{(PseudoValue)NULL, &sumIntResolver, a, b};
}

int main() {
	Runtime R;
	Program program = {
		createSum(
			createSum(createConstInt(1), createConstInt(15)),
			createConstInt(2)
		),
		createConstInt(69),

	};
	for (auto instruction : program) {
		instruction->resolve(&R);
		std::cout << R.acc << std::endl;
	}
	return 0;
}
