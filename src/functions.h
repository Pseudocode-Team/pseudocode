#include "pdc.h"
#include "runtime.h"
#include "astnode.h"

void returnResolver(Runtime* r, ASTNode* self);
void functionDeclarationResolver(Runtime* r, ASTNode* self);
void functionCallResolver(Runtime *r, ASTNode* self);
ASTNode* createReturn(ASTNode* value);
ASTNode* createFunctionDeclaration(std::string rawFunctionName, Instructions functionArgs, Instructions functionInstructions);
ASTNode* createFunctionCall(std::string rawFunctionName, Instructions functionArgs);
