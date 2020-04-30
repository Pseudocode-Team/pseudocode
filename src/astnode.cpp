#include "pdc.h"

struct ASTNode {
	PseudoValue value;
	void (*resolver)(Runtime* r, ASTNode* self);
	ASTNode* left;
	ASTNode* right;
	void resolve(Runtime* r) {
		resolver(r, this);
	}
};
