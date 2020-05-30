#include "pdc.h"
#include "runtime.h"
#include <vector>

#ifndef ASTNODE
#define ASTNODE

struct ASTNode {
	PseudoValue* value;
	void (*resolver)(Runtime* r, ASTNode* self);
	Instructions args;
	void resolve(Runtime* r) {
		resolver(r, this);
	}
};

#endif
