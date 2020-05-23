#include "pdc.h"

struct ASTNode {
	PseudoValue* value;
	void (*resolver)(Runtime* r, ASTNode* self);
	Instructions args;
	void resolve(Runtime* r) {
		resolver(r, this);
	}
};
