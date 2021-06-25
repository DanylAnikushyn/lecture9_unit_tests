#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    INT = 1,
    ADD,
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