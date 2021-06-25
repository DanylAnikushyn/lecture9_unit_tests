#ifndef AST_H
#define AST_H

#include "token.h"

#include <memory>

enum class ASTType 
{
    COMPOUND = 1,
    INT,                            
    BINOP                 
};

class AST
{
public:
    ASTType type;
    std::unique_ptr<AST> value;
    std::unique_ptr<AST> left;
    std::unique_ptr<AST> right;
    TokenType op;
    int int_value;

    AST(ASTType type);
};

#endif