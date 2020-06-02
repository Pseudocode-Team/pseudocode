#include <string>

#ifndef PSEUDO_TYPE
#define PSEUDO_TYPE
enum PseudoType {
	Nil,
	Int,
	Float,
	String,
	Bool,
	VarName,
	Operator,
};

static const char *PSEUDO_TYPES[] = {
	"nil",
	"Int",
	"Float",
	"String",
	"Bool",
	"VarName",
	"Operator",
};
#endif


#define ADD 	'+'
#define SUBSTRACT	'-'
#define MULTIPLY	'*'
#define DIVIDE	'/'

#ifndef PSEUDO_VALUE
#define PSEUDO_VALUE
struct PseudoValue {
	std::string value;
	PseudoType type;
	PseudoValue(std::string _value, PseudoType _type): value(_value), type(_type) {};
};

extern PseudoValue* NIL;
#endif

#define Instructions std::vector<ASTNode*>
#define EMPTY_ARGS new Instructions{}
#define isNumeric(type)	(type == Int || type == Float || type == Bool)
