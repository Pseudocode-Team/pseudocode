#pragma once
#include "pdc.h"
#include "runtime.h"
#include "astnode.h"

PseudoValue* NIL = new PseudoValue{"nil", Nil};

void constResolver(Runtime* r, ASTNode* self) {
	r->acc = self->value;
}

void assignmentResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value->value;
	self->args->at(0)->resolve(r);
	r->currentScope->setVar(varName, r->acc);
}

void variableResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value->value;
	r->acc = r->currentScope->getVar(varName);
}

ASTNode* createConst(std::string rawValue, PseudoType dataType) {
	PseudoValue* value = new PseudoValue(rawValue, dataType);
	return new ASTNode{value, &constResolver, EMPTY_ARGS};
}

ASTNode* createConstInt(std::string rawValue) {
	return createConst(rawValue, Int);
}

ASTNode* createConstFloat(std::string rawValue) {
	return createConst(rawValue, Float);
}

ASTNode* createConstString(std::string rawValue) {
	return createConst(rawValue, String);
}

ASTNode* createConstBool(std::string rawValue) {
	return createConst(rawValue, Bool);
}

ASTNode* createGetVariable(std::string rawVarName) {
	PseudoValue* varName = new PseudoValue( rawVarName, VarName );
	return new ASTNode{varName, &variableResolver, EMPTY_ARGS};
}

ASTNode* createAssignment(std::string rawVarName, ASTNode* value) {
	PseudoValue* varName = new PseudoValue( rawVarName, VarName );
	Instructions* args = new Instructions{ value };
	return new ASTNode{varName, &assignmentResolver, args};
}

