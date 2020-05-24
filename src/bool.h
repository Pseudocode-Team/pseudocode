#ifndef BOOL_OPERATORS
#define BOOL_OPERATORS
extern PseudoValue* EQUAL;
extern PseudoValue* INEQUAL;
extern PseudoValue* GREATER;
extern PseudoValue* LESS;
extern PseudoValue* GREATER_OR_EQUAL;
extern PseudoValue* LESS_OR_EQUAL;
extern PseudoValue* AND;
extern PseudoValue* OR;

extern PseudoValue* TRUE;
extern PseudoValue* FALSE;
#endif

PseudoValue* mapBool(bool value);
bool mapBool(PseudoValue* value);
PseudoValue* negation(PseudoValue* boolValue);
PseudoValue* andComparator(PseudoValue* a, PseudoValue* b);
PseudoValue* orComparator(PseudoValue* a, PseudoValue* b);
PseudoValue* equalComparator(PseudoValue* a, PseudoValue* b);
bool isNumeric(PseudoType type);
PseudoValue* greaterComparator(PseudoValue* a, PseudoValue* b);
void comparisonResolver(Runtime* r, ASTNode* self);
ASTNode* createComparison(PseudoValue* op, ASTNode* left, ASTNode* right);
