#include "pdc.h"
#include "runtime.h"
#include "astnode.h"

class Return {};


void returnResolver(Runtime* r, ASTNode* self) {
	self->args[0]->resolve(r);
	throw Return();
}

void functionDeclarationResolver(Runtime* r, ASTNode* self) {
	r->functionStack[self->value->value] = self->args[0];
}

void functionCallResolver(Runtime *r, ASTNode* self) {
	r->newScope();
	try {
		r->functionStack[self->value->value]->resolve(r);
	} catch(Return e) {
		// Keep value in acc
	}
	r->destroyScope();
}

ASTNode* createReturn(ASTNode* value) {
	Instructions args = { value };
	return new ASTNode{nullptr, &returnResolver, args};
}

ASTNode* createFunctionDeclaration(std::string rawFunctionName, Instructions functionArgs, Instructions functionInstructions) {
	PseudoValue* functionName = new PseudoValue{ rawFunctionName, VarName };
	return new ASTNode{functionName, &functionDeclarationResolver, Instructions{functionInstructions}};
}

ASTNode* createFunctionCall(std::string rawFunctionName, Instructions functionArgs) {
	PseudoValue* functionName = new PseudoValue{ rawFunctionName, VarName };
	return new ASTNode{functionName, &functionCallResolver, EMPTY_ARGS};
}
