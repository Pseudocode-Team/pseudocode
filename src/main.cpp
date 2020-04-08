#include <stdio.h>
#include <vector>
#include <iostream>

#define ADD 		'+'
#define SUBSTRACT	'-'
#define MULTIPLY	'*'
#define DIVIDE		'/'

//TODO: Dynamic typing, for now every value is int
#define PseudoValue int // To enable dynamic typing we need to use one type for every value

#define Program std::vector<ASTNode*>

struct Runtime {
	PseudoValue acc;
	void error(char* err) {
		std::cerr << "Unexpected error has occured:\n" << err << std::endl;
		std::exit(1);
	}
};


struct ASTNode {
	PseudoValue value;
	void (*resolver)(Runtime* r, ASTNode* self);
	ASTNode* left;
	ASTNode* right;
	void resolve(Runtime* r) {
		resolver(r, this);
	}
};

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
