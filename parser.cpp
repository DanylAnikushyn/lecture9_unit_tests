#include "parser.h"
#include "AST.h"
#include <iostream>
#include <memory>

Parser::Parser(std::unique_ptr<Lexer> lexer) : m_lexer(std::move(lexer))
{
    m_token = m_lexer->next_token(); 
}

void Parser::eat_token(TokenType type) 
{
    if (m_token->type != type)
    {
        std::cerr << "Parser (eat): Unexpected token: " << (int)m_token->type << ", was expecting " << (int)type << std::endl;
        exit(1);
    }
    m_token = m_lexer->next_token();
}

std::unique_ptr<AST> Parser::parse() 
{
    return parse_compound();
}

std::unique_ptr<AST> Parser::parse_paren()
{
  eat_token(TokenType::LPA);
  auto ast = std::make_unique<AST>(ASTType::COMPOUND);
  if (m_token->type != TokenType::RPA)
  {
    ast->value = parse_low_prior();
  }
  else
  {
    std::cerr << "Parser: empty parentheses" << std::endl;
    exit(1);
  }
  eat_token(TokenType::RPA);
  return ast;
}

std::unique_ptr<AST> Parser::parse_int()
{
    int int_value = std::stoi(m_token->value);
    eat_token(TokenType::INT);
    auto ast = std::make_unique<AST>(ASTType::INT);
    ast->op = TokenType::INT; 
    ast->int_value = int_value;
    return ast;
}

std::unique_ptr<AST> Parser::parse_high_prior()
{
    switch (m_token->type)
    {
        case TokenType::LPA: return parse_paren();
        case TokenType::INT: return parse_int();
        default: {
            std::cerr << "Parser (parse_high_prior): Unexpected token " << (int)m_token->type << std::endl;
            exit(1);
        }
    }
}

std::unique_ptr<AST> Parser::parse_middle_prior()
{
  auto ast_left = parse_high_prior();
  if (
      m_token->type == TokenType::MUL ||
      m_token->type == TokenType::DIV
  )
  {
    auto ast_binop = std::make_unique<AST>(ASTType::BINOP);
    ast_binop->left = std::move(ast_left);
    ast_binop->op = m_token->type;
    eat_token(m_token->type); 
    ast_binop->right = parse_middle_prior();
    return ast_binop;
  }  

  return ast_left;
}

std::unique_ptr<AST> Parser::parse_low_prior()
{
  auto ast_left = parse_middle_prior();
  if (
      m_token->type == TokenType::ADD ||
      m_token->type == TokenType::SUB
  )
  {
    auto ast_binop = std::make_unique<AST>(ASTType::BINOP);
    ast_binop->left = std::move(ast_left);
    ast_binop->op = m_token->type;
    eat_token(m_token->type);
    ast_binop->right = parse_low_prior();
    return ast_binop;
  }
  
  return ast_left;
}

std::unique_ptr<AST> Parser::parse_compound()
{
    auto compound = std::make_unique<AST>(ASTType::COMPOUND);

    while (m_token->type != TokenType::END)
    {
      compound->value = parse_low_prior();
    }

    return compound;
}