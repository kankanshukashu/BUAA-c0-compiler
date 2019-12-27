#pragma once

#include <string>

enum TokenType {
    EOF_ERROR,
    UNACCEPTABLE_ERROR,
    DIGIT_ALPHA_ERROR,
    INTEGER_OVERFLOW_ERROR,
    UNEQUAL_SIGN_ERROR,
    EXCEPTED_ERROR,
    UNSIGNED_INTEGER,
    IDENTIFIER,
    CONST,
    VOID,
    INT,
    CHAR,
    DOUBLE,
    STRUCT,
    IF,
    ELSE,
    SWITCH,
    CASE,
    DEFAULT,
    WHILE,
    FOR,
    DO,
    RETURN,
    BREAK,
    CONTINUE,
    PRINT,
    SCAN,
    PLUS_SIGN,
    MINUS_SIGN,
    MULTIPLICATION_SIGN,
    DIVISION_SIGN,
    DOUBLE_EQUAL_SIGH,
    EQUAL_SIGN,
    LESS_THAN_SIGN,
    GREATER_THAN_SIGN,
    LESS_THAN_OR_EQUAL_SIGN,
    GREATER_THAN_OR_EQUAL_SIGN,
    COMMA,
    UNEQUAL_SIGN,
    SEMICOLON,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE
};

class Token {
public:
    Token();
    Token(std::string s);
    Token(TokenType t);
    Token(std::string s, TokenType t);
    std::string _value;
    TokenType _type;
};