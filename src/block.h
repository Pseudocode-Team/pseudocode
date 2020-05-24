#include "pdc.h"
#include "runtime.h"
#include "astnode.h"

void instructionBlockResolver(Runtime* r, ASTNode* self);
ASTNode* createInstructionBlock(Instructions instructions);
