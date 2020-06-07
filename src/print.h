#pragma once
#include "runtime.h"
#include "astnode.h"

void printResolver(Runtime* r, ASTNode* self) {
	self->args->at(0)->resolve(r);
	std::string printingValue = r->acc->value;
	std::cout << printingValue << std::endl;
}


ASTNode* createPrint(ASTNode* arg) {
	Instructions* args = new Instructions{ arg };
	return new ASTNode{nullptr, &printResolver, args};
}
