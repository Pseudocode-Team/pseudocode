#include "astnode.h"
#include "runtime.h"

void loopResolver(Runtime* r, ASTNode* self);
ASTNode* createWhileLoop(ASTNode* condition, ASTNode* block);
ASTNode* createForLoop(ASTNode* startExpression, ASTNode* condition, ASTNode* endExpression, ASTNode* block);
ASTNode* createDoWhileLoop(ASTNode* condition, ASTNode* block);
