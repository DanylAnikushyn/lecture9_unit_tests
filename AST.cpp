#include "AST.h"
#include "token.h"

AST::AST(ASTType type) : type(type), op(TokenType::NOT)
{
    value = nullptr;
    left = nullptr;
    right = nullptr;
}

AST::~AST()
{
    delete value;
    delete left;
    delete right;
}
