#pragma once
#include "runtime.h"
#include "astnode.h"
#include "bool.h"

void conditionalStatementResolver(Runtime* r, ASTNode* self) {
	// Evaluate condition
	self->args->at(0)->resolve(r);
	if(mapBool(r->acc)) {
		// Run TRUE block
		self->args->at(1)->resolve(r);
	} else {
		// Run FALSE block
		self->args->at(2)->resolve(r);
	}
}

ASTNode* createConditionalStatement(ASTNode* condition, ASTNode* trueBlock, ASTNode* falseBlock) {
	Instructions* args = new Instructions{ condition, trueBlock, falseBlock };
	return new ASTNode{nullptr, &conditionalStatementResolver, args};
}