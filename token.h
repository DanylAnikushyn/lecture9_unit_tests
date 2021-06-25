#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    NOT = 0,
    INT,
    ADD,
    SUB,
    MUL,
    DIV,
    LPA,
    RPA,
    END
};

struct Token
{
    std::string value;
    TokenType type;
    Token(const std::string& val, TokenType t) : value(val), type(t)
    {}
};


#endif