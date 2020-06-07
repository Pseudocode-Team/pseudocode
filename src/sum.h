#pragma once
#include "runtime.h"
#include "astnode.h"

void sumResolver(Runtime* r, ASTNode* self) {
	self->args->at(0)->resolve(r);
	PseudoValue* leftValue = r->acc;
	self->args->at(1)->resolve(r);
	PseudoValue* rightValue = r->acc;
	if (leftValue->type == rightValue->type) {
		if (isNumeric(leftValue->type) && isNumeric(rightValue->type)) {
			r->acc = new PseudoValue(
					std::to_string(
						std::stof(leftValue->value) +
						std::stof(rightValue->value)
					),
					leftValue->type
				);
		} else if (leftValue->type == String) {
			r->acc = new PseudoValue(
					leftValue->value + rightValue->value,
					String
				);
		}
	} else if (isNumeric(leftValue->type) && isNumeric(rightValue->type)) {
		r->acc = new PseudoValue(
				std::to_string(
					std::stof(leftValue->value) +
					std::stof(rightValue->value)
				),
				Float
			);
	} else {
		char* err;
		sprintf(err, "Cannot add %s to %s", PSEUDO_TYPES[leftValue->type], PSEUDO_TYPES[rightValue->type]);
		r->error(err);
	}
}

ASTNode* createSum(ASTNode* a, ASTNode* b) {
	Instructions* args = new Instructions{ a, b };
	return new ASTNode{nullptr, &sumResolver, args};
}