#include "pdc.h"
#include "runtime.h"
#include "astnode.h"
#include <iterator>
#include <algorithm>
#include <vector>

#include "bool.h"

PseudoValue* EQUAL = new PseudoValue{"=", Operator};
PseudoValue* INEQUAL = new PseudoValue{"!=", Operator};
PseudoValue* GREATER = new PseudoValue{">", Operator};
PseudoValue* LESS = new PseudoValue{"<", Operator};
PseudoValue* GREATER_OR_EQUAL = new PseudoValue{">=", Operator};
PseudoValue* LESS_OR_EQUAL = new PseudoValue{"<=", Operator};
PseudoValue* AND = new PseudoValue{"&&", Operator};
PseudoValue* OR = new PseudoValue{"||", Operator};

PseudoValue* TRUE = new PseudoValue{"1", Bool};
PseudoValue* FALSE = new PseudoValue{"0", Bool};

PseudoValue* mapBool(bool value) {
	return value ? TRUE : FALSE;
}

bool mapBool(PseudoValue* value) {
	/* Only the TRUE object (initialized above) is true. That means all other values
	like 1, 2.6, "string" will be false. */
	return value == TRUE;
}

PseudoValue* negation(PseudoValue* boolValue) {
	return mapBool(!mapBool(boolValue));
}

PseudoValue* andComparator(PseudoValue* a, PseudoValue* b) {
	return mapBool(mapBool(a) && mapBool(b));
}

PseudoValue* orComparator(PseudoValue* a, PseudoValue* b) {
	return mapBool(mapBool(a) || mapBool(b));
}

PseudoValue* equalComparator(PseudoValue* a, PseudoValue* b) {
	return mapBool(a->value == b->value && a->type == b->type);
}

PseudoValue* greaterComparator(PseudoValue* a, PseudoValue* b) {
	PseudoValue* value;
	if (isNumeric(a->type) && isNumeric(b->type)) {
		value = mapBool(std::stof(a->value) > std::stof(b->value));
	} else if (a->type == String || b->type == String) {
		throw "Cannot compare type String";
	} else {
		char* err;
		sprintf(err, "Comparison of type %s with %s not implemented", PSEUDO_TYPES[a->type], PSEUDO_TYPES[b->type]);
		throw err;
	}
	return value;
}


void comparisonResolver(Runtime* r, ASTNode* self) {
	self->args->at(0)->resolve(r);
	PseudoValue* leftValue = r->acc;
	self->args->at(1)->resolve(r);
	PseudoValue* rightValue = r->acc;
	try {
		if(self->value == EQUAL) {
			r->acc = equalComparator(leftValue, rightValue);
		} else if (self->value == INEQUAL) {
			r->acc = negation(equalComparator(leftValue, rightValue));
		} else if (self->value == GREATER) {
			r->acc = greaterComparator(leftValue, rightValue);
		} else if (self->value == LESS) {
			r->acc = greaterComparator(rightValue, leftValue);
		} else if (self->value == GREATER_OR_EQUAL) {
			r->acc = orComparator(greaterComparator(leftValue, rightValue), equalComparator(leftValue, rightValue));
		} else if (self->value == LESS_OR_EQUAL) {
			r->acc = orComparator(greaterComparator(rightValue, leftValue), equalComparator(leftValue, rightValue));
		} else if (self->value == AND) {
			r->acc = andComparator(rightValue, leftValue);
		} else if (self->value == OR) {
			r->acc = orComparator(rightValue, leftValue);
		} else {
			char* err;
			sprintf(err, "Comparison operator %s not implemented", self->value->value);
			r->error(err);
		}
	} catch (char const* err) {
		r->error((char*)err);
	} catch (char* err) {
		r->error(err);
	}
}

ASTNode* createComparison(PseudoValue* op, ASTNode* left, ASTNode* right) {
	return new ASTNode{op, &comparisonResolver, new Instructions{ left, right }};
}
