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
    AST* value;
    AST* left;
    AST* right;
    TokenType op;
    int int_value;

    AST(ASTType type);
    ~AST();
};

#endif