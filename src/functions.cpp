#include "pdc.h"
#include "runtime.h"
#include "astnode.h"

class Return {};

void returnResolver(Runtime* r, ASTNode* self) {
	self->args->at(0)->resolve(r);
	throw Return();
}

void functionDeclarationResolver(Runtime* r, ASTNode* self) {
	r->functionStack[self->value->value] = self->args;
}

void functionCallResolver(Runtime *r, ASTNode* self) {
	r->newScope();
	Instructions* function = r->functionStack[self->value->value];
	if (function == nullptr) {
		char* err;
		sprintf(err, "Function %s is not defined", self->value->value.c_str());
		r->error(err);
	}
	ASTNode* functionArgs = function->at(0);
	if (self->args->at(0)->args->size() != functionArgs->args->size()) {
		r->error((char*)"Given args and declared args length does not match");
	}

	// Initialize function arguments
	for (unsigned i = 0; i < self->args->at(0)->args->size(); i++) {
		self->args->at(0)->args->at(i)->resolve(r);
		r->currentScope->setVar(
			functionArgs->args->at(i)->value->value,
			r->acc
		);
	}

	// Run function instructions
	try {
		r->functionStack[self->value->value]->at(1)->resolve(r);
		r->acc = NIL;
	} catch(Return e) {
		// Keep value in acc
	}
	r->destroyScope();
}

ASTNode* createReturn(ASTNode* value) {
	Instructions* args = new Instructions{ value };
	return new ASTNode{nullptr, &returnResolver, args};
}

ASTNode* createFunctionDeclaration(
		std::string rawFunctionName,
		ASTNode* functionArgs,
		ASTNode* functionInstructions
) {
	PseudoValue* functionName = new PseudoValue{ rawFunctionName, VarName };
	return new ASTNode{
		functionName,
		&functionDeclarationResolver,
		new Instructions{functionArgs, functionInstructions}
	};
}

ASTNode* createFunctionCall(std::string rawFunctionName, ASTNode* functionArgs) {
	PseudoValue* functionName = new PseudoValue{ rawFunctionName, VarName };
	return new ASTNode{functionName, &functionCallResolver, new Instructions{functionArgs}};
}
