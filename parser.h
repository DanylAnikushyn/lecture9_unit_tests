#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "AST.h"

#include <memory>

class Parser 
{
public:
    Parser(std::unique_ptr<Lexer> lexer) {}
    std::unique_ptr<AST> parse() 
    {
        return std::make_unique<AST>(AST());
    }
};

#endif