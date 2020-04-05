#include <stdio.h>
#include <iostream>

#define ADD 		'+'
#define SUBSTRACT	'-'
#define MULTIPLY	'*'
#define DIVIDE		'/'

//TODO: Dynamic typing, for now every value is int
#define PseudoValue int // To enable dynamic typing we need to use one type for every value

struct ASTNode {
	PseudoValue value;
	void (*resolver)(PseudoValue* acc, ASTNode* self, ASTNode* left, ASTNode* right);
	ASTNode* left;
	ASTNode* right;
	void resolve(PseudoValue* acc) {
		resolver(acc, this, left, right);
	}
};

void constIntResolver(PseudoValue* acc, ASTNode* self, ASTNode* left, ASTNode* right) {
	*acc = self->value;
}

ASTNode* createConstInt(PseudoValue value) {
	return new ASTNode{value, &constIntResolver, nullptr, nullptr};
}

void sumIntResolver(PseudoValue* acc, ASTNode* self, ASTNode* left, ASTNode* right) {
	left->resolve(acc);
	PseudoValue leftValue = *acc;
	right->resolve(acc);
	PseudoValue rightValue = *acc;
	*acc = leftValue + rightValue;
}

ASTNode* createSum(ASTNode* a, ASTNode* b) {
	return new ASTNode{(PseudoValue)NULL, &sumIntResolver, a, b};
}


int main() {
	PseudoValue Accumulator;
	ASTNode* program = createSum(
			createSum(createConstInt(1), createConstInt(15)),
			createConstInt(2)
		);
	program->resolve(&Accumulator);
	std::cout << Accumulator;
	return 0;
}
