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
    std::unique_ptr<AST> parse();
private: 
    std::unique_ptr<Lexer> m_lexer;
    std::unique_ptr<Token> m_token;
    // to next token
    void eat_token(TokenType type);
    // parse parentheses
    std::unique_ptr<AST> parse_paren();
    // parse int
    std::unique_ptr<AST> parse_int();
    // parsing parts of expression with different priorities
    // high priority - number or parentheses
    // middle priority - mul / div operations
    // low priority - add / sub operations
    std::unique_ptr<AST> parse_high_prior();
    std::unique_ptr<AST> parse_middle_prior();
    std::unique_ptr<AST> parse_low_prior();
    // parsing whole expressions or subexpression in parentheses
    std::unique_ptr<AST> parse_compound();
};

#endif