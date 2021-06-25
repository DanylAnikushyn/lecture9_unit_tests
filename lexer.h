#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <iostream>
#include <memory>

class Lexer 
{
public:
    Lexer(const std::string& str);
    // to next token
    std::unique_ptr<Token> next_token();
private:
    void skip_whitespaces();
    // to next symbol
    void next_symb();
    // to next symbol and return current token
    std::unique_ptr<Token> next_symb_with_current(TokenType type);
    // get token number
    std::unique_ptr<Token> get_number();

    std::string m_buffer;
    std::size_t m_size;
    char m_current_symbol;
    std::size_t m_current_index;
};

#endif