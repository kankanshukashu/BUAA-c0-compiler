#pragma once

#include "token.h"

#include <sstream>
#include <cctype>
#include <exception>

extern std::string inputFile;
extern long long inputSize;
extern long long index;

enum DFAState {
    INITIAL_STATE,
    ZERO_STATE,
    DECIMAL_INTEGER_STATE,
    HEXADECIMAL_INTEGER_STATE,
    PLUS_SIGN_STATE,
    MINUS_SIGN_STATE,
    DIVISION_SIGN_STATE,
    MULTIPLICATION_SIGN_STATE,
    IDENTIFIER_STATE,
    EQUAL_SIGN_STATE,
    SEMICOLON_STATE,
    LESS_THAN_SIGN_STATE,
    GREATER_THAN_SIGN_STATE,
    COMMA_STATE,
    UNEQUAL_SIGN_STATE,
    LEFT_BRACKET_STATE,
    RIGHT_BRACKET_STATE,
    LEFT_BRACE_STATE,
    RIGHT_BRACE_STATE,
};

Token nextToken();