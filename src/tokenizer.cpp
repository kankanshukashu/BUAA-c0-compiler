#include "tokenizer.h"

using namespace std;

Token nextToken()
{
    char ch;
    bool isEnd;
    stringstream ss;
    DFAState current_state = INITIAL_STATE;
    while (true) {
        if (index < inputSize) {
            ch = inputFile[index];
            isEnd = false;
        }
        else
            isEnd = true;
        index++;

        switch (current_state) {
        case INITIAL_STATE: {

            if (isEnd)
                return Token(EOF_ERROR);
            if (ch < -1 || ch >127)
                return Token(UNACCEPTABLE_ERROR);

            if (isspace(ch))
                current_state = INITIAL_STATE;
            else if (ch=='0')
                current_state = ZERO_STATE;
            else if (isdigit(ch))
                current_state = DECIMAL_INTEGER_STATE;
            else if (isalpha(ch))
                current_state = IDENTIFIER_STATE;
            else {
                switch (ch) {
                case '=':
                    current_state = EQUAL_SIGN_STATE;
                    break;
                case '-':
                    current_state = MINUS_SIGN_STATE;
                    break;
                case '+':
                    current_state = PLUS_SIGN_STATE;
                    break;
                case '*':
                    current_state = MULTIPLICATION_SIGN_STATE;
                    break;
                case '/':{
                    if (index >= inputSize)
                        return Token(EOF_ERROR);
                    if (inputFile[index] == '/') {
                        while (inputFile[index] != 0x0A && inputFile[index] != 0x0D) {
                            index++;
                            if (index >= inputSize)
                                return Token(EOF_ERROR);
                        }
                        current_state = INITIAL_STATE;
                    }
                    else if (inputFile[index] == '*') {
                        index++;
                        if (index + 1 >= inputSize)
                            return Token(EOF_ERROR);
                        while (inputFile[index] != '*' || inputFile[index + 1] != '/') {
                            index++;
                            if (index + 1 >= inputSize)
                                return Token(EOF_ERROR);
                        }
                        index += 2;
                        current_state = INITIAL_STATE;
                    }
                    else
                        current_state = DIVISION_SIGN_STATE;
                    break;
                }
                case '(':
                    current_state = LEFT_BRACKET_STATE;
                    break;
                case ')':
                    current_state = RIGHT_BRACKET_STATE;
                    break;
                case ';':
                    current_state = SEMICOLON_STATE;
                    break;
                case '{':
                    current_state = LEFT_BRACE_STATE;
                    break;
                case '}':
                    current_state = RIGHT_BRACE_STATE;
                    break;
                case ',':
                    current_state = COMMA_STATE;
                    break;
                case '!':
                    current_state = UNEQUAL_SIGN_STATE;
                    break;
                case '<':
                    current_state = LESS_THAN_SIGN_STATE;
                    break;
                case '>':
                    current_state = GREATER_THAN_SIGN_STATE;
                    break;
                default:
                    return Token(UNACCEPTABLE_ERROR);
                }
            }
            if (current_state != INITIAL_STATE)
                ss << ch;
            break;
        }
        case ZERO_STATE: {
            if (isEnd)
                return Token("0", UNSIGNED_INTEGER);
            else {
                if (ch == 'x' || ch == 'X') {
                    current_state = HEXADECIMAL_INTEGER_STATE;
                    ss << ch;
                }
                else if (isalpha(ch))
                    return Token(DIGIT_ALPHA_ERROR);
                else {
                    index--;
                    return Token("0", UNSIGNED_INTEGER);
                }
            }
            break;
        }
        case DECIMAL_INTEGER_STATE: {
            if (isEnd) {
                int tmpi;
                try {
                    string tmps;
                    ss >> tmps;
                    tmpi = stoi(tmps, 0, 10);
                }
                catch (exception) {
                    return Token(INTEGER_OVERFLOW_ERROR);
                }
                return Token(to_string(tmpi), UNSIGNED_INTEGER);
            }
            else if (isdigit(ch))
                ss << ch;
            else if(isalpha(ch))
                return Token(DIGIT_ALPHA_ERROR);
            else {
                index--;
                int tmpi;
                try {
                    string tmps;
                    ss >> tmps;
                    tmpi = stoi(tmps, 0, 10);
                }
                catch (exception) {
                    return Token(INTEGER_OVERFLOW_ERROR);
                }
                return Token(to_string(tmpi), UNSIGNED_INTEGER);
            }
            break;
        }
        case HEXADECIMAL_INTEGER_STATE: {
            if (isEnd) {
                int tmpi;
                try {
                    string tmps;
                    ss >> tmps;
                    tmpi = stoi(tmps, 0, 16);
                }
                catch (exception) {
                    return Token(INTEGER_OVERFLOW_ERROR);
                }
                return Token(to_string(tmpi), UNSIGNED_INTEGER);
            }
            else if (isxdigit(ch))
                ss << ch;
            else if (isalpha(ch))
                return Token(DIGIT_ALPHA_ERROR);
            else {
                index--;
                int tmpi;
                try {
                    string tmps;
                    ss >> tmps;
                    tmpi = stoi(tmps, 0, 16);
                }
                catch (exception) {
                    return Token(INTEGER_OVERFLOW_ERROR);
                }
                return Token(to_string(tmpi), UNSIGNED_INTEGER);
            }
            break;
        }
        case IDENTIFIER_STATE: {
            if (isEnd || !isalnum(ch)) {
                if (!isEnd)
                    index--;
                string tmps;
                ss >> tmps;
                if (tmps == "const")
                    return Token(tmps, CONST);
                else if (tmps == "void")
                    return Token(tmps, VOID);
                else if (tmps == "int")
                    return Token(tmps, INT);
                else if (tmps == "char")
                    return Token(tmps, CHAR);
                else if (tmps == "double")
                    return Token(tmps, DOUBLE);
                else if (tmps == "struct")
                    return Token(tmps, STRUCT);
                else if (tmps == "if")
                    return Token(tmps, IF);
                else if (tmps == "else")
                    return Token(tmps, ELSE);
                else if (tmps == "switch")
                    return Token(tmps, SWITCH);
                else if (tmps == "case")
                    return Token(tmps, CASE);
                else if (tmps == "default")
                    return Token(tmps, DEFAULT);
                else if (tmps == "while")
                    return Token(tmps, WHILE);
                else if (tmps == "for")
                    return Token(tmps, FOR);
                else if (tmps == "do")
                    return Token(tmps, DO);
                else if (tmps == "return")
                    return Token(tmps, RETURN);
                else if (tmps == "break")
                    return Token(tmps, BREAK);
                else if (tmps == "continue")
                    return Token(tmps, CONTINUE);
                else if (tmps == "print")
                    return Token(tmps, PRINT);
                else if (tmps == "scan")
                    return Token(tmps, SCAN);
                else
                    return Token(tmps, IDENTIFIER);
            }
            ss << ch;
            break;
        }
        case PLUS_SIGN_STATE: {
            index--;
            return Token("+", PLUS_SIGN);
        }
        case MINUS_SIGN_STATE: {
            index--;
            return Token("-", MINUS_SIGN);
        }
        case MULTIPLICATION_SIGN_STATE: {
            index--;
            return Token("*", MULTIPLICATION_SIGN);
        }
        case DIVISION_SIGN_STATE: {
            index--;
            return Token("/", DIVISION_SIGN);
        }
        case EQUAL_SIGN_STATE: {
            if (!isEnd)
                if (ch == '=')
                    return Token("==", DOUBLE_EQUAL_SIGH);
            index--;
            return Token("=", EQUAL_SIGN);
        }
        case LEFT_BRACKET_STATE: {
            index--;
            return Token("(", LEFT_BRACKET);
        }
        case RIGHT_BRACKET_STATE: {
            index--;
            return Token(")", RIGHT_BRACKET);
        }
        case LEFT_BRACE_STATE: {
            index--;
            return Token("{", LEFT_BRACE);
        }
        case RIGHT_BRACE_STATE: {
            index--;
            return Token("}", RIGHT_BRACE);
        }
        case COMMA_STATE: {
            index--;
            return Token(",", COMMA);
        }
        case SEMICOLON_STATE: {
            index--;
            return Token(";", SEMICOLON);
        }
        case UNEQUAL_SIGN_STATE: {
            if (!isEnd)
                if (ch == '=')
                    return Token("!=", UNEQUAL_SIGN);
            return Token(UNEQUAL_SIGN_ERROR);
        }
        case LESS_THAN_SIGN_STATE: {
            if (!isEnd)
                if (ch == '=')
                    return Token("<=", LESS_THAN_OR_EQUAL_SIGN);
            index--;
            return Token("<", LESS_THAN_SIGN);
        }
        case GREATER_THAN_SIGN_STATE: {
            if (!isEnd)
                if (ch == '=')
                    return Token(">=", GREATER_THAN_OR_EQUAL_SIGN);
            index--;
            return Token(">", GREATER_THAN_SIGN);
        }
        }
    }
    return Token(EXCEPTED_ERROR);
}

Token::Token() {

}

Token::Token(string s)
{
    _value = s;
}

Token::Token(TokenType t)
{
    _type = t;
}

Token::Token(string s, TokenType t)
{
    _value.assign(s);
    _type = t;
}