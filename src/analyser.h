#pragma once

#include "token.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

extern long long listIndex;
extern std::vector<Token> tokenList;
extern long long listSize;

struct funcInfo {
    std::string name;
    bool isVoid;
    int paramSlot;
};

extern std::vector<funcInfo> funcList;
extern std::vector<std::string> startCode;
extern std::vector<std::vector<std::string> > funcCode;

std::string analyseProgram();

std::string analyseVariableDeclaration();

std::string analyseConstVariableDeclaration();

std::string analyseIntVariableDeclaration();

std::string analyseExpression();

std::string analyseAdditiveExpression();

std::string analyseMultiplicativeExpression();

std::string analyseUnaryExpression();

std::string analyseFunctionDeclaration();

std::string analyseParameterClause();

std::string analyseCompoundStatement();

std::string analyseParameterDeclaration();

std::string analyseStatementSeq();

std::string analyseStatement();

std::string analyseConditionStatement();

std::string analyseCondition();

std::string analyseLoopStatement();

std::string analyseJumpStatement(bool);

std::string analysePrintStatement();

std::string analysePrintableList();

std::string analyseScanStatement();

std::string analyseAssignmentExpression();

std::string analyseFunctionCall();

std::string analyseExpressionList(int &);

bool findDT(Token, std::vector<std::pair<std::string, bool> >);

bool isConst(Token, std::vector<std::pair<std::string, bool> >);

void sout(std::string);

void oout(std::string);