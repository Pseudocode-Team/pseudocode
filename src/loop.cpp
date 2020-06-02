#include "pdc.h"
#include "astnode.h"
#include "bool.h"
#include "block.h"

void loopResolver(Runtime* r, ASTNode* self) {
	// self->args[0] is the loop condition
	self->args[0]->resolve(r);
	while(mapBool(r->acc)) {
		self->args[1]->resolve(r);
		// check again loop condition
		self->args[0]->resolve(r);
	}
}

ASTNode* createWhileLoop(ASTNode* condition, ASTNode* block) {
	Instructions args = { condition, block };
	return new ASTNode{nullptr, &loopResolver, args };
}

ASTNode* createForLoop(ASTNode* startExpression, ASTNode* condition, ASTNode* endExpression, ASTNode* block) {
	// Put endExpression at the end of instruction block
	block->args.push_back(endExpression);
	Instructions args = { condition, block };
	ASTNode* loop = new ASTNode{nullptr, &loopResolver, args };
	// Put startExpression before the loop
	return createInstructionBlock(Instructions{ startExpression, loop });
}

ASTNode* createDoWhileLoop(ASTNode* condition, ASTNode* block) {
	Instructions args = { condition, block };
	ASTNode* loop = new ASTNode{nullptr, &loopResolver, args };
	return createInstructionBlock(Instructions{ block, loop });
}
