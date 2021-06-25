#include "parser.h"
#include "AST.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <sstream>

Parser::Parser(std::unique_ptr<Lexer> lexer) : m_lexer(std::move(lexer))
{
    m_token = m_lexer->next_token(); 
}

void Parser::eat_token(TokenType type) 
{
    if (m_token->type != type)
    {
        auto msg = std::stringstream("Parser (eat): Unexpected token: ");
        msg << (int)m_token->type << ", was expecting " << (int)type << std::endl;
        throw(std::runtime_error(msg.str()));
    }
    m_token = m_lexer->next_token();
}

AST* Parser::parse() 
{
    return parse_compound();
}

AST* Parser::parse_paren()
{
  eat_token(TokenType::LPA);
  auto ast = new AST(ASTType::COMPOUND);
  if (m_token->type != TokenType::RPA)
  {
    ast->value = parse_low_prior();
  }
  else
  {
    throw std::runtime_error("Parser: empty parentheses\n");
  }
  eat_token(TokenType::RPA);
  return ast;
}

AST* Parser::parse_int()
{
    int int_value = std::stoi(m_token->value);
    eat_token(TokenType::INT);
    auto ast = new AST(ASTType::INT);
    ast->op = TokenType::INT; 
    ast->int_value = int_value;
    return ast;
}

AST* Parser::parse_high_prior()
{
    switch (m_token->type)
    {
        case TokenType::LPA: return parse_paren();
        case TokenType::INT: return parse_int();
        default: {
            auto msg = std::stringstream("Parser (parse_high_prior): Unexpected token ");
            msg << (int)m_token->type << std::endl; 
            throw std::runtime_error(msg.str());
        }
    }
}

AST* Parser::parse_middle_prior()
{
  auto ast_left = parse_high_prior();
  if (
      m_token->type == TokenType::MUL ||
      m_token->type == TokenType::DIV
  )
  {
    auto ast_binop = new AST(ASTType::BINOP);
    ast_binop->left = std::move(ast_left);
    ast_binop->op = m_token->type;
    eat_token(m_token->type); 
    ast_binop->right = parse_middle_prior();
    return ast_binop;
  }  

  return ast_left;
}

AST* Parser::parse_low_prior()
{
  auto ast_left = parse_middle_prior();
  if (
      m_token->type == TokenType::ADD ||
      m_token->type == TokenType::SUB
  )
  {
    auto ast_binop = new AST(ASTType::BINOP);
    ast_binop->left = std::move(ast_left);
    ast_binop->op = m_token->type;
    eat_token(m_token->type);
    ast_binop->right = parse_low_prior();
    return ast_binop;
  }
  
  return ast_left;
}

AST* Parser::parse_compound()
{
    auto compound = new AST(ASTType::COMPOUND);

    while (m_token->type != TokenType::END)
    {
      compound->value = parse_low_prior();
    }

    return compound;
}