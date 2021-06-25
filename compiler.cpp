#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "AST.h"

#include <string>
#include <stdexcept>
#include <memory>

int Compiler::compile(const std::string& expression) {
    auto lexer = std::make_unique<Lexer>(expression);
    auto parser = std::make_unique<Parser>(std::move(lexer));
    auto root = parser->parse();

    return calculate(root);
}

int Compiler::calculate(AST* ast) {
    if (ast->type == ASTType::COMPOUND) 
    {
        return calculate(ast->value);
    }
    if (ast->type == ASTType::INT) 
    {
        return ast->int_value;
    }
    switch (ast->op) 
    {
        case TokenType::ADD: {
            if (ast->right != nullptr && 
                    (ast->right->left != nullptr && ast->right->right != nullptr))
            {
                if (ast->right->op == TokenType::ADD) 
                    return calculate(ast->left) + calculate(ast->right->left) + calculate(ast->right->right);
                if (ast->right->op == TokenType::SUB)
                    return calculate(ast->left) + calculate(ast->right->left) - calculate(ast->right->right);       
            }
            return calculate(ast->left) + calculate(ast->right);
        }
        case TokenType::SUB: {
            if (ast->right != nullptr && 
                    ast->right->left != nullptr && ast->right->right != nullptr) 
            {
                if (ast->right->op == TokenType::ADD)
                    return calculate(ast->left) - calculate(ast->right->left) + calculate(ast->right->right);
                if (ast->right->op == TokenType::SUB)
                    return calculate(ast->left) - calculate(ast->right->left) - calculate(ast->right->right);       
            }
            return calculate(ast->left) - calculate(ast->right);
        }
        case TokenType::MUL: { 
            if (ast->right != nullptr && 
                    ast->right->left != nullptr && ast->right->right != nullptr) 
            {
                if (ast->right->op == TokenType::MUL)
                    return calculate(ast->left) * calculate(ast->right->left) * calculate(ast->right->right);
                if (ast->right->op == TokenType::DIV)
                    return calculate(ast->left) * calculate(ast->right->left) / calculate(ast->right->right);       
            }
            return calculate(ast->left) * calculate(ast->right);
        }
        case TokenType::DIV: {
            if (ast->right != nullptr && 
                    ast->right->left != nullptr && ast->right->right != nullptr) 
            {
                if (ast->right->op == TokenType::MUL)
                    return calculate(ast->left) / calculate(ast->right->left) * calculate(ast->right->right);
                if (ast->right->op == TokenType::DIV)
                    return calculate(ast->left) / calculate(ast->right->left) / calculate(ast->right->right);       
            }
            return calculate(ast->left) / calculate(ast->right);
        }
        default: throw std::runtime_error("Fault token");
    }
} 