#include <string>

#ifndef PSEUDO_TYPE
#define PSEUDO_TYPE
enum PseudoType {
	Int,
	Float,
	String,
	Bool,
	VarName,
	Operator,
};

static const char *PSEUDO_TYPES[] = {
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
#endif

#define Instructions std::vector<ASTNode*>
