#include <stdio.h>
#include <iostream>

#define ADD 		'+'
#define SUBSTRACT	'-'
#define MULTIPLY	'*'
#define DIVIDE		'/'

struct ASTNode {
	int value; //TODO: Dynamic typing, for now every value is int
	void (*resolver)(int* acc, ASTNode* self, ASTNode* left, ASTNode* right);
	ASTNode* left;
	ASTNode* right;
	void resolve(int* acc) {
		resolver(acc, this, left, right);
	}
};

void constIntResolver(int* acc, ASTNode* self, ASTNode* left, ASTNode* right) {
	*acc = self->value;
}

ASTNode* createConstInt(int value) {
	return new ASTNode{value, &constIntResolver, nullptr, nullptr};
}

void sumIntResolver(int* acc, ASTNode* self, ASTNode* left, ASTNode* right) {
	left->resolve(acc);
	int leftValue = *acc;
	right->resolve(acc);
	int rightValue = *acc;
	*acc = leftValue + rightValue;
}

ASTNode* createSum(int a, int b) {
	return new ASTNode{(int)NULL, &sumIntResolver, createConstInt(a), createConstInt(b)};
}


int main() {
	int Accumulator;
	ASTNode* program = createSum(1, 2);
	program->resolve(&Accumulator);
	std::cout << Accumulator;
	return 0;
}
