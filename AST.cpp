#include "AST.h"
#include "token.h"

AST::AST(ASTType type) : type(type), op(TokenType::NOT)
{
    value = std::unique_ptr<AST>();
    left = std::unique_ptr<AST>();
    right = std::unique_ptr<AST>();
}