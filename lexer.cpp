#include "lexer.h"

#include <memory>
#include <stdexcept>
#include <sstream>

Lexer::Lexer(const std::string& str) : m_buffer(str), m_size(str.length()) 
{
    if (m_buffer.length() == 0) throw std::runtime_error("Expression with zero length");
    m_current_index = 0;
    m_current_symbol = m_buffer[m_current_index];
}

void Lexer::skip_whitespaces() 
{
    while (m_current_symbol == ' '  || 
           m_current_symbol == '\t' || 
           m_current_symbol == 10   || 
           m_current_symbol == 13)
        next_symb();
}

void Lexer::next_symb()
{
    if (m_current_index < m_size) 
    {
        ++m_current_index;
        m_current_symbol = m_buffer[m_current_index];
    }
}

std::unique_ptr<Token> Lexer::next_symb_with_current(TokenType type) 
{
    std::string value;
    value += m_current_symbol;

    auto token = std::make_unique<Token>(value, type);
    next_symb();

    return token;
}

std::unique_ptr<Token> Lexer::get_number()
{
    std::string value;

    while (isdigit(m_current_symbol))
    {
        value += m_current_symbol;
        next_symb();
    }

    return std::make_unique<Token>(value, TokenType::INT);
}

std::unique_ptr<Token> Lexer::next_token()
{
    while(m_current_symbol != '\0') 
    {
        skip_whitespaces();
        if (isdigit(m_current_symbol)) {
            return get_number();
        }
        switch (m_current_symbol) {
            case '+': return next_symb_with_current(TokenType::ADD);
            case '-': return next_symb_with_current(TokenType::SUB); 
            case '*': return next_symb_with_current(TokenType::MUL);
            case '/': return next_symb_with_current(TokenType::DIV);
            case '(': return next_symb_with_current(TokenType::LPA);
            case ')': return next_symb_with_current(TokenType::RPA);
            case '\0': break;
            default: {
                std::stringstream msg("Lexer :: Unexpected character ");
                msg << m_current_symbol << "\n";
                throw std::runtime_error(msg.str());  
                break;
            }
        }
    }
    return std::make_unique<Token>("", TokenType::END);
}