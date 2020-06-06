#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "pdc.h"
#include "runtime.h"
#include "astnode.h"
#include "block.h"
#include "bool.h"
#include "loop.h"
#include "functions.h"

PseudoValue* NIL = new PseudoValue{"nil", Nil};

void constResolver(Runtime* r, ASTNode* self) {
	r->acc = self->value;
}

ASTNode* createConst(std::string rawValue, PseudoType dataType) {
	PseudoValue* value = new PseudoValue(rawValue, dataType);
	return new ASTNode{value, &constResolver, EMPTY_ARGS};
}

ASTNode* createConstInt(std::string rawValue) {
	return createConst(rawValue, Int);
}

ASTNode* createConstFloat(std::string rawValue) {
	return createConst(rawValue, Float);
}

ASTNode* createConstString(std::string rawValue) {
	return createConst(rawValue, String);
}

ASTNode* createConstBool(std::string rawValue) {
	return createConst(rawValue, Bool);
}

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

void printResolver(Runtime* r, ASTNode* self) {
	self->args->at(0)->resolve(r);
	std::string printingValue = r->acc->value;
	std::cout << printingValue << std::endl;
}

void assignmentResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value->value;
	self->args->at(0)->resolve(r);
	r->currentScope->setVar(varName, r->acc);
}

void variableResolver(Runtime* r, ASTNode* self) {
	std::string varName = self->value->value;
	r->acc = r->currentScope->getVar(varName);
}

void conditionalStatementResolver(Runtime* r, ASTNode* self) {
	// Evaluate condition
	self->args->at(0)->resolve(r);
	if(mapBool(r->acc)) {
		// Run TRUE block
		self->args->at(1)->resolve(r);
	} else {
		// Run FALSE block
		self->args->at(2)->resolve(r);
	}
}

ASTNode* createSum(ASTNode* a, ASTNode* b) {
	Instructions* args = new Instructions{ a, b };
	return new ASTNode{nullptr, &sumResolver, args};
}

ASTNode* createPrint(ASTNode* arg) {
	Instructions* args = new Instructions{ arg };
	return new ASTNode{nullptr, &printResolver, args};
}

ASTNode* createAssignment(std::string rawVarName, ASTNode* value) {
	PseudoValue* varName = new PseudoValue( rawVarName, VarName );
	Instructions* args = new Instructions{ value };
	return new ASTNode{varName, &assignmentResolver, args};
}

ASTNode* createGetVariable(std::string rawVarName) {
	PseudoValue* varName = new PseudoValue( rawVarName, VarName );
	return new ASTNode{varName, &variableResolver, EMPTY_ARGS};
}

ASTNode* createConditionalStatement(ASTNode* condition, ASTNode* trueBlock, ASTNode* falseBlock) {
	Instructions* args = new Instructions{ condition, trueBlock, falseBlock };
	return new ASTNode{nullptr, &conditionalStatementResolver, args};
}

int main(int argc, char *argv[]) {

	// TODO: 
	std::ofstream myfile;
	myfile.open (argv[1]);


	Runtime R;
	Instructions program = {};
	for (auto instruction : program) {
		instruction->resolve(&R);
	}

	myfile.close();
	return 0;
}

	// Instructions program = {
	// 	createPrint(createSum(
	// 		createSum(createConstInt("1"), createConstInt("15")),
	// 		createConstFloat("2.1")
	// 	)),
	// 	createPrint(createConstInt("69")),
	// 	createAssignment("a", createConstInt("6")),
	// 	createPrint(createGetVariable("a")),
	// 	createAssignment("a", createSum(createGetVariable("a"), createConstInt("1"))),
	// 	createPrint(createGetVariable("a")),
	// 	createPrint(createComparison(EQUAL, createConstString("10"), createConstInt("10"))),
	// 	createConditionalStatement(
	// 		createComparison(EQUAL, createConstString("aaa"), createConstString("aaa")),
	// 		createInstructionBlock(new Instructions{
	// 			createPrint(createConstString("TRUE")),
	// 		}),
	// 		createInstructionBlock(new Instructions{
	// 			createPrint(createConstString("FALSE")),
	// 		})
	// 	),
	// 	createForLoop(
	// 		createAssignment("b", createConstInt("0")),
	// 		createComparison(LESS, createGetVariable("b"), createConstInt("10")),
	// 		createAssignment("b", createSum(createGetVariable("b"), createConstInt("1"))),
	// 		createInstructionBlock(new Instructions{
	// 			createPrint(createGetVariable("b"))
	// 		})
	// 	),
	// 	createFunctionDeclaration(
	// 		"add",
	// 		new ASTNode{nullptr, nullptr, new Instructions{
	// 			new ASTNode{new PseudoValue{"a", VarName}, nullptr, EMPTY_ARGS},
	// 			new ASTNode{new PseudoValue{"b", VarName}, nullptr, EMPTY_ARGS},
	// 		}},
	// 		createInstructionBlock(new Instructions{
	// 			createPrint(createConstString("Funkcja test")),
	// 			createReturn(createSum(
	// 				createGetVariable("a"),
	// 				createGetVariable("b")
	// 			))
	// 		})
	// 	),
	// 	createPrint(createFunctionCall("add", new ASTNode{nullptr, nullptr, new Instructions{
	// 		createConstInt("2"),
	// 		createConstInt("3")
	// 	}})),
	// };