#include "lexer.h"
#include "utils.h"
#include "print.h"
#include "operators.h"
#include "variables.h"

Lexer::Lexer(std::istream &stream) {
    std::string strLine;
    while (std::getline(stream, strLine)) {
        std::cout << "New line: " <<  strLine << std::endl;
        line = {};
        split<std::vector<std::string>>(strLine, line);

        index = 0;
        while(index < line.size()) {
            // Comments implemented only for testing purposes
            if (line[index] == "#")
                continue;

            if (line[index] == "write") {
                if (++index >= line.size()) {
                    throw new SyntaxError();
                }

                ASTNode * node = getValue();
                program.push_back(createPrint(node));
            } else if (index + 1 < line.size() && line[index + 1] == ":="){
                ASTNode * node;
                std::string varName = line[index];
                index += 2;
                if (index >= line.size()) {
                    throw new SyntaxError();
                } else {
                    node = getValue();
                }

                program.push_back(createAssignment(varName, node));
            }
            index++;
        }
    }
}

Instructions Lexer::getProgram() { return program; }
 
ASTNode * Lexer::getStringNode() {
    char firstSignToEndSequence = line[index][0];
    bool canEnd = false;
    std::string result = "";
    while(index < line.size()) {
        int j = 0;
        while(j < line[index].size()) {
            if (line[index][j] != firstSignToEndSequence) {
                result.push_back(line[index][j]);
            } else if (canEnd) {
                if (++j < line[index].size()) {
                    throw new SyntaxError();
                } else if (index + 1 < line.size() && line[index + 1][0] != '+') {
                    throw new SyntaxError();
                } else if (line[index + 1][0] == '+') {
                    index += 2;
                    return createSum(createConstString(result), getValue());
                }
                return createConstString(result);
            } else {
                canEnd = true;
            }
            j++;
        }
        index++;
    }
    return createConstString(result);
}

ASTNode * Lexer::getNumericNode() {
    bool isInt = true;
    std::string result = "";
    int j = 0;
    while(j < line[index].size()) {
        if (line[index][j] == '.') {
            if (isInt) isInt = false;
            result.push_back(line[index][j]);
        } else if (line[index][j] >= '0' && line[index][j] <= '9') {
            result.push_back(line[index][j]);
        } else {
            throw new SyntaxError();
        }
        j++;
    }
    if (index + 1 >= line.size()) {
        return isInt ? createConstInt(result) : createConstFloat(result);
    } else if (line[index + 1] == "+") {
        index += 2;
        return createSum(isInt ? createConstInt(result) : createConstFloat(result), getNumericNode());
    } else if (line[index + 1] == "-") {
        index += 2;
        return createSubstraction(isInt ? createConstInt(result) : createConstFloat(result), getNumericNode());
    } else {
        throw new SyntaxError();
    }
}

ASTNode * Lexer::getValue() {
    if (line[index][0] == '"' || line[index][0] == "'"[0]) {
        return getStringNode();
    } else if (line[index][0] >= '0' && line[index][0] <= '9') {
        return getNumericNode();
    } else {
        return getVariable();
    }	
}

ASTNode * Lexer::getVariable() {
    ASTNode * node = createGetVariable(line[index]);
    if (index + 1 >= line.size()) {
        return node;
    } else if (line[index + 1] == "+") {
        index += 2;
        return createSum(node, getValue());
    } else if (line[index + 1] == "-") {
        index += 2;
        return createSubstraction(node, getValue());
    } else {
        throw new SyntaxError();
    }
}
