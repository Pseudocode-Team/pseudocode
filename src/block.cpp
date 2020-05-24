#include "pdc.h"
#include "runtime.h"
#include "astnode.h"

void instructionBlockResolver(Runtime* r, ASTNode* self) {
	for ( auto instruction: self->args ) {
		instruction->resolve(r);
	}
}

ASTNode* createInstructionBlock(Instructions instructions) {
	return new ASTNode{nullptr, &instructionBlockResolver, instructions};
}
