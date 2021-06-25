#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"
#include "AST.h"

#include <memory>

class Parser 
{
public:
    Parser(std::unique_ptr<Lexer> lexer); 
    AST* parse();
private: 
    std::unique_ptr<Lexer> m_lexer;
    std::unique_ptr<Token> m_token;
    // to next token
    void eat_token(TokenType type);
    // parse parentheses
    AST* parse_paren();
    // parse int
    AST* parse_int();
    // parsing parts of expression with different priorities
    // high priority - number or parentheses
    // middle priority - mul / div operations
    // low priority - add / sub operations
    AST* parse_high_prior();
    AST* parse_middle_prior();
    AST* parse_low_prior();
    // parsing whole expressions or subexpression in parentheses
    AST* parse_compound();
};

#endif