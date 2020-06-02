#include "pdc.h"
#include <vector>

#ifndef ASTNODE
#define ASTNODE
#include "runtime.h"

struct ASTNode {
	PseudoValue* value;
	void (*resolver)(Runtime* r, ASTNode* self);
	Instructions* args;
	void resolve(Runtime* r) {
		resolver(r, this);
	}
};

#endif
