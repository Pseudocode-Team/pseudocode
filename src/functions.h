#include "pdc.h"
#include "runtime.h"
#include "astnode.h"

#define EMPTY_FUNCTION_ARGS new ASTNode{nullptr, nullptr, EMPTY_ARGS}

void returnResolver(Runtime* r, ASTNode* self);
void functionDeclarationResolver(Runtime* r, ASTNode* self);
void functionCallResolver(Runtime *r, ASTNode* self);
ASTNode* createReturn(ASTNode* value);
ASTNode* createFunctionDeclaration(std::string rawFunctionName, ASTNode* functionArgs, ASTNode* functionInstructions);
ASTNode* createFunctionCall(std::string rawFunctionName, ASTNode* functionArgs);
